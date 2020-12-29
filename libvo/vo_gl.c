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
 *
 * You can alternatively redistribute this file and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "config.h"
#include "sub/ass_mp.h"
#include "mp_msg.h"
#include "subopt-helper.h"
#include "video_out.h"
#define NO_DRAW_FRAME
#include "video_out_internal.h"
#include "libmpcodecs/vf.h"
#include "sub/font_load.h"
#include "sub/sub.h"

#include "gl_common.h"
#include "aspect.h"
#include "fastmemcpy.h"
#include "sub/eosd.h"

#ifdef CONFIG_GL_SDL
#include "sdl_common.h"
#endif

static const vo_info_t info =
{
  "OpenGL",
  "gl",
  "Reimar Doeffinger <Reimar.Doeffinger@gmx.de>",
  ""
};

const LIBVO_EXTERN(gl)


static const vo_info_t info_nosw =
{
  "OpenGL no software rendering",
  "gl_nosw",
  "Reimar Doeffinger <Reimar.Doeffinger@gmx.de>",
  ""
};
static int preinit_nosw(const char *arg);
#define info info_nosw
#define preinit preinit_nosw
const LIBVO_EXTERN(gl_nosw)
#undef info
#undef preinit

static MPGLContext glctx;

static int use_osd;
static int scaled_osd;
//! How many parts the OSD may consist of at most
#define MAX_OSD_PARTS 20
//! Textures for OSD
static GLuint osdtex[MAX_OSD_PARTS];
#ifndef FAST_OSD
//! Alpha textures for OSD
static GLuint osdatex[MAX_OSD_PARTS];
#endif
//! Coordinates and texture sizes for OSD
static struct osdcoords {
  int x, y, w, h, sx, sy;
} osdcoords[MAX_OSD_PARTS];
static GLuint *eosdtex;
#define LARGE_EOSD_TEX_SIZE 512
#define TINYTEX_SIZE 16
#define TINYTEX_COLS (LARGE_EOSD_TEX_SIZE/TINYTEX_SIZE)
#define TINYTEX_MAX (TINYTEX_COLS*TINYTEX_COLS)
#define SMALLTEX_SIZE 32
#define SMALLTEX_COLS (LARGE_EOSD_TEX_SIZE/SMALLTEX_SIZE)
#define SMALLTEX_MAX (SMALLTEX_COLS*SMALLTEX_COLS)
static GLuint largeeosdtex[2];
//! Display list that draws the EOSD parts
static GLuint eosdDispList;
//! How many parts the OSD currently consists of
static int osdtexCnt;
static int eosdtexCnt;
static int osd_color;

static int use_aspect;
static int use_ycbcr;
#define MASK_ALL_YUV (~(1 << YUV_CONVERSION_NONE))
#define MASK_NOT_COMBINERS (~((1 << YUV_CONVERSION_NONE) | (1 << YUV_CONVERSION_COMBINERS) | (1 << YUV_CONVERSION_COMBINERS_ATI) | (1 << YUV_CONVERSION_TEXT_FRAGMENT)))
#define MASK_GAMMA_SUPPORT MASK_NOT_COMBINERS
static int use_yuv;
static int colorspace;
static int levelconv;
static int is_yuv;
static int is_xyz;
static int lscale;
static int cscale;
static float filter_strength;
static float noise_strength;
static int yuvconvtype;
static int use_rectangle;
static int using_tex_rect;
static int err_shown;
static int draw_width, draw_height;
static uint32_t image_width;
static uint32_t image_height;
static uint32_t image_format;
static int many_fmts;
static int ati_hack;
static int force_pbo;
static int mesa_buffer;
static int use_glFinish;
static int swap_interval;
static GLenum gl_target;
static GLint gl_texfmt;
static GLenum gl_format;
static GLenum gl_type;
static GLuint gl_buffer;
static GLuint gl_buffer_uv[2];
static int gl_buffersize;
static int gl_buffersize_uv;
static void *gl_bufferptr;
static void *gl_bufferptr_uv[2];
static int mesa_buffersize;
static void *mesa_bufferptr;
static GLuint fragprog;
static GLuint default_texs[22];
static char *custom_prog;
static char *custom_tex;
static int custom_tlin;
static int custom_trect;
static int mipmap_gen;
static int stereo_mode;
static int stipple;
static enum MPGLType backend;

static int eq_bri = 0;
static int eq_cont = 0;
static int eq_sat = 0;
static int eq_hue = 0;
static int eq_rgamma = 0;
static int eq_ggamma = 0;
static int eq_bgamma = 0;

static int texture_width;
static int texture_height;
static int mpi_flipped;
static int vo_flipped;
static int ass_border_l, ass_border_r, ass_border_t, ass_border_b;

static unsigned int slice_height = 1;

// performance statistics
static int imgcnt, dr_imgcnt, dr_rejectcnt;

static int did_render;

static void redraw(void);

static float video_matrix[16];
static float osd_matrix[16];

static void resize(void) {
  int i;
  draw_width  = (vo_rotate & 1) ? vo_dheight : vo_dwidth;
  draw_height = (vo_rotate & 1) ? vo_dwidth  : vo_dheight;
  // simple orthogonal projection for 0-image_width;0-image_height
  memset(video_matrix, 0, sizeof(video_matrix));
  video_matrix[0]  = 2.0/image_width;
  video_matrix[5]  = -2.0/image_height;
  video_matrix[12] = -1;
  video_matrix[13] = 1;
  video_matrix[15] = 1;
  memcpy(osd_matrix, video_matrix, sizeof(osd_matrix));
  if (!scaled_osd) {
    // simple orthogonal projection for 0-vo_dwidth;0-vo_dheight
    osd_matrix[0] =  2.0/draw_width;
    osd_matrix[5] = -2.0/draw_height;
  }
  mp_msg(MSGT_VO, MSGL_V, "[gl] Resize: %dx%d\n", vo_dwidth, vo_dheight);
  if (WinID >= 0) {
    int left = 0, top = 0, w = vo_dwidth, h = vo_dheight;
    geometry(&left, &top, &w, &h, vo_dwidth, vo_dheight);
    top = vo_dheight - h - top;
    mpglViewport(left, top, w, h);
  } else if (vo_fs) {
    mpglViewport(vo_fs_border_l, vo_fs_border_b, vo_dwidth, vo_dheight);
  } else
    mpglViewport(0, 0, vo_dwidth, vo_dheight);

  for (i = 0; i < (vo_rotate & 3); i++) {
    int j;
    for (j = 0; j < 16; j += 4) {
      ROTATE(float, video_matrix[j], video_matrix[j+1]);
      ROTATE(float, osd_matrix[j], osd_matrix[j+1]);
    }
  }

  ass_border_l = ass_border_t = 0;
  if (aspect_scaling() && use_aspect) {
    int new_w, new_h;
    double scale_x, scale_y;
    aspect(&new_w, &new_h, A_WINZOOM);
    panscan_calc_windowed();
    new_w += vo_panscan_x;
    new_h += vo_panscan_y;
    scale_x = (double)new_w / (double)vo_dwidth;
    scale_y = (double)new_h / (double)vo_dheight;
    video_matrix[0]  *= scale_x;
    video_matrix[4]  *= scale_x;
    video_matrix[12] *= scale_x;
    video_matrix[12] += (apply_border_pos(vo_dwidth, new_w, vo_border_pos_x) - apply_border_pos(vo_dwidth, new_w, 0.5)) * 2.0 / vo_dwidth;
    video_matrix[1]  *= scale_y;
    video_matrix[5]  *= scale_y;
    video_matrix[13] *= scale_y;
    video_matrix[13] -= (apply_border_pos(vo_dheight, new_h, vo_border_pos_y) - apply_border_pos(vo_dheight, new_h, 0.5)) * 2.0 / vo_dheight;
    if (vo_rotate & 1) {
      int tmp = new_w; new_w = new_h; new_h = tmp;
    }
    ass_border_l = apply_border_pos(draw_width, new_w, vo_border_pos_x);
    ass_border_t = apply_border_pos(draw_height, new_h, vo_border_pos_y);
    ass_border_r = draw_width  - new_w - ass_border_l;
    ass_border_b = draw_height - new_h - ass_border_t;
  }
  mpglLoadMatrixf(video_matrix);

  if (!scaled_osd) {
#ifdef CONFIG_FREETYPE
    // adjust font size to display size
    force_load_font = 1;
#endif
    vo_osd_changed(OSDTYPE_OSD);
  }
  mpglClear(GL_COLOR_BUFFER_BIT);
  redraw();
}

static void texSize(int w, int h, int *texw, int *texh) {
  if (use_rectangle) {
    *texw = w; *texh = h;
  } else {
    *texw = 32;
    while (*texw < w)
      *texw *= 2;
    *texh = 32;
    while (*texh < h)
      *texh *= 2;
  }
  if (mesa_buffer) *texw = (*texw + 63) & ~63;
  else if (ati_hack) *texw = (*texw + 511) & ~511;
}

//! maximum size of custom fragment program
#define MAX_CUSTOM_PROG_SIZE (1024 * 1024)
static void update_yuvconv(void) {
  int xs, ys, depth;
  float bri = eq_bri / 100.0;
  float cont = (eq_cont + 100) / 100.0;
  float hue = eq_hue / 100.0 * 3.1415927;
  float sat = (eq_sat + 100) / 100.0;
  float rgamma = exp(log(8.0) * eq_rgamma / 100.0);
  float ggamma = exp(log(8.0) * eq_ggamma / 100.0);
  float bgamma = exp(log(8.0) * eq_bgamma / 100.0);
  gl_conversion_params_t params = {gl_target, yuvconvtype,
      {colorspace, levelconv, bri, cont, hue, sat, rgamma, ggamma, bgamma, 0},
      texture_width, texture_height, 0, 0, filter_strength, noise_strength,
      stereo_mode == GL_3D_STIPPLE};
  mp_get_chroma_shift(image_format, &xs, &ys, &depth);
  params.chrom_texw = params.texw >> xs;
  params.chrom_texh = params.texh >> ys;
  params.csp_params.input_shift = -depth & 7;
  params.is_planar = is_yuv;
  if (is_xyz) {
    params.csp_params.format = MP_CSP_XYZ;
    params.csp_params.input_shift = 0;
    params.csp_params.rgamma *= 2.2;
    params.csp_params.ggamma *= 2.2;
    params.csp_params.bgamma *= 2.2;
  }
  glSetupYUVConversion(&params);
  if (custom_prog) {
    FILE *f = fopen(custom_prog, "rb");
    if (!f) {
      mp_msg(MSGT_VO, MSGL_WARN,
             "[gl] Could not read customprog %s\n", custom_prog);
    } else {
      char *prog = calloc(1, MAX_CUSTOM_PROG_SIZE + 1);
      fread(prog, 1, MAX_CUSTOM_PROG_SIZE, f);
      fclose(f);
      loadGPUProgram(GL_FRAGMENT_PROGRAM, prog);
      free(prog);
    }
    mpglProgramEnvParameter4f(GL_FRAGMENT_PROGRAM, 0,
               1.0 / texture_width, 1.0 / texture_height,
               texture_width, texture_height);
  }
  if (custom_tex) {
    FILE *f = fopen(custom_tex, "rb");
    if (!f) {
      mp_msg(MSGT_VO, MSGL_WARN,
             "[gl] Could not read customtex %s\n", custom_tex);
    } else {
      int width, height, maxval;
      mpglActiveTexture(GL_TEXTURE3);
      if (glCreatePPMTex(custom_trect?GL_TEXTURE_RECTANGLE:GL_TEXTURE_2D, 0,
                         custom_tlin?GL_LINEAR:GL_NEAREST,
                         f, &width, &height, &maxval)) {
        mpglProgramEnvParameter4f(GL_FRAGMENT_PROGRAM, 1,
                   1.0 / width, 1.0 / height, width, height);
      } else
        mp_msg(MSGT_VO, MSGL_WARN,
               "[gl] Error parsing customtex %s\n", custom_tex);
      fclose(f);
      mpglActiveTexture(GL_TEXTURE0);
    }
  }
}

/**
 * \brief remove all OSD textures and display-lists, thus clearing it.
 */
static void clearOSD(void) {
  if (!osdtexCnt)
    return;
  mpglDeleteTextures(osdtexCnt, osdtex);
#ifndef FAST_OSD
  mpglDeleteTextures(osdtexCnt, osdatex);
#endif
  osdtexCnt = 0;
}

/**
 * \brief remove textures, display list and free memory used by EOSD
 */
static void clearEOSD(void) {
  if (eosdDispList)
    mpglDeleteLists(eosdDispList, 1);
  eosdDispList = 0;
  if (eosdtexCnt)
    mpglDeleteTextures(eosdtexCnt, eosdtex);
  eosdtexCnt = 0;
  free(eosdtex);
  eosdtex = NULL;
}

static inline int is_tinytex(struct mp_eosd_image *i, int tinytexcur) {
  return i->w < TINYTEX_SIZE && i->h < TINYTEX_SIZE && tinytexcur < TINYTEX_MAX;
}

static inline int is_smalltex(struct mp_eosd_image *i, int smalltexcur) {
  return i->w < SMALLTEX_SIZE && i->h < SMALLTEX_SIZE && smalltexcur < SMALLTEX_MAX;
}

static inline void tinytex_pos(int tinytexcur, int *x, int *y) {
  *x = (tinytexcur % TINYTEX_COLS) * TINYTEX_SIZE;
  *y = (tinytexcur / TINYTEX_COLS) * TINYTEX_SIZE;
}

static inline void smalltex_pos(int smalltexcur, int *x, int *y) {
  *x = (smalltexcur % SMALLTEX_COLS) * SMALLTEX_SIZE;
  *y = (smalltexcur / SMALLTEX_COLS) * SMALLTEX_SIZE;
}

/**
 * \brief construct display list from ass image list
 * \param img image list to create OSD from.
 *            A value of NULL has the same effect as clearEOSD()
 */
static void genEOSD(struct mp_eosd_image_list *imgs) {
  int sx, sy;
  int tinytexcur = 0;
  int smalltexcur = 0;
  GLuint *curtex;
  GLint scale_type = scaled_osd ? GL_LINEAR : GL_NEAREST;
  struct mp_eosd_image *img = eosd_image_first(imgs);
  struct mp_eosd_image *i;

  if (imgs->changed == 0) // there are elements, but they are unchanged
      return;
  if (img && imgs->changed == 1) // there are elements, but they just moved
      goto skip_upload;

  clearEOSD();
  if (!img)
    return;
  if (!largeeosdtex[0]) {
    mpglGenTextures(2, largeeosdtex);
    mpglBindTexture(gl_target, largeeosdtex[0]);
    glCreateClearTex(gl_target, GL_ALPHA, GL_ALPHA, GL_UNSIGNED_BYTE, scale_type, LARGE_EOSD_TEX_SIZE, LARGE_EOSD_TEX_SIZE, 0);
    mpglBindTexture(gl_target, largeeosdtex[1]);
    glCreateClearTex(gl_target, GL_ALPHA, GL_ALPHA, GL_UNSIGNED_BYTE, scale_type, LARGE_EOSD_TEX_SIZE, LARGE_EOSD_TEX_SIZE, 0);
  }
  for (i = img; i; i = eosd_image_next(imgs))
  {
    if (i->w <= 0 || i->h <= 0 || i->stride < i->w)
      continue;
    if (is_tinytex(i, tinytexcur))
      tinytexcur++;
    else if (is_smalltex(i, smalltexcur))
      smalltexcur++;
    else
      eosdtexCnt++;
  }
  mp_msg(MSGT_VO, MSGL_DBG2, "EOSD counts (tiny, small, all): %i, %i, %i\n",
         tinytexcur, smalltexcur, eosdtexCnt);
  if (eosdtexCnt) {
    eosdtex = calloc(eosdtexCnt, sizeof(GLuint));
    mpglGenTextures(eosdtexCnt, eosdtex);
  }
  tinytexcur = smalltexcur = 0;
  for (i = eosd_image_first(imgs), curtex = eosdtex; i; i = eosd_image_next(imgs)) {
    int x = 0, y = 0;
    if (i->w <= 0 || i->h <= 0 || i->stride < i->w) {
      mp_msg(MSGT_VO, MSGL_V, "Invalid dimensions OSD for part!\n");
      continue;
    }
    if (is_tinytex(i, tinytexcur)) {
      tinytex_pos(tinytexcur, &x, &y);
      mpglBindTexture(gl_target, largeeosdtex[0]);
      tinytexcur++;
    } else if (is_smalltex(i, smalltexcur)) {
      smalltex_pos(smalltexcur, &x, &y);
      mpglBindTexture(gl_target, largeeosdtex[1]);
      smalltexcur++;
    } else {
      texSize(i->w, i->h, &sx, &sy);
      mpglBindTexture(gl_target, *curtex++);
      glCreateClearTex(gl_target, GL_ALPHA, GL_ALPHA, GL_UNSIGNED_BYTE, scale_type, sx, sy, 0);
    }
    glUploadTex(gl_target, GL_ALPHA, GL_UNSIGNED_BYTE, i->bitmap, i->stride,
                x, y, i->w, i->h, 0);
  }
  eosdDispList = mpglGenLists(1);
skip_upload:
  mpglNewList(eosdDispList, GL_COMPILE);
  tinytexcur = smalltexcur = 0;
  for (i = eosd_image_first(imgs), curtex = eosdtex; i; i = eosd_image_next(imgs)) {
    int x = 0, y = 0;
    if (i->w <= 0 || i->h <= 0 || i->stride < i->w)
      continue;
    mpglColor4ub(i->color >> 24, (i->color >> 16) & 0xff, (i->color >> 8) & 0xff, 255 - (i->color & 0xff));
    if (is_tinytex(i, tinytexcur)) {
      tinytex_pos(tinytexcur, &x, &y);
      sx = sy = LARGE_EOSD_TEX_SIZE;
      mpglBindTexture(gl_target, largeeosdtex[0]);
      tinytexcur++;
    } else if (is_smalltex(i, smalltexcur)) {
      smalltex_pos(smalltexcur, &x, &y);
      sx = sy = LARGE_EOSD_TEX_SIZE;
      mpglBindTexture(gl_target, largeeosdtex[1]);
      smalltexcur++;
    } else {
      texSize(i->w, i->h, &sx, &sy);
      mpglBindTexture(gl_target, *curtex++);
    }
    glDrawTex(i->dst_x, i->dst_y, i->w, i->h, x, y, i->w, i->h, sx, sy, using_tex_rect, 0, 0, 0);
  }
  mpglEndList();
  mpglBindTexture(gl_target, 0);
}

/**
 * \brief uninitialize OpenGL context, freeing textures, buffers etc.
 */
static void uninitGl(void) {
  int i = 0;
  mp_msg(MSGT_VO, MSGL_V, "Drawn %i frames, %i using DR, DR refused %i\n",
         imgcnt, dr_imgcnt, dr_rejectcnt);
  if (mpglDeletePrograms && fragprog)
    mpglDeletePrograms(1, &fragprog);
  fragprog = 0;
  while (default_texs[i] != 0)
    i++;
  if (i)
    mpglDeleteTextures(i, default_texs);
  default_texs[0] = 0;
  clearOSD();
  clearEOSD();
  if (largeeosdtex[0])
    mpglDeleteTextures(2, largeeosdtex);
  largeeosdtex[0] = 0;
  if (mpglDeleteBuffers && gl_buffer)
    mpglDeleteBuffers(1, &gl_buffer);
  gl_buffer = 0; gl_buffersize = 0;
  gl_bufferptr = NULL;
  if (mpglDeleteBuffers && gl_buffer_uv[0])
    mpglDeleteBuffers(2, gl_buffer_uv);
  gl_buffer_uv[0] = gl_buffer_uv[1] = 0; gl_buffersize_uv = 0;
  gl_bufferptr_uv[0] = gl_bufferptr_uv[1] = 0;
#ifdef CONFIG_GL_X11
  if (mesa_bufferptr)
    mpglFreeMemoryMESA(mDisplay, mScreen, mesa_bufferptr);
#endif
  mesa_bufferptr = NULL;
  err_shown = 0;
}

static int isSoftwareGl(void)
{
  const char *renderer = mpglGetString(GL_RENDERER);
  return !renderer || strcmp(renderer, "Software Rasterizer") == 0 ||
         strstr(renderer, "Software Renderer") ||
         strstr(renderer, "llvmpipe");
}

static void autodetectGlExtensions(void) {
  const char *extensions = mpglGetString(GL_EXTENSIONS);
  const char *vendor     = mpglGetString(GL_VENDOR);
  const char *version    = mpglGetString(GL_VERSION);
  const char *renderer   = mpglGetString(GL_RENDERER);
  int is_ati = vendor && strstr(vendor, "ATI") != NULL;
  int ati_broken_pbo = 0;
  mp_msg(MSGT_VO, MSGL_V, "[gl] Running on OpenGL '%s' by '%s', version '%s'\n", renderer, vendor, version);
  if (is_ati && strncmp(version, "2.1.", 4) == 0) {
    int ver = atoi(version + 4);
    mp_msg(MSGT_VO, MSGL_V, "[gl] Detected ATI driver version: %i\n", ver);
    ati_broken_pbo = ver && ver < 8395;
  }
  if (ati_hack      == -1) ati_hack      = ati_broken_pbo;
  if (force_pbo     == -1) {
    force_pbo = 0;
    // memcpy is just too slow at least on PPC.
    // PBO is vastly faster on Apple Silicon, assume that is the
    // same for all AArch64 SOCs
    if ((ARCH_X86 || ARCH_AARCH64) && extensions && strstr(extensions, "_pixel_buffer_object"))
      force_pbo = is_ati || ARCH_AARCH64;
  }
  if (use_rectangle == -1) {
    use_rectangle = 0;
    if (extensions) {
//      if (strstr(extensions, "_texture_non_power_of_two"))
      if (strstr(extensions, "_texture_rectangle"))
        use_rectangle = renderer && strstr(renderer, "Mesa DRI R200") ? 1 : 0;
    }
  }
  if (use_osd == -1)
    use_osd = mpglBindTexture != NULL;
  if (use_yuv == -1) {
    use_yuv = glAutodetectYUVConversion();
    if (stereo_mode == GL_3D_STIPPLE &&
        !glYUVSupportsAlphaTex(use_yuv))
      use_yuv = 0;
  }
  if (use_ycbcr == -1)
    use_ycbcr = extensions && strstr(extensions, "ycbcr_422");
  if (is_ati && (lscale == 1 || lscale == 2 || cscale == 1 || cscale == 2))
    mp_msg(MSGT_VO, MSGL_WARN, "[gl] Selected scaling mode may be broken on ATI cards.\n"
             "Tell _them_ to fix GL_REPEAT if you have issues.\n");
  mp_msg(MSGT_VO, MSGL_V, "[gl] Settings after autodetection: ati-hack = %i, force-pbo = %i, rectangle = %i, yuv = %i, ycbcr = %i\n",
         ati_hack, force_pbo, use_rectangle, use_yuv, use_ycbcr);
}

static GLint get_scale_type(int chroma) {
  int nearest = (chroma ? cscale : lscale) & 64;
  if (nearest)
    return mipmap_gen ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;
  return mipmap_gen ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR;
}

/**
 * \brief Initialize a (new or reused) OpenGL context.
 * set global gl-related variables to their default values
 */
static int initGl(void) {
  GLint scale_type = get_scale_type(0);
  autodetectGlExtensions();
  using_tex_rect = gl_format == GL_YCBCR_422_APPLE || use_rectangle == 1;
  gl_target = using_tex_rect ? GL_TEXTURE_RECTANGLE : GL_TEXTURE_2D;
  yuvconvtype = SET_YUV_CONVERSION(use_yuv) |
                SET_YUV_LUM_SCALER(lscale) |
                SET_YUV_CHROM_SCALER(cscale);

  texSize(image_width, image_height, &texture_width, &texture_height);

  mpglDisable(GL_BLEND);
  mpglDisable(GL_DEPTH_TEST);
  mpglDepthMask(GL_FALSE);
  mpglDisable(GL_CULL_FACE);
  mpglEnable(gl_target);
  if (mpglDrawBuffer)
    mpglDrawBuffer(vo_doublebuffering?GL_BACK:GL_FRONT);
  mpglTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  mp_msg(MSGT_VO, MSGL_V, "[gl] Creating %dx%d texture...\n",
          texture_width, texture_height);

  glCreateClearTex(gl_target, gl_texfmt, gl_format, gl_type, scale_type,
                   texture_width, texture_height, 0);
  if (mipmap_gen)
    mpglTexParameteri(gl_target, GL_GENERATE_MIPMAP, GL_TRUE);

  if (is_yuv || is_xyz || custom_prog || stereo_mode == GL_3D_STIPPLE) {
    int i;
    mpglGenTextures(21, default_texs);
    default_texs[21] = 0;
    for (i = 0; i < 7; i++) {
      mpglActiveTexture(GL_TEXTURE1 + i);
      mpglBindTexture(GL_TEXTURE_2D, default_texs[i]);
      mpglBindTexture(GL_TEXTURE_RECTANGLE, default_texs[i + 7]);
      mpglBindTexture(GL_TEXTURE_3D, default_texs[i + 14]);
    }
    mpglActiveTexture(GL_TEXTURE0);
  }
  if (stereo_mode == GL_3D_STIPPLE)
    glSetupAlphaStippleTex(stipple);
  if (is_yuv) {
    int xs, ys, depth;
    int chroma_clear_val = 128;
    scale_type = get_scale_type(1);
    mp_get_chroma_shift(image_format, &xs, &ys, &depth);
    chroma_clear_val >>= -depth & 7;
    mpglActiveTexture(GL_TEXTURE1);
    glCreateClearTex(gl_target, gl_texfmt, gl_format, gl_type, scale_type,
                     texture_width >> xs, texture_height >> ys,
                     chroma_clear_val);
    if (mipmap_gen)
      mpglTexParameteri(gl_target, GL_GENERATE_MIPMAP, GL_TRUE);
    mpglActiveTexture(GL_TEXTURE2);
    glCreateClearTex(gl_target, gl_texfmt, gl_format, gl_type, scale_type,
                     texture_width >> xs, texture_height >> ys,
                     chroma_clear_val);
    if (mipmap_gen)
      mpglTexParameteri(gl_target, GL_GENERATE_MIPMAP, GL_TRUE);
    mpglActiveTexture(GL_TEXTURE0);
    mpglBindTexture(gl_target, 0);
  }
  if (is_yuv || is_xyz || custom_prog)
  {
    if ((MASK_NOT_COMBINERS & (1 << use_yuv)) || custom_prog) {
      if (!mpglGenPrograms || !mpglBindProgram) {
        mp_msg(MSGT_VO, MSGL_ERR, "[gl] fragment program functions missing!\n");
      } else {
        mpglGenPrograms(1, &fragprog);
        mpglBindProgram(GL_FRAGMENT_PROGRAM, fragprog);
      }
    }
    update_yuvconv();
  }

  resize();

  mpglClearColor( 0.0f,0.0f,0.0f,0.0f );
  mpglClear( GL_COLOR_BUFFER_BIT );
  if (mpglSwapInterval && swap_interval >= 0)
    mpglSwapInterval(swap_interval);
  return 1;
}

static int create_window(uint32_t d_width, uint32_t d_height, uint32_t flags, const char *title)
{
  if (stereo_mode == GL_3D_QUADBUFFER)
    flags |= VOFLAG_STEREO;
#ifdef CONFIG_GL_SDL
  if (glctx.type == GLTYPE_SDL) {
    // Ugly to do this here, but SDL ignores it if set later
    if (swap_interval >= 0) {
#if SDL_VERSION_ATLEAST(1, 3, 0)
      SDL_GL_SetSwapInterval(swap_interval);
#elif SDL_VERSION_ATLEAST(1, 2, 10)
      SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, swap_interval);
#endif
    }
  }
#endif
  return mpglcontext_create_window(&glctx, d_width, d_height, flags, title);
}

/* connect to server, create and map window,
 * allocate colors and (shared) memory
 */
static int
config(uint32_t width, uint32_t height, uint32_t d_width, uint32_t d_height, uint32_t flags, char *title, uint32_t format)
{
  int xs, ys;
  image_height = height;
  image_width = width;
  image_format = format;
  is_yuv = mp_get_chroma_shift(image_format, &xs, &ys, NULL) > 0;
  is_yuv |= (xs << 8) | (ys << 16);
  is_xyz = IMGFMT_IS_XYZ(image_format);
  glFindFormat(format, NULL, &gl_texfmt, &gl_format, &gl_type);

  vo_flipped = !!(flags & VOFLAG_FLIPPING);

  if (create_window(d_width, d_height, flags, title) < 0)
    return -1;

  if (vo_config_count)
    uninitGl();
  if (glctx.setGlWindow(&glctx) == SET_WINDOW_FAILED)
    return -1;
  if (mesa_buffer && !mpglAllocateMemoryMESA) {
    mp_msg(MSGT_VO, MSGL_ERR, "Can not enable mesa-buffer because AllocateMemoryMESA was not found\n");
    mesa_buffer = 0;
  }
  initGl();

#ifdef CONFIG_GL_OSX
  vo_osx_redraw_func = resize;
#endif
  return 0;
}

static void check_events(void)
{
    int e=glctx.check_events();
    if(e&VO_EVENT_REINIT) {
        uninitGl();
        initGl();
    }
    if(e&VO_EVENT_RESIZE) resize();
    else if(e&VO_EVENT_EXPOSE) redraw();
}

/**
 * Creates the textures and the display list needed for displaying
 * an OSD part.
 * Callback function for vo_draw_text().
 */
static void create_osd_texture(int x0, int y0, int w, int h,
                                 unsigned char *src, unsigned char *srca,
                                 int stride)
{
  // initialize to 8 to avoid special-casing on alignment
  int sx = 8, sy = 8;
  GLint scale_type = scaled_osd ? GL_LINEAR : GL_NEAREST;

  if (w <= 0 || h <= 0 || stride < w) {
    mp_msg(MSGT_VO, MSGL_V, "Invalid dimensions OSD for part!\n");
    return;
  }
  texSize(w, h, &sx, &sy);

  if (osdtexCnt >= MAX_OSD_PARTS) {
    mp_msg(MSGT_VO, MSGL_ERR, "Too many OSD parts, contact the developers!\n");
    return;
  }

  // create Textures for OSD part
  mpglGenTextures(1, &osdtex[osdtexCnt]);
  mpglBindTexture(gl_target, osdtex[osdtexCnt]);
  glCreateClearTex(gl_target, GL_LUMINANCE, GL_LUMINANCE, GL_UNSIGNED_BYTE, scale_type, sx, sy, 0);
  glUploadTex(gl_target, GL_LUMINANCE, GL_UNSIGNED_BYTE, src, stride,
              0, 0, w, h, 0);

#ifndef FAST_OSD
  mpglGenTextures(1, &osdatex[osdtexCnt]);
  mpglBindTexture(gl_target, osdatex[osdtexCnt]);
  glCreateClearTex(gl_target, GL_ALPHA, GL_ALPHA, GL_UNSIGNED_BYTE, scale_type, sx, sy, 0);
  {
  int i;
  char *tmp = malloc(stride * h);
  // convert alpha from weird MPlayer scale.
  // in-place is not possible since it is reused for future OSDs
  for (i = h * stride - 1; i >= 0; i--)
    tmp[i] = -srca[i];
  glUploadTex(gl_target, GL_ALPHA, GL_UNSIGNED_BYTE, tmp, stride,
              0, 0, w, h, 0);
  free(tmp);
  }
#endif

  mpglBindTexture(gl_target, 0);

  osdcoords[osdtexCnt] = (struct osdcoords){x0, y0, w, h, sx, sy};

  osdtexCnt++;
}

#define RENDER_OSD  1
#define RENDER_EOSD 2

/**
 * \param type bit 0: render OSD, bit 1: render EOSD
 */
static void do_render_osd(int type) {
  int i;
  int draw_osd  = (type & RENDER_OSD)  && osdtexCnt > 0;
  int draw_eosd = (type & RENDER_EOSD) && eosdDispList;
  if (!draw_osd && !draw_eosd)
    return;
  // set special rendering parameters
  mpglLoadMatrixf(osd_matrix);
  mpglEnable(GL_BLEND);
  if (draw_eosd) {
    mpglTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    mpglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    mpglCallList(eosdDispList);
    mpglTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  }
  if (draw_osd) {
    if (osd_color != 0xffffff)
      mpglTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    mpglColor4ub((osd_color >> 16) & 0xff, (osd_color >> 8) & 0xff, osd_color & 0xff, 0xff - (osd_color >> 24));
    // draw OSD
#ifndef FAST_OSD
    // render alpha
    mpglBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
    for (i = 0; i < osdtexCnt; i++) {
      struct osdcoords *c = osdcoords + i;
      mpglBindTexture(gl_target, osdatex[i]);
      glDrawTex(c->x, c->y, c->w, c->h, 0, 0, c->w, c->h, c->sx, c->sy, using_tex_rect, 0, 0, 0);
    }
#endif
    // render luminance OSD
    mpglBlendFunc(GL_SRC_ALPHA, GL_ONE);
    for (i = 0; i < osdtexCnt; i++) {
      struct osdcoords *c = osdcoords + i;
      mpglBindTexture(gl_target, osdtex[i]);
      glDrawTex(c->x, c->y, c->w, c->h, 0, 0, c->w, c->h, c->sx, c->sy, using_tex_rect, 0, 0, 0);
    }
    if (osd_color != 0xffffff)
      mpglTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  }
  // set rendering parameters back to defaults
  mpglDisable(GL_BLEND);
  mpglLoadMatrixf(video_matrix);
  mpglBindTexture(gl_target, 0);
}

static void draw_osd(void)
{
  if (!use_osd) return;
  if (vo_osd_changed(0)) {
    int osd_h, osd_w;
    clearOSD();
    osd_w = scaled_osd ? image_width  : draw_width;
    osd_h = scaled_osd ? image_height : draw_height;
    vo_draw_text_ext(osd_w, osd_h, ass_border_l, ass_border_t, ass_border_r, ass_border_b,
                     image_width, image_height, create_osd_texture);
  }
  if (vo_doublebuffering) do_render_osd(RENDER_OSD);
}

static void do_render(void) {
  mpglColor4ub(255, 255, 255, 255);
  if (is_yuv || is_xyz || custom_prog)
    glEnableYUVConversion(gl_target, yuvconvtype);
  if (stereo_mode) {
    glEnable3DLeft(stereo_mode);
    glDrawTex(0, 0, image_width, image_height,
              0, 0, image_width >> 1, image_height,
              texture_width, texture_height,
              using_tex_rect, is_yuv,
              mpi_flipped ^ vo_flipped, stereo_mode == GL_3D_STIPPLE);
    glEnable3DRight(stereo_mode);
    glDrawTex(0, 0, image_width, image_height,
              image_width >> 1, 0, image_width >> 1, image_height,
              texture_width, texture_height,
              using_tex_rect, is_yuv,
              mpi_flipped ^ vo_flipped, stereo_mode == GL_3D_STIPPLE);
    glDisable3D(stereo_mode);
  } else {
    glDrawTex(0, 0, image_width, image_height,
              0, 0, image_width, image_height,
              texture_width, texture_height,
              using_tex_rect, is_yuv,
              mpi_flipped ^ vo_flipped, 0);
  }
  if (is_yuv || is_xyz || custom_prog)
    glDisableYUVConversion(gl_target, yuvconvtype);
  did_render = 1;
}

static void flip_page(void) {
  int need_clear = aspect_scaling() && use_aspect;
  // We might get an expose event between draw_image and its
  // corresponding flip_page.
  // For double-buffering we would then flip in a clear backbuffer.
  // Easiest way to handle it is by keeping track if the
  // current GL buffer contains a properly rendered video.
  // did_render will always be false for single buffer.
  if (!did_render) {
    if (!vo_doublebuffering && need_clear)
      mpglClear(GL_COLOR_BUFFER_BIT);
    do_render();
    do_render_osd(RENDER_OSD | RENDER_EOSD);
  }
  if (use_glFinish) mpglFinish();
  else if (glctx.type == GLTYPE_OSX && vo_doublebuffering && !is_yuv)
    // At least on PPC Mac Mini this combination leads to
    // no image at all being show, however for the planar YUV
    // case the flush causes a significant slowdown.
    // Note that always using single-buffering is not a good solution since
    // it causes artefacts with planar YUV.
    mpglFlush();

  if (vo_doublebuffering) {
    glctx.swapGlBuffers(&glctx);
    if (need_clear)
      mpglClear(GL_COLOR_BUFFER_BIT);
  } else if (!use_glFinish)
    mpglFlush();

  did_render = 0;
}

static void redraw(void) {
  flip_page();
}

static int draw_slice(uint8_t *src[], int stride[], int w,int h,int x,int y)
{
  mpi_flipped = stride[0] < 0;
  glUploadTex(gl_target, gl_format, gl_type, src[0], stride[0],
              x, y, w, h, slice_height);
  if (is_yuv) {
    int xs, ys;
    mp_get_chroma_shift(image_format, &xs, &ys, NULL);
    mpglActiveTexture(GL_TEXTURE1);
    glUploadTex(gl_target, gl_format, gl_type, src[1], stride[1],
                x >> xs, y >> ys, w >> xs, h >> ys, slice_height);
    mpglActiveTexture(GL_TEXTURE2);
    glUploadTex(gl_target, gl_format, gl_type, src[2], stride[2],
                x >> xs, y >> ys, w >> xs, h >> ys, slice_height);
    mpglActiveTexture(GL_TEXTURE0);
  }
  return 0;
}

static int get_pbo_image(mp_image_t *mpi) {
  int needed_size;
  if (!mpglGenBuffers || !mpglBindBuffer || !mpglBufferData || !mpglMapBufferRange) {
    if (!err_shown)
      mp_msg(MSGT_VO, MSGL_ERR, "[gl] extensions missing for dr\n"
                                "Expect a _major_ speed penalty\n");
    err_shown = 1;
    return 0;
  }
  mpi->stride[0] = mpi->width * mpi->bpp / 8;
  needed_size = mpi->stride[0] * mpi->height + 31;
  if (mesa_buffer) {
#ifdef CONFIG_GL_X11
    if (mesa_bufferptr && needed_size > mesa_buffersize) {
      mpglFreeMemoryMESA(mDisplay, mScreen, mesa_bufferptr);
      mesa_bufferptr = NULL;
    }
    if (!mesa_bufferptr)
      mesa_bufferptr = mpglAllocateMemoryMESA(mDisplay, mScreen, needed_size, 0, 1.0, 1.0);
    mesa_buffersize = needed_size;
#endif
    mpi->planes[0] = mesa_bufferptr;
  } else {
    if (!gl_buffer)
      mpglGenBuffers(1, &gl_buffer);
    mpglBindBuffer(GL_PIXEL_UNPACK_BUFFER, gl_buffer);
    if (needed_size > gl_buffersize) {
      gl_buffersize = needed_size;
      mpglBufferData(GL_PIXEL_UNPACK_BUFFER, gl_buffersize,
                     NULL, GL_DYNAMIC_DRAW);
    }
    if (!gl_bufferptr)
      gl_bufferptr = mpglMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, needed_size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    mpi->priv = gl_bufferptr;
    mpi->planes[0] = (uint8_t *)gl_bufferptr + (-(intptr_t)gl_bufferptr & 31);
    mpglBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
  }
  if (!mpi->planes[0]) {
    if (!err_shown)
      mp_msg(MSGT_VO, MSGL_ERR, "[gl] could not acquire buffer for dr\n"
                                "Expect a _major_ speed penalty\n");
    err_shown = 1;
    return 0;
  }
  if (is_yuv) {
    // planar YUV
    int xs, ys;
    int bpp;
    mp_get_chroma_shift(image_format, &xs, &ys, &bpp);
    bpp = (bpp + 7) / 8;
    mpi->flags |= MP_IMGFLAG_COMMON_STRIDE | MP_IMGFLAG_COMMON_PLANE;
    mpi->stride[0] = mpi->width * bpp;
    mpi->planes[1] = mpi->planes[0] + mpi->stride[0] * mpi->height;
    mpi->stride[1] = (mpi->width >> xs) * bpp;
    mpi->planes[2] = mpi->planes[1] + mpi->stride[1] * (mpi->height >> ys);
    mpi->stride[2] = mpi->stride[1];
    if (ati_hack && !mesa_buffer) {
      mpi->flags &= ~MP_IMGFLAG_COMMON_PLANE;
      if (!gl_buffer_uv[0]) mpglGenBuffers(2, gl_buffer_uv);
      if (mpi->stride[1] * mpi->height > gl_buffersize_uv) {
        mpglBindBuffer(GL_PIXEL_UNPACK_BUFFER, gl_buffer_uv[0]);
        mpglBufferData(GL_PIXEL_UNPACK_BUFFER, mpi->stride[1] * mpi->height,
                       NULL, GL_DYNAMIC_DRAW);
        mpglBindBuffer(GL_PIXEL_UNPACK_BUFFER, gl_buffer_uv[1]);
        mpglBufferData(GL_PIXEL_UNPACK_BUFFER, mpi->stride[1] * mpi->height,
                       NULL, GL_DYNAMIC_DRAW);
        gl_buffersize_uv = mpi->stride[1] * mpi->height;
      }
      if (!gl_bufferptr_uv[0]) {
        mpglBindBuffer(GL_PIXEL_UNPACK_BUFFER, gl_buffer_uv[0]);
        gl_bufferptr_uv[0] = mpglMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, mpi->stride[1] * mpi->height, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
        mpglBindBuffer(GL_PIXEL_UNPACK_BUFFER, gl_buffer_uv[1]);
        gl_bufferptr_uv[1] = mpglMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, mpi->stride[1] * mpi->height, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
      }
      mpi->planes[1] = gl_bufferptr_uv[0];
      mpi->planes[2] = gl_bufferptr_uv[1];
    }
  }
  return 1;
}

static uint32_t get_image(mp_image_t *mpi) {
  dr_rejectcnt++;
  if (gl_bufferptr) return VO_FALSE;
  if (mpi->flags & MP_IMGFLAG_READABLE) return VO_FALSE;
  if (mpi->type != MP_IMGTYPE_STATIC && mpi->type != MP_IMGTYPE_TEMP &&
      mpi->type != MP_IMGTYPE_IPB &&
      mpi->type != MP_IMGTYPE_NUMBERED)
    return VO_FALSE;
  if (mesa_buffer) mpi->width = texture_width;
  else if (ati_hack) {
    mpi->width = texture_width;
    mpi->height = texture_height;
  }
  if (!get_pbo_image(mpi))
    return VO_FALSE;
  mpi->flags |= MP_IMGFLAG_DIRECT;
  dr_rejectcnt--;
  return VO_TRUE;
}

static void clear_border(uint8_t *dst, int start, int stride, int height, int full_height, int value) {
  int right_border = stride - start;
  int bottom_border = full_height - height;
  while (height > 0) {
    memset(dst + start, value, right_border);
    dst += stride;
    height--;
  }
  if (bottom_border > 0)
    memset(dst, value, stride * bottom_border);
}

static uint32_t draw_image(mp_image_t *mpi) {
  // Ask for TexImage instead of TexSubImage for the rectangle + YCBCR + nodr special case.
  int slice = gl_target == GL_TEXTURE_RECTANGLE && gl_format == GL_YCBCR_422_APPLE ? -1 : slice_height;
  int stride[3];
  unsigned char *planes[3];
  mp_image_t mpi2 = *mpi;
  int w = mpi->w, h = mpi->h;
  imgcnt++;
  if (mpi->flags & MP_IMGFLAG_DRAW_CALLBACK)
    goto skip_upload;
  dr_imgcnt += !!(mpi->flags & MP_IMGFLAG_DIRECT);
  mpi2.flags = 0; mpi2.type = MP_IMGTYPE_TEMP;
  mpi2.width = mpi2.w; mpi2.height = mpi2.h;
  if (force_pbo && !(mpi->flags & MP_IMGFLAG_DIRECT) && !gl_bufferptr && get_image(&mpi2) == VO_TRUE) {
    int bpp;
    int line_bytes, line_bytes_c;
    int xs, ys;
    mp_get_chroma_shift(image_format, &xs, &ys, &bpp);
    bpp = is_yuv ? (bpp + 7) & ~7 : mpi->bpp;
    line_bytes = mpi->w * bpp / 8;
    line_bytes_c = (mpi->w >> xs) * bpp / 8;
    memcpy_pic(mpi2.planes[0], mpi->planes[0], line_bytes, mpi->h, mpi2.stride[0], mpi->stride[0]);
    if (is_yuv) {
      memcpy_pic(mpi2.planes[1], mpi->planes[1], line_bytes_c, mpi->h >> ys, mpi2.stride[1], mpi->stride[1]);
      memcpy_pic(mpi2.planes[2], mpi->planes[2], line_bytes_c, mpi->h >> ys, mpi2.stride[2], mpi->stride[2]);
    }
    if (ati_hack) { // since we have to do a full upload we need to clear the borders
      clear_border(mpi2.planes[0], line_bytes, mpi2.stride[0], mpi->h, mpi2.height, 0);
      if (is_yuv) {
        clear_border(mpi2.planes[1], line_bytes_c, mpi2.stride[1], mpi->h >> ys, mpi2.height >> ys, 128);
        clear_border(mpi2.planes[2], line_bytes_c, mpi2.stride[2], mpi->h >> ys, mpi2.height >> ys, 128);
      }
    }
    mpi = &mpi2;
  }
  stride[0] = mpi->stride[0]; stride[1] = mpi->stride[1]; stride[2] = mpi->stride[2];
  planes[0] = mpi->planes[0]; planes[1] = mpi->planes[1]; planes[2] = mpi->planes[2];
  mpi_flipped = stride[0] < 0;
  if (mpi->flags & MP_IMGFLAG_DIRECT) {
    if (mesa_buffer) {
      mpglPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE, 1);
      w = texture_width;
    } else {
      intptr_t base = (intptr_t)mpi->priv;
      if (ati_hack) { w = texture_width; h = texture_height; }
      if (mpi_flipped)
        base += (mpi->h - 1) * stride[0];
      planes[0] -= base;
      planes[1] -= base;
      planes[2] -= base;
      mpglBindBuffer(GL_PIXEL_UNPACK_BUFFER, gl_buffer);
      mpglUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
      gl_bufferptr = NULL;
      if (!(mpi->flags & MP_IMGFLAG_COMMON_PLANE))
        planes[0] = planes[1] = planes[2] = NULL;
    }
    slice = 0; // always "upload" full texture
  }
  glUploadTex(gl_target, gl_format, gl_type, planes[0], stride[0],
              mpi->x, mpi->y, w, h, slice);
  if (is_yuv) {
    int xs, ys;
    mp_get_chroma_shift(image_format, &xs, &ys, NULL);
    if ((mpi->flags & MP_IMGFLAG_DIRECT) && !(mpi->flags & MP_IMGFLAG_COMMON_PLANE)) {
      mpglBindBuffer(GL_PIXEL_UNPACK_BUFFER, gl_buffer_uv[0]);
      mpglUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
      gl_bufferptr_uv[0] = NULL;
    }
    mpglActiveTexture(GL_TEXTURE1);
    glUploadTex(gl_target, gl_format, gl_type, planes[1], stride[1],
                mpi->x >> xs, mpi->y >> ys, w >> xs, h >> ys, slice);
    if ((mpi->flags & MP_IMGFLAG_DIRECT) && !(mpi->flags & MP_IMGFLAG_COMMON_PLANE)) {
      mpglBindBuffer(GL_PIXEL_UNPACK_BUFFER, gl_buffer_uv[1]);
      mpglUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
      gl_bufferptr_uv[1] = NULL;
    }
    mpglActiveTexture(GL_TEXTURE2);
    glUploadTex(gl_target, gl_format, gl_type, planes[2], stride[2],
                mpi->x >> xs, mpi->y >> ys, w >> xs, h >> ys, slice);
    mpglActiveTexture(GL_TEXTURE0);
  }
  if (mpi->flags & MP_IMGFLAG_DIRECT) {
    if (mesa_buffer) mpglPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE, 0);
    else mpglBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
  }
skip_upload:
  if (vo_doublebuffering) do_render();
  return VO_TRUE;
}

static int
query_format(uint32_t format)
{
    int depth;
    int caps = VFCAP_CSP_SUPPORTED | VFCAP_CSP_SUPPORTED_BY_HW |
               VFCAP_FLIP | VFCAP_ACCEPT_STRIDE |
               VFCAP_HWSCALE_UP | VFCAP_HWSCALE_DOWN;
    if (use_osd)
      caps |= VFCAP_OSD | VFCAP_EOSD | (scaled_osd ? 0 : VFCAP_EOSD_UNSCALED);
    if (format == IMGFMT_RGB24 || format == IMGFMT_RGBA)
        return caps;
    if (use_yuv && mp_get_chroma_shift(format, NULL, NULL, &depth) &&
        (depth == 8 || depth == 16 || glYUVLargeRange(use_yuv)) &&
        (IMGFMT_IS_YUVP16_NE(format) || !IMGFMT_IS_YUVP16(format)))
        return caps;
    if ((MASK_NOT_COMBINERS & (1 << use_yuv)) && IMGFMT_IS_XYZ(format))
        return caps;
    // HACK, otherwise we get only b&w with some filters (e.g. -vf eq)
    // ideally MPlayer should be fixed instead not to use Y800 when it has the choice
    if (!use_yuv && (format == IMGFMT_Y8 || format == IMGFMT_Y800))
        return 0;
    if (!use_ycbcr && (format == IMGFMT_YUY2 || format == IMGFMT_UYVY))
        return 0;
    if (many_fmts &&
         glFindFormat(format, NULL, NULL, NULL, NULL))
        return caps;
    return 0;
}


static void
uninit(void)
{
  uninitGl();
  free(custom_prog);
  custom_prog = NULL;
  free(custom_tex);
  custom_tex = NULL;
  uninit_mpglcontext(&glctx);
}

static int valid_backend(void *p)
{
  int *backend = p;
  return *backend >= GLTYPE_AUTO && *backend < GLTYPE_COUNT;
}

static int valid_csp(void *p)
{
  int *csp = p;
  return *csp >= -1 && *csp < MP_CSP_COUNT;
}

static int valid_csp_lvl(void *p)
{
  int *lvl = p;
  return *lvl >= -1 && *lvl < MP_CSP_LEVELCONV_COUNT;
}

static const opt_t subopts[] = {
  {"manyfmts",     OPT_ARG_BOOL, &many_fmts,    NULL},
  {"osd",          OPT_ARG_BOOL, &use_osd,      NULL},
  {"scaled-osd",   OPT_ARG_BOOL, &scaled_osd,   NULL},
  {"aspect",       OPT_ARG_BOOL, &use_aspect,   NULL},
  {"ycbcr",        OPT_ARG_BOOL, &use_ycbcr,    NULL},
  {"slice-height", OPT_ARG_INT,  &slice_height, int_non_neg},
  {"rectangle",    OPT_ARG_INT,  &use_rectangle,int_non_neg},
  {"yuv",          OPT_ARG_INT,  &use_yuv,      int_non_neg},
  {"colorspace",   OPT_ARG_INT,  &colorspace,   valid_csp},
  {"levelconv",    OPT_ARG_INT,  &levelconv,    valid_csp_lvl},
  {"lscale",       OPT_ARG_INT,  &lscale,       int_non_neg},
  {"cscale",       OPT_ARG_INT,  &cscale,       int_non_neg},
  {"filter-strength", OPT_ARG_FLOAT, &filter_strength, NULL},
  {"noise-strength", OPT_ARG_FLOAT, &noise_strength, NULL},
  {"ati-hack",     OPT_ARG_BOOL, &ati_hack,     NULL},
  {"force-pbo",    OPT_ARG_BOOL, &force_pbo,    NULL},
  {"mesa-buffer",  OPT_ARG_BOOL, &mesa_buffer,  NULL},
  {"glfinish",     OPT_ARG_BOOL, &use_glFinish, NULL},
  {"swapinterval", OPT_ARG_INT,  &swap_interval,NULL},
  {"customprog",   OPT_ARG_MSTRZ,&custom_prog,  NULL},
  {"customtex",    OPT_ARG_MSTRZ,&custom_tex,   NULL},
  {"customtlin",   OPT_ARG_BOOL, &custom_tlin,  NULL},
  {"customtrect",  OPT_ARG_BOOL, &custom_trect, NULL},
  {"mipmapgen",    OPT_ARG_BOOL, &mipmap_gen,   NULL},
  {"osdcolor",     OPT_ARG_INT,  &osd_color,    NULL},
  {"stereo",       OPT_ARG_INT,  &stereo_mode,  NULL},
  {"stipple",      OPT_ARG_INT,  &stipple,      NULL},
  {"backend",      OPT_ARG_INT,  &backend,      valid_backend},
  {NULL}
};

static int preinit_internal(const char *arg, int allow_sw)
{
    // set defaults
    backend = GLTYPE_AUTO;
    many_fmts = 1;
    use_osd = -1;
    scaled_osd = 0;
    use_aspect = 1;
    use_ycbcr = -1;
    use_yuv = -1;
    colorspace = -1;
    levelconv = -1;
    lscale = 0;
    cscale = 0;
    filter_strength = 0.5;
    noise_strength = 0.0;
    use_rectangle = -1;
    use_glFinish = 0;
    ati_hack = -1;
    force_pbo = -1;
    mesa_buffer = 0;
    swap_interval = 1;
    slice_height = 0;
    custom_prog = NULL;
    custom_tex = NULL;
    custom_tlin = 1;
    custom_trect = 0;
    mipmap_gen = 0;
    osd_color = 0xffffff;
    stereo_mode = 0;
    stipple = 0x0f0f;
    if (subopt_parse(arg, subopts) != 0) {
      mp_msg(MSGT_VO, MSGL_FATAL,
              "\n-vo gl command line help:\n"
              "Example: mplayer -vo gl:slice-height=4\n"
              "\nOptions:\n"
              "  nomanyfmts\n"
              "    Disable extended color formats for OpenGL 1.2 and later\n"
              "  slice-height=<0-...>\n"
              "    Slice size for texture transfer, 0 for whole image\n"
              "  noosd\n"
              "    Do not use OpenGL OSD code\n"
              "  scaled-osd\n"
              "    Render OSD at movie resolution and scale it\n"
              "  noaspect\n"
              "    Do not do aspect scaling\n"
              "  rectangle=<0,1,2>\n"
              "    0: use power-of-two textures\n"
              "    1: use texture_rectangle\n"
              "    2: use texture_non_power_of_two\n"
              "  ati-hack\n"
              "    Workaround ATI bug with PBOs\n"
              "  force-pbo\n"
              "    Force use of PBO even if this involves an extra memcpy\n"
              "  glfinish\n"
              "    Call glFinish() before swapping buffers\n"
              "  swapinterval=<n>\n"
              "    Interval in displayed frames between to buffer swaps.\n"
              "    1 is equivalent to enable VSYNC, 0 to disable VSYNC.\n"
              "    Requires GLX_SGI_swap_control support to work.\n"
              "  ycbcr\n"
              "    also try to use the GL_APPLE_ycbcr_422 extension\n"
              "  yuv=<n>\n"
              "    0: use software YUV to RGB conversion.\n"
              "    1: use register combiners (nVidia only, for older cards).\n"
              "    2: use fragment program with gamma correction.\n"
              "    3: same as 2.\n"
              "    4: use fragment program with gamma correction via lookup.\n"
              "    5: use ATI-specific method (for older cards).\n"
              "    6: use lookup via 3D texture.\n"
              "  colorspace=<n>\n"
              "    0: MPlayer's default YUV to RGB conversion\n"
              "    1: YUV to RGB according to BT.601\n"
              "    2: YUV to RGB according to BT.709\n"
              "    3: YUV to RGB according to SMPT-240M\n"
              "    4: YUV to RGB according to EBU\n"
              "    5: XYZ to RGB\n"
              "  levelconv=<n>\n"
              "    0: YUV to RGB converting TV to PC levels\n"
              "    1: YUV to RGB converting PC to TV levels\n"
              "    2: YUV to RGB without converting levels\n"
              "  lscale=<n>\n"
              "    0: use standard bilinear scaling for luma.\n"
              "    1: use improved bicubic scaling for luma.\n"
              "    2: use cubic in X, linear in Y direction scaling for luma.\n"
              "    3: as 1 but without using a lookup texture.\n"
              "    4: experimental unsharp masking (sharpening).\n"
              "    5: experimental unsharp masking (sharpening) with larger radius.\n"
              "   64: nearest neighbor scaling.\n"
              "  cscale=<n>\n"
              "    as lscale but for chroma (2x slower with little visible effect).\n"
              "  filter-strength=<value>\n"
              "    set the effect strength for some lscale/cscale filters\n"
              "  noise-strength=<value>\n"
              "    set how much noise to add. 1.0 is suitable for dithering to 6 bit.\n"
              "  customprog=<filename>\n"
              "    use a custom YUV conversion program\n"
              "  customtex=<filename>\n"
              "    use a custom YUV conversion lookup texture\n"
              "  nocustomtlin\n"
              "    use GL_NEAREST scaling for customtex texture\n"
              "  customtrect\n"
              "    use texture_rectangle for customtex texture\n"
              "  mipmapgen\n"
              "    generate mipmaps for the video image (use with TXB in customprog)\n" );
      mp_msg(MSGT_VO, MSGL_FATAL,
              "  osdcolor=<0xAARRGGBB>\n"
              "    use the given color for the OSD\n"
              "  stereo=<n> (add 32 to swap left and right)\n"
              "    0: normal display\n"
              "    1: side-by-side to red-cyan stereo\n"
              "    2: side-by-side to green-magenta stereo\n"
              "    3: side-by-side to quadbuffer stereo\n"
              "    4: side-by-side to pixel pattern\n"
              "  stipple=<0xBBBB>\n"
              "    16 bits representing 4x4 pattern to use for stereo=4\n"
              "  backend=<n>\n"
              "   -1: auto-select\n"
              "    0: Win32/WGL\n"
              "    1: X11/GLX\n"
              "    2: SDL\n"
              "    3: X11/EGL (experimental)\n"
              "    4: OSX/Cocoa\n"
              "    5: Android (experimental)\n"
              "\n" );
      return -1;
    }
    if (!init_mpglcontext(&glctx, backend))
      goto err_out;
    if (use_yuv == -1 || !allow_sw) {
      if (create_window(320, 200, VOFLAG_HIDDEN, NULL) < 0)
        goto err_out;
      if (glctx.setGlWindow(&glctx) != SET_WINDOW_FAILED) {
        if (!allow_sw && isSoftwareGl())
          goto err_out;
        autodetectGlExtensions();
      }
    }
    if (use_yuv == -1)
      use_yuv = glctx.type == GLTYPE_EGL_X11 || glctx.type == GLTYPE_EGL_ANDROID ? YUV_CONVERSION_SL_PROGRAM : YUV_CONVERSION_FRAGMENT_LOOKUP; // mostly sensible fallback
    if (use_ycbcr == -1) {
      // rare feature, not worth creating a window to detect
      use_ycbcr = 0;
    }
    if (many_fmts)
      mp_msg(MSGT_VO, MSGL_INFO, "[gl] using extended formats. "
               "Use -vo gl:nomanyfmts if playback fails.\n");
    mp_msg(MSGT_VO, MSGL_V, "[gl] Using %d as slice height "
             "(0 means image height).\n", slice_height);
    imgcnt = dr_imgcnt = dr_rejectcnt = 0;

    return 0;

err_out:
    uninit();
    return -1;
}

static int preinit(const char *arg)
{
    return preinit_internal(arg, 1);
}

static int preinit_nosw(const char *arg)
{
    return preinit_internal(arg, 0);
}

static const struct {
  const char *name;
  int *value;
  int supportmask;
} eq_map[] = {
  {"brightness",  &eq_bri,    MASK_NOT_COMBINERS},
  {"contrast",    &eq_cont,   MASK_NOT_COMBINERS},
  {"saturation",  &eq_sat,    MASK_ALL_YUV      },
  {"hue",         &eq_hue,    MASK_ALL_YUV      },
  {"gamma",       &eq_rgamma, MASK_GAMMA_SUPPORT},
  {"red_gamma",   &eq_rgamma, MASK_GAMMA_SUPPORT},
  {"green_gamma", &eq_ggamma, MASK_GAMMA_SUPPORT},
  {"blue_gamma",  &eq_bgamma, MASK_GAMMA_SUPPORT},
  {NULL,          NULL,       0                 }
};

static int control(uint32_t request, void *data)
{
  switch (request) {
  case VOCTRL_QUERY_FORMAT:
    return query_format(*(uint32_t*)data);
  case VOCTRL_GET_IMAGE:
    return get_image(data);
  case VOCTRL_DRAW_IMAGE:
    return draw_image(data);
  case VOCTRL_DRAW_EOSD:
    if (!data)
      return VO_FALSE;
    genEOSD(data);
    if (vo_doublebuffering) do_render_osd(RENDER_EOSD);
    return VO_TRUE;
  case VOCTRL_GET_EOSD_RES:
    {
      struct mp_eosd_settings *r = data;
      r->w = draw_width; r->h = draw_height;
      r->srcw = image_width; r->srch = image_height;
      r->mt = r->mb = r->ml = r->mr = 0;
      if (scaled_osd) {r->w = image_width; r->h = image_height;}
      else if (aspect_scaling()) {
        r->ml = ass_border_l;
        r->mr = ass_border_r;
        r->mt = ass_border_t;
        r->mb = ass_border_b;
      }
    }
    return VO_TRUE;
  case VOCTRL_GUISUPPORT:
    return VO_TRUE;
  case VOCTRL_ONTOP:
    glctx.ontop();
    return VO_TRUE;
  case VOCTRL_FULLSCREEN:
    glctx.fullscreen();
    resize();
    return VO_TRUE;
  case VOCTRL_BORDER:
    glctx.border();
    resize();
    return VO_TRUE;
  case VOCTRL_GET_PANSCAN:
    if (!use_aspect) return VO_NOTIMPL;
    return VO_TRUE;
  case VOCTRL_SET_PANSCAN:
    if (!use_aspect) return VO_NOTIMPL;
    resize();
    return VO_TRUE;
  case VOCTRL_GET_EQUALIZER:
    if (is_yuv) {
      int i;
      vf_equalizer_t *eq=data;
      for (i = 0; eq_map[i].name; i++)
        if (strcmp(eq->item, eq_map[i].name) == 0) break;
      if (!(eq_map[i].supportmask & (1 << use_yuv)))
        break;
      eq->value = *eq_map[i].value;
      return VO_TRUE;
    }
    break;
  case VOCTRL_SET_EQUALIZER:
    if (is_yuv) {
      int i;
      vf_equalizer_t *eq=data;
      for (i = 0; eq_map[i].name; i++)
        if (strcmp(eq->item, eq_map[i].name) == 0) break;
      if (!(eq_map[i].supportmask & (1 << use_yuv)))
        break;
      *eq_map[i].value = eq->value;
      if (strcmp(eq->item, "gamma") == 0)
        eq_rgamma = eq_ggamma = eq_bgamma = eq->value;
      update_yuvconv();
      return VO_TRUE;
    }
    break;
  case VOCTRL_UPDATE_SCREENINFO:
    glctx.update_xinerama_info();
    return VO_TRUE;
  }
  return VO_NOTIMPL;
}
