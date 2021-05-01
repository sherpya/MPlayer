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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "config.h"
#include "mp_msg.h"
#include "help_mp.h"

#include "img_format.h"
#include "mp_image.h"
#include "vf.h"
#include "av_helpers.h"
#include "libavcodec/avcodec.h"


struct vf_priv_s {
    AVCodecContext* context;
    AVFrame* pic;
    const AVCodec* codec;
    vo_mpegpes_t pes;
    AVPacket *pkt;
    int pkt_has_ref;
};

#define lavc_venc_context (*vf->priv->context)

//===========================================================================//

static int config(struct vf_instance *vf,
        int width, int height, int d_width, int d_height,
	unsigned int flags, unsigned int outfmt){
    if(vf_next_query_format(vf,IMGFMT_MPEGPES)<=0) return 0;

    lavc_venc_context.width = width;
    lavc_venc_context.height = height;

    if(!lavc_venc_context.time_base.num || !lavc_venc_context.time_base.den){
	// guess FPS:
	switch(height){
	case 240:
	case 480:
	    lavc_venc_context.time_base= (AVRational){1001,30000};
	    break;
	case 576:
	case 288:
	default:
	    lavc_venc_context.time_base= (AVRational){1,25};
	    break;
//	    lavc_venc_context.frame_rate=vo_fps*FRAME_RATE_BASE; // same as src
	}
    }

    if (vf->priv->pkt_has_ref) av_packet_unref(vf->priv->pkt);

    if (avcodec_open2(&lavc_venc_context, vf->priv->codec, NULL) != 0) {
	mp_msg(MSGT_MENCODER,MSGL_ERR,MSGTR_CantOpenCodec);
	return 0;
    }

    return vf_next_config(vf,width,height,d_width,d_height,flags,IMGFMT_MPEGPES);
}

static int put_image(struct vf_instance *vf, mp_image_t *mpi, double pts, double endpts){
    mp_image_t* dmpi;
    AVFrame *pic= vf->priv->pic;
    AVPacket *pkt = vf->priv->pkt;
    int ret;

    pic->data[0]=mpi->planes[0];
    pic->data[1]=mpi->planes[1];
    pic->data[2]=mpi->planes[2];
    pic->linesize[0]=mpi->stride[0];
    pic->linesize[1]=mpi->stride[1];
    pic->linesize[2]=mpi->stride[2];

    if (vf->priv->pkt_has_ref) av_packet_unref(pkt);
    ret = avcodec_send_frame(&lavc_venc_context, pic);
    if (ret >= 0) {
        ret = avcodec_receive_packet(&lavc_venc_context, pkt);
    }
    vf->priv->pkt_has_ref = ret >= 0;

    if(ret<0) return 1;

    dmpi=vf_get_image(vf->next,IMGFMT_MPEGPES,
	MP_IMGTYPE_EXPORT, 0,
	mpi->w, mpi->h);

    vf->priv->pes.data=pkt->data;
    vf->priv->pes.size=pkt->size;
    vf->priv->pes.id=0x1E0;
    vf->priv->pes.timestamp=-1; // dunno

    dmpi->planes[0]=(unsigned char*)&vf->priv->pes;

    return vf_next_put_image(vf, dmpi, pts, endpts);
}

//===========================================================================//

static int query_format(struct vf_instance *vf, unsigned int fmt){
    switch(fmt){
    case IMGFMT_YV12:
    case IMGFMT_I420:
    case IMGFMT_IYUV:
	return vf_next_query_format(vf, IMGFMT_MPEGPES) & (~(VFCAP_CSP_SUPPORTED_BY_HW | VFCAP_ACCEPT_STRIDE));
    }
    return 0;
}

static int vf_open(vf_instance_t *vf, char *args){
    int p_quality=0;
    float p_fps=0;

    vf->config=config;
    vf->put_image=put_image;
    vf->query_format=query_format;
    vf->priv=malloc(sizeof(struct vf_priv_s));
    memset(vf->priv,0,sizeof(struct vf_priv_s));

    init_avcodec();

    vf->priv->codec = avcodec_find_encoder_by_name("mpeg1video");
    if (!vf->priv->codec) {
	mp_msg(MSGT_MENCODER,MSGL_ERR,MSGTR_MissingLAVCcodec, "mpeg1video");
	return 0;
    }

    vf->priv->context=avcodec_alloc_context3(vf->priv->codec);
    vf->priv->pic = av_frame_alloc();
    vf->priv->pkt = av_packet_alloc();

    // TODO: parse args ->
    if(args) sscanf(args, "%d:%f", &p_quality, &p_fps);

    if(p_quality<32){
	// fixed qscale
	lavc_venc_context.flags = AV_CODEC_FLAG_QSCALE;
	lavc_venc_context.global_quality =
	vf->priv->pic->quality = (int)(FF_QP2LAMBDA * ((p_quality<1) ? 1 : p_quality) + 0.5);
    } else {
	// fixed bitrate (in kbits)
	lavc_venc_context.bit_rate = 1000*p_quality;
    }
    lavc_venc_context.time_base.num = 1000*1001;
    lavc_venc_context.time_base.den = (p_fps<1.0) ? 1000*1001*25 : (p_fps * lavc_venc_context.time_base.num);
    lavc_venc_context.gop_size = 0; // I-only
    lavc_venc_context.pix_fmt= AV_PIX_FMT_YUV420P;

    return 1;
}

const vf_info_t vf_info_lavc = {
    "realtime mpeg1 encoding with libavcodec",
    "lavc",
    "A'rpi",
    "",
    vf_open,
    NULL
};

//===========================================================================//
