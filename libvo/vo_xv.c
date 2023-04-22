/*
 * X11 Xv interface
 *
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

// Number of buffers _FOR_DOUBLEBUFFERING_MODE_
// Use option -double to enable double buffering! (default: single buffer)
#define NUM_BUFFERS 3

/*
Buffer allocation:

-nodr:
  1: TEMP
  2: 2*TEMP

-dr:
  1: TEMP
  3: 2*STATIC+TEMP
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "mp_msg.h"
#include "help_mp.h"
#include "video_out.h"
#define NO_DRAW_FRAME
#include "video_out_internal.h"
#include "libmpcodecs/vf.h"


#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <errno.h>

#include "x11_common.h"

#include "fastmemcpy.h"
#include "sub/sub.h"
#include "aspect.h"

#include "subopt-helper.h"

#include "input/input.h"

#include "libavutil/common.h"

static const vo_info_t info = {
    "X11/Xv",
    "xv",
    "Gerd Knorr <kraxel@goldbach.in-berlin.de> and others",
    ""
};

const LIBVO_EXTERN(xv)
#ifdef HAVE_SHM
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>

static XShmSegmentInfo Shminfo[NUM_BUFFERS];
static int Shmem_Flag;
#endif

// Note: depends on the inclusion of X11/extensions/XShm.h
#include <X11/extensions/Xv.h>
#include <X11/extensions/Xvlib.h>

// FIXME: dynamically allocate this stuff
static void allocate_xvimage(int);
static unsigned int ver, rel, req, ev, err;
static unsigned int formats, adaptors, xv_format;
static XvAdaptorInfo *ai = NULL;
static XvImageFormatValues *fo=NULL;

static int current_buf = 0;
static int current_ip_buf = 0;
static int num_buffers = 1;     // default
static int visible_buf = -1;    // -1 means: no buffer was drawn yet
static XvImage *xvimage[NUM_BUFFERS];


static uint32_t image_width;
static uint32_t image_height;
static uint32_t image_format;

static int int_pause;

static struct vo_rect src_rect;
static struct vo_rect dst_rect;
static uint32_t max_width = 0, max_height = 0; // zero means: not set

static vo_draw_alpha_func draw_alpha_func;

static void fixup_osd_position(int *x0, int *y0, int *w, int *h)
{
    *x0 += image_width * (vo_panscan_x >> 1) / (vo_dwidth + vo_panscan_x);
    *w = av_clip(*w, 0, image_width);
    *h = av_clip(*h, 0, image_height);
    *x0 = FFMIN(*x0, image_width  - *w);
    *y0 = FFMIN(*y0, image_height - *h);
}

static void draw_alpha(int x0, int y0, int w, int h,
                            unsigned char *src, unsigned char *srca,
                            int stride)
{
    if (!draw_alpha_func) return;
    fixup_osd_position(&x0, &y0, &w, &h);
    x0 *= pixel_stride(xv_format);
    draw_alpha_func(w, h, src, srca, stride,
                       xvimage[current_buf]->data +
                       xvimage[current_buf]->offsets[0] +
                       xvimage[current_buf]->pitches[0] * y0 + x0,
                       xvimage[current_buf]->pitches[0]);
}


static void deallocate_xvimage(int foo);

static void resize(void)
{
    calc_src_dst_rects(image_width, image_height, &src_rect, &dst_rect, NULL, NULL);
//    vo_x11_clearwindow_part(mDisplay, vo_window, dst_rect.width, dst_rect.height, vo_fs);
    vo_xv_draw_colorkey(dst_rect.left, dst_rect.top, dst_rect.width, dst_rect.height);
}

/*
 * connect to server, create and map window,
 * allocate colors and (shared) memory
 */
static int config(uint32_t width, uint32_t height, uint32_t d_width,
                       uint32_t d_height, uint32_t flags, char *title,
                       uint32_t format)
{
    XVisualInfo vinfo;
    XSetWindowAttributes xswa;
    XWindowAttributes attribs;
    unsigned long xswamask;
    int depth;

#ifdef CONFIG_XF86VM
    int vm = flags & VOFLAG_MODESWITCHING;
#endif

    image_height = height;
    image_width = width;
    image_format = format;

    if ((max_width != 0 && max_height != 0) &&
        (image_width > max_width || image_height > max_height))
    {
        mp_msg( MSGT_VO, MSGL_ERR, MSGTR_VO_XV_ImagedimTooHigh,
                image_width, image_height, max_width, max_height);
        return -1;
    }

    int_pause = 0;
    visible_buf = -1;

    num_buffers =
        vo_doublebuffering ? (vo_directrendering ? NUM_BUFFERS : 2) : 1;

    /* check image formats */
    {
        unsigned int i;

        xv_format = 0;
        for (i = 0; i < formats; i++)
        {
            mp_msg(MSGT_VO, MSGL_V,
                   "Xvideo image format: 0x%x (%4.4s) %s\n", fo[i].id,
                   (char *) &fo[i].id,
                   (fo[i].format == XvPacked) ? "packed" : "planar");
            if (fo[i].id == format)
                xv_format = fo[i].id;
        }
        if (!xv_format)
            return -1;
    }

    {
#ifdef CONFIG_XF86VM
        if (vm)
        {
            vo_vm_switch();
        }
#endif
        XGetWindowAttributes(mDisplay, DefaultRootWindow(mDisplay),
                             &attribs);
        depth = attribs.depth;
        if (depth != 15 && depth != 16 && depth != 24 && depth != 32)
            depth = 24;
        XMatchVisualInfo(mDisplay, mScreen, depth, TrueColor, &vinfo);

        xswa.border_pixel = 0;
        xswamask = CWBorderPixel;
        if (xv_ck_info.method == CK_METHOD_BACKGROUND)
        {
          xswa.background_pixel = xv_colorkey;
          xswamask |= CWBackPixel;
        }

            vo_x11_create_vo_window(&vinfo, vo_dx, vo_dy, vo_dwidth, vo_dheight,
                   flags, CopyFromParent, "xv", title);
            XChangeWindowAttributes(mDisplay, vo_window, xswamask, &xswa);

#ifdef CONFIG_XF86VM
        if (vm)
        {
            /* Grab the mouse pointer in our window */
            if (vo_grabpointer)
                XGrabPointer(mDisplay, vo_window, True, 0,
                             GrabModeAsync, GrabModeAsync,
                             vo_window, None, CurrentTime);
            XSetInputFocus(mDisplay, vo_window, RevertToNone, CurrentTime);
        }
#endif
    }

    mp_msg(MSGT_VO, MSGL_V, "using Xvideo port %d for hw scaling\n",
           xv_port);

    draw_alpha_func = vo_get_draw_alpha(xv_format);

    if (vo_config_count)
        for (current_buf = 0; current_buf < num_buffers; ++current_buf)
            deallocate_xvimage(current_buf);

    for (current_buf = 0; current_buf < num_buffers; ++current_buf)
        allocate_xvimage(current_buf);

    current_buf = 0;
    current_ip_buf = 0;

    resize();

    return 0;
}

static void allocate_xvimage(int foo)
{
    /*
     * allocate XvImages.  FIXME: no error checking, without
     * mit-shm this will bomb... trzing to fix ::atmos
     */
#ifdef HAVE_SHM
    if (mLocalDisplay && XShmQueryExtension(mDisplay))
        Shmem_Flag = 1;
    else
    {
        Shmem_Flag = 0;
        mp_msg(MSGT_VO, MSGL_INFO,
               MSGTR_LIBVO_XV_SharedMemoryNotSupported);
    }
    if (Shmem_Flag)
    {
        xvimage[foo] =
            (XvImage *) XvShmCreateImage(mDisplay, xv_port, xv_format,
                                         NULL, image_width, image_height,
                                         &Shminfo[foo]);

        Shminfo[foo].shmid =
            shmget(IPC_PRIVATE, xvimage[foo]->data_size, IPC_CREAT | SHM_R | SHM_W);
        Shminfo[foo].shmaddr = (char *) shmat(Shminfo[foo].shmid, 0, 0);
        Shminfo[foo].readOnly = False;

        xvimage[foo]->data = Shminfo[foo].shmaddr;
        XShmAttach(mDisplay, &Shminfo[foo]);
        XSync(mDisplay, False);
        shmctl(Shminfo[foo].shmid, IPC_RMID, 0);
    } else
#endif
    {
        xvimage[foo] =
            (XvImage *) XvCreateImage(mDisplay, xv_port, xv_format, NULL,
                                      image_width, image_height);
        xvimage[foo]->data = malloc(xvimage[foo]->data_size);
        XSync(mDisplay, False);
    }
    memset(xvimage[foo]->data, 128, xvimage[foo]->data_size);
    return;
}

static void deallocate_xvimage(int foo)
{
#ifdef HAVE_SHM
    if (Shmem_Flag)
    {
        XShmDetach(mDisplay, &Shminfo[foo]);
        shmdt(Shminfo[foo].shmaddr);
    } else
#endif
    {
        free(xvimage[foo]->data);
        xvimage[foo]->data = NULL;
    }
    XFree(xvimage[foo]);
    xvimage[foo] = NULL;

    XSync(mDisplay, False);
    return;
}

static inline void put_xvimage( XvImage * xvi )
{
#ifdef HAVE_SHM
    if (Shmem_Flag)
    {
        XvShmPutImage(mDisplay, xv_port, vo_window, vo_gc,
                      xvi,
                      src_rect.left, src_rect.top, src_rect.width, src_rect.height,
                      dst_rect.left, dst_rect.top, dst_rect.width, dst_rect.height,
                      False);
    } else
#endif
    {
        XvPutImage(mDisplay, xv_port, vo_window, vo_gc,
                   xvi,
                   src_rect.left, src_rect.top, src_rect.width, src_rect.height,
                   dst_rect.left, dst_rect.top, dst_rect.width, dst_rect.height);
    }
}

static void check_events(void)
{
    int e = vo_x11_check_events(mDisplay);

    if (e & VO_EVENT_EXPOSE || e & VO_EVENT_RESIZE)
    {
        resize();
    }

    if ((e & VO_EVENT_EXPOSE || e & VO_EVENT_RESIZE) && int_pause)
    {
        /* did we already draw a buffer */
        if ( visible_buf != -1 )
        {
          /* redraw the last visible buffer */
          put_xvimage( xvimage[visible_buf] );
        }
    }
}

static void draw_osd(void)
{
    vo_draw_text(image_width -
                 image_width * vo_panscan_x / (vo_dwidth + vo_panscan_x),
                 image_height, draw_alpha);
}

static void flip_page(void)
{
    put_xvimage( xvimage[current_buf] );

    /* remember the currently visible buffer */
    visible_buf = current_buf;

    if (num_buffers > 1)
    {
        current_buf =
            vo_directrendering ? 0 : ((current_buf + 1) % num_buffers);
        XFlush(mDisplay);
    } else
        XSync(mDisplay, False);
    return;
}

static int draw_slice(uint8_t * image[], int stride[], int w, int h,
                           int x, int y)
{
    uint8_t *dst;
    int idx_p1 = 1, idx_p2 = 2;

    dst = xvimage[current_buf]->data + xvimage[current_buf]->offsets[0] +
        xvimage[current_buf]->pitches[0] * y + x;
    memcpy_pic(dst, image[0], w, h, xvimage[current_buf]->pitches[0],
               stride[0]);

    switch (image_format) {
    case IMGFMT_YV12:
        idx_p1 = 2; idx_p2 = 1;
    case IMGFMT_I420:
    case IMGFMT_IYUV:
        x /= 2;
        y /= 2;
        w /= 2;
        h /= 2;

        dst = xvimage[current_buf]->data + xvimage[current_buf]->offsets[1] +
            xvimage[current_buf]->pitches[1] * y + x;
        memcpy_pic(dst, image[idx_p1], w, h, xvimage[current_buf]->pitches[1],
            stride[idx_p1]);

        dst = xvimage[current_buf]->data + xvimage[current_buf]->offsets[2] +
            xvimage[current_buf]->pitches[2] * y + x;
        memcpy_pic(dst, image[idx_p2], w, h, xvimage[current_buf]->pitches[2],
            stride[idx_p2]);
        break;
    case IMGFMT_NV12:
    case IMGFMT_NV21:
        x &= ~1;
        y /= 2;
        w &= ~1;
        h /= 2;

        dst = xvimage[current_buf]->data + xvimage[current_buf]->offsets[1] +
            xvimage[current_buf]->pitches[1] * y + x;
        memcpy_pic(dst, image[idx_p1], w, h, xvimage[current_buf]->pitches[1],
            stride[idx_p1]);
        break;
    }

    return 0;
}

static uint32_t draw_image(mp_image_t * mpi)
{
    if (mpi->flags & MP_IMGFLAG_DIRECT)
    {
        // direct rendering:
        current_buf = (intptr_t)mpi->priv;        // hack!
        return VO_TRUE;
    }
    if (mpi->flags & MP_IMGFLAG_DRAW_CALLBACK)
        return VO_TRUE;         // done
    if (mpi->flags & MP_IMGFLAG_PLANAR)
    {
        draw_slice(mpi->planes, mpi->stride, mpi->w, mpi->h, 0, 0);
        return VO_TRUE;
    }
    if (mpi->flags & MP_IMGFLAG_YUV)
    {
        // packed YUV:
        memcpy_pic(xvimage[current_buf]->data +
                   xvimage[current_buf]->offsets[0], mpi->planes[0],
                   mpi->w * (mpi->bpp / 8), mpi->h,
                   xvimage[current_buf]->pitches[0], mpi->stride[0]);
        return VO_TRUE;
    }
    return VO_FALSE;            // not (yet) supported
}

static uint32_t get_image(mp_image_t * mpi)
{
    int buf = current_buf;      // we shouldn't change current_buf unless we do DR!

    if (mpi->type == MP_IMGTYPE_STATIC && num_buffers > 1)
        return VO_FALSE;        // it is not static
    if (mpi->imgfmt != image_format)
        return VO_FALSE;        // needs conversion :(
//    if(mpi->flags&MP_IMGFLAG_READABLE) return VO_FALSE; // slow video ram
    if (mpi->flags & MP_IMGFLAG_READABLE &&
        (mpi->type == MP_IMGTYPE_IPB || mpi->type == MP_IMGTYPE_IP))
    {
        // reference (I/P) frame of IP or IPB:
        if (num_buffers < 2)
            return VO_FALSE;    // not enough
        current_ip_buf ^= 1;
        // for IPB with 2 buffers we can DR only one of the 2 P frames:
        if (mpi->type == MP_IMGTYPE_IPB && num_buffers < 3
            && current_ip_buf)
            return VO_FALSE;
        buf = current_ip_buf;
        if (mpi->type == MP_IMGTYPE_IPB)
            ++buf;              // preserve space for B
    }
    if (mpi->height > xvimage[buf]->height)
        return VO_FALSE;        //buffer to small
    if (mpi->width * (mpi->bpp / 8) > xvimage[buf]->pitches[0])
        return VO_FALSE;        //buffer to small
    if ((mpi->flags & (MP_IMGFLAG_ACCEPT_STRIDE | MP_IMGFLAG_ACCEPT_WIDTH))
        || (mpi->width * (mpi->bpp / 8) == xvimage[buf]->pitches[0]))
    {
        int idx_p1 = 1, idx_p2 = 2;
        current_buf = buf;
        mpi->planes[0] =
            xvimage[current_buf]->data + xvimage[current_buf]->offsets[0];
        mpi->stride[0] = xvimage[current_buf]->pitches[0];
        mpi->width = mpi->stride[0] / (mpi->bpp / 8);
        switch (image_format) {
        case IMGFMT_YV12:
            idx_p1 = 2; idx_p2 = 1;
        case IMGFMT_I420:
        case IMGFMT_IYUV:
            mpi->planes[1] =
                xvimage[current_buf]->data +
                xvimage[current_buf]->offsets[idx_p1];
            mpi->planes[2] =
                xvimage[current_buf]->data +
                xvimage[current_buf]->offsets[idx_p2];
            mpi->stride[1] = xvimage[current_buf]->pitches[idx_p1];
            mpi->stride[2] = xvimage[current_buf]->pitches[idx_p2];
            break;
        case IMGFMT_NV12:
        case IMGFMT_NV21:
            mpi->planes[1] =
                xvimage[current_buf]->data +
                xvimage[current_buf]->offsets[1];
            mpi->stride[1] = xvimage[current_buf]->pitches[1];
            break;
        }
        mpi->flags |= MP_IMGFLAG_DIRECT;
        mpi->priv = (void *)(intptr_t)current_buf;
//      printf("mga: get_image() SUCCESS -> Direct Rendering ENABLED\n");
        return VO_TRUE;
    }
    return VO_FALSE;
}

static int is_supported_format(uint32_t format)
{
    switch (format) {
    case IMGFMT_YV12:
    case IMGFMT_I420:
    case IMGFMT_IYUV:
    case IMGFMT_NV12:
    case IMGFMT_NV21:
        return 1;
    default:
        return !mp_get_chroma_shift(image_format, NULL, NULL, NULL);
    }
}

static int query_format(uint32_t format)
{
    uint32_t i;
    int flag = VFCAP_CSP_SUPPORTED | VFCAP_CSP_SUPPORTED_BY_HW | VFCAP_HWSCALE_UP | VFCAP_HWSCALE_DOWN | VFCAP_OSD | VFCAP_ACCEPT_STRIDE;       // FIXME! check for DOWN
    if (!is_supported_format(format)) {
        return 0;
    }

    /* check image formats */
    for (i = 0; i < formats; i++)
    {
        if (fo[i].id == format)
            return flag;        //xv_format = fo[i].id;
    }
    return 0;
}

static void uninit(void)
{
    int i;

    if (!vo_config_count)
        return;
    visible_buf = -1;
    XvFreeAdaptorInfo(ai);
    ai = NULL;
    if(fo){
        XFree(fo);
        fo=NULL;
    }
    for (i = 0; i < num_buffers; i++)
        deallocate_xvimage(i);
#ifdef CONFIG_XF86VM
    vo_vm_close();
#endif
    mp_input_rm_event_fd(ConnectionNumber(mDisplay));
    vo_x11_uninit();
}

static int preinit(const char *arg)
{
    XvPortID xv_p;
    int busy_ports = 0;
    unsigned int i;
    strarg_t ck_src_arg = { 0, NULL };
    strarg_t ck_method_arg = { 0, NULL };
    int xv_adaptor = -1;

    const opt_t subopts[] =
    {
      /* name         arg type     arg var         test */
      {  "port",      OPT_ARG_INT, &xv_port,       int_pos },
      {  "adaptor",   OPT_ARG_INT, &xv_adaptor,    int_non_neg },
      {  "ck",        OPT_ARG_STR, &ck_src_arg,    xv_test_ck },
      {  "ck-method", OPT_ARG_STR, &ck_method_arg, xv_test_ckm },
      {  NULL }
    };

    xv_port = 0;

    /* parse suboptions */
    if ( subopt_parse( arg, subopts ) != 0 )
    {
      return -1;
    }

    /* modify colorkey settings according to the given options */
    xv_setup_colorkeyhandling( ck_method_arg.str, ck_src_arg.str );

    if (!vo_init())
        return -1;

    /* check for Xvideo extension */
    if (Success != XvQueryExtension(mDisplay, &ver, &rel, &req, &ev, &err))
    {
        mp_msg(MSGT_VO, MSGL_ERR,
               MSGTR_LIBVO_XV_XvNotSupportedByX11);
        return -1;
    }

    /* check for Xvideo support */
    if (Success !=
        XvQueryAdaptors(mDisplay, DefaultRootWindow(mDisplay), &adaptors,
                        &ai))
    {
        mp_msg(MSGT_VO, MSGL_ERR, MSGTR_LIBVO_XV_XvQueryAdaptorsFailed);
        return -1;
    }

    /* check adaptors */
    if (xv_port)
    {
        int port_found;

        for (port_found = 0, i = 0; !port_found && i < adaptors; i++)
        {
            if ((ai[i].type & XvInputMask) && (ai[i].type & XvImageMask))
            {
                for (xv_p = ai[i].base_id;
                     xv_p < ai[i].base_id + ai[i].num_ports; ++xv_p)
                {
                    if (xv_p == xv_port)
                    {
                        port_found = 1;
                        break;
                    }
                }
            }
        }
        if (port_found)
        {
            if (XvGrabPort(mDisplay, xv_port, CurrentTime))
                xv_port = 0;
        } else
        {
            mp_msg(MSGT_VO, MSGL_WARN,
                   MSGTR_LIBVO_XV_InvalidPortParameter);
            xv_port = 0;
        }
    }

    for (i = 0; i < adaptors && xv_port == 0; i++)
    {
        /* check if adaptor number has been specified */
        if (xv_adaptor != -1 && xv_adaptor != i)
          continue;

        if ((ai[i].type & XvInputMask) && (ai[i].type & XvImageMask))
        {
            for (xv_p = ai[i].base_id;
                 xv_p < ai[i].base_id + ai[i].num_ports; ++xv_p)
                if (!XvGrabPort(mDisplay, xv_p, CurrentTime))
                {
                    xv_port = xv_p;
                    mp_msg(MSGT_VO, MSGL_V,
                           "[VO_XV] Using Xv Adapter #%d (%s)\n",
                           i, ai[i].name);
                    break;
                } else
                {
                    mp_msg(MSGT_VO, MSGL_WARN,
                           MSGTR_LIBVO_XV_CouldNotGrabPort, (int) xv_p);
                    ++busy_ports;
                }
        }
    }
    if (!xv_port)
    {
        if (busy_ports)
            mp_msg(MSGT_VO, MSGL_ERR,
                   MSGTR_LIBVO_XV_CouldNotFindFreePort);
        else
            mp_msg(MSGT_VO, MSGL_ERR,
                   MSGTR_LIBVO_XV_NoXvideoSupport);
        return -1;
    }

    if ( !vo_xv_init_colorkey() )
    {
      return -1; // bail out, colorkey setup failed
    }
    vo_xv_enable_vsync();
    vo_xv_get_max_img_dim( &max_width, &max_height );

    fo = XvListImageFormats(mDisplay, xv_port, (int *) &formats);

    mp_input_add_event_fd(ConnectionNumber(mDisplay), check_events);
    return 0;
}

static int control(uint32_t request, void *data)
{
    switch (request)
    {
        case VOCTRL_PAUSE:
            return int_pause = 1;
        case VOCTRL_RESUME:
            return int_pause = 0;
        case VOCTRL_QUERY_FORMAT:
            return query_format(*((uint32_t *) data));
        case VOCTRL_GET_IMAGE:
            return get_image(data);
        case VOCTRL_DRAW_IMAGE:
            return draw_image(data);
        case VOCTRL_GUISUPPORT:
            return VO_TRUE;
        case VOCTRL_GET_PANSCAN:
            return VO_TRUE;
        case VOCTRL_FULLSCREEN:
            vo_x11_fullscreen();
            /* indended, fallthrough to update panscan on fullscreen/windowed switch */
        case VOCTRL_SET_PANSCAN:
                    resize();
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
        case VOCTRL_ONTOP:
            vo_x11_ontop();
            return VO_TRUE;
        case VOCTRL_UPDATE_SCREENINFO:
            update_xinerama_info();
            return VO_TRUE;
    }
    return VO_NOTIMPL;
}
