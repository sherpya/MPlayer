/*
 * video output driver for AAlib
 *
 * copyright (c) 2001 Folke Ashberg <folke@ashberg.de>
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

#include <stdio.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <unistd.h>

#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#include "config.h"
#include "libavutil/avstring.h"
#include "video_out.h"
#include "video_out_internal.h"
#include "libmpcodecs/vf.h"
#include "aspect.h"
#include "libswscale/swscale.h"
#include "libmpcodecs/vf_scale.h"
#include "sub/font_load.h"
#include "sub/sub.h"

#include "osdep/keycodes.h"
#include <aalib.h>
#include "subopt-helper.h"
#include "help_mp.h"
#include "mp_msg.h"
#include "mp_fifo.h"


#define MESSAGE_DURATION 3
#define MESSAGE_SIZE 512
#define MESSAGE_DEKO " +++ %s +++ "

	static const vo_info_t info = {
	    "AAlib",
	    "aa",
	    "Alban Bedel <albeu@free.fr> and Folke Ashberg <folke@ashberg.de>",
	    ""
	};

const LIBVO_EXTERN(aa)

/* aa's main context we use */
aa_context *c;
aa_renderparams *p;
static int fast =0;
/* used for the sws */
static uint8_t * image[MP_MAX_PLANES];
static int image_stride[MP_MAX_PLANES];

/* image infos */
static int image_format;
static int image_width;
static int image_height;
static int image_x, image_y;
static int screen_x, screen_y;
static int screen_w, screen_h;
static int src_width;
static int src_height;

/* osd stuff */
time_t stoposd = 0;
static int showosdmessage = 0;
char osdmessagetext[MESSAGE_SIZE];
char posbar[MESSAGE_SIZE];
static int osdx, osdy;
static int osd_text_length = 0;
int aaconfigmode=1;
font_desc_t* vo_font_save = NULL;
font_desc_t* sub_font_save = NULL;
static struct SwsContext *sws=NULL;

/* configuration */
int aaopt_osdcolor = AA_SPECIAL;
int aaopt_subcolor = AA_SPECIAL;

static void
resize(void){
    /*
     * this function is called by aa lib if windows resizes
     * further during init, because here we have to calculate
     * a little bit
     */

    aa_resize(c);

    aspect_save_screenres(aa_imgwidth(c),aa_imgheight(c));
    image_height =  aa_imgheight(c); //src_height;
    image_width = aa_imgwidth(c); //src_width;

    aspect(&image_width,&image_height,A_ZOOM);

    image_x = (aa_imgwidth(c) - image_width) / 2;
    image_y = (aa_imgheight(c) - image_height) / 2;
    screen_w = image_width * aa_scrwidth(c) / aa_imgwidth(c);
    screen_h = image_height * aa_scrheight(c) / aa_imgheight(c);
    screen_x = (aa_scrwidth(c) - screen_w) / 2;
    screen_y = (aa_scrheight(c) - screen_h) / 2;

    if(sws) sws_freeContext(sws);
    sws = sws_getContextFromCmdLine(src_width,src_height,image_format,
				   image_width,image_height,IMGFMT_Y8);

    memset(image, 0, sizeof(image));
    image[0] = aa_image(c) + image_y * aa_imgwidth(c) + image_x;

    memset(image_stride, 0, sizeof(image_stride));
    image_stride[0] = aa_imgwidth(c);

    showosdmessage=0;

}

static void
osdmessage(int duration, int deko, const char *fmt, ...)
{
    /*
     * for outputting a centered string at the bottom
     * of our window for a while
     */
    va_list ar;
    char m[MESSAGE_SIZE];
    unsigned int old_len = strlen(osdmessagetext);

    va_start(ar, fmt);
    vsprintf(m, fmt, ar);
    va_end(ar);
    if (deko==1) sprintf(osdmessagetext, MESSAGE_DEKO , m);
    else strcpy(osdmessagetext, m);

    if(old_len > strlen(osdmessagetext)) {
      memset(c->textbuffer + osdy * aa_scrwidth(c) + osdx,' ',old_len);
      memset(c->attrbuffer + osdy * aa_scrwidth(c) + osdx,0,old_len);
    }
    showosdmessage=1;
    stoposd = time(NULL) + duration;
    osdx=(aa_scrwidth(c) / 2) - (strlen(osdmessagetext) / 2 ) ;
    posbar[0]='\0';
}

static void
osdpercent(int duration, int deko, int min, int max, int val, const char * desc, const char * unit)
{
    /*
     * prints a bar for setting values
     */
    float step;
    int where;
    int i;


    step=(float)aa_scrwidth(c) /(float)(max-min);
    where=(val-min)*step;
    osdmessage(duration,deko,"%s: %i%s",desc, val, unit);
    posbar[0]='|';
    posbar[aa_scrwidth(c)-1]='|';
    for (i=0;i<aa_scrwidth(c);i++){
	if (i==where) posbar[i]='#';
	else posbar[i]='-';
    }
    if (where!=0) posbar[0]='|';
    if (where!=(aa_scrwidth(c)-1) ) posbar[aa_scrwidth(c)-1]='|';

    posbar[aa_scrwidth(c)]='\0';

}

static void
printosdtext(void)
{
  if(osd_text_length > 0 && !vo_osd_text) {
    memset(c->textbuffer,' ',osd_text_length);
    memset(c->attrbuffer,0,osd_text_length);
    osd_text_length = 0;
  }
    /*
     * places the mplayer status osd
     */
  if (vo_osd_text && vo_osd_text[0] != 0) {
    int len;
    if(vo_osd_text[0] < 32) {
      len = strlen(sub_osd_names_short[vo_osd_text[0]]) + strlen(vo_osd_text+1) + 2;
      aa_printf(c, 0, 0 , aaopt_osdcolor, "%s %s ", sub_osd_names_short[vo_osd_text[0]], vo_osd_text+1);
    } else {
      len = strlen(vo_osd_text) + 1;
      aa_printf(c, 0, 0 , aaopt_osdcolor, "%s ",vo_osd_text);
    }

    if(len < osd_text_length) {
      memset(c->textbuffer + len,' ',osd_text_length - len);
      memset(c->attrbuffer + len,0,osd_text_length - len);
    }
    osd_text_length = len;

  }
}

static void
printosdprogbar(void){
    /* print mplayer osd-progbar */
    if (vo_osd_progbar_type!=-1){
        osdpercent(1,1,0,255,vo_osd_progbar_value, sub_osd_names[vo_osd_progbar_type], "");
    }
}
static int
config(uint32_t width, uint32_t height, uint32_t d_width,
	    uint32_t d_height, uint32_t flags, char *title,
	    uint32_t format) {
    /*
     * main init
     * called by mplayer
     */

    int i;

    aspect_save_orig(width,height);
    aspect_save_prescale(d_width,d_height);

    src_height = height;
    src_width = width;
    image_format = format;

    /* nothing will change its size, be we need some values initialized */
    resize();

    /* now init our own 'font' */
    if(!vo_font_save) vo_font_save = vo_font;
    if(vo_font == vo_font_save) {
      sub_font_save = sub_font;
      sub_font=vo_font=malloc(sizeof(font_desc_t));//if(!desc) return NULL;
      memset(vo_font,0,sizeof(font_desc_t));
      vo_font->pic_a[0]=malloc(sizeof(raw_file));
      memset(vo_font->pic_a[0],0,sizeof(raw_file));
      vo_font->pic_b[0]=malloc(sizeof(raw_file));
      memset(vo_font->pic_b[0],0,sizeof(raw_file));

#ifdef CONFIG_FREETYPE
      vo_font->dynamic = 0;
#endif

      vo_font->spacewidth=1;
      vo_font->charspace=0;
      vo_font->height=1;
      vo_font->pic_a[0]->bmp=malloc(255);
      vo_font->pic_a[0]->pal=NULL;
      vo_font->pic_b[0]->bmp=malloc(255);
      vo_font->pic_b[0]->pal=NULL;
      vo_font->pic_a[0]->w=1;
      vo_font->pic_a[0]->h=1;
      for (i=0; i<255; i++){
	vo_font->width[i]=1;
	vo_font->font[i]=0;
	vo_font->start[i]=i;
	vo_font->pic_a[0]->bmp[i]=i;
	vo_font->pic_b[0]->bmp[i]=i;
      }
    }

    /* say hello */
    osdmessage(5, 1, "Welcome to ASCII ART MPlayer");

    mp_msg(MSGT_VO,MSGL_V,"VO: [aa] screendriver:   %s\n", c->driver->name);
    mp_msg(MSGT_VO,MSGL_V,"VO: [aa] keyboarddriver: %s\n", c->kbddriver->name);

    mp_msg(MSGT_VO,MSGL_INFO,
		"\n"
		"Important suboptions\n"
		"\textended  use use all 256 characters\n"
		"\teight     use eight bit ascii\n"
		"\tdriver    set recommended aalib driver (X11,curses,linux)\n"
		"\thelp      to see all options provided by aalib\n"
		"\n"
		"AA-MPlayer Keys\n"
		"\t1 : contrast -\n"
		"\t2 : contrast +\n"
		"\t3 : brightness -\n"
		"\t4 : brightness +\n"
		"\t5 : fast rendering\n"
		"\t6 : dithering\n"
		"\t7 : invert image\n"
	        "\ta : toggles between aa and mplayer control\n"

		"\n"
		"All other keys are MPlayer defaults.\n"


	  );

    return 0;
}

static int
query_format(uint32_t format) {
    /*
     * ...are we able to... ?
     * called by mplayer
     * All input format supported by the sws
     */
    switch(format){
	case IMGFMT_YV12:
	case IMGFMT_I420:
	case IMGFMT_IYUV:
	case IMGFMT_IYU2:
	case IMGFMT_BGR32:
	case IMGFMT_BGR24:
	case IMGFMT_BGR16:
	case IMGFMT_BGR15:
	case IMGFMT_RGB32:
	case IMGFMT_RGB24:
	case IMGFMT_Y8:
	case IMGFMT_Y800:
	    return VFCAP_CSP_SUPPORTED | VFCAP_SWSCALE | VFCAP_OSD;
    }
    return 0;
}

static int
draw_frame(uint8_t *src[]) {
  int stride[MP_MAX_PLANES] = {0};

  switch(image_format) {
  case IMGFMT_BGR15:
  case IMGFMT_BGR16:
    stride[0] = src_width*2;
    break;
  case IMGFMT_IYU2:
  case IMGFMT_BGR24:
    stride[0] = src_width*3;
    break;
  case IMGFMT_BGR32:
    stride[0] = src_width*4;
    break;
  }

  sws_scale(sws,src,stride,0,src_height,image,image_stride);

   /* Now 'ASCIInate' the image */
  if (fast)
    aa_fastrender(c, screen_x, screen_y, screen_w + screen_x, screen_h + screen_y );
  else
    aa_render(c, p,screen_x, screen_y, screen_w + screen_x, screen_h + screen_y );

  return 0;
}

static int
draw_slice(uint8_t *src[], int stride[],
	    int w, int h, int x, int y) {

  int dx1 = screen_x + (x * screen_w / src_width);
  int dy1 = screen_y + (y * screen_h / src_height);
  int dx2 = screen_x + ((x+w) * screen_w / src_width);
  int dy2 = screen_y + ((y+h) * screen_h / src_height);

  sws_scale(sws,src,stride,y,h,image,image_stride);

  /* Now 'ASCIInate' the image */
  if (fast)
    aa_fastrender(c, dx1, dy1, dx2, dy2 );
  else
    aa_render(c, p,dx1, dy1, dx2, dy2 );


  return 0;
}

static void
flip_page(void) {

   /* do we have to put *our* (messages, progbar) osd to aa's txtbuf ? */
    if (showosdmessage)
      {
	if (time(NULL)>=stoposd ) {
	  showosdmessage=0;
	  if(*osdmessagetext) {
	    memset(c->textbuffer + osdy * aa_scrwidth(c) + osdx,' ',strlen(osdmessagetext));
	    memset(c->attrbuffer + osdy * aa_scrwidth(c) + osdx ,0,strlen(osdmessagetext));
	    osdmessagetext[0] = '\0';
	  }
	  if(*posbar) {
	    memset(c->textbuffer + (osdy+1) * aa_scrwidth(c),' ',strlen(posbar));
	    memset(c->attrbuffer + (osdy+1) * aa_scrwidth(c),0,strlen(posbar));
	  }
	} else {
	  /* update osd */
	  aa_puts(c, osdx, osdy, AA_SPECIAL, osdmessagetext);
	  /* posbar? */
	  if (posbar[0]!='\0')
	    aa_puts(c, 0, osdy + 1, AA_SPECIAL, posbar);
	}
      }
    /* OSD time & playmode , subtitles */
    printosdtext();


    /* print out */
    aa_flush(c);
}

static void
check_events(void) {
    /*
     * any events?
     * called by show_image and mplayer
     */
    int key;
    while ((key=aa_getevent(c,0))!=AA_NONE ){
	if (key>255){
	    /* some conversations */
	    switch (key) {
		case AA_UP:
		    mplayer_put_key(KEY_UP);
		    break;
		case AA_DOWN:
		    mplayer_put_key(KEY_DOWN);
		    break;
		case AA_LEFT:
		    mplayer_put_key(KEY_LEFT);
		    break;
		case AA_RIGHT:
		    mplayer_put_key(KEY_RIGHT);
		    break;
		case AA_ESC:
		    mplayer_put_key(KEY_ESC);
		    break;
		case 65765:
		    mplayer_put_key(KEY_PAGE_UP);
		    break;
		case 65766:
		    mplayer_put_key(KEY_PAGE_DOWN);
		    break;
		default:
		    continue; /* aa lib special key */
		    break;
	    }
	}
	if (key=='a' || key=='A'){
	    aaconfigmode=!aaconfigmode;
	    osdmessage(MESSAGE_DURATION, 1, "aa config mode is now %s",
		    aaconfigmode==1 ? "on. use keys 5-7" : "off");
	}
	if (aaconfigmode==1) {
	    switch (key) {
		/* AA image controls */
		case '5':
		    fast=!fast;
		    osdmessage(MESSAGE_DURATION, 1, "Fast mode is now %s", fast==1 ? "on" : "off");
		    break;
		case '6':
		    if (p->dither==AA_FLOYD_S){
			p->dither=AA_NONE;
			osdmessage(MESSAGE_DURATION, 1, "Dithering: Off");
		    }else if (p->dither==AA_NONE){
			p->dither=AA_ERRORDISTRIB;
			osdmessage(MESSAGE_DURATION, 1, "Dithering: Error Distribution");
		    }else if (p->dither==AA_ERRORDISTRIB){
			p->dither=AA_FLOYD_S;
			osdmessage(MESSAGE_DURATION, 1, "Dithering: Floyd Steinberg");
		    }
		    break;
		case '7':
		    p->inversion=!p->inversion;
		    osdmessage(MESSAGE_DURATION, 1, "Invert mode is now %s",
				p->inversion==1 ? "on" : "off");
		    break;

		default :
		    /* nothing if we're interested in?
		     * the mplayer should handle it!
		     */
		    mplayer_put_key(key);
		    break;
	    }
	}// aaconfigmode
	else mplayer_put_key(key);
    }
}

static void
uninit(void) {
    /*
     * THE END
     */

    if (strstr(c->driver->name,"Curses") || strstr(c->driver->name,"Linux")){
	freopen("/dev/tty", "w", stderr);
    }
    if(vo_font_save) {
      free(vo_font->pic_a[0]->bmp);
      free(vo_font->pic_a[0]);
      free(vo_font->pic_b[0]->bmp);
      free(vo_font->pic_b[0]);
      free(vo_font);
      vo_font = vo_font_save;
      vo_font_save = NULL;
    }
    if(sub_font_save) {
      sub_font = sub_font_save;
      sub_font_save = NULL;
    }
    aa_close(c);
}

static void draw_alpha(int x,int y, int w,int h, unsigned char* src, unsigned char *srca, int stride){
    int i,j;
    for (i = 0; i < h; i++) {
	for (j = 0; j < w; j++) {
	    if (src[i*stride+j] > 0) {
		c->textbuffer[x + j + (y+i)*aa_scrwidth(c)] = src[i*stride+j];
		c->attrbuffer[x + j + (y+i)*aa_scrwidth(c)] = aaopt_subcolor;
	    }
	}
    }
}

static void clear_alpha(int x0,int y0, int w,int h) {
  int l;

  for(l = 0 ; l < h ; l++) {
    memset(c->textbuffer + (y0 + l) * aa_scrwidth(c) + x0,' ',w);
    memset(c->attrbuffer + (y0 + l) * aa_scrwidth(c) + x0,0,w);
  }
}


static void
draw_osd(void){
    char * vo_osd_text_save;
    int vo_osd_progbar_type_save;

    printosdprogbar();
    /* let vo_draw_text only write subtitle */
    vo_osd_text_save=vo_osd_text; /* we have to save the osd_text */
    vo_osd_text=NULL;
    vo_osd_progbar_type_save=vo_osd_progbar_type;
    vo_osd_progbar_type=-1;
    vo_remove_text(aa_scrwidth(c), aa_scrheight(c),clear_alpha);
    vo_draw_text(aa_scrwidth(c), aa_scrheight(c), draw_alpha);
    vo_osd_text=vo_osd_text_save;
    vo_osd_progbar_type=vo_osd_progbar_type_save;
}

static int
getcolor(char * s){
    int i;
    char * rest;
    if  (s==NULL) return -1;
    i=strtol(s, &rest, 10);
    if ((rest==NULL || strlen(rest)==0) && i>=0 && i<=5) return i;
    if (!av_strcasecmp(s, "normal")) return AA_NORMAL;
    else if (!av_strcasecmp(s, "dim")) return AA_DIM;
    else if (!av_strcasecmp(s, "bold")) return AA_BOLD;
    else if (!av_strcasecmp(s, "boldfont")) return AA_BOLDFONT;
    else if (!av_strcasecmp(s, "special")) return AA_SPECIAL;
    else return -1;
}

static int parse_suboptions(const char *arg) {
    char *pseudoargv[4], *osdcolor = NULL, *subcolor = NULL, **strings,
         *helpmsg = NULL;
    int pseudoargc, displayhelp = 0, *booleans;
    const opt_t extra_opts[] = {
            {"osdcolor", OPT_ARG_MSTRZ, &osdcolor,    NULL},
            {"subcolor", OPT_ARG_MSTRZ, &subcolor,    NULL},
            {"help",     OPT_ARG_BOOL,  &displayhelp, NULL} };
    opt_t *subopts = NULL, *p;
    char * const strings_list[] = {"-driver", "-kbddriver", "-mousedriver", "-font",
        "-width", "-height", "-minwidth", "-minheight", "-maxwidth",
        "-maxheight", "-recwidth", "-recheight", "-bright",  "-contrast",
        "-gamma",  "-dimmul", "-boldmul", "-random" };
    char * const booleans_list[] = {"-dim", "-bold", "-reverse", "-normal",
        "-boldfont", "-inverse", "-extended", "-eight", "-dither",
        "-floyd_steinberg", "-error_distribution"};
    char * const nobooleans_list[] = {"-nodim", "-nobold", "-noreverse", "-nonormal",
        "-noboldfont", "-noinverse", "-noextended", "-noeight", "-nodither",
        "-nofloyd_steinberg", "-noerror_distribution"};
    const int nstrings = sizeof(strings_list) / sizeof(char*);
    const int nbooleans = sizeof(booleans_list) / sizeof(char*);
    const int nextra_opts = sizeof(extra_opts) / sizeof(opt_t);
    const int nsubopts = nstrings + nbooleans + nextra_opts;
    int i, retval = 0;

    subopts = calloc(nsubopts + 1, sizeof(opt_t));
    strings = calloc(nstrings, sizeof(char*));
    booleans = calloc(nbooleans, sizeof(int));

    p = subopts;
    for (i=0; i<nstrings; i++, p++) {
        p->name = strings_list[i] + 1; // skip '-'
        p->type = OPT_ARG_MSTRZ;
        p->valp = &strings[i];
    }
    for (i=0; i<nbooleans; i++, p++) {
        p->name = booleans_list[i] + 1;
        p->type = OPT_ARG_BOOL;
        p->valp = &booleans[i];
        booleans[i] = -1;
    }
    memcpy(p, extra_opts, sizeof(extra_opts));

    retval = subopt_parse(arg, subopts);

    if (retval == 0 && displayhelp) {
        helpmsg = strdup(aa_help);
        for (i=0; i<(signed)strlen(helpmsg); i++)
            if (helpmsg[i] == '-') helpmsg[i] = ' ';
        mp_msg(MSGT_VO, MSGL_INFO, MSGTR_VO_AA_HelpHeader);
        mp_msg(MSGT_VO, MSGL_INFO, "%s\n\n", helpmsg);
        mp_msg(MSGT_VO, MSGL_INFO, MSGTR_VO_AA_AdditionalOptions);
        retval = -1;
    }
    if (retval == 0) {
        pseudoargv[3] = NULL;
        for (i=0; i<nstrings; i++) {
            pseudoargc = 3;         // inside loop because aalib changes it
            if (strings[i] != NULL) {
                pseudoargv[1] = strings_list[i];
                pseudoargv[2] = strings[i];
                aa_parseoptions(&aa_defparams, &aa_defrenderparams,
                                                &pseudoargc, pseudoargv);
            }
        }
        pseudoargv[2] = NULL;
        for (i=0; i<nbooleans; i++) {
            if (booleans[i] == -1) continue;
            pseudoargc = 2;
            if (booleans[i]) pseudoargv[1] = booleans_list[i];
            else pseudoargv[1] = nobooleans_list[i];
            aa_parseoptions(&aa_defparams, &aa_defrenderparams,
                                                &pseudoargc, pseudoargv);
        }
        if (osdcolor) aaopt_osdcolor = getcolor(osdcolor);
        if (subcolor) aaopt_subcolor = getcolor(subcolor);
    }

    free(subopts);
    free(booleans);
    if (strings) {
        for (i=0; i<nstrings; i++)
            free(strings[i]);
        free(strings);
    }
    free(osdcolor);
    free(subcolor);
    free(helpmsg);
    return retval;
}

static int preinit(const char *arg)
{
    char * hidis = NULL;

    if(arg)
    {
        if (parse_suboptions(arg) != 0)
	return ENOSYS;
    }

        /* initializing of aalib */

    hidis=aa_getfirst(&aa_displayrecommended);
    if ( hidis==NULL ){
	struct stat sbuf;
	char fname[12];
	FILE *fp = NULL;
	int vt;
	/* check /dev/vcsa<vt> */
	/* check only, if no driver is explicit set */
	if (fstat (2, &sbuf) != -1) {
	// vt number stored in device minor
	vt = sbuf.st_rdev & 0xff;
	sprintf (fname, "/dev/vcsa%2.2i", vt);
	fp = fopen (fname, "w+");
	}
	if (fp==NULL){
	    fprintf(stderr,"VO: [aa] cannot open %s for writing,"
			"so we'll not use linux driver\n", fname);
    	    aa_recommendlowdisplay("linux");
    	    aa_recommendhidisplay("curses");
    	    aa_recommendhidisplay("X11");
	}else fclose(fp);
    } else aa_recommendhidisplay(hidis);
    c = aa_autoinit(&aa_defparams);

    if (c == NULL) {
	mp_msg(MSGT_VO,MSGL_ERR,"Cannot initialize aalib\n");
	return VO_ERROR;
    }
    if (!aa_autoinitkbd(c,0)) {
	mp_msg(MSGT_VO,MSGL_ERR,"Cannot initialize keyboard\n");
	aa_close(c);
	return VO_ERROR;
    }

    aa_resizehandler(c, (void *)resize);
    aa_hidecursor(c);
    p = aa_getrenderparams();

    if ((strstr(c->driver->name,"Curses")) || (strstr(c->driver->name,"Linux"))){
	freopen("/dev/null", "w", stderr);
	/* disable console blanking */
	printf("\033[9;0]");
    }

    memset(image,0,3*sizeof(uint8_t));
    osdmessagetext[0] = '\0';
    osdx = osdy = 0;

    return 0;
}

static int control(uint32_t request, void *data)
{
  switch (request) {
  case VOCTRL_QUERY_FORMAT:
    return query_format(*((uint32_t*)data));
  case VOCTRL_SET_EQUALIZER: {
    vf_equalizer_t *eq=data;

    if(strcmp(eq->item,"contrast") == 0)
      p->contrast = ( eq->value + 100 ) * 64 / 100;
    else if(strcmp(eq->item,"brightness") == 0)
      p->bright = ( eq->value + 100) * 128 / 100;
    return VO_TRUE;
  }
  case VOCTRL_GET_EQUALIZER: {
    vf_equalizer_t *eq=data;

    if(strcmp(eq->item,"contrast") == 0)
      eq->value = (p->contrast - 64) * 100 / 64;
    else if(strcmp(eq->item,"brightness") == 0)
      eq->value = (p->bright - 128) * 100 / 128;

    return VO_TRUE;
  }
  }
  return VO_NOTIMPL;
}
