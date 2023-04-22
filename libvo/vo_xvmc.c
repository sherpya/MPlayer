/*
 * This file is part of MPlayer.
 *
 * MPlayer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * MPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with MPlayer; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "config.h"
#include "mp_msg.h"
#include "video_out.h"
#define NO_DRAW_FRAME
#include "video_out_internal.h"
#include "osdep/timer.h"
#include "libmpcodecs/vf.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#ifdef HAVE_SHM
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>
#endif

#include <X11/extensions/Xv.h>
#include <X11/extensions/Xvlib.h>
#include <X11/extensions/XvMClib.h>

#include "x11_common.h"
#include "libavcodec/avcodec.h"
#include "libavcodec/xvmc.h"

#include "sub/sub.h"
#include "aspect.h"

#include "subopt-helper.h"

#include "libavutil/common.h"

//no chance for xinerama to be supported in the near future
#undef CONFIG_XINERAMA

#undef NDEBUG
#include <assert.h>


static int benchmark;
static int use_sleep;
static int first_frame;//draw colorkey on first frame
static int use_queue;
static int xv_port_request = 0;
static int xv_adaptor = -1;
static int bob_deinterlace;
static int top_field_first;

static int image_width,image_height;
static int image_format;

#define NO_SUBPICTURE      0
#define OVERLAY_SUBPICTURE 1
#define BLEND_SUBPICTURE   2
#define BACKEND_SUBPICTURE 3

static int subpicture_mode;
static int subpicture_alloc;
static XvMCSubpicture subpicture;
static XvImageFormatValues subpicture_info;
static int subpicture_clear_color;//transparent color for the subpicture or color key for overlay

static XvMCSurfaceInfo surface_info;
static XvMCContext ctx;
static XvMCBlockArray data_blocks;
static XvMCMacroBlockArray mv_blocks;

#define MAX_SURFACES 8
static int number_of_surfaces=0;
static XvMCSurface surface_array[MAX_SURFACES];
struct xvmc_render{
    struct xvmc_pix_fmt shared;
#define STATE_DISPLAY_PENDING 1  /**  the surface should be shown, the video driver manipulates this */
#define STATE_PREDICTION      2  /**  the surface is needed for prediction, the codec manipulates this */
#define STATE_OSD_SOURCE      4  /**  the surface is needed for subpicture rendering */
    int state;
    void *p_osd_target_surface_render;
    mp_image_t *mpi;
};
static struct xvmc_render *surface_render;

static struct xvmc_render *p_render_surface_to_show = NULL;
static struct xvmc_render *p_render_surface_visible = NULL;

//display queue, kinda render ahead
static struct xvmc_render *show_queue[MAX_SURFACES];
static int free_element;


static void (*draw_osd_fnc)(int x0,int y0, int w,int h, unsigned char* src, unsigned char *srca, int stride);
static void (*clear_osd_fnc)(int x0,int y0, int w,int h);
static void (*init_osd_fnc)(void);

static void   draw_osd_AI44(int x0,int y0, int w,int h, unsigned char* src, unsigned char *srca, int stride);
static void   draw_osd_IA44(int x0,int y0, int w,int h, unsigned char* src, unsigned char *srca, int stride);
static void   clear_osd_subpic(int x0,int y0, int w,int h);
static void   init_osd_yuv_pal(void);


static const struct{
    int id;//id as xvimages or as mplayer RGB|{8,15,16,24,32}
    void (* init_func_ptr)(void);
    void (* draw_func_ptr)(int, int, int, int, unsigned char *, unsigned char *, int);
    void (* clear_func_ptr)(int, int, int, int);
    } osd_render[]={
                        {0x34344149,init_osd_yuv_pal,draw_osd_AI44,clear_osd_subpic},
                        {0x34344941,init_osd_yuv_pal,draw_osd_IA44,clear_osd_subpic},
                        {0,NULL,NULL,NULL}
                    };

static void xvmc_free(void);
static void xvmc_clean_surfaces(void);
static int count_free_surfaces(void);
static struct xvmc_render *find_free_surface(void);

static const vo_info_t info = {
    "XVideo Motion Compensation",
    "xvmc",
    "Ivan Kalvachev <iive@users.sf.net>",
    ""
};

const LIBVO_EXTERN(xvmc);

//shm stuff from vo_xv
#ifdef HAVE_SHM
static XShmSegmentInfo Shminfo;
static int Shmem_Flag;
#endif
static XvImage * xvimage;

static void add_state(struct xvmc_render *cur_render, int flags)
{
    int bit;
    if (cur_render->mpi) {
        for (bit = 1; flags >= bit; bit <<= 1) {
            if (flags & ~cur_render->state & bit)
                cur_render->mpi->usage_count++;
        }
    }
    cur_render->state |= flags;
}

static void remove_state(struct xvmc_render *cur_render, int flags)
{
    int bit;
    if (cur_render->mpi) {
        for (bit = 1; flags >= bit; bit <<= 1) {
            if (flags & cur_render->state & bit)
                cur_render->mpi->usage_count--;
        }
    }
    cur_render->state &= ~flags;
}

static int in_use(struct xvmc_render *cur_render)
{
    return cur_render->state || (cur_render->mpi && cur_render->mpi->usage_count);
}

static int allocate_xvimage(int xvimage_width,int xvimage_height,int xv_format)
{
 /*
  * allocate XvImages.
  */
#ifdef HAVE_SHM
    if ( mLocalDisplay && XShmQueryExtension( mDisplay ) ) Shmem_Flag = 1;
    else
    {
        Shmem_Flag = 0;
        mp_msg(MSGT_VO,MSGL_WARN, "Shared memory not supported\nReverting to normal Xv\n" );
    }
    if ( Shmem_Flag )
    {
        xvimage = (XvImage *) XvShmCreateImage(mDisplay, xv_port, xv_format,
                             NULL, xvimage_width, xvimage_height, &Shminfo);
        if (!xvimage)
            goto noshmimage;
        if (!xvimage->data_size)
            goto shmgetfail;

        Shminfo.shmid    = shmget(IPC_PRIVATE, xvimage->data_size, IPC_CREAT | SHM_R | SHM_W);
        if (Shminfo.shmid == -1)
            goto shmgetfail;
        Shminfo.shmaddr  = (char *) shmat(Shminfo.shmid, 0, 0);
        if (Shminfo.shmaddr == (void *)-1)
            goto shmatfail;
        Shminfo.readOnly = False;

        xvimage->data = Shminfo.shmaddr;
        if (!XShmAttach(mDisplay, &Shminfo))
            goto shmattachfail;
        XSync(mDisplay, False);
        shmctl(Shminfo.shmid, IPC_RMID, 0);
        return 0;
shmattachfail:
        shmdt(Shminfo.shmaddr);
shmatfail:
        shmctl(Shminfo.shmid, IPC_RMID, 0);
shmgetfail:
        XFree(xvimage);
        xvimage = NULL;
noshmimage:
        Shmem_Flag = 0;
    }
#endif

    xvimage = (XvImage *) XvCreateImage(mDisplay, xv_port, xv_format, NULL, xvimage_width, xvimage_height);
    if (!xvimage) return -1;
    if (!xvimage->data_size)
    {
        mp_msg(MSGT_VO, MSGL_WARN,
                "XServer's XvCreateImage implementation is buggy (returned 0-sized image)\n" );
        XFree(xvimage);
        xvimage = NULL;
        return -1;
    }
    xvimage->data = malloc(xvimage->data_size);
    XSync(mDisplay,False);
// memset(xvimage->data,128,xvimage->data_size);
    return 0;
}

static void deallocate_xvimage(void)
{
#ifdef HAVE_SHM
    if ( Shmem_Flag )
    {
        XShmDetach( mDisplay,&Shminfo );
        shmdt( Shminfo.shmaddr );
    }
    else
#endif
    {
        free(xvimage->data);
        xvimage->data = NULL;
    }
    XFree(xvimage);
    xvimage = NULL;

    XSync(mDisplay, False);
    return;
}
//end of vo_xv shm/xvimage code

static int xvmc_check_surface_format(uint32_t format, XvMCSurfaceInfo * surf_info){
    if ( format == IMGFMT_XVMC_IDCT_MPEG2 ){
        if( surf_info->mc_type != (XVMC_IDCT|XVMC_MPEG_2) ) return -1;
        if( surf_info->chroma_format != XVMC_CHROMA_FORMAT_420 ) return -1;
        return 0;
    }
    if ( format == IMGFMT_XVMC_MOCO_MPEG2 ){
        if(surf_info->mc_type != XVMC_MPEG_2) return -1;
        if(surf_info->chroma_format != XVMC_CHROMA_FORMAT_420) return -1;
        return 0;
    }
return -1;//fail
}

//print all info needed to add new format
static void print_xvimage_format_values(XvImageFormatValues *xifv){
    int i;
    const int msgl=MSGL_DBG4;

    mp_msg(MSGT_VO,msgl,"Format_ID = 0x%X\n",xifv->id);

    mp_msg(MSGT_VO,msgl,"  type = ");
    if(xifv->type == XvRGB) mp_msg(MSGT_VO,msgl,"RGB\n");
    else if(xifv->type == XvYUV) mp_msg(MSGT_VO,msgl,"YUV\n");
    else mp_msg(MSGT_VO,msgl,"Unknown\n");

    mp_msg(MSGT_VO,msgl,"  byte_order = ");
    if(xifv->byte_order == LSBFirst) mp_msg(MSGT_VO,msgl,"LSB First\n");
    else if(xifv->byte_order == MSBFirst) mp_msg(MSGT_VO,msgl,"MSB First\n");
    else mp_msg(MSGT_VO,msgl,"Unknown\n");//yes Linux support other types too

    mp_msg(MSGT_VO,msgl,"  guid = ");
    for(i=0;i<16;i++)
        mp_msg(MSGT_VO,msgl,"%02X ",(unsigned char)xifv->guid[i]);
    mp_msg(MSGT_VO,msgl,"\n");

    mp_msg(MSGT_VO,msgl,"  bits_per_pixel = %d\n",xifv->bits_per_pixel);

    mp_msg(MSGT_VO,msgl,"  format = ");
    if(xifv->format == XvPacked) mp_msg(MSGT_VO,msgl,"XvPacked\n");
    else if(xifv->format == XvPlanar) mp_msg(MSGT_VO,msgl,"XvPlanar\n");
    else mp_msg(MSGT_VO,msgl,"Unknown\n");

    mp_msg(MSGT_VO,msgl,"  num_planes = %d\n",xifv->num_planes);

    if(xifv->type == XvRGB){
        mp_msg(MSGT_VO,msgl,"  red_mask = %0X\n",  xifv->red_mask);
        mp_msg(MSGT_VO,msgl,"  green_mask = %0X\n",xifv->green_mask);
        mp_msg(MSGT_VO,msgl,"  blue_mask = %0X\n", xifv->blue_mask);
    }
    if(xifv->type == XvYUV){
        mp_msg(MSGT_VO,msgl,"  y_sample_bits = %d\n  u_sample_bits = %d\n  v_sample_bits = %d\n",
                xifv->y_sample_bits,xifv->u_sample_bits,xifv->v_sample_bits);
        mp_msg(MSGT_VO,msgl,"  horz_y_period = %d\n  horz_u_period = %d\n  horz_v_period = %d\n",
                xifv->horz_y_period,xifv->horz_u_period,xifv->horz_v_period);
        mp_msg(MSGT_VO,msgl,"  vert_y_period = %d\n  vert_u_period = %d\n  vert_v_period = %d\n",
                xifv->vert_y_period,xifv->vert_u_period,xifv->vert_v_period);

        mp_msg(MSGT_VO,msgl,"  component_order = ");
        for(i=0;i<32;i++)
            if(xifv->component_order[i]>=32)
                mp_msg(MSGT_VO,msgl,"%c",xifv->component_order[i]);
        mp_msg(MSGT_VO,msgl,"\n");

        mp_msg(MSGT_VO,msgl,"  scanline = ");
        if(xifv->scanline_order == XvTopToBottom) mp_msg(MSGT_VO,msgl,"XvTopToBottom\n");
        else if(xifv->scanline_order == XvBottomToTop) mp_msg(MSGT_VO,msgl,"XvBottomToTop\n");
        else mp_msg(MSGT_VO,msgl,"Unknown\n");
    }
    mp_msg(MSGT_VO,msgl,"\n");
}

// WARNING This function may changes xv_port and surface_info!
static int xvmc_find_surface_by_format(int format,int width,int height,
                                        XvMCSurfaceInfo * surf_info,int query){
    int rez;
    XvAdaptorInfo * ai;
    int num_adaptors,i;
    unsigned long p;
    int s,mc_surf_num;
    XvMCSurfaceInfo * mc_surf_list;

    rez = XvQueryAdaptors(mDisplay,DefaultRootWindow(mDisplay),&num_adaptors,&ai);
    if( rez != Success ) return -1;
    mp_msg(MSGT_VO,MSGL_DBG3,"vo_xvmc: Querying %d adaptors\n",num_adaptors);
    for(i=0; i<num_adaptors; i++)
    {
        /* check if adaptor number has been specified */
        if (xv_adaptor != -1 && xv_adaptor != i)
            continue;
        mp_msg(MSGT_VO,MSGL_DBG3,"vo_xvmc: Querying adaptor #%d\n",i);
        if( ai[i].type == 0 ) continue;// we need at least dummy type!
//probing ports
        for(p=ai[i].base_id; p<ai[i].base_id+ai[i].num_ports; p++)
        {
            mp_msg(MSGT_VO,MSGL_DBG3,"vo_xvmc: probing port #%ld\n",p);
            mc_surf_list = XvMCListSurfaceTypes(mDisplay,p,&mc_surf_num);
            if( mc_surf_list == NULL || mc_surf_num == 0){
                mp_msg(MSGT_VO,MSGL_DBG3,"vo_xvmc: No XvMC supported. \n");
                continue;
            }
            mp_msg(MSGT_VO,MSGL_DBG3,"vo_xvmc: XvMC list have %d surfaces\n",mc_surf_num);
//we have XvMC list!
            for(s=0; s<mc_surf_num; s++)
            {
                if( width > mc_surf_list[s].max_width ) continue;
                if( height > mc_surf_list[s].max_height ) continue;
                if( xvmc_check_surface_format(format,&mc_surf_list[s])<0 ) continue;
//we have match!
                /* respect the users wish */
                if ( xv_port_request != 0 && xv_port_request != p )
                {
                    continue;
                }

                if(!query){
                    rez = XvGrabPort(mDisplay,p,CurrentTime);
                    if(rez != Success){
                        mp_msg(MSGT_VO,MSGL_DBG3,"vo_xvmc: Fail to grab port %ld\n",p);
                        continue;
                    }
                    mp_msg(MSGT_VO,MSGL_INFO,"vo_xvmc: Using Xv Adaptor #%d (%s)\n", i, ai[i].name);
                    mp_msg(MSGT_VO,MSGL_INFO,"vo_xvmc: Port %ld grabed\n",p);
                    xv_port = p;
                }
                goto surface_found;
            }//for mc surf
            XFree(mc_surf_list);//if mc_surf_num==0 is list==NULL ?
        }//for ports
    }//for adaptors
    XvFreeAdaptorInfo(ai);

    if(!query) mp_msg(MSGT_VO,MSGL_ERR,"vo_xvmc: Could not find free matching surface. Sorry.\n");
    return 0;

// somebody know cleaner way to escape from 3 internal loops?
surface_found:
    XvFreeAdaptorInfo(ai);

    memcpy(surf_info,&mc_surf_list[s],sizeof(XvMCSurfaceInfo));
    mp_msg(MSGT_VO, query?MSGL_INFO:MSGL_DBG3,
                "vo_xvmc: Found matching surface with id=%X on %ld port at %d adapter\n",
                mc_surf_list[s].surface_type_id,p,i);
    return mc_surf_list[s].surface_type_id;
}

static uint32_t xvmc_draw_image(mp_image_t *mpi){
    struct xvmc_render *rndr;

    assert(mpi!=NULL);
    assert(mpi->flags &MP_IMGFLAG_DIRECT);
//   assert(mpi->flags &MP_IMGFLAGS_DRAWBACK);

    rndr = (struct xvmc_render*)mpi->priv; //there is copy in plane[2]
    assert( rndr != NULL );
    assert( rndr->shared.xvmc_id == AV_XVMC_ID );
    mp_msg(MSGT_VO,MSGL_DBG4,"vo_xvmc: draw_image(show rndr=%p)\n",rndr);
// the surface have passed vf system without been skiped, it will be displayed
    add_state(rndr, STATE_DISPLAY_PENDING);
    p_render_surface_to_show = rndr;
    top_field_first = mpi->fields & MP_IMGFIELD_TOP_FIRST;
    return VO_TRUE;
}

static int preinit(const char *arg){
    int xv_version,xv_release,xv_request_base,xv_event_base,xv_error_base;
    int mc_eventBase,mc_errorBase;
    int mc_ver,mc_rev;
    strarg_t ck_src_arg = { 0, NULL };
    strarg_t ck_method_arg = { 0, NULL };
    const opt_t subopts [] =
    {
        /* name         arg type      arg var           test */
        {  "port",      OPT_ARG_INT,  &xv_port_request, int_pos },
        {  "adaptor",   OPT_ARG_INT,  &xv_adaptor,      int_non_neg },
        {  "ck",        OPT_ARG_STR,  &ck_src_arg,      xv_test_ck },
        {  "ck-method", OPT_ARG_STR,  &ck_method_arg,   xv_test_ckm },
        {  "benchmark", OPT_ARG_BOOL, &benchmark,       NULL },
        {  "sleep",     OPT_ARG_BOOL, &use_sleep,       NULL },
        {  "queue",     OPT_ARG_BOOL, &use_queue,       NULL },
        {  "bobdeint",  OPT_ARG_BOOL, &bob_deinterlace, NULL },
        {  NULL }
    };

    //Obtain display handler
    if (!vo_init()) return -1;//vo_xv

    //XvMC is subdivision of XVideo
    if (Success != XvQueryExtension(mDisplay,&xv_version,&xv_release,&xv_request_base,
                                    &xv_event_base,&xv_error_base) ){
        mp_msg(MSGT_VO,MSGL_ERR,"Sorry, Xv(MC) not supported by this X11 version/driver\n");
        mp_msg(MSGT_VO,MSGL_ERR,"********** Try with  -vo x11  or  -vo sdl  ***********\n");
        return -1;
    }
    mp_msg(MSGT_VO,MSGL_INFO,"vo_xvmc: X-Video extension %d.%d\n",xv_version,xv_release);

    if( True != XvMCQueryExtension(mDisplay,&mc_eventBase,&mc_errorBase) ){
        mp_msg(MSGT_VO,MSGL_ERR,"vo_xvmc: No X-Video MotionCompensation Extension on %s\n",
                XDisplayName(NULL));
        return -1;
    }

    if(Success == XvMCQueryVersion(mDisplay, &mc_ver, &mc_rev) ){
        mp_msg(MSGT_VO,MSGL_INFO,"vo_xvmc: X-Video MotionCompensation Extension version %i.%i\n",
                mc_ver,mc_rev);
    }
    else{
        mp_msg(MSGT_VO,MSGL_ERR,"vo_xvmc: Error querying version info!\n");
        return -1;
    }
    surface_render = NULL;
    xv_port = 0;
    number_of_surfaces = 0;
    subpicture_alloc = 0;

    benchmark = 0; //disable PutImageto allow faster display than screen refresh
    use_sleep = 0;
    use_queue = 0;
    bob_deinterlace = 0;

    /* parse suboptions */
    if ( subopt_parse( arg, subopts ) != 0 )
    {
        return -1;
    }

    xv_setup_colorkeyhandling( ck_method_arg.str, ck_src_arg.str );

    return 0;
}

static int config(uint32_t width, uint32_t height,
                       uint32_t d_width, uint32_t d_height,
                       uint32_t flags, char *title, uint32_t format){
    int i,mode_id,rez;
    int numblocks,blocks_per_macroblock;//bpmb we have 6,8,12

//from vo_xv
    XVisualInfo vinfo;
    XSetWindowAttributes xswa;
    XWindowAttributes attribs;
    unsigned long xswamask;
    int depth;
#ifdef CONFIG_XF86VM
    int vm = flags & VOFLAG_MODESWITCHING;
#endif
//end of vo_xv

    if( !IMGFMT_IS_XVMC(format) )
    {
        assert(0);//should never happen, abort on debug or
        return 1;//return error on relese
    }

// Find free port that supports MC, by querying adaptors
    if( xv_port != 0 || number_of_surfaces != 0 ){
        if( height==image_height && width==image_width && image_format==format){
            xvmc_clean_surfaces();
            goto skip_surface_allocation;
        }
        xvmc_free();
    };
    numblocks=((width+15)/16)*((height+15)/16);
// Find Supported Surface Type
    mode_id = xvmc_find_surface_by_format(format,width,height,&surface_info,0);//false=1 to grab port, not query
    if ( mode_id == 0 )
    {
        return -1;
    }

    mp_msg(MSGT_VO,MSGL_DBG4, "vo_xvmc: XvMCCreateContext(mDisplay %p, xv_port=%d, mode_id=0x%08x, width=%d, height=%d, XVMC_DIRECT=%d,ctx=%p)\n",
                            mDisplay, xv_port,mode_id,width,height,XVMC_DIRECT,&ctx);

    rez = XvMCCreateContext(mDisplay, xv_port,mode_id,width,height,XVMC_DIRECT,&ctx);
    if( rez != Success ){
        mp_msg(MSGT_VO,MSGL_ERR,"vo_xvmc: XvMCCreateContext failed with error %d\n",rez);
        return -1;
    }
    if( ctx.flags & XVMC_DIRECT ){
        mp_msg(MSGT_VO,MSGL_INFO,"vo_xvmc: Allocated Direct Context\n");
    }else{
        mp_msg(MSGT_VO,MSGL_INFO,"vo_xvmc: Allocated Indirect Context!\n");
    }


    blocks_per_macroblock = 6;
    if(surface_info.chroma_format == XVMC_CHROMA_FORMAT_422)
        blocks_per_macroblock = 8;
    if(surface_info.chroma_format == XVMC_CHROMA_FORMAT_444)
        blocks_per_macroblock = 12;

    rez = XvMCCreateBlocks(mDisplay,&ctx,numblocks*blocks_per_macroblock,&data_blocks);
    if( rez != Success ){
        XvMCDestroyContext(mDisplay,&ctx);
        return -1;
    }
    mp_msg(MSGT_VO,MSGL_INFO,"vo_xvmc: data_blocks allocated\n");

    rez = XvMCCreateMacroBlocks(mDisplay,&ctx,numblocks,&mv_blocks);
    if( rez != Success ){
        XvMCDestroyBlocks(mDisplay,&data_blocks);
        XvMCDestroyContext(mDisplay,&ctx);
        return -1;
    }
    mp_msg(MSGT_VO,MSGL_INFO,"vo_xvmc: mv_blocks allocated\n");

    if(surface_render==NULL)
        surface_render = malloc(MAX_SURFACES * sizeof(struct xvmc_render)); //easy mem debug
    memset(surface_render, 0, MAX_SURFACES * sizeof(struct xvmc_render));

    for(i=0; i<MAX_SURFACES; i++){
        rez=XvMCCreateSurface(mDisplay,&ctx,&surface_array[i]);
        if( rez != Success )
            break;
        surface_render[i].shared.xvmc_id = AV_XVMC_ID;
        surface_render[i].shared.data_blocks = data_blocks.blocks;
        surface_render[i].shared.mv_blocks = mv_blocks.macro_blocks;
        surface_render[i].shared.allocated_mv_blocks = numblocks;
        surface_render[i].shared.allocated_data_blocks = numblocks*blocks_per_macroblock;
        surface_render[i].shared.idct = (surface_info.mc_type & XVMC_IDCT) == XVMC_IDCT;
        surface_render[i].shared.unsigned_intra = (surface_info.flags & XVMC_INTRA_UNSIGNED) == XVMC_INTRA_UNSIGNED;
        surface_render[i].shared.p_surface = &surface_array[i];
        mp_msg(MSGT_VO,MSGL_DBG4,"vo_xvmc: surface[%d] = %p .rndr=%p\n",
                i,&surface_array[i], &surface_render[i]);
    }
    number_of_surfaces = i;
    if( number_of_surfaces < 4 ){// +2 I or P and +2 for B (to avoid visible motion drawing)
        mp_msg(MSGT_VO,MSGL_ERR,"vo_xvmc: Unable to allocate at least 4 Surfaces\n");
        uninit();
        return -1;
    }
    mp_msg(MSGT_VO,MSGL_INFO,"vo_xvmc: Motion Compensation context allocated - %d surfaces\n",
            number_of_surfaces);

    //debug
    mp_msg(MSGT_VO,MSGL_INFO,"vo_xvmc: idct=%d unsigned_intra=%d\n",
            (surface_info.mc_type & XVMC_IDCT) == XVMC_IDCT,
            (surface_info.flags & XVMC_INTRA_UNSIGNED) == XVMC_INTRA_UNSIGNED);

// Find way to display OSD & subtitle
    mp_msg(MSGT_VO,MSGL_INFO,"vo_xvmc: looking for OSD support\n");
    subpicture_mode = NO_SUBPICTURE;
    if(surface_info.flags & XVMC_OVERLAID_SURFACE)
        subpicture_mode = OVERLAY_SUBPICTURE;

    if(surface_info.subpicture_max_width  != 0 &&
       surface_info.subpicture_max_height != 0  ){
        int s,k,num_subpic;

        XvImageFormatValues * xvfmv;
        xvfmv = XvMCListSubpictureTypes(mDisplay, xv_port,
                        surface_info.surface_type_id, &num_subpic);

        if(num_subpic != 0 && xvfmv != NULL){
            if( mp_msg_test(MSGT_VO,MSGL_DBG4) ){//Print all subpicture types for debug
                for(s=0;s<num_subpic;s++)
                    print_xvimage_format_values(&xvfmv[s]);
            }

            for(s=0;s<num_subpic;s++){
                for(k=0;osd_render[k].draw_func_ptr!=NULL;k++){
                    if(xvfmv[s].id == osd_render[k].id)
                    {
                        init_osd_fnc  = osd_render[k].init_func_ptr;
                        draw_osd_fnc  = osd_render[k].draw_func_ptr;
                        clear_osd_fnc = osd_render[k].clear_func_ptr;

                        subpicture_mode = BLEND_SUBPICTURE;
                        subpicture_info = xvfmv[s];
                        mp_msg(MSGT_VO,MSGL_INFO,"    Subpicture id 0x%08X\n",subpicture_info.id);
                        goto found_subpic;
                    }
                }
            }
found_subpic:
            XFree(xvfmv);
        }
        //Blend2 supicture is always possible, blend1 only at backend
        if( (subpicture_mode == BLEND_SUBPICTURE) &&
            (surface_info.flags & XVMC_BACKEND_SUBPICTURE) )
        {
            subpicture_mode = BACKEND_SUBPICTURE;
        }

    }

    switch(subpicture_mode){
        case NO_SUBPICTURE:
            mp_msg(MSGT_VO,MSGL_WARN,"vo_xvmc: No OSD support for this mode\n");
            break;
        case OVERLAY_SUBPICTURE:
            mp_msg(MSGT_VO,MSGL_WARN,"vo_xvmc: OSD support via color key tricks\n");
            mp_msg(MSGT_VO,MSGL_WARN,"vo_xvmc: not yet implemented:(\n");
            break;
        case BLEND_SUBPICTURE:
            mp_msg(MSGT_VO,MSGL_INFO,"vo_xvmc: OSD support by additional frontend rendering\n");
            break;
        case BACKEND_SUBPICTURE:
            mp_msg(MSGT_VO,MSGL_INFO,"vo_xvmc: OSD support by backend rendering (fast)\n");
            break;
    }

//take keycolor value and choose method for handling it
    if ( !vo_xv_init_colorkey() )
    {
        return -1; // bail out, colorkey setup failed
    }

    vo_xv_enable_vsync();//it won't break anything

//taken from vo_xv
    image_height = height;
    image_width = width;

skip_surface_allocation:

    {
#ifdef CONFIG_XF86VM
        if ( vm )
        {
            vo_vm_switch();
        }
//        else
#endif
        XGetWindowAttributes(mDisplay, DefaultRootWindow(mDisplay), &attribs);
        depth=attribs.depth;
        if (depth != 15 && depth != 16 && depth != 24 && depth != 32) depth = 24;
        XMatchVisualInfo(mDisplay, mScreen, depth, TrueColor, &vinfo);

        xswa.border_pixel     = 0;
        xswamask = CWBorderPixel;
        if (xv_ck_info.method == CK_METHOD_BACKGROUND){
            xswa.background_pixel = xv_colorkey;
            xswamask |= CWBackPixel;
        }

        vo_x11_create_vo_window(&vinfo, vo_dx, vo_dy, d_width, d_height, flags,
                                CopyFromParent, "xvmc", title);
        XChangeWindowAttributes(mDisplay, vo_window, xswamask, &xswa);

#ifdef CONFIG_XF86VM
        if ( vm )
        {
            /* Grab the mouse pointer in our window */
            if(vo_grabpointer)
                XGrabPointer(mDisplay, vo_window, True, 0,
                             GrabModeAsync, GrabModeAsync,
                             vo_window, None, CurrentTime );
            XSetInputFocus(mDisplay, vo_window, RevertToNone, CurrentTime);
        }
#endif
    }

//end vo_xv

    /* store image dimesions for displaying */
    p_render_surface_visible = NULL;
    p_render_surface_to_show = NULL;

    free_element = 0;
    first_frame = 1;

    image_format=format;
    return 0;
}

static void init_osd_yuv_pal(void) {
    char * palette;
    int rez;
    int i,j;
    int snum,seb;
    int Y,U,V;

    subpicture_clear_color = 0;

    if(subpicture.num_palette_entries > 0){

        snum = subpicture.num_palette_entries;
        seb = subpicture.entry_bytes;
        palette = malloc(snum*seb);//check fail
        if(palette == NULL) return;
        for(i=0; i<snum; i++){
            // 0-black max-white the other are gradients
            Y = i*(1 << subpicture_info.y_sample_bits)/snum;//snum=2;->(0),(1*(1<<1)/2)
            U = 1 << (subpicture_info.u_sample_bits - 1);
            V = 1 << (subpicture_info.v_sample_bits - 1);
            for(j=0; j<seb; j++)
                switch(subpicture.component_order[j]){
                    case 'U': palette[i*seb+j] = U; break;
                    case 'V': palette[i*seb+j] = V; break;
                    case 'Y':
                    default:
                              palette[i*seb+j] = Y; break;
                }
        }
        rez = XvMCSetSubpicturePalette(mDisplay, &subpicture, palette);
        if(rez!=Success){
            mp_msg(MSGT_VO,MSGL_ERR,"vo_xvmc: Setting palette failed.\n");
        }
        free(palette);
    }
}

static void clear_osd_subpic(int x0, int y0, int w, int h){
    int rez;

    rez=XvMCClearSubpicture(mDisplay, &subpicture,
                            x0, y0, w,h,
                            subpicture_clear_color);
    if(rez != Success)
        mp_msg(MSGT_VO,MSGL_ERR,"vo_xvmc: XvMCClearSubpicture failed!\n");
}

static void OSD_init(void) {
    unsigned short osd_height, osd_width;
    int rez;

    if(subpicture_alloc){
        mp_msg(MSGT_VO,MSGL_DBG4,"vo_xvmc: destroying subpicture\n");
        XvMCDestroySubpicture(mDisplay,&subpicture);
        deallocate_xvimage();
        subpicture_alloc = 0;
    }

/*   if(surface_info.flags & XVMC_SUBPICTURE_INDEPENDENT_SCALING){
      osd_width = vo_dwidth;
      osd_height = vo_dheight;
   }else*/
    {
        osd_width = image_width;
        osd_height = image_height;
    }

    if(osd_width > surface_info.subpicture_max_width)
        osd_width = surface_info.subpicture_max_width;
    if(osd_height > surface_info.subpicture_max_height)
        osd_height = surface_info.subpicture_max_height;
    if(osd_width == 0 || osd_height == 0)
        return;//if called before window size is known

    mp_msg(MSGT_VO,MSGL_DBG4,"vo_xvmc: creating subpicture (%d,%d) format %X\n",
                osd_width,osd_height,subpicture_info.id);

    rez = XvMCCreateSubpicture(mDisplay,&ctx,&subpicture,
                                osd_width,osd_height,subpicture_info.id);
    if(rez != Success){
        subpicture_mode = NO_SUBPICTURE;
        mp_msg(MSGT_VO,MSGL_WARN,"vo_xvmc: Create Subpicture failed, OSD disabled\n");
        return;
    }
    if( mp_msg_test(MSGT_VO,MSGL_DBG4) ){
        int i;
        mp_msg(MSGT_VO,MSGL_DBG4,"vo_xvmc: Created Subpicture:\n");
        mp_msg(MSGT_VO,MSGL_DBG4,"         xvimage_id=0x%X\n",subpicture.xvimage_id);
        mp_msg(MSGT_VO,MSGL_DBG4,"         width=%d\n",subpicture.width);
        mp_msg(MSGT_VO,MSGL_DBG4,"         height=%d\n",subpicture.height);
        mp_msg(MSGT_VO,MSGL_DBG4,"         num_palette_entries=0x%X\n",subpicture.num_palette_entries);
        mp_msg(MSGT_VO,MSGL_DBG4,"         entry_bytes=0x%X\n",subpicture.entry_bytes);

        mp_msg(MSGT_VO,MSGL_DBG4,"         component_order=\"");
        for(i=0; i<4; i++)
            if(subpicture.component_order[i] >= 32)
                mp_msg(MSGT_VO,MSGL_DBG4,"%c", subpicture.component_order[i]);
        mp_msg(MSGT_VO,MSGL_DBG4,"\"\n");
    }

    //call init for the surface type
    init_osd_fnc();//init palete,clear color etc ...
    mp_msg(MSGT_VO,MSGL_DBG4,"vo_xvmc: clearing subpicture\n");
    clear_osd_fnc(0, 0, subpicture.width, subpicture.height);

    if (allocate_xvimage(subpicture.width, subpicture.height, subpicture_info.id))
    {
        subpicture_mode = NO_SUBPICTURE;
        mp_msg(MSGT_VO,MSGL_WARN, "vo_xvmc: OSD disabled\n");
        return;
    }
    subpicture_alloc = 1;
}

static void draw_osd_IA44(int x0,int y0, int w,int h, unsigned char* src, unsigned char *srca, int stride){
    int ox,oy;
    int rez;

    mp_msg(MSGT_VO,MSGL_DBG4,"vo_xvmc:composite AI44 subpicture (%d,%d - %d,%d)\n",x0,y0,w,h);

    for(ox=0; ox<w; ox++){
        for(oy=0; oy<h; oy++){
            xvimage->data[oy*xvimage->width+ox] = (src[oy*stride+ox]>>4) | ((0-srca[oy*stride+ox])&0xf0);
        }
    }
    rez = XvMCCompositeSubpicture(mDisplay, &subpicture, xvimage, 0, 0,
                                    w,h,x0,y0);
    if(rez != Success){
        mp_msg(MSGT_VO,MSGL_WARN,"vo_xvmc: composite subpicture failed\n");
        assert(0);
    }
}

static void draw_osd_AI44(int x0,int y0, int w,int h, unsigned char* src, unsigned char *srca, int stride){
    int ox,oy;
    int rez;

    mp_msg(MSGT_VO,MSGL_DBG4,"vo_xvmc:composite AI44 subpicture (%d,%d - %d,%d)\n",x0,y0,w,h);

    for(ox=0; ox<w; ox++){
        for(oy=0; oy<h; oy++){
            xvimage->data[oy*xvimage->width+ox] = (src[oy*stride+ox]&0xf0) | (((0-srca[oy*stride+ox])>>4)&0xf);
        }
    }
    rez = XvMCCompositeSubpicture(mDisplay, &subpicture, xvimage, 0, 0,
                                    w,h,x0,y0);
    if(rez != Success){
        mp_msg(MSGT_VO,MSGL_WARN,"vo_xvmc: composite subpicture failed\n");
        assert(0);
    }
}

static void draw_osd(void){
    struct xvmc_render *osd_rndr;
    int osd_has_changed;
    int have_osd_to_draw;
    int rez;

    mp_msg(MSGT_VO,MSGL_DBG4,"vo_xvmc: draw_osd ,OSD_mode=%d, surface_to_show=%p\n",
                subpicture_mode,p_render_surface_to_show);

    if(subpicture_mode == BLEND_SUBPICTURE ||
        subpicture_mode == BACKEND_SUBPICTURE ){

        if(!subpicture_alloc) //allocate subpicture when dimensions are known
            OSD_init();
        if(!subpicture_alloc)
            return;//dimensions still unknown.

        osd_has_changed = vo_update_osd(subpicture.width, subpicture.height);
        have_osd_to_draw = vo_osd_check_range_update(0, 0, subpicture.width,
                                                         subpicture.height);

        if(!have_osd_to_draw)
            return;//nothing to draw,no subpic, no blend

        if(osd_has_changed){
            //vo_remove_text(subpicture.width, subpicture.height,clear_osd_fnc)
            clear_osd_fnc(0,0,subpicture.width,subpicture.height);
            vo_draw_text(subpicture.width, subpicture.height, draw_osd_fnc);
        }
        XvMCSyncSubpicture(mDisplay,&subpicture);//todo usleeep wait!

        if(subpicture_mode == BLEND_SUBPICTURE){
            osd_rndr = find_free_surface();
            if(osd_rndr == NULL)
                return;// no free surface to draw OSD in

            rez = XvMCBlendSubpicture2(mDisplay,
                        p_render_surface_to_show->shared.p_surface, osd_rndr->shared.p_surface,
                        &subpicture,
                        0, 0, subpicture.width, subpicture.height,
                        0, 0, image_width, image_height);
            if(rez!=Success){
                mp_msg(MSGT_VO,MSGL_WARN,"vo_xvmc: BlendSubpicture failed rez=%d\n",rez);
                assert(0);
                return;
            }
//       XvMCFlushSurface(mDisplay,osd_rndr->p_surface);//fixme- should I?

            //When replaceing the surface with osd one, save the flags too!
            osd_rndr->shared.picture_structure = p_render_surface_to_show->shared.picture_structure;
//add more if needed    osd_rndr-> = p_render_surface_to_show->;

            add_state(p_render_surface_to_show, STATE_OSD_SOURCE);
            remove_state(p_render_surface_to_show, STATE_DISPLAY_PENDING);
            p_render_surface_to_show->p_osd_target_surface_render = osd_rndr;

            p_render_surface_to_show = osd_rndr;
            add_state(p_render_surface_to_show, STATE_DISPLAY_PENDING);

            mp_msg(MSGT_VO,MSGL_DBG4,"vo_xvmc:draw_osd: surface_to_show changed to %p\n",osd_rndr);
        }//endof if(BLEND)
        if(subpicture_mode == BACKEND_SUBPICTURE){
            rez = XvMCBlendSubpicture(mDisplay,
                        p_render_surface_to_show->shared.p_surface,
                        &subpicture,
                        0, 0, subpicture.width, subpicture.height,
                        0, 0, image_width, image_height);

        }

    }//if(BLEND||BACKEND)
}

static void xvmc_sync_surface(XvMCSurface * srf){
    int status,rez;

    rez = XvMCGetSurfaceStatus(mDisplay,srf,&status);
    assert(rez==Success);
    if((status & XVMC_RENDERING) == 0)
        return;//surface is already complete
    if(use_sleep){
        rez = XvMCFlushSurface(mDisplay, srf);
        assert(rez==Success);

        do{
            usec_sleep(1000);//1ms (may be 20ms on linux)
            XvMCGetSurfaceStatus(mDisplay,srf,&status);
        } while (status & XVMC_RENDERING);
        return;//done
    }

    XvMCSyncSurface(mDisplay, srf);
}

static void put_xvmc_image(struct xvmc_render *p_render_surface,
                           int draw_ck){
    int rez;
    struct vo_rect src_rect, dst_rect;
    int i;

    if(p_render_surface == NULL)
        return;

    calc_src_dst_rects(image_width, image_height, &src_rect, &dst_rect, NULL, NULL);

    if(draw_ck)
        vo_xv_draw_colorkey(dst_rect.left, dst_rect.top, dst_rect.width, dst_rect.height);

    if(benchmark)
        return;

    for (i = 1; i <= bob_deinterlace + 1; i++) {
        int field = top_field_first ? i : i ^ 3;
        rez = XvMCPutSurface(mDisplay, p_render_surface->shared.p_surface,
                            vo_window,
                            src_rect.left, src_rect.top, src_rect.width, src_rect.height,
                            dst_rect.left, dst_rect.top, dst_rect.width, dst_rect.height,
                            bob_deinterlace ? field : 3);
        if(rez != Success){
            mp_msg(MSGT_VO,MSGL_ERR,"vo_xvmc: PutSurface failer, critical error %d!\n",rez);
            assert(0);
        }
    }
    XFlush(mDisplay);
}

static void flip_page(void){
    int i,cfs;

    mp_msg(MSGT_VO,MSGL_DBG4,"vo_xvmc: flip_page  show(rndr=%p)\n\n",p_render_surface_to_show);

    if(p_render_surface_to_show == NULL) return;
    assert( p_render_surface_to_show->shared.xvmc_id == AV_XVMC_ID );
//fixme   assert( p_render_surface_to_show != p_render_surface_visible);

    if(use_queue){
        // fill the queue until only n free surfaces remain
        // after that start displaying
        cfs = count_free_surfaces();
        show_queue[free_element++] = p_render_surface_to_show;
        if(cfs > 3){//well have 3 free surfaces after add queue
            if(free_element > 1)//a little voodoo magic
                xvmc_sync_surface(show_queue[0]->shared.p_surface);
            return;
        }
        p_render_surface_to_show=show_queue[0];
        mp_msg(MSGT_VO,MSGL_DBG5,"vo_xvmc: flip_queue free_element=%d\n",free_element);
        free_element--;
        for(i=0; i<free_element; i++){
            show_queue[i] = show_queue[i+1];
        }
        show_queue[free_element] = NULL;
    }

// make sure the rendering is done
    xvmc_sync_surface(p_render_surface_to_show->shared.p_surface);

//the visible surface won't be displayed anymore, mark it as free
    if(p_render_surface_visible != NULL)
        remove_state(p_render_surface_visible, STATE_DISPLAY_PENDING);

//!!fixme   assert(p_render_surface_to_show->state & STATE_DISPLAY_PENDING);

    //show it, displaying is always vsynced, so skip it for benchmark
    put_xvmc_image(p_render_surface_to_show,first_frame);
    first_frame=0;//make sure we won't draw it anymore

    p_render_surface_visible = p_render_surface_to_show;
    p_render_surface_to_show = NULL;
}

static void check_events(void){
    int e=vo_x11_check_events(mDisplay);

    if(e&VO_EVENT_RESIZE)
    {
        e |= VO_EVENT_EXPOSE;
    }
    if ( e & VO_EVENT_EXPOSE )
    {
        put_xvmc_image(p_render_surface_visible,1);
    }
}

static void xvmc_free(void){
    int i;

    if( subpicture_alloc ){

        XvMCDestroySubpicture(mDisplay,&subpicture);
        deallocate_xvimage();

        subpicture_alloc = 0;

        mp_msg(MSGT_VO,MSGL_DBG4,"vo_xvmc: subpicture destroyed\n");
    }

    if( number_of_surfaces ){

        XvMCDestroyMacroBlocks(mDisplay,&mv_blocks);
        XvMCDestroyBlocks(mDisplay,&data_blocks);

        for(i=0; i<number_of_surfaces; i++)
        {
            XvMCHideSurface(mDisplay,&surface_array[i]);//it doesn't hurt, I hope
            XvMCDestroySurface(mDisplay,&surface_array[i]);

            if( (surface_render[i].state != 0) &&
                (p_render_surface_visible != &surface_render[i]) )
                mp_msg(MSGT_VO,MSGL_INFO,"vo_xvmc::uninit surface_render[%d].status=%d\n",i,
                        surface_render[i].state);
        }

        memset(surface_render, 0, MAX_SURFACES * sizeof(struct xvmc_render)); //for debugging
        free(surface_render);surface_render=NULL;

        XvMCDestroyContext(mDisplay,&ctx);
        number_of_surfaces = 0;

        mp_msg(MSGT_VO,MSGL_DBG4,"vo_xvmc: Context sucessfuly freed\n");
    }


    if( xv_port !=0 ){
        XvUngrabPort(mDisplay,xv_port,CurrentTime);
        xv_port = 0;
        mp_msg(MSGT_VO,MSGL_DBG4,"vo_xvmc: xv_port sucessfuly ungrabed\n");
    }
}

static void uninit(void){
    mp_msg(MSGT_VO,MSGL_DBG4,"vo_xvmc: uninit called\n");
    xvmc_free();
 //from vo_xv
#ifdef CONFIG_XF86VM
    vo_vm_close();
#endif
    vo_x11_uninit();
}

static int query_format(uint32_t format){
    uint32_t flags;
    XvMCSurfaceInfo qsurface_info;
    int mode_id;

    mp_msg(MSGT_VO,MSGL_DBG4,"vo_xvmc: query_format=%X\n",format);

    if(!IMGFMT_IS_XVMC(format)) return 0;// no caps supported
    mode_id = xvmc_find_surface_by_format(format, 16, 16, &qsurface_info, 1);//true=1 - querying

    if( mode_id == 0 ) return 0;

    flags = VFCAP_CSP_SUPPORTED |
            VFCAP_CSP_SUPPORTED_BY_HW |
            VFCAP_ACCEPT_STRIDE;

    if( (qsurface_info.subpicture_max_width  != 0) &&
        (qsurface_info.subpicture_max_height != 0) )
        flags|=VFCAP_OSD;
    return flags;
}


static int draw_slice(uint8_t *image[], int stride[],
                           int w, int h, int x, int y){
    struct xvmc_render *rndr;
    int rez;

    mp_msg(MSGT_VO,MSGL_DBG4,"vo_xvmc: draw_slice y=%d\n",y);

    rndr = (struct xvmc_render*)image[2]; //this is copy of priv-ate
    assert( rndr != NULL );
    assert( rndr->shared.xvmc_id == AV_XVMC_ID );

    rez = XvMCRenderSurface(mDisplay,&ctx,rndr->shared.picture_structure,
                            rndr->shared.p_surface,
                            rndr->shared.p_past_surface,
                            rndr->shared.p_future_surface,
                            rndr->shared.flags,
                            rndr->shared.filled_mv_blocks_num,rndr->shared.start_mv_blocks_num,
                            &mv_blocks,&data_blocks);
    if(rez != Success)
    {
    int i;
        mp_msg(MSGT_VO,MSGL_ERR,"vo_xvmc::slice: RenderSirface returned %d\n",rez);

        mp_msg(MSGT_VO,MSGL_ERR,"vo_xvmc::slice: pict=%d,flags=%x,start_blocks=%d,num_blocks=%d\n",
                rndr->shared.picture_structure,rndr->shared.flags,rndr->shared.start_mv_blocks_num,
                rndr->shared.filled_mv_blocks_num);
        mp_msg(MSGT_VO,MSGL_ERR,"vo_xvmc::slice: this_surf=%p, past_surf=%p, future_surf=%p\n",
                rndr->shared.p_surface,rndr->shared.p_past_surface,rndr->shared.p_future_surface);

        for(i=0; i<rndr->shared.filled_mv_blocks_num; i++){
            XvMCMacroBlock* testblock;
            testblock = &mv_blocks.macro_blocks[i];

            mp_msg(MSGT_VO,MSGL_ERR,"vo_xvmc::slice: mv_block - x=%d,y=%d,mb_type=0x%x,mv_type=0x%x,mv_field_select=%d\n",
                    testblock->x,testblock->y,testblock->macroblock_type,
                    testblock->motion_type,testblock->motion_vertical_field_select);
            mp_msg(MSGT_VO,MSGL_ERR,"vo_xvmc::slice: dct_type=%d,data_index=0x%x,cbp=%d,pad0=%d\n",
                    testblock->dct_type,testblock->index,testblock->coded_block_pattern,
                    testblock->pad0);
            mp_msg(MSGT_VO,MSGL_ERR,"vo_xvmc::slice: PMV[0][0][0/1]=(%d,%d)\n",
                    testblock->PMV[0][0][0],testblock->PMV[0][0][1]);
        }
    }
    assert(rez==Success);
    mp_msg(MSGT_VO,MSGL_DBG4,"vo_xvmc: flush surface\n");
    rez = XvMCFlushSurface(mDisplay, rndr->shared.p_surface);
    assert(rez==Success);

//   rndr->start_mv_blocks_num += rndr->filled_mv_blocks_num;
    rndr->shared.start_mv_blocks_num = 0;
    rndr->shared.filled_mv_blocks_num = 0;

    rndr->shared.next_free_data_block_num = 0;

    return VO_TRUE;
}

//XvMCHide hides the surface on next retrace, so
//check if the surface is not still displaying
static void check_osd_source(struct xvmc_render *src_rndr) {
    struct xvmc_render *osd_rndr;
    int stat;

    //If this is source surface, check does the OSD rendering is compleate
    if(src_rndr->state & STATE_OSD_SOURCE){
        mp_msg(MSGT_VO,MSGL_DBG4,"vo_xvmc: OSD surface=%p querying\n",src_rndr);
        osd_rndr = src_rndr->p_osd_target_surface_render;
        XvMCGetSurfaceStatus(mDisplay, osd_rndr->shared.p_surface, &stat);
        if(!(stat & XVMC_RENDERING))
            remove_state(src_rndr, STATE_OSD_SOURCE);
    }
}
static int count_free_surfaces(void) {
    int i,num;

    num=0;
    for(i=0; i<number_of_surfaces; i++){
        check_osd_source(&surface_render[i]);
        if(!in_use(surface_render + i))
            num++;
    }
    return num;
}

static struct xvmc_render *find_free_surface(void) {
    int i,t;
    int stat;
    struct xvmc_render *visible_rndr;

    visible_rndr = NULL;
    for(i=0; i<number_of_surfaces; i++){

        check_osd_source(&surface_render[i]);
        if( !in_use(surface_render + i)){
            XvMCGetSurfaceStatus(mDisplay, surface_render[i].shared.p_surface,&stat);
            if( (stat & XVMC_DISPLAYING) == 0 )
                return &surface_render[i];
            visible_rndr = &surface_render[i];// remember it, use as last resort
        }
    }

    //all surfaces are busy, but there is one that will be free
    //on next monitor retrace, we just have to wait
    if(visible_rndr != NULL){
        mp_msg(MSGT_VO,MSGL_INFO,"vo_xvmc: waiting retrace\n");
        for(t=0;t<1000;t++){
            usec_sleep(1000);//1ms
            XvMCGetSurfaceStatus(mDisplay, visible_rndr->shared.p_surface,&stat);
            if( (stat & XVMC_DISPLAYING) == 0 )
                return visible_rndr;
        }
    }
//todo remove when stable
    mp_msg(MSGT_VO,MSGL_WARN,"vo_xvmc: no free surfaces, this should not happen in g1\n");
    for(i=0;i<number_of_surfaces;i++)
        mp_msg(MSGT_VO,MSGL_WARN,"vo_xvmc: surface[%d].state=%d\n",i,surface_render[i].state);
    return NULL;
}

static void xvmc_clean_surfaces(void){
    int i;

    for(i=0; i<number_of_surfaces; i++){

        remove_state(surface_render + i, STATE_DISPLAY_PENDING | STATE_OSD_SOURCE);
        surface_render[i].p_osd_target_surface_render=NULL;
        if(surface_render[i].state != 0){
            mp_msg(MSGT_VO,MSGL_WARN,"vo_xvmc: surface[%d].state=%d\n",
                                   i,surface_render[i].state);
        }
    }
    free_element=0;//clean up the queue
}

static uint32_t get_image(mp_image_t *mpi){
    struct xvmc_render *rndr;

    rndr = find_free_surface();

    if(rndr == NULL){
        mp_msg(MSGT_VO,MSGL_ERR,"vo_xvmc: get_image failed\n");
        return VO_FALSE;
    }

assert(rndr->shared.start_mv_blocks_num == 0);
assert(rndr->shared.filled_mv_blocks_num == 0);
assert(rndr->shared.next_free_data_block_num == 0);

    mpi->flags |= MP_IMGFLAG_DIRECT;
//keep strides 0 to avoid field manipulations
    mpi->stride[0] = 0;
    mpi->stride[1] = 0;
    mpi->stride[2] = 0;

// these are shared!! so watch out
// do call RenderSurface before overwriting
    mpi->planes[0] = (char*)data_blocks.blocks;
    mpi->planes[1] = (char*)mv_blocks.macro_blocks;
    mpi->priv =
    mpi->planes[2] = (char*)rndr;

    rndr->shared.picture_structure = 0;
    rndr->shared.flags = 0;
    rndr->state = 0;
    rndr->mpi = mpi;
    rndr->shared.start_mv_blocks_num = 0;
    rndr->shared.filled_mv_blocks_num = 0;
    rndr->shared.next_free_data_block_num = 0;

    mp_msg(MSGT_VO,MSGL_DBG4,"vo_xvmc: get_image: rndr=%p (surface=%p) \n",
                rndr,rndr->shared.p_surface);
return VO_TRUE;
}

static int control(uint32_t request, void *data)
{
    switch (request){
        case VOCTRL_GET_DEINTERLACE:
            *(int*)data = bob_deinterlace;
            return VO_TRUE;
        case VOCTRL_SET_DEINTERLACE:
            bob_deinterlace = *(int*)data;
            return VO_TRUE;
        case VOCTRL_QUERY_FORMAT:
            return query_format(*((uint32_t*)data));
        case VOCTRL_DRAW_IMAGE:
            return xvmc_draw_image((mp_image_t *)data);
        case VOCTRL_GET_IMAGE:
            return get_image((mp_image_t *)data);
        //vo_xv
        case VOCTRL_GUISUPPORT:
            return VO_TRUE;
        case VOCTRL_ONTOP:
            vo_x11_ontop();
            return VO_TRUE;
        case VOCTRL_FULLSCREEN:
            vo_x11_fullscreen();
        // indended, fallthrough to update panscan on fullscreen/windowed switch
        case VOCTRL_SET_PANSCAN:
            if ( ( vo_fs && ( vo_panscan != vo_panscan_amount ) ) || ( !vo_fs && vo_panscan_amount ) )
            {
                int old_y = vo_panscan_y;
                panscan_calc();

                if(old_y != vo_panscan_y)
                {
                    //this also draws the colorkey
                    put_xvmc_image(p_render_surface_visible,1);
                }
            }
            return VO_TRUE;
        case VOCTRL_GET_PANSCAN:
            if ( !vo_config_count || !vo_fs ) return VO_FALSE;
            return VO_TRUE;
        case VOCTRL_SET_EQUALIZER:
        {
            vf_equalizer_t *eq=data;
            return vo_xv_set_eq(xv_port, eq->item, eq->value);
        }

        case VOCTRL_GET_EQUALIZER:
        {
            vf_equalizer_t *eq=data;
            return vo_xv_get_eq(xv_port, eq->item, &eq->value);
        }
        case VOCTRL_UPDATE_SCREENINFO:
            update_xinerama_info();
            return VO_TRUE;
    }
    return VO_NOTIMPL;
}
