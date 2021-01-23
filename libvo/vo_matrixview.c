/*
 * MatrixView video output driver for MPlayer
 *
 * by Pigeon <pigeon at pigeond.net>
 *
 * Based on MatrixView the screensaver from http://rss-glx.sf.net/
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

#include "config.h"

#include <stdint.h>

#include "libavutil/avstring.h"

#include "mp_msg.h"
#include "subopt-helper.h"
#include "video_out.h"
#define NO_DRAW_FRAME
#include "video_out_internal.h"
#include "libmpcodecs/vf.h"
#include "gl_common.h"
#include "libswscale/swscale.h"
#include "libmpcodecs/vf_scale.h"
#include "osdep/timer.h"

#include "matrixview.h"

static const vo_info_t info = {
    "MatrixView (OpenGL)",
    "matrixview",
    "Pigeon <pigeon@pigeond.net>",
    "Based on MatrixView from rss-glx.sf.net"
};

const LIBVO_EXTERN(matrixview)

static MPGLContext glctx;

static int eq_contrast;
static int eq_brightness;
static uint32_t image_width;
static uint32_t image_height;
static uint32_t image_format;
static struct SwsContext *sws;

static uint8_t *map_image[MP_MAX_PLANES];
static int map_stride[MP_MAX_PLANES];

#define DEFAULT_MATRIX_ROWS 96
#define DEFAULT_MATRIX_COLS 128
static int matrix_rows;
static int matrix_cols;

#define DEFAULT_CONTRAST        0.90f
#define CONTRAST_MULTIPLIER     0.02f

#define DEFAULT_BRIGHTNESS      1.0f
#define BRIGHTNESS_MULTIPLIER   0.02f


static void contrast_set(int value)
{
    float contrast = value * CONTRAST_MULTIPLIER + DEFAULT_CONTRAST;
    eq_contrast = value;
    if (contrast < 0)
         contrast = 0;
    matrixview_contrast_set(contrast);
}


static void brightness_set(int value)
{
    float brightness = value * BRIGHTNESS_MULTIPLIER + DEFAULT_BRIGHTNESS;
    eq_brightness = value;
    if (brightness < 0)
        brightness = 0;
    matrixview_brightness_set(brightness);
}

static void resize(void)
{
    matrixview_reshape(vo_dwidth, vo_dheight);
    flip_page();
}

static int config(uint32_t width, uint32_t height,
                  uint32_t d_width, uint32_t d_height,
                  uint32_t flags, char *title, uint32_t format)
{
    image_height = height;
    image_width  = width;
    image_format = format;

    if (mpglcontext_create_window(&glctx, d_width, d_height, flags, title) < 0)
        return -1;
    if (glctx.setGlWindow(&glctx) == SET_WINDOW_FAILED)
        return -1;

    if (sws)
        sws_freeContext(sws);

    sws = sws_getContextFromCmdLine(image_width, image_height, image_format,
                                    matrix_cols, matrix_rows, IMGFMT_Y8);
    if (!sws) {
        mp_msg(MSGT_VO, MSGL_ERR, "[matrixview] Cannot create SwsContext context\n");
        return -1;
    }

    if (!map_image[0])
        map_image[0] = calloc(matrix_cols, matrix_rows);

    map_stride[0] = matrix_cols;

    matrixview_init(vo_dwidth, vo_dheight);
    matrixview_matrix_resize(matrix_cols, matrix_rows);

    contrast_set(eq_contrast);
    brightness_set(eq_brightness);
    matrixview_reshape(vo_dwidth, vo_dheight);

#ifdef CONFIG_GL_OSX
    vo_osx_redraw_func = resize;
#endif
    return 0;
}


static void check_events(void)
{
    int e = glctx.check_events();
    if (e & VO_EVENT_RESIZE) {
        resize();
    } else if (e & VO_EVENT_EXPOSE)
        flip_page();
}


static void draw_osd(void)
{
    return;
}


static void flip_page(void)
{
    matrixview_draw(GetTimer(), map_image[0]);
    // Needed at least on PPC Mac Mini on OSX. Should not hurt
    // much to do always.
    mpglFlush();
    glctx.swapGlBuffers(&glctx);
}



static int draw_slice(uint8_t *src[], int stride[], int w, int h, int x, int y)
{
    sws_scale(sws, src, stride, y, h, map_image, map_stride);
    return 0;
}


static int query_format(uint32_t format)
{
    int caps = VFCAP_CSP_SUPPORTED | VFCAP_HWSCALE_UP | VFCAP_HWSCALE_DOWN | VFCAP_ACCEPT_STRIDE;

    switch (format) {
    case IMGFMT_YV12:
    case IMGFMT_BGR32:
    case IMGFMT_BGR24:
    case IMGFMT_BGR16:
    case IMGFMT_BGR15:
    case IMGFMT_RGB32:
    case IMGFMT_RGB24:
        return caps;
    default:
        break;
    }

    return 0;
}


static void uninit(void)
{
    if (!vo_config_count)
        return;
    uninit_mpglcontext(&glctx);
    free(map_image[0]);
    map_image[0] = NULL;
    sws_freeContext(sws);
    sws = NULL;
}


static const opt_t subopts[] =
{
    { "rows", OPT_ARG_INT, &matrix_rows, int_pos },
    { "cols", OPT_ARG_INT, &matrix_cols, int_pos },
    { NULL }
};


static int preinit(const char *arg)
{
    enum MPGLType gltype = GLTYPE_AUTO;
    if (!init_mpglcontext(&glctx, gltype))
        return -1;

    matrix_rows = DEFAULT_MATRIX_ROWS;
    matrix_cols = DEFAULT_MATRIX_COLS;

    if (subopt_parse(arg, subopts) != 0) {
        mp_msg(MSGT_VO, MSGL_FATAL,
                "\n-vo matrixview command line help:\n"
                "Example: mplayer -vo matrixview:cols=320:rows=240\n"
                "\n"
                "Options:\n"
                "\n"
                "  cols=<12-320>\n"
                "    Specify the number of columns of the matrix view, default %d\n"
                "\n"
                "  rows=<12-240>\n"
                "    Specify the number of rows of the matrix view, default %d\n"
                "\n"
                ,
                DEFAULT_MATRIX_COLS, DEFAULT_MATRIX_ROWS
              );
        return -1;
    }

    return 0;
}


static int control(uint32_t request, void *data)
{
    switch (request) {
    case VOCTRL_QUERY_FORMAT:
        return query_format(*(uint32_t*)data);
    case VOCTRL_ONTOP:
        glctx.ontop();
        return VO_TRUE;
    case VOCTRL_FULLSCREEN:
        glctx.fullscreen();
        resize();
        return VO_TRUE;
    case VOCTRL_BORDER:
        glctx.border();
        return VO_TRUE;
    case VOCTRL_GET_EQUALIZER:
        {
            vf_equalizer_t *eq=data;

            if (av_strcasecmp(eq->item, "contrast") == 0) {
                eq->value = eq_contrast;
            } else if (av_strcasecmp(eq->item, "brightness") == 0) {
                eq->value = eq_brightness;
            }
        }
        return VO_TRUE;
    case VOCTRL_SET_EQUALIZER:
        {
            vf_equalizer_t *eq=data;
            if (av_strcasecmp(eq->item, "contrast") == 0) {
                contrast_set(eq->value);
            } else if (av_strcasecmp(eq->item, "brightness") == 0) {
                brightness_set(eq->value);
            }
        }
        return VO_TRUE;
    case VOCTRL_UPDATE_SCREENINFO:
        glctx.update_xinerama_info();
        return VO_TRUE;
    case VOCTRL_GUISUPPORT:
        return VO_TRUE;
    }
    return VO_NOTIMPL;
}
