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
#include <assert.h>
#include <time.h>

#include "config.h"
#include "mp_msg.h"
#include "help_mp.h"
#include "av_opts.h"
#include "av_helpers.h"

#include "libavutil/common.h"
#include "libavutil/avstring.h"
#include "libavutil/dict.h"
#include "libavutil/intreadwrite.h"
#include "libavutil/opt.h"
#include "mpbswap.h"
#include "fmt-conversion.h"

#include "vd_internal.h"

#if CONFIG_VDPAU
#include "libavcodec/vdpau.h"
#include "vdpau_frame_data.h"
#endif
#include "libavutil/pixdesc.h"

static const vd_info_t info = {
    "FFmpeg's libavcodec codec family",
    "ffmpeg",
    "A'rpi",
    "A'rpi, Michael, Alex",
    "native codecs"
};

LIBVD_EXTERN(ffmpeg)

#include "libavcodec/avcodec.h"

#ifndef AV_EF_COMPLIANT
#define AV_EF_COMPLIANT 0
#endif

#ifndef AV_EF_CAREFUL
#define AV_EF_CAREFUL 0
#endif

#ifndef AV_EF_AGGRESSIVE
#define AV_EF_AGGRESSIVE 0
#endif

#if AVPALETTE_SIZE > 1024
#error palette too large, adapt libmpcodecs/vf.c:vf_get_image
#endif

#if CONFIG_XVMC
#include "libavcodec/xvmc.h"
#endif

typedef struct {
    AVCodecContext *avctx;
    AVFrame *pic;
    AVFrame *refcount_frame;
    enum AVPixelFormat pix_fmt;
    int do_slices;
    int do_dr1;
    int nonref_dr; ///< allow dr only for non-reference frames
    int vo_initialized;
    int best_csp;
    int qp_stat[32];
    double qp_sum;
    double inv_qp_sum;
    int ip_count;
    int b_count;
    AVRational last_sample_aspect_ratio;
    int palette_sent;
    int use_vdpau;
} vd_ffmpeg_ctx;

#include "m_option.h"

static int get_buffer(AVCodecContext *avctx, AVFrame *pic, int isreference);
static int mpcodec_default_get_buffer(AVCodecContext *avctx, AVFrame *frame);
static int get_buffer2(AVCodecContext *avctx, AVFrame *frame, int flags);
static void release_buffer(AVCodecContext *avctx, AVFrame *pic);
static void mpcodec_default_release_buffer(AVCodecContext *s, AVFrame *pic);
static void draw_slice(struct AVCodecContext *s, const AVFrame *src, int offset[4],
                       int y, int type, int height);

static enum AVPixelFormat get_format(struct AVCodecContext *avctx,
                                     const enum AVPixelFormat *pix_fmt);

static int lavc_param_workaround_bugs= FF_BUG_AUTODETECT;
static int lavc_param_error_resilience=2;
static int lavc_param_error_concealment=3;
static int lavc_param_gray=0;
static int lavc_param_vstats=0;
static int lavc_param_idct_algo=0;
static int lavc_param_debug=0;
#ifdef AV_CODEC_FLAG2_SHOW_ALL
static int lavc_param_wait_keyframe=0;
#endif
static int lavc_param_skip_top=0;
static int lavc_param_skip_bottom=0;
static int lavc_param_fast=0;
static int lavc_param_lowres=0;
static char *lavc_param_lowres_str=NULL;
static char *lavc_param_skip_loop_filter_str = NULL;
static char *lavc_param_skip_idct_str = NULL;
static char *lavc_param_skip_frame_str = NULL;
static int lavc_param_threads=1;
static int lavc_param_bitexact=0;
static char *lavc_avopt = NULL;
static enum AVDiscard skip_idct;
static enum AVDiscard skip_frame;

static const mp_image_t mpi_no_picture =
{
	.type = MP_IMGTYPE_INCOMPLETE
};

const m_option_t lavc_decode_opts_conf[]={
    {"bug"           , &lavc_param_workaround_bugs      , CONF_TYPE_INT     , CONF_RANGE, -1, 999999, NULL},
    {"er"            , &lavc_param_error_resilience     , CONF_TYPE_INT     , CONF_RANGE, 0, 99, NULL},
    {"gray"          , &lavc_param_gray                 , CONF_TYPE_FLAG    , 0, 0, AV_CODEC_FLAG_GRAY, NULL},
    {"idct"          , &lavc_param_idct_algo            , CONF_TYPE_INT     , CONF_RANGE, 0, 99, NULL},
    {"ec"            , &lavc_param_error_concealment    , CONF_TYPE_INT     , CONF_RANGE, 0, 99, NULL},
    {"vstats"        , &lavc_param_vstats               , CONF_TYPE_FLAG    , 0, 0, 1, NULL},
    {"debug"         , &lavc_param_debug                , CONF_TYPE_INT     , CONF_RANGE, 0, 9999999, NULL},
#ifdef AV_CODEC_FLAG2_SHOW_ALL
    {"wait_keyframe" , &lavc_param_wait_keyframe        , CONF_TYPE_FLAG    , 0, 0, 1, NULL},
#endif
    {"st"            , &lavc_param_skip_top             , CONF_TYPE_INT     , CONF_RANGE, 0, 999, NULL},
    {"sb"            , &lavc_param_skip_bottom          , CONF_TYPE_INT     , CONF_RANGE, 0, 999, NULL},
    {"fast"          , &lavc_param_fast                 , CONF_TYPE_FLAG    , 0, 0, AV_CODEC_FLAG2_FAST, NULL},
    {"lowres"        , &lavc_param_lowres_str           , CONF_TYPE_STRING  , 0, 0, 0, NULL},
    {"skiploopfilter", &lavc_param_skip_loop_filter_str , CONF_TYPE_STRING  , 0, 0, 0, NULL},
    {"skipidct"      , &lavc_param_skip_idct_str        , CONF_TYPE_STRING  , 0, 0, 0, NULL},
    {"skipframe"     , &lavc_param_skip_frame_str       , CONF_TYPE_STRING  , 0, 0, 0, NULL},
    {"threads"       , &lavc_param_threads              , CONF_TYPE_INT     , CONF_RANGE, 1, 32, NULL},
    {"bitexact"      , &lavc_param_bitexact             , CONF_TYPE_FLAG    , 0, 0, AV_CODEC_FLAG_BITEXACT, NULL},
    {"o"             , &lavc_avopt                      , CONF_TYPE_STRING  , 0, 0, 0, NULL},
    {NULL, NULL, 0, 0, 0, 0, NULL}
};

static enum AVDiscard str2AVDiscard(char *str) {
    if (!str)                               return AVDISCARD_DEFAULT;
    if (av_strcasecmp(str, "none"   ) == 0) return AVDISCARD_NONE;
    if (av_strcasecmp(str, "default") == 0) return AVDISCARD_DEFAULT;
    if (av_strcasecmp(str, "nonref" ) == 0) return AVDISCARD_NONREF;
    if (av_strcasecmp(str, "bidir"  ) == 0) return AVDISCARD_BIDIR;
    if (av_strcasecmp(str, "nonkey" ) == 0) return AVDISCARD_NONKEY;
    if (av_strcasecmp(str, "all"    ) == 0) return AVDISCARD_ALL;
    mp_msg(MSGT_DECVIDEO, MSGL_ERR, "Unknown discard value %s\n", str);
    return AVDISCARD_DEFAULT;
}

// to set/get/query special features/parameters
static int control(sh_video_t *sh, int cmd, void *arg, ...){
    vd_ffmpeg_ctx *ctx = sh->context;
    AVCodecContext *avctx = ctx->avctx;
    switch(cmd){
    case VDCTRL_QUERY_FORMAT:
    {
        int format =(*((int *)arg));
        if(format == ctx->best_csp) return CONTROL_TRUE;//supported
        // possible conversions:
        switch(format){
        case IMGFMT_YV12:
        case IMGFMT_IYUV:
        case IMGFMT_I420:
            // "converted" using pointer/stride modification
            if(ctx->best_csp == IMGFMT_YV12) return CONTROL_TRUE;// u/v swap
            if(ctx->best_csp == IMGFMT_422P && !ctx->do_dr1) return CONTROL_TRUE;// half stride
            break;
#if CONFIG_XVMC
        case IMGFMT_XVMC_IDCT_MPEG2:
        case IMGFMT_XVMC_MOCO_MPEG2:
            if(avctx->pix_fmt == AV_PIX_FMT_XVMC) return CONTROL_TRUE;
#endif
        }
        return CONTROL_FALSE;
    }
    case VDCTRL_RESYNC_STREAM:
        avcodec_flush_buffers(avctx);
        return CONTROL_TRUE;
    case VDCTRL_QUERY_UNSEEN_FRAMES:
        // "has_b_frames" contains the (e.g. reorder) delay as specified
        // in the standard. "delay" contains the libavcodec-specific delay
        // e.g. due to frame multithreading
        return avctx->has_b_frames + avctx->delay + 10;
    }
    return CONTROL_UNKNOWN;
}

#if CONFIG_VDPAU
static int vdpau_render_wrapper(AVCodecContext *s, AVFrame *f,
    const VdpPictureInfo *info, uint32_t count, const VdpBitstreamBuffer *buffers)
{
    mp_image_t *mpi = f->opaque;
    sh_video_t *sh = s->opaque;
    struct vdpau_frame_data data;
    uint8_t *planes[4] = {(void *)&data};
    data.surface = (VdpVideoSurface)mpi->priv;
    data.info = info;
    data.bitstream_buffers_used = count;
    data.bitstream_buffers = buffers;
    mpcodecs_draw_slice(sh, planes, NULL, sh->disp_w, sh->disp_h, 0, 0);
    return 0;
}
#endif

static int pixfmt2imgfmt2(enum AVPixelFormat fmt, enum AVCodecID cid)
{
    if (fmt == AV_PIX_FMT_VDPAU)
        switch (cid) {
        case AV_CODEC_ID_H264:       return IMGFMT_VDPAU_H264;
        case AV_CODEC_ID_MPEG1VIDEO: return IMGFMT_VDPAU_MPEG1;
        case AV_CODEC_ID_MPEG2VIDEO: return IMGFMT_VDPAU_MPEG2;
        case AV_CODEC_ID_MPEG4:      return IMGFMT_VDPAU_MPEG4;
        case AV_CODEC_ID_WMV3:       return IMGFMT_VDPAU_WMV3;
        case AV_CODEC_ID_VC1:        return IMGFMT_VDPAU_VC1;
        case AV_CODEC_ID_HEVC:       return IMGFMT_VDPAU_HEVC;
        }
    return pixfmt2imgfmt(fmt);
}


/**
 * Function to set slice/dr related settings that need to be reset after
 * initializing hardware acceleration failed */
static void set_dr_slice_settings(struct AVCodecContext *avctx, const AVCodec *lavc_codec)
{
    sh_video_t *sh     = avctx->opaque;
    vd_ffmpeg_ctx *ctx = sh->context;
    // slice is rather broken with threads, so disable that combination unless
    // explicitly requested
    int use_slices = vd_use_slices > 0 || (vd_use_slices <  0 && lavc_param_threads <= 1);

    ctx->do_slices = use_slices && (lavc_codec->capabilities & AV_CODEC_CAP_DRAW_HORIZ_BAND);

    ctx->do_dr1 = (lavc_codec->capabilities & AV_CODEC_CAP_DR1) &&
        lavc_codec->id != AV_CODEC_ID_INTERPLAY_VIDEO &&
        lavc_codec->id != AV_CODEC_ID_H264 &&
        lavc_codec->id != AV_CODEC_ID_HEVC;
    ctx->nonref_dr = 0;
    // TODO: fix and enable again. This currently causes issues when using filters
    // and seeking, usually failing with the "Ran out of numbered images" message,
    // but ticket #2118 might be related as well.
    if (0 && lavc_codec->id == AV_CODEC_ID_H264) {
        ctx->do_dr1 = 1;
        ctx->nonref_dr = 1;
    }
    if(ctx->do_dr1){
        avctx->get_buffer2 = get_buffer2;
    } else if (lavc_codec->capabilities & AV_CODEC_CAP_DR1) {
        avctx->get_buffer2 = avcodec_default_get_buffer2;
    }
    avctx->slice_flags = 0;
}

static void set_format_params(struct AVCodecContext *avctx,
                              enum AVPixelFormat fmt)
{
    sh_video_t *sh     = avctx->opaque;
    vd_ffmpeg_ctx *ctx = sh->context;
    int imgfmt;
    if (fmt == AV_PIX_FMT_NONE)
        return;
    ctx->use_vdpau = fmt == AV_PIX_FMT_VDPAU;
    imgfmt = pixfmt2imgfmt2(fmt, avctx->codec_id);
#if CONFIG_VDPAU
    if (!ctx->use_vdpau) {
        av_freep(&avctx->hwaccel_context);
    } else {
        AVVDPAUContext *vdpc = avctx->hwaccel_context;
        if (!vdpc)
            avctx->hwaccel_context = vdpc = av_alloc_vdpaucontext();
        vdpc->render2 = vdpau_render_wrapper;
    }
#endif
    if (IMGFMT_IS_HWACCEL(imgfmt)) {
        ctx->do_dr1    = 1;
        ctx->nonref_dr = 0;
        avctx->get_buffer2 = get_buffer2;
        mp_msg(MSGT_DECVIDEO, MSGL_V, IMGFMT_IS_XVMC(imgfmt) ?
               MSGTR_MPCODECS_XVMCAcceleratedMPEG2 :
               "[VD_FFMPEG] VDPAU accelerated decoding\n");
        if (ctx->use_vdpau) {
            avctx->draw_horiz_band = NULL;
            avctx->slice_flags = 0;
            ctx->do_slices = 0;
        } else {
            avctx->draw_horiz_band = draw_slice;
            avctx->slice_flags = SLICE_FLAG_CODED_ORDER|SLICE_FLAG_ALLOW_FIELD;
            ctx->do_slices = 1;
        }
    } else {
        set_dr_slice_settings(avctx, avctx->codec);
    }
}

// init driver
static int init(sh_video_t *sh){
    AVCodecContext *avctx;
    vd_ffmpeg_ctx *ctx;
    AVCodec *lavc_codec;
    int lowres_w=0;
    AVDictionary *opts = NULL;

    init_avcodec();

    ctx = sh->context = calloc(1, sizeof(*ctx));
    if (!ctx)
        return 0;

    lavc_codec = avcodec_find_decoder_by_name(codec_idx2str(sh->codec->dll_idx));
    if(!lavc_codec){
        mp_msg(MSGT_DECVIDEO, MSGL_ERR, MSGTR_MissingLAVCcodec, codec_idx2str(sh->codec->dll_idx));
        uninit(sh);
        return 0;
    }

    ctx->ip_count= ctx->b_count= 0;

    ctx->pic = av_frame_alloc();
    ctx->avctx = avcodec_alloc_context3(lavc_codec);
    avctx = ctx->avctx;
    avctx->opaque = sh;
    avctx->codec_id = lavc_codec->id;

    avctx->get_format = get_format;
    avctx->flags|= lavc_param_bitexact;

    avctx->coded_width = sh->disp_w;
    avctx->coded_height= sh->disp_h;
    avctx->workaround_bugs= lavc_param_workaround_bugs;
    switch (lavc_param_error_resilience) {
    case 5:
        avctx->err_recognition |= AV_EF_EXPLODE | AV_EF_COMPLIANT | AV_EF_CAREFUL;
        break;
    case 4:
    case 3:
        avctx->err_recognition |= AV_EF_AGGRESSIVE;
        // Fallthrough
    case 2:
        avctx->err_recognition |= AV_EF_COMPLIANT;
        // Fallthrough
    case 1:
        avctx->err_recognition |= AV_EF_CAREFUL;
    }
    lavc_param_gray|= AV_CODEC_FLAG_GRAY;
#ifdef AV_CODEC_FLAG2_SHOW_ALL
    if(!lavc_param_wait_keyframe) avctx->flags2 |= AV_CODEC_FLAG2_SHOW_ALL;
#endif
    avctx->flags2|= lavc_param_fast;
    avctx->codec_tag= sh->format;
    avctx->idct_algo= lavc_param_idct_algo;
    avctx->error_concealment= lavc_param_error_concealment;
    avctx->debug= lavc_param_debug;
    if (lavc_param_debug)
        av_log_set_level(AV_LOG_DEBUG);
    avctx->skip_top   = lavc_param_skip_top;
    avctx->skip_bottom= lavc_param_skip_bottom;
    if(lavc_param_lowres_str != NULL)
    {
        sscanf(lavc_param_lowres_str, "%d,%d", &lavc_param_lowres, &lowres_w);
        if(lavc_param_lowres < 1 || lavc_param_lowres > 16 || (lowres_w > 0 && avctx->width < lowres_w))
            lavc_param_lowres = 0;
        avctx->lowres = lavc_param_lowres;
    }
    avctx->skip_loop_filter = str2AVDiscard(lavc_param_skip_loop_filter_str);
    avctx->skip_idct        = str2AVDiscard(lavc_param_skip_idct_str);
    avctx->skip_frame       = str2AVDiscard(lavc_param_skip_frame_str);
    av_opt_set_int(avctx, "skip_alpha", 1, 0);

    if(lavc_avopt){
        if (parse_avopts(avctx, lavc_avopt) < 0 &&
            (!lavc_codec->priv_class ||
             parse_avopts(avctx->priv_data, lavc_avopt) < 0)) {
            mp_msg(MSGT_DECVIDEO, MSGL_ERR, "Your options /%s/ look like gibberish to me pal\n", lavc_avopt);
            uninit(sh);
            return 0;
        }
    }

    skip_idct = avctx->skip_idct;
    skip_frame = avctx->skip_frame;

    mp_dbg(MSGT_DECVIDEO, MSGL_DBG2, "libavcodec.size: %d x %d\n", avctx->width, avctx->height);
    switch (sh->format) {
    case mmioFOURCC('S','V','Q','3'):
    /* SVQ3 extradata can show up as sh->ImageDesc if demux_mov is used, or
       in the phony AVI header if demux_lavf is used. The first case is
       handled here; the second case falls through to the next section. */
        if (sh->ImageDesc) {
            avctx->extradata_size = (*(int *)sh->ImageDesc) - sizeof(int);
            avctx->extradata = av_mallocz(avctx->extradata_size + AV_INPUT_BUFFER_PADDING_SIZE);
            memcpy(avctx->extradata, ((int *)sh->ImageDesc)+1, avctx->extradata_size);
            break;
        }
        /* fallthrough */

    case mmioFOURCC('A','V','R','n'):
    case mmioFOURCC('M','J','P','G'):
    /* AVRn stores huffman table in AVI header */
    /* Pegasus MJPEG stores it also in AVI header, but it uses the common
       MJPG fourcc :( */
        if (!sh->bih || sh->bih->biSize <= sizeof(*sh->bih))
            break;
        av_dict_set(&opts, "extern_huff", "1", 0);
        avctx->extradata_size = sh->bih->biSize-sizeof(*sh->bih);
        avctx->extradata = av_mallocz(avctx->extradata_size + AV_INPUT_BUFFER_PADDING_SIZE);
        memcpy(avctx->extradata, sh->bih+1, avctx->extradata_size);

#if 0
        {
            int x;
            uint8_t *p = avctx->extradata;

            for (x=0; x<avctx->extradata_size; x++)
                mp_msg(MSGT_DECVIDEO, MSGL_INFO, "[%x] ", p[x]);
            mp_msg(MSGT_DECVIDEO, MSGL_INFO, "\n");
        }
#endif
        break;

    case mmioFOURCC('R', 'V', '1', '0'):
    case mmioFOURCC('R', 'V', '1', '3'):
    case mmioFOURCC('R', 'V', '2', '0'):
    case mmioFOURCC('R', 'V', '3', '0'):
    case mmioFOURCC('R', 'V', '4', '0'):
        if(sh->bih->biSize<sizeof(*sh->bih)+8){
            /* only 1 packet per frame & sub_id from fourcc */
            avctx->extradata_size= 8;
            avctx->extradata = av_mallocz(avctx->extradata_size + AV_INPUT_BUFFER_PADDING_SIZE);
            ((uint32_t *)avctx->extradata)[0] = 0;
            ((uint32_t *)avctx->extradata)[1] =
                (sh->format == mmioFOURCC('R', 'V', '1', '3')) ? 0x10003001 : 0x10000000;
        } else {
            /* has extra slice header (demux_rm or rm->avi streamcopy) */
            avctx->extradata_size = sh->bih->biSize-sizeof(*sh->bih);
            avctx->extradata = av_mallocz(avctx->extradata_size + AV_INPUT_BUFFER_PADDING_SIZE);
            memcpy(avctx->extradata, sh->bih+1, avctx->extradata_size);
        }

//        printf("%X %X %d %d\n", extrahdr[0], extrahdr[1]);
        break;

    default:
        if (!sh->bih || sh->bih->biSize <= (int)sizeof(*sh->bih))
            break;
        avctx->extradata_size = sh->bih->biSize-sizeof(*sh->bih);
        avctx->extradata = av_mallocz(avctx->extradata_size + AV_INPUT_BUFFER_PADDING_SIZE);
        if (!avctx->extradata) {
            avctx->extradata_size = 0;
            break;
        }
        memcpy(avctx->extradata, sh->bih+1, avctx->extradata_size);
        break;
    }

    if(sh->bih)
        avctx->bits_per_coded_sample= sh->bih->biBitCount;

    set_dr_slice_settings(avctx, lavc_codec);
    avctx->thread_count = lavc_param_threads;
    avctx->thread_type = FF_THREAD_FRAME | FF_THREAD_SLICE;
    av_dict_set(&opts, "refcounted_frames", "1", 0);

    /* open it */
    if (avcodec_open2(avctx, lavc_codec, &opts) < 0) {
        mp_msg(MSGT_DECVIDEO, MSGL_ERR, MSGTR_CantOpenCodec);
        uninit(sh);
        return 0;
    }
    av_dict_free(&opts);
    // this is necessary in case get_format was never called and init_vo is
    // too late e.g. for H.264 VDPAU
    set_format_params(avctx, avctx->pix_fmt);
    mp_msg(MSGT_DECVIDEO, MSGL_V, "INFO: libavcodec init OK!\n");
    return 1; //mpcodecs_config_vo(sh, sh->disp_w, sh->disp_h, IMGFMT_YV12);
}

// uninit driver
static void uninit(sh_video_t *sh){
    vd_ffmpeg_ctx *ctx = sh->context;
    AVCodecContext *avctx = ctx->avctx;

    if (ctx->refcount_frame) {
        av_frame_unref(ctx->refcount_frame);
        ctx->refcount_frame = NULL;
    }
    if(lavc_param_vstats){
        int i;
        for(i=0; i<32; i++){
            mp_msg(MSGT_DECVIDEO, MSGL_INFO, "QP: %d, count: %d\n", i, ctx->qp_stat[i]);
        }
        mp_msg(MSGT_DECVIDEO, MSGL_INFO, MSGTR_MPCODECS_ArithmeticMeanOfQP,
            ctx->qp_sum / avctx->frame_number,
            1.0/(ctx->inv_qp_sum / avctx->frame_number)
            );
    }

    if (avctx) {
        if (avctx->codec && avcodec_close(avctx) < 0)
            mp_msg(MSGT_DECVIDEO, MSGL_ERR, MSGTR_CantCloseCodec);

        av_freep(&avctx->extradata);
        av_freep(&avctx->hwaccel_context);
        av_freep(&avctx->slice_offset);
    }

    avcodec_free_context(&avctx);
    av_frame_free(&ctx->pic);
    free(ctx);
}

static void draw_slice(struct AVCodecContext *s,
                        const AVFrame *src, int offset[4],
                        int y, int type, int height){
    mp_image_t *mpi = src->opaque;
    sh_video_t *sh = s->opaque;
    vd_ffmpeg_ctx *ctx = sh->context;
    uint8_t *source[MP_MAX_PLANES]= {src->data[0] + offset[0], src->data[1] + offset[1], src->data[2] + offset[2]};
    int strides[MP_MAX_PLANES] = {src->linesize[0], src->linesize[1], src->linesize[2]};
    if (!src->data[0]) {
        mp_msg(MSGT_DECVIDEO, MSGL_FATAL, "BUG in FFmpeg, draw_slice called with NULL pointer!\n");
        return;
    }
    if (mpi && ctx->use_vdpau) {
        mp_msg(MSGT_DECVIDEO, MSGL_FATAL, "BUG in FFmpeg, draw_slice called for VDPAU!\n");
        return;
    }
    if (height < 0)
    {
        int i;
        height = -height;
        y -= height;
        for (i = 0; i < MP_MAX_PLANES; i++)
        {
            strides[i] = -strides[i];
            source[i] -= strides[i];
        }
    }
    if (y < sh->disp_h) {
        height = FFMIN(height, sh->disp_h-y);
        mpcodecs_draw_slice (sh, source, strides, sh->disp_w, height, 0, y);
    }
}

// ignore_aspect is used to not bother with aspect changes during get_buffer.
// The aspect can be wrong when get_buffer is called before decoding and it
// should simply not be necessary.
// In particular this stops spurious get_buffer aspect values from causing
// a container value to be overwritten.
static void update_configuration(sh_video_t *sh, enum AVPixelFormat pix_fmt, int ignore_aspect) {
    vd_ffmpeg_ctx *ctx = sh->context;
    AVCodecContext *avctx = ctx->avctx;
    int aspect_change = av_cmp_q(avctx->sample_aspect_ratio, ctx->last_sample_aspect_ratio);
    if (!avctx->sample_aspect_ratio.den) {
        mp_msg(MSGT_DECVIDEO, MSGL_WARN, "FFmpeg bug: Invalid aspect\n");
        avctx->sample_aspect_ratio.den = 1;
    }
     // it is possible another vo buffers to be used after vo config()
     // lavc reset its buffers on width/heigh change but not on aspect change!!!
    if ((!ignore_aspect && aspect_change) ||
        pix_fmt != ctx->pix_fmt ||
        !ctx->vo_initialized)
    {
        float aspect= av_q2d(avctx->sample_aspect_ratio) * avctx->width / avctx->height;
        ctx->vo_initialized = 0;
        // this is a special-case HACK for MPEG-1/2 VDPAU that uses neither get_format nor
        // sets the value correctly in avcodec_open.
        set_format_params(avctx, pix_fmt);

        if (!ignore_aspect) {
            mp_msg(MSGT_DECVIDEO, MSGL_V, "[ffmpeg] aspect_ratio: %f\n", aspect);

            // Do not overwrite sh->original_aspect on the first call,
            // if a container aspect is available (which is preferred then).
            // But set it even if the sample aspect did not change, since a
            // resolution change can cause an aspect change even if the
            // _sample_ aspect is unchanged.
            if (sh->original_aspect == 0 || (aspect_change && ctx->last_sample_aspect_ratio.den))
                sh->original_aspect = aspect;
            ctx->last_sample_aspect_ratio = avctx->sample_aspect_ratio;
        }
        ctx->pix_fmt = pix_fmt;
        ctx->best_csp = pixfmt2imgfmt2(pix_fmt, avctx->codec_id);
    }
}

static int is_in_format_list(sh_video_t *sh, int imgfmt)
{
    int i;
    for (i = 0; i < CODECS_MAX_OUTFMT; i++)
        if (sh->codec->outfmt[i] == imgfmt)
            return 1;
    return 0;
}

static int init_vo(sh_video_t *sh, enum AVPixelFormat pix_fmt, int ignore_aspect)
{
    vd_ffmpeg_ctx *ctx = sh->context;
    const AVCodecContext *avctx = ctx->avctx;
    int width, height;
    int imgfmt = pixfmt2imgfmt2(pix_fmt, avctx->codec_id);

    // avoid initialization for formats not on the supported
    // list in the codecs.conf entry.
    if (!is_in_format_list(sh, imgfmt)) {
        if (imgfmt)
            mp_msg(MSGT_DECVIDEO, MSGL_WARN, "Unexpected decoder output format %s\n",
                   vo_format_name(imgfmt));
        return -1;
    }

    width = avctx->width;
    height = avctx->height;

    // HACK!
    // if sh->ImageDesc is non-NULL, it means we decode QuickTime(tm) video.
    // use dimensions from BIH to avoid black borders at the right and bottom.
    if (sh->bih && sh->ImageDesc) {
        width = sh->bih->biWidth>>lavc_param_lowres;
        height = sh->bih->biHeight>>lavc_param_lowres;
    }
    if (width != sh->disp_w  || height != sh->disp_h)
        ctx->vo_initialized = 0;

    update_configuration(sh, pix_fmt, ignore_aspect);
    if (!ctx->vo_initialized)
    {
        sh->disp_w = width;
        sh->disp_h = height;
        if (!mpcodecs_config_vo(sh, sh->disp_w, sh->disp_h, ctx->best_csp))
            return -1;
        ctx->vo_initialized = 1;
    }
    return 0;
}

static int get_buffer(AVCodecContext *avctx, AVFrame *pic, int isreference){
    sh_video_t *sh = avctx->opaque;
    vd_ffmpeg_ctx *ctx = sh->context;
    mp_image_t *mpi=NULL;
    int flags= MP_IMGFLAG_ACCEPT_ALIGNED_STRIDE | MP_IMGFLAG_PREFER_ALIGNED_STRIDE;
    int type= MP_IMGTYPE_IPB;
    int width = FFMAX(avctx->width,  -(-avctx->coded_width  >> avctx->lowres));
    int height= FFMAX(avctx->height, -(-avctx->coded_height >> avctx->lowres));
    // special case to handle reget_buffer
    if (pic->opaque && pic->data[0])
        return 0;
    avcodec_align_dimensions(avctx, &width, &height);
//printf("get_buffer %d %d %d\n", pic->reference, ctx->ip_count, ctx->b_count);

        if(!isreference){
            ctx->b_count++;
            flags|=(avctx->skip_idct<=AVDISCARD_DEFAULT && avctx->skip_frame<=AVDISCARD_DEFAULT && ctx->do_slices) ?
                     MP_IMGFLAG_DRAW_CALLBACK:0;
        }else{
            ctx->ip_count++;
            flags|= MP_IMGFLAG_PRESERVE|MP_IMGFLAG_READABLE
                      | (ctx->do_slices ? MP_IMGFLAG_DRAW_CALLBACK : 0);
        }
        if(avctx->has_b_frames || ctx->b_count){
            type= MP_IMGTYPE_IPB;
        }else{
            type= MP_IMGTYPE_IP;
        }

    if (ctx->nonref_dr) {
        if (flags & MP_IMGFLAG_PRESERVE)
            return mpcodec_default_get_buffer(avctx, pic);
        // Use NUMBERED since for e.g. TEMP vos assume there will
        // be no other frames between the get_image and matching put_image.
        type = MP_IMGTYPE_NUMBERED;
    }

    if(init_vo(sh, avctx->pix_fmt, 1) < 0){
        goto disable_dr1;
    }

    if (IMGFMT_IS_HWACCEL(ctx->best_csp)) {
        type =  MP_IMGTYPE_NUMBERED;
    } else if (avctx->has_b_frames) {
        // HACK/TODO: slices currently do not work properly with B-frames,
        // causing out-of-order frames or crashes with e.g. -vf scale,unsharp
        // or -vf screenshot,unsharp.
        flags &= ~MP_IMGFLAG_DRAW_CALLBACK;
    }
    if (type == MP_IMGTYPE_IP || type == MP_IMGTYPE_IPB) {
        if(ctx->b_count>1 || ctx->ip_count>2){
            mp_msg(MSGT_DECVIDEO, MSGL_WARN, MSGTR_MPCODECS_DRIFailure);
            goto disable_dr1;
        }

        mp_msg(MSGT_DECVIDEO, MSGL_DBG2, type== MP_IMGTYPE_IPB ? "using IPB\n" : "using IP\n");
    }

    // Make sure to not leak dead pointers.
    pic->data[0]= NULL;
    pic->data[1]= NULL;
    pic->data[2]= NULL;
    pic->data[3]= NULL;

    if (ctx->best_csp == IMGFMT_RGB8 || ctx->best_csp == IMGFMT_BGR8)
        flags |= MP_IMGFLAG_RGB_PALETTE;
    mpi= mpcodecs_get_image(sh, type, flags, width, height);
    if (!mpi) return -1;

    // ok, let's see what did we get:
    if(mpi->flags&MP_IMGFLAG_DRAW_CALLBACK &&
       !(mpi->flags&MP_IMGFLAG_DIRECT)){
        // nice, filter/vo likes draw_callback :)
        avctx->draw_horiz_band= draw_slice;
    } else
        avctx->draw_horiz_band= NULL;
#if CONFIG_VDPAU
    if (ctx->use_vdpau) {
        VdpVideoSurface surface = (VdpVideoSurface)mpi->priv;
        avctx->draw_horiz_band= NULL;
        mpi->planes[3] = surface;
    }
#endif
#if CONFIG_XVMC
    if(IMGFMT_IS_XVMC(mpi->imgfmt)) {
        struct xvmc_pix_fmt *render = mpi->priv; //same as data[2]
        if(!(mpi->flags & MP_IMGFLAG_DIRECT)) {
            mp_msg(MSGT_DECVIDEO, MSGL_ERR, MSGTR_MPCODECS_OnlyBuffersAllocatedByVoXvmcAllowed);
            assert(0);
            return -1;//!!fixme check error conditions in ffmpeg
        }
        if(mp_msg_test(MSGT_DECVIDEO, MSGL_DBG5))
            mp_msg(MSGT_DECVIDEO, MSGL_DBG5, "vd_ffmpeg::get_buffer (xvmc render=%p)\n", render);
        assert(render != 0);
        assert(render->xvmc_id == AV_XVMC_ID);
        avctx->draw_horiz_band= draw_slice;
    }
#endif

    pic->data[0]= mpi->planes[0];
    pic->data[1]= mpi->planes[1];
    pic->data[2]= mpi->planes[2];
    pic->data[3]= mpi->planes[3];

#if 0
    assert(mpi->width >= ((width +align)&(~align)));
    assert(mpi->height >= ((height+align)&(~align)));
    assert(mpi->stride[0] >= mpi->width);
    if(mpi->imgfmt==IMGFMT_I420 || mpi->imgfmt==IMGFMT_YV12 || mpi->imgfmt==IMGFMT_IYUV){
        const int y_size= mpi->stride[0] * (mpi->h-1) + mpi->w;
        const int c_size= mpi->stride[1] * ((mpi->h>>1)-1) + (mpi->w>>1);

        assert(mpi->planes[0] > mpi->planes[1] || mpi->planes[0] + y_size <= mpi->planes[1]);
        assert(mpi->planes[0] > mpi->planes[2] || mpi->planes[0] + y_size <= mpi->planes[2]);
        assert(mpi->planes[1] > mpi->planes[0] || mpi->planes[1] + c_size <= mpi->planes[0]);
        assert(mpi->planes[1] > mpi->planes[2] || mpi->planes[1] + c_size <= mpi->planes[2]);
        assert(mpi->planes[2] > mpi->planes[0] || mpi->planes[2] + c_size <= mpi->planes[0]);
        assert(mpi->planes[2] > mpi->planes[1] || mpi->planes[2] + c_size <= mpi->planes[1]);
    }
#endif

    /* Note, some (many) codecs in libavcodec must have stride1==stride2 && no changes between frames
     * lavc will check that and die with an error message, if its not true
     */
    pic->linesize[0]= mpi->stride[0];
    pic->linesize[1]= mpi->stride[1];
    pic->linesize[2]= mpi->stride[2];
    pic->linesize[3]= mpi->stride[3];

    pic->width  = avctx->width;
    pic->height = avctx->height;
    pic->format = avctx->pix_fmt;

    pic->opaque = mpi;
//printf("%X\n", (int)mpi->planes[0]);
#if 0
if(mpi->flags&MP_IMGFLAG_DIRECT)
    printf("D");
else if(mpi->flags&MP_IMGFLAG_DRAW_CALLBACK)
    printf("S");
else
    printf(".");
#endif
    return 0;

disable_dr1:
    ctx->do_dr1 = 0;
    // For frame-multithreading these contexts aren't
    // the same and must both be updated.
    ctx->avctx->get_buffer2   =
    avctx->get_buffer2 = avcodec_default_get_buffer2;
    if (pic->data[0])
        mpcodec_default_release_buffer(avctx, pic);
    return avctx->get_buffer2(avctx, pic,0);
}

static void release_buffer(struct AVCodecContext *avctx, AVFrame *pic){
    mp_image_t *mpi= pic->opaque;
    sh_video_t *sh = avctx->opaque;
    vd_ffmpeg_ctx *ctx = sh->context;
    int i;
    if (pic->opaque == NULL) {
        mpcodec_default_release_buffer(avctx, pic);
        return;
    }

    if (mpi) {
//printf("release buffer %d %d %d\n", mpi ? mpi->flags&MP_IMGFLAG_PRESERVE : -99, ctx->ip_count, ctx->b_count);

        if(mpi->flags&MP_IMGFLAG_PRESERVE)
            ctx->ip_count--;
        else
            ctx->b_count--;

        // release mpi (in case MPI_IMGTYPE_NUMBERED is used, e.g. for VDPAU)
        mpi->usage_count--;
        if (mpi->usage_count < 0) {
            mp_msg(MSGT_DECVIDEO, MSGL_ERR, "Bad mp_image usage count, please report!\n");
            mpi->usage_count = 0;
        }
    }

    for(i=0; i<4; i++){
        pic->data[i]= NULL;
    }
//printf("R%X %X\n", pic->linesize[0], pic->data[0]);
}

static av_unused void swap_palette(void *pal)
{
    int i;
    uint32_t *p = pal;
    for (i = 0; i < AVPALETTE_COUNT; i++)
        p[i] = le2me_32(p[i]);
}

// decode a frame
static mp_image_t *decode(sh_video_t *sh, void *data, int len, int flags){
    int got_picture=0;
    int ret;
    vd_ffmpeg_ctx *ctx = sh->context;
    AVFrame *pic= ctx->pic;
    AVCodecContext *avctx = ctx->avctx;
    mp_image_t *mpi=NULL;
    int dr1= ctx->do_dr1;
    AVPacket pkt;

    if (ctx->refcount_frame) {
        av_frame_unref(ctx->refcount_frame);
        ctx->refcount_frame = NULL;
    }
    if(data && len<=0) return NULL; // skipped frame

//ffmpeg interlace (mpeg2) bug have been fixed. no need of -noslices
    if (!dr1)
    avctx->draw_horiz_band=NULL;
    if(ctx->vo_initialized && !(flags&3) && !dr1){
        mpi=mpcodecs_get_image(sh, MP_IMGTYPE_EXPORT, MP_IMGFLAG_PRESERVE |
            (ctx->do_slices?MP_IMGFLAG_DRAW_CALLBACK:0),
            sh->disp_w, sh->disp_h);
        if(mpi && mpi->flags&MP_IMGFLAG_DRAW_CALLBACK){
            // vd core likes slices!
            avctx->draw_horiz_band=draw_slice;
        }
    }

    avctx->skip_idct = skip_idct;
    avctx->skip_frame = skip_frame;

    if (flags&3) {
        avctx->skip_frame = AVDISCARD_NONREF;
        if (flags&2)
            avctx->skip_idct = AVDISCARD_ALL;
    }

    if (data)
    mp_msg(MSGT_DECVIDEO, MSGL_DBG2, "vd_ffmpeg data: %04x, %04x, %04x, %04x\n",
           ((int *)data)[0], ((int *)data)[1], ((int *)data)[2], ((int *)data)[3]);
    av_init_packet(&pkt);
    pkt.data = data;
    pkt.size = len;
    // Necessary to decode e.g. CorePNG and ZeroCodec correctly
    pkt.flags = (sh->ds->flags & 1) ? AV_PKT_FLAG_KEY : 0;
    mp_packet_split_side_data(&pkt);
    if (av_packet_get_side_data(&pkt, AV_PKT_DATA_PALETTE, NULL))
        ctx->palette_sent = 1;
    if (!ctx->palette_sent && sh->bih && sh->bih->biBitCount <= 8) {
        /* Pass palette to codec */
        uint8_t *pal_data = (uint8_t *)(sh->bih+1);
        unsigned palsize = sh->bih->biSize - sizeof(*sh->bih);
        unsigned needed_size = 4 << sh->bih->biBitCount;
        // Assume palette outside bih in rest of chunk.
        // Fixes samples/V-codecs/QPEG/MITSUMI.AVI
        if (palsize < needed_size &&
            sh->bih_size > sh->bih->biSize &&
            sh->bih_size - sh->bih->biSize > palsize) {
            pal_data = (uint8_t *)sh->bih + sh->bih->biSize;
            palsize = sh->bih_size - sh->bih->biSize;
        }
        // if still 0, we simply have no palette in extradata.
        if (palsize) {
            uint8_t *pal = av_packet_new_side_data(&pkt, AV_PKT_DATA_PALETTE, AVPALETTE_SIZE);
            memcpy(pal, pal_data, FFMIN(palsize, AVPALETTE_SIZE));
        }
        ctx->palette_sent = 1;
    }
    if (!sh->needs_parsing && sh->ds->buffer_pos < len)
        mp_msg(MSGT_DECVIDEO, MSGL_ERR, "Bad stream state, please report as bug!\n");
    ret = avcodec_send_packet(avctx, !pkt.data && !pkt.size ? NULL : &pkt);
    if (ret == AVERROR(EAGAIN)) {
        if (!sh->needs_parsing && sh->ds->buffer_pos >= len) sh->ds->buffer_pos -= len;
        ret = 0;
    }
    if (ret >= 0 || ret == AVERROR_EOF) {
        ret = avcodec_receive_frame(avctx, pic);
        got_picture = ret >= 0;
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) ret = 0;
    }
    ctx->refcount_frame = pic;
    pkt.data = NULL;
    pkt.size = 0;
    av_packet_free_side_data(&pkt);

    // even when we do dr we might actually get a buffer we had
    // FFmpeg allocate - this mostly happens with nonref_dr.
    // Ensure we treat it correctly.
    dr1= ctx->do_dr1 && pic->opaque != NULL;
    if(ret<0) mp_msg(MSGT_DECVIDEO, MSGL_WARN, "Error while decoding frame! (%i)\n", ret);
//printf("repeat: %d\n", pic->repeat_pict);
//-- vstats generation
    while(lavc_param_vstats){ // always one time loop
        static FILE *fvstats=NULL;
        char filename[20];
        static long long int all_len=0;
        static int frame_number=0;
        static double all_frametime=0.0;
        double quality=0.0;

        if(!got_picture) break;

        if(!fvstats) {
            time_t today2;
            struct tm *today;
            today2 = time(NULL);
            today = localtime(&today2);
            sprintf(filename, "vstats_%02d%02d%02d.log", today->tm_hour,
                today->tm_min, today->tm_sec);
            fvstats = fopen(filename, "w");
            if(!fvstats) {
                perror("fopen");
                lavc_param_vstats=0; // disable block
                break;
                /*exit(1);*/
            }
        }

        // average MB quantizer
        {
// TODO: still possible in new FFmpeg API?
#if 0
            int x, y;
            int w = ((avctx->width  << lavc_param_lowres)+15) >> 4;
            int h = ((avctx->height << lavc_param_lowres)+15) >> 4;
            int qstride;
            int dummy;
            int8_t *q = av_frame_get_qp_table(pic, &qstride, &dummy);
            for(y = 0; y < h; y++) {
                for(x = 0; x < w; x++)
                    quality += q[x];
                q += qstride;
            }
            quality /= w * h;
#endif
        }

        all_len+=len;
        all_frametime+=sh->frametime;
        fprintf(fvstats, "frame= %5d q= %2.2f f_size= %6d s_size= %8.0fkB ",
            ++frame_number, quality, len, (double)all_len/1024);
        fprintf(fvstats, "time= %0.3f br= %7.1fkbits/s avg_br= %7.1fkbits/s ",
           all_frametime, (double)(len*8)/sh->frametime/1000.0,
           (double)(all_len*8)/all_frametime/1000.0);
        switch(pic->pict_type){
        case AV_PICTURE_TYPE_I:
            fprintf(fvstats, "type= I\n");
            break;
        case AV_PICTURE_TYPE_P:
            fprintf(fvstats, "type= P\n");
            break;
        case AV_PICTURE_TYPE_S:
            fprintf(fvstats, "type= S\n");
            break;
        case AV_PICTURE_TYPE_B:
            fprintf(fvstats, "type= B\n");
            break;
        default:
            fprintf(fvstats, "type= ? (%d)\n", pic->pict_type);
            break;
        }

        ctx->qp_stat[(int)(quality+0.5)]++;
        ctx->qp_sum += quality;
        ctx->inv_qp_sum += 1.0/FFMAX(quality, 1);

        break;
    }
//--

    if(!got_picture) {
        if (avctx->codec->id == AV_CODEC_ID_H264 &&
	    skip_frame <= AVDISCARD_DEFAULT)
	    return &mpi_no_picture; // H.264 first field only
	else
	    return NULL;    // skipped image
    }

    if(init_vo(sh, avctx->pix_fmt, 0) < 0) return NULL;

    if(dr1 && pic->opaque){
        mpi=pic->opaque;
    }

    if(!mpi)
    mpi=mpcodecs_get_image(sh, MP_IMGTYPE_EXPORT, MP_IMGFLAG_PRESERVE,
        pic->width, pic->height);
    if(!mpi){        // temporary!
        mp_msg(MSGT_DECVIDEO, MSGL_WARN, MSGTR_MPCODECS_CouldntAllocateImageForCodec);
        return NULL;
    }

    if (mpi->w != avctx->width || mpi->h != avctx->height ||
        pic->width < mpi->w || pic->height < mpi->h) {
        mp_msg(MSGT_DECVIDEO, MSGL_ERR, "Dropping frame with size not matching configured size (%ix%i vs %ix%i vs %ix%i)\n",
               mpi->w, mpi->h, pic->width, pic->height, avctx->width, avctx->height);
        return NULL;
    }

    if(!dr1){
        mpi->planes[0]=pic->data[0];
        mpi->planes[1]=pic->data[1];
        mpi->planes[2]=pic->data[2];
        mpi->planes[3]=pic->data[3];
        mpi->stride[0]=pic->linesize[0];
        mpi->stride[1]=pic->linesize[1];
        mpi->stride[2]=pic->linesize[2];
        mpi->stride[3]=pic->linesize[3];
    }

    if (!mpi->planes[0])
        return NULL;

    if(ctx->best_csp == IMGFMT_422P && mpi->chroma_y_shift==1){
        // we have 422p but user wants 420p
        mpi->stride[1]*=2;
        mpi->stride[2]*=2;
    }

#if HAVE_BIGENDIAN
    // FIXME: this might cause problems for buffers with FF_BUFFER_HINTS_PRESERVE
    if (mpi->bpp == 8)
        swap_palette(mpi->planes[1]);
#endif
/* to comfirm with newer lavc style */
// TODO: still possible in new FFmpeg API?
//    mpi->qscale = av_frame_get_qp_table(pic, &mpi->qstride, &mpi->qscale_type);
    mpi->pict_type=pic->pict_type;
    mpi->fields = MP_IMGFIELD_ORDERED;
    if(pic->interlaced_frame) mpi->fields |= MP_IMGFIELD_INTERLACED;
    if(pic->top_field_first ) mpi->fields |= MP_IMGFIELD_TOP_FIRST;
    if(pic->repeat_pict == 1) mpi->fields |= MP_IMGFIELD_REPEAT_FIRST;

    return mpi;
}

static enum AVPixelFormat get_format(struct AVCodecContext *avctx,
                                     const enum AVPixelFormat *fmt)
{
    enum AVPixelFormat selected_format;
    int imgfmt;
    sh_video_t *sh = avctx->opaque;
    vd_ffmpeg_ctx *ctx = sh->context;
    int i;

    // Try to select identical format to avoid reinitializations
    if (ctx->vo_initialized && ctx->pix_fmt != AV_PIX_FMT_NONE)
        for (i = 0; fmt[i] != AV_PIX_FMT_NONE; i++)
            if (fmt[i] == ctx->pix_fmt) return ctx->pix_fmt;

    for(i=0;fmt[i]!=AV_PIX_FMT_NONE;i++){
        imgfmt = pixfmt2imgfmt2(fmt[i], avctx->codec_id);
        if(!IMGFMT_IS_HWACCEL(imgfmt) || !is_in_format_list(sh, imgfmt)) continue;
        mp_msg(MSGT_DECVIDEO, MSGL_V, MSGTR_MPCODECS_TryingPixfmt, i);
        if(init_vo(sh, fmt[i], 1) >= 0) {
            break;
        }
    }
    selected_format = fmt[i];
    if (selected_format == AV_PIX_FMT_NONE) {
        selected_format = avcodec_default_get_format(avctx, fmt);
        update_configuration(sh, selected_format, 1);
    }
    set_format_params(avctx, selected_format);
    return selected_format;
}


/*
 FFWrapper
*/
static int mpcodec_default_get_buffer(AVCodecContext *avctx, AVFrame *frame)
{
    return avcodec_default_get_buffer2(avctx, frame, 0);
}

static void mpcodec_default_release_buffer(AVCodecContext *s, AVFrame *pic)
{
    av_frame_unref(pic);
}

typedef struct CompatReleaseBufPriv {
    AVCodecContext avctx;
    AVFrame frame;
    uint8_t avframe_padding[1024]; // hack to allow linking to a avutil with larger AVFrame
} CompatReleaseBufPriv;

static void compat_free_buffer(void *opaque, uint8_t *data)
{
    CompatReleaseBufPriv *priv = opaque;
    release_buffer(&priv->avctx, &priv->frame);
    av_freep(&priv);
}

static void compat_release_buffer(void *opaque, uint8_t *data)
{
    AVBufferRef *buf = opaque;
    av_buffer_unref(&buf);
}

static int get_buffer2(AVCodecContext *avctx, AVFrame *frame, int flags)
{
    /*
     * Wrap an old get_buffer()-allocated buffer in a bunch of AVBuffers.
     * We wrap each plane in its own AVBuffer. Each of those has a reference to
     * a dummy AVBuffer as its private data, unreffing it on free.
     * When all the planes are freed, the dummy buffer's free callback calls
     * release_buffer().
     */
    CompatReleaseBufPriv *priv = NULL;
    AVBufferRef *dummy_buf = NULL;
    int planes, i, ret, w = avctx->width, h = avctx->height;

    ret = get_buffer(avctx, frame, flags & AV_GET_BUFFER_FLAG_REF);
    if (ret < 0)
        return ret;

    /* return if the buffers are already set up
     * this would happen e.g. when a custom get_buffer() calls
     * avcodec_default_get_buffer
     */
    if (frame->buf[0])
        goto end0;

    priv = av_mallocz(sizeof(*priv));
    if (!priv) {
        ret = AVERROR(ENOMEM);
        goto fail;
    }
    priv->avctx = *avctx;
    priv->frame = *frame;

    dummy_buf = av_buffer_create(NULL, 0, compat_free_buffer, priv, 0);
    if (!dummy_buf) {
        ret = AVERROR(ENOMEM);
        goto fail;
    }

#define WRAP_PLANE(ref_out, data, data_size)                            \
do {                                                                    \
    AVBufferRef *dummy_ref = av_buffer_ref(dummy_buf);                  \
    if (!dummy_ref) {                                                   \
        ret = AVERROR(ENOMEM);                                          \
        goto fail;                                                      \
    }                                                                   \
    ref_out = av_buffer_create(data, data_size, compat_release_buffer,  \
                               dummy_ref, 0);                           \
    if (!ref_out) {                                                     \
        av_buffer_unref(&dummy_ref);                                    \
        av_frame_unref(frame);                                          \
        ret = AVERROR(ENOMEM);                                          \
        goto fail;                                                      \
    }                                                                   \
} while (0)

    {
        const AVPixFmtDescriptor *desc = av_pix_fmt_desc_get(frame->format);

        planes = av_pix_fmt_count_planes(frame->format);
        /* workaround for AVHWAccel plane count of 0, buf[0] is used as
           check for allocated buffers: make libavcodec happy */
        /* newest FFmpeg versions in addition need to coded_width
         * as frame width value in case of hwaccel codecs or they
         * will apply the cropping amount twice */
        if (desc && desc->flags & AV_PIX_FMT_FLAG_HWACCEL) {
            planes = 1;
            w = avctx->coded_width;
            h = avctx->coded_height;
        }
        if (!desc || planes <= 0) {
            ret = AVERROR(EINVAL);
            goto fail;
        }

        for (i = 0; i < planes; i++) {
            int v_shift    = (i == 1 || i == 2) ? desc->log2_chroma_h : 0;
            int plane_size = (frame->height >> v_shift) * frame->linesize[i];

            WRAP_PLANE(frame->buf[i], frame->data[i], plane_size);
        }
    }

    av_buffer_unref(&dummy_buf);

end0:
    frame->width  = w;
    frame->height = h;

    return 0;

fail:
    release_buffer(avctx, frame);
    av_freep(&priv);
    av_buffer_unref(&dummy_buf);
    return ret;
}
