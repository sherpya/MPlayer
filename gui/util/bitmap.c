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

/**
 * @file
 * @brief Image loader and bitmap mask rendering
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "bitmap.h"
#include "gui/app/gui.h"

#include "help_mp.h"
#include "mp_msg.h"
#include "libavcodec/avcodec.h"
#include "libavutil/common.h"
#include "libavutil/intreadwrite.h"
#include "libvo/fastmemcpy.h"

static uint32_t palette[256];

/**
 * @brief Check whether a (PNG) file exists.
 *
 * @param fname filename (with path, but may lack extension)
 *
 * @return path including extension (ok) or NULL (not accessible)
 */
static const char *fExist(const char *fname)
{
    static const char ext[][4] = { "png", "PNG" };
    static char buf[512];
    unsigned int i;

    if (access(fname, R_OK) == 0)
        return fname;

    for (i = 0; i < FF_ARRAY_ELEMS(ext); i++) {
        snprintf(buf, sizeof(buf), "%s.%s", fname, ext[i]);

        if (access(buf, R_OK) == 0)
            return buf;
    }

    return NULL;
}

/**
 * @brief Read and decode a PNG file into bitmap data.
 *
 * @param fname filename (with path)
 * @param img memory location to store the image data
 * @param pix_fmt pointer to the location to store the pixel format
 *
 * @return 0 (ok), 1 (decoding error), 2 (file open error), 3 (file too big),
 *                 4 (out of memory), 5 (read error), 6 (avcodec alloc error),
 *                 7 (avcodec open error)
 */
static int pngRead(const char *fname, guiImage *img, int *pix_fmt)
{
    FILE *file;
    size_t len, l;
    void *data;
    int decode_ok, bpl;
    AVCodecContext *avctx;
    AVFrame *frame;
    AVPacket pkt;

    file = fopen(fname, "rb");

    if (!file)
        return 2;

    fseek(file, 0, SEEK_END);
    len = ftell(file);

    if (len > 50 * 1024 * 1024) {
        fclose(file);
        return 3;
    }

    data = av_malloc(len + AV_INPUT_BUFFER_PADDING_SIZE);

    if (!data) {
        fclose(file);
        return 4;
    }

    fseek(file, 0, SEEK_SET);
    l = fread(data, len, 1, file);
    fclose(file);

    if (l != 1) {
        av_free(data);
        return 5;
    }

    avctx = avcodec_alloc_context3(NULL);
    frame = av_frame_alloc();

    if (!(avctx && frame)) {
        av_free(frame);
        av_free(avctx);
        av_free(data);
        return 6;
    }

    avcodec_register_all();

    if (avcodec_open2(avctx, avcodec_find_decoder(AV_CODEC_ID_PNG), NULL) < 0) {
        av_free(frame);
        av_free(avctx);
        av_free(data);
        return 7;
    }

    av_init_packet(&pkt);
    pkt.data = data;
    pkt.size = len;
    /* HACK: Make PNGs decode normally instead of as CorePNG delta frames. */
    pkt.flags = AV_PKT_FLAG_KEY;

    avcodec_decode_video2(avctx, frame, &decode_ok, &pkt);

    memset(img, 0, sizeof(*img));
    memset(palette, 0, sizeof(palette));

    *pix_fmt = avctx->pix_fmt;

    switch (*pix_fmt) {
    case AV_PIX_FMT_PAL8:
        img->Bpp = 8;
        break;

    case AV_PIX_FMT_RGB24:
        img->Bpp = 24;
        break;

    case AV_PIX_FMT_RGBA:
        img->Bpp = 32;
        break;

    default:
        img->Bpp = 0;
        mp_msg(MSGT_GPLAYER, MSGL_DBG2, "[bitmap] unsupported pixel format: %d\n", *pix_fmt);
        break;
    }

    if (decode_ok && img->Bpp) {
        img->Width  = avctx->width;
        img->Height = avctx->height;
        bpl = img->Width * (img->Bpp / 8);
        img->ImageSize = bpl * img->Height;

        mp_msg(MSGT_GPLAYER, MSGL_DBG2, "[bitmap] file: %s\n", fname);
        mp_msg(MSGT_GPLAYER, MSGL_DBG2, "[bitmap]  size: %ux%u, color depth: %u\n", img->Width, img->Height, img->Bpp);
        mp_msg(MSGT_GPLAYER, MSGL_DBG2, "[bitmap]  image size: %u\n", img->ImageSize);

        img->Image = malloc(img->ImageSize);

        if (img->Image) {
            memcpy_pic(img->Image, frame->data[0], bpl, img->Height, bpl, frame->linesize[0]);

            if (frame->data[1])
                memcpy(palette, frame->data[1], sizeof(palette));
        } else
            decode_ok = False;
    }

    avcodec_close(avctx);
    av_free(frame);
    av_free(avctx);
    av_free(data);

    return !(decode_ok && img->Bpp);
}

/**
 * @brief Convert a 24-bit RGB or 32-bit RGBA image into a 32-bit ARGB image.
 *
 * @param img image to be converted
 *
 * @return #True (ok) or #False (error)
 *
 * @note This is an in-place conversion,
 *       new memory will be allocated for @a img if necessary.
 */
static int convert_ARGB(guiImage *img, int pix_fmt)
{
    unsigned int orgSize;
    unsigned char *orgImage;
    unsigned int i, c;

    if (pix_fmt == AV_PIX_FMT_PAL8 || pix_fmt == AV_PIX_FMT_RGB24) {
        orgSize  = img->ImageSize;
        orgImage = img->Image;

        img->Bpp       = 32;
        img->ImageSize = img->Width * img->Height * 4;
        img->Image     = calloc(1, img->ImageSize);

        if (!img->Image) {
            free(orgImage);
            mp_msg(MSGT_GPLAYER, MSGL_DBG2, "[bitmap] not enough memory: %u\n", img->ImageSize);
            return False;
        }

        mp_msg(MSGT_GPLAYER, MSGL_DBG2, "[bitmap] 32 bpp conversion size: %u\n", img->ImageSize);

        if (pix_fmt == AV_PIX_FMT_PAL8)
            for (i = 0, c = 0; c < orgSize; i += 4, c++)
                *(uint32_t *)&img->Image[i] = palette[orgImage[c]];

        if (pix_fmt == AV_PIX_FMT_RGB24)
            for (i = 0, c = 0; i < img->ImageSize; i += 4, c += 3)
                *(uint32_t *)&img->Image[i] = ALPHA_OPAQUE | AV_RB24(&orgImage[c]);

        free(orgImage);
    } else if (pix_fmt == AV_PIX_FMT_RGBA) {
        mp_msg(MSGT_GPLAYER, MSGL_DBG2, "[bitmap] 32 bpp ARGB conversion\n");

        for (i = 0; i < img->ImageSize; i += 4)
            *(uint32_t *)&img->Image[i] = (img->Image[i + 3] << 24) | AV_RB24(&img->Image[i]);
    } else
        return False;

    return True;
}

/**
 * @brief Read a PNG file.
 *
 * @param fname filename (with path, but may lack extension)
 * @param img memory location to store the image data
 *
 * @return 0 (ok), -1 (color depth too low), -2 (not accessible),
 *                 -5 (#pngRead() error) or -8 (#convert_ARGB() error)
 */
int bpRead(const char *fname, guiImage *img)
{
    int r, pix_fmt;

    fname = fExist(fname);

    if (!fname)
        return -2;

    r = pngRead(fname, img, &pix_fmt);

    if (r != 0) {
        mp_msg(MSGT_GPLAYER, MSGL_DBG2, "[bitmap] read error #%d: %s\n", r, fname);
        return -5;
    }

    if (!img->Bpp)
        return -1;

    if (!convert_ARGB(img, pix_fmt))
        return -8;

    return 0;
}

/**
 * @brief Free all memory allocated to an image and set all its pointers to NULL.
 *
 * @param img image to be freed
 */
void bpFree(guiImage *img)
{
    free(img->Image);
    memset(img, 0, sizeof(*img));
}

/**
 * @brief Render a bitmap mask for an image.
 *
 * @param in image to render a bitmap mask from
 * @param out bitmap mask
 *
 * @return #True (ok) or #False (error)
 *
 * @note As a side effect, transparent pixels of @a in will be rendered black.
 */
int bpRenderMask(const guiImage *in, guiImage *out)
{
    uint32_t *buf;
    unsigned int x, y;
    unsigned int i = 0, c = 0;
    unsigned char tmp = 0, b = 1;
    int shaped = 0;

    out->Width     = in->Width;
    out->Height    = in->Height;
    out->Bpp       = 1;
    out->ImageSize = ((out->Width + 7) / 8) * out->Height;
    out->Image     = calloc(1, out->ImageSize);

    if (!out->Image) {
        mp_msg(MSGT_GPLAYER, MSGL_DBG2, "[bitmap] not enough memory: %u\n", out->ImageSize);
        return False;
    }

    buf = (uint32_t *)in->Image;

    for (y = 0; y < in->Height; y++) {
        for (x = 0; x < in->Width; x++) {
            if (!IS_TRANSPARENT(buf[i]))
                tmp |= b;
            else {
                buf[i] = 0; // pixel should be black (if transparency isn't supported)
                shaped = 1;
            }

            i++;
            b <<= 1;

            if (b == 0) {
                out->Image[c++] = tmp;
                tmp = 0;
                b   = 1;
            }
        }

        if (b != 1) {
            out->Image[c++] = tmp;
            tmp = 0;
            b   = 1;
        }
    }

    if (!shaped)
        bpFree(out);

    mp_msg(MSGT_GPLAYER, MSGL_DBG2, "[bitmap] 1 bpp conversion size: %u\n", out->ImageSize);

    return True;
}
