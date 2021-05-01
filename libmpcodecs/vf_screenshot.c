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

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "mp_msg.h"

#include "img_format.h"
#include "mp_image.h"
#include "vf.h"
#include "vf_scale.h"

#include "libavutil/mem.h"
#include "libswscale/swscale.h"
#include "libavcodec/avcodec.h"

struct vf_priv_s {
    int frameno;
    char fname[PATH_MAX];
    char *prefix;
    /// shot stores current screenshot mode:
    /// 0: don't take screenshots
    /// 1: take single screenshot, reset to 0 afterwards
    /// 2: take screenshots of each frame
    int shot, store_slices;
    int dw, dh;
    AVFrame *pic;
    AVPacket *pkt;
    struct SwsContext *ctx;
    AVCodecContext *avctx;
};

//===========================================================================//

static void draw_slice(struct vf_instance *vf, unsigned char** src,
                       int* stride, int w,int h, int x, int y)
{
    if (vf->priv->store_slices) {
        sws_scale(vf->priv->ctx, src, stride, y, h, vf->priv->pic->data, vf->priv->pic->linesize);
    }
    vf_next_draw_slice(vf,src,stride,w,h,x,y);
}

static int config(struct vf_instance *vf,
                  int width, int height, int d_width, int d_height,
                  unsigned int flags, unsigned int outfmt)
{
    int res;
    if (vf->priv->ctx) sws_freeContext(vf->priv->ctx);
    vf->priv->ctx=sws_getContextFromCmdLine(width, height, outfmt,
                                 d_width, d_height, IMGFMT_RGB24);

    if (!vf->priv->avctx) {
        vf->priv->avctx = avcodec_alloc_context3(NULL);
        vf->priv->avctx->pix_fmt = AV_PIX_FMT_RGB24;
        vf->priv->avctx->width = d_width;
        vf->priv->avctx->height = d_height;
        vf->priv->avctx->time_base.num = 1;
        vf->priv->avctx->time_base.den = 1;
        vf->priv->avctx->compression_level = 0;
        if (avcodec_open2(vf->priv->avctx, avcodec_find_encoder(AV_CODEC_ID_PNG), NULL)) {
            mp_msg(MSGT_VFILTER, MSGL_FATAL, "Could not open libavcodec PNG encoder\n");
            return 0;
        }
    }
    vf->priv->dw = d_width;
    vf->priv->dh = d_height;
    vf->priv->pic->linesize[0] = (3*vf->priv->dw+15)&~15;

    av_freep(&vf->priv->pic->data[0]); // probably reconfigured

    res = vf_next_config(vf,width,height,d_width,d_height,flags,outfmt);
    // Our draw_slice only works properly if the
    // following filter can do slices.
    vf->draw_slice=vf->next->draw_slice ? draw_slice : NULL;
    return res;
}

static void write_png(struct vf_priv_s *priv)
{
    char *fname = priv->fname;
    FILE * fp;
    AVPacket *pkt = priv->pkt;
    int res;

    priv->pic->width = priv->avctx->width;
    priv->pic->height = priv->avctx->height;
    priv->pic->format = priv->avctx->pix_fmt;
    res = avcodec_send_frame(priv->avctx, priv->pic);
    if (res >= 0) {
        res = avcodec_receive_packet(priv->avctx, pkt);
        if (res == AVERROR(EAGAIN)) {
            avcodec_send_frame(priv->avctx, NULL);
            res = avcodec_receive_packet(priv->avctx, pkt);
        }
    }
    if (res < 0 || pkt->size <= 0) {
        mp_msg(MSGT_VFILTER,MSGL_ERR,"\nFailed to encode screenshot %s!\n", fname);
        return;
    }

    fp = fopen (fname, "wb");
    if (fp == NULL) {
        mp_msg(MSGT_VFILTER,MSGL_ERR,"\nPNG Error opening %s for writing!\n", fname);
        return;
    }

    fwrite(pkt->data, pkt->size, 1, fp);
    av_packet_unref(pkt);

    fclose (fp);
    mp_msg(MSGT_VFILTER,MSGL_INFO,"*** screenshot '%s' ***\n",priv->fname);
}

static int fexists(char *fname)
{
    struct stat dummy;
    return stat(fname, &dummy) == 0;
}

static void gen_fname(struct vf_priv_s* priv)
{
    do {
        snprintf(priv->fname, sizeof(priv->fname), "%s%04d.png", priv->prefix, ++priv->frameno);
    } while (fexists(priv->fname) && priv->frameno < 100000);
    if (fexists(priv->fname)) {
        priv->fname[0] = '\0';
        return;
    }
}

static void scale_image(struct vf_priv_s* priv, mp_image_t *mpi)
{
    if (!priv->pic->data[0])
        priv->pic->data[0] = av_malloc(priv->pic->linesize[0]*priv->dh);

    sws_scale(priv->ctx, mpi->planes, mpi->stride, 0, mpi->height, priv->pic->data, priv->pic->linesize);
}

static void start_slice(struct vf_instance *vf, mp_image_t *mpi)
{
    mpi->priv=
    vf->dmpi=vf_get_image(vf->next,mpi->imgfmt,
        mpi->type, mpi->flags, mpi->width, mpi->height);
    if (vf->priv->shot) {
        vf->priv->store_slices = 1;
        if (!vf->priv->pic->data[0])
            vf->priv->pic->data[0] = av_malloc(vf->priv->pic->linesize[0]*vf->priv->dh);
    }

}

static void get_image(struct vf_instance *vf, mp_image_t *mpi)
{
    // FIXME: should vf.c really call get_image when using slices??
    if (mpi->flags & MP_IMGFLAG_DRAW_CALLBACK)
      return;
    vf->dmpi= vf_get_image(vf->next, mpi->imgfmt,
                           mpi->type, mpi->flags/* | MP_IMGFLAG_READABLE*/, mpi->width, mpi->height);

    mpi->planes[0]=vf->dmpi->planes[0];
    mpi->stride[0]=vf->dmpi->stride[0];
    if(mpi->flags&MP_IMGFLAG_PLANAR){
        mpi->planes[1]=vf->dmpi->planes[1];
        mpi->planes[2]=vf->dmpi->planes[2];
        mpi->stride[1]=vf->dmpi->stride[1];
        mpi->stride[2]=vf->dmpi->stride[2];
    }
    mpi->width=vf->dmpi->width;

    mpi->flags|=MP_IMGFLAG_DIRECT;

    mpi->priv=vf->dmpi;
}

static int put_image(struct vf_instance *vf, mp_image_t *mpi, double pts, double endpts)
{
    mp_image_t *dmpi = (mp_image_t *)mpi->priv;

    if(!(mpi->flags&(MP_IMGFLAG_DIRECT|MP_IMGFLAG_DRAW_CALLBACK))){
        dmpi=vf_get_image(vf->next,mpi->imgfmt,
                                    MP_IMGTYPE_EXPORT, 0,
                                    mpi->width, mpi->height);
        vf_clone_mpi_attributes(dmpi, mpi);
        dmpi->planes[0]=mpi->planes[0];
        dmpi->planes[1]=mpi->planes[1];
        dmpi->planes[2]=mpi->planes[2];
        dmpi->stride[0]=mpi->stride[0];
        dmpi->stride[1]=mpi->stride[1];
        dmpi->stride[2]=mpi->stride[2];
        dmpi->width=mpi->width;
        dmpi->height=mpi->height;
    }

    if(vf->priv->shot) {
        vf->priv->shot &= ~1;
        gen_fname(vf->priv);
        if (vf->priv->fname[0]) {
            if (!vf->priv->store_slices)
              scale_image(vf->priv, dmpi);
            write_png(vf->priv);
        }
        vf->priv->store_slices = 0;
    }

    return vf_next_put_image(vf, dmpi, pts, endpts);
}

static int control (vf_instance_t *vf, int request, void *data)
{
    /** data contains an integer argument
     * 0: take screenshot with the next frame
     * 1: take screenshots with each frame until the same command is given once again
     **/
    if(request==VFCTRL_SCREENSHOT) {
        if (data && *(int*)data) { // repeated screenshot mode
            vf->priv->shot ^= 2;
        } else { // single screenshot
            vf->priv->shot |= 1;
        }
        return CONTROL_TRUE;
    }
    return vf_next_control (vf, request, data);
}


//===========================================================================//

static int query_format(struct vf_instance *vf, unsigned int fmt)
{
    switch(fmt){
    case IMGFMT_YV12:
    case IMGFMT_I420:
    case IMGFMT_IYUV:
    case IMGFMT_UYVY:
    case IMGFMT_YUY2:
    case IMGFMT_BGR32:
    case IMGFMT_BGR24:
    case IMGFMT_BGR16:
    case IMGFMT_BGR15:
    case IMGFMT_BGR12:
    case IMGFMT_RGB32:
    case IMGFMT_RGB24:
    case IMGFMT_Y800:
    case IMGFMT_Y8:
    case IMGFMT_YVU9:
    case IMGFMT_IF09:
    case IMGFMT_444P:
    case IMGFMT_422P:
    case IMGFMT_411P:
        return vf_next_query_format(vf, fmt);
    }
    return 0;
}

static void uninit(vf_instance_t *vf)
{
    avcodec_close(vf->priv->avctx);
    av_freep(&vf->priv->avctx);
    if(vf->priv->ctx) sws_freeContext(vf->priv->ctx);
    av_freep(&vf->priv->pic->data[0]);
    av_frame_free(&vf->priv->pic);
    av_packet_free(&vf->priv->pkt);
    free(vf->priv->prefix);
    free(vf->priv);
}

static int vf_open(vf_instance_t *vf, char *args)
{
    vf->config=config;
    vf->control=control;
    vf->put_image=put_image;
    vf->query_format=query_format;
    vf->start_slice=start_slice;
    vf->draw_slice=draw_slice;
    vf->get_image=get_image;
    vf->uninit=uninit;
    vf->priv = calloc(1, sizeof(struct vf_priv_s));
    vf->priv->pic = av_frame_alloc();
    vf->priv->pkt = av_packet_alloc();
    vf->priv->prefix = strdup(args ? args : "shot");
    if (!avcodec_find_encoder(AV_CODEC_ID_PNG)) {
        mp_msg(MSGT_VFILTER, MSGL_FATAL, "Could not find libavcodec PNG encoder\n");
        return 0;
    }
    return 1;
}


const vf_info_t vf_info_screenshot = {
    "screenshot to file",
    "screenshot",
    "A'rpi, Jindrich Makovicka",
    "",
    vf_open,
    NULL
};

//===========================================================================//
