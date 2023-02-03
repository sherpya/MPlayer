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

#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <assert.h>

#if !defined(INFINITY) && defined(HUGE_VAL)
#define INFINITY HUGE_VAL
#endif

#include "config.h"
#include "libavutil/avstring.h"
#include "libavutil/intreadwrite.h"
#include "mencoder.h"
#include "mp_msg.h"
#include "help_mp.h"
#include "av_opts.h"
#include "osdep/strsep.h"

#include "codec-cfg.h"
#include "stream/stream.h"
#include "libmpdemux/demuxer.h"
#include "libmpdemux/stheader.h"

#include "libmpdemux/muxer.h"

#include "img_format.h"
#include "fmt-conversion.h"
#include "mp_image.h"
#include "ve.h"
#include "vf.h"
#include "av_helpers.h"

//===========================================================================//

#include "libavcodec/avcodec.h"


/* video options */
static char *lavc_param_vcodec = "mpeg4";
static int lavc_param_vbitrate = -1;
static int lavc_param_vrate_tolerance = 1000*8;
static int lavc_param_mb_decision = 0; /* default is realtime encoding */
static int lavc_param_v4mv = 0;
static float lavc_param_vqscale = -1;
static int lavc_param_vqmin = 2;
static int lavc_param_vqmax = 31;
static float lavc_param_mb_lmin = 2;
static float lavc_param_mb_lmax = 31;
static int lavc_param_vqdiff = 3;
static float lavc_param_vqcompress = 0.5;
static float lavc_param_vqblur = 0.5;
static float lavc_param_vb_qfactor = 1.25;
static float lavc_param_vb_qoffset = 1.25;
static float lavc_param_vi_qfactor = 0.8;
static float lavc_param_vi_qoffset = 0.0;
static int lavc_param_vmax_b_frames = 0;
static int lavc_param_keyint = -1;
static int lavc_param_vpass = 0;
static int lavc_param_vb_strategy = 0;
static int lavc_param_packet_size= 0;
static int lavc_param_strict= -1;
static int lavc_param_data_partitioning= 0;
static int lavc_param_gray=0;
static char *lavc_param_rc_override_string=NULL;
static int lavc_param_rc_buffer_size=0;
static int lavc_param_rc_max_rate=0;
static int lavc_param_rc_min_rate=0;
static float lavc_param_rc_initial_buffer_occupancy=0.9;
static int lavc_param_mpeg_quant=0;
static int lavc_param_fdct=0;
static int lavc_param_idct=0;
static char* lavc_param_aspect = NULL;
static int lavc_param_autoaspect = 0;
static float lavc_param_lumi_masking= 0.0;
static float lavc_param_dark_masking= 0.0;
static float lavc_param_temporal_cplx_masking= 0.0;
static float lavc_param_spatial_cplx_masking= 0.0;
static float lavc_param_p_masking= 0.0;
static int lavc_param_interlaced_dct= 0;
static int lavc_param_prediction_method= 0; // method left (for some encoders)
static int lavc_param_format= IMGFMT_YV12;
static int lavc_param_debug= 0;
static int lavc_param_psnr= 0;
static int lavc_param_me_pre_cmp= 0;
static int lavc_param_me_cmp= 0;
static int lavc_param_me_sub_cmp= 0;
static int lavc_param_mb_cmp= 0;
#ifdef FF_CMP_VSAD
static int lavc_param_ildct_cmp= FF_CMP_VSAD;
#endif
static int lavc_param_pre_dia_size= 0;
static int lavc_param_dia_size= 0;
static int lavc_param_qpel= 0;
static int lavc_param_trell= 0;
static int lavc_param_lowdelay= 0;
static int lavc_param_bit_exact = 0;
static int lavc_param_aic= 0;
static int lavc_param_aiv= 0;
static int lavc_param_umv= 0;
static int lavc_param_obmc= 0;
static int lavc_param_loop= 0;
static int lavc_param_last_pred= 0;
static int lavc_param_pre_me= 0;
static int lavc_param_me_subpel_quality= 8;
static int lavc_param_me_range= 0;
static int lavc_param_coder= 0;
static int lavc_param_context= 0;
static char *lavc_param_intra_matrix = NULL;
static char *lavc_param_inter_matrix = NULL;
static int lavc_param_mv0= 0;
static int lavc_param_noise_reduction= 0;
static int lavc_param_sc_threshold= 0;
static int lavc_param_ss= 0;
static int lavc_param_top= -1;
static int lavc_param_alt= 0;
static int lavc_param_ilme= 0;
static int lavc_param_nssew= 8;
static int lavc_param_closed_gop = 0;
static int lavc_param_dc_precision = 8;
static int lavc_param_threads= 1;
static int lavc_param_turbo = 0;
static int lavc_param_skip_threshold=0;
static int lavc_param_skip_factor=0;
static int lavc_param_skip_exp=0;
static int lavc_param_skip_cmp = FF_CMP_DCTMAX;
static int lavc_param_brd_scale = 0;
static int lavc_param_bidir_refine = 0;
static int lavc_param_video_global_header= 0;
static int lavc_param_mv0_threshold = 256;
static int lavc_param_refs = 1;
static int lavc_param_b_sensitivity = 40;
static int lavc_param_level = FF_LEVEL_UNKNOWN;

char *lavc_param_acodec = "mp2";
int lavc_param_atag = 0;
int lavc_param_abitrate = 224;
int lavc_param_audio_global_header= 0;
static char *lavc_param_avopt = NULL;
char *lavc_param_audio_avopt = NULL;

#include "m_option.h"

#define MAX_BITRATE 100000000 /* 100Mbit */

const m_option_t lavcopts_conf[]={
	{"acodec", &lavc_param_acodec, CONF_TYPE_STRING, 0, 0, 0, NULL},
	{"abitrate", &lavc_param_abitrate, CONF_TYPE_INT, CONF_RANGE, 1, 1000000, NULL},
	{"atag", &lavc_param_atag, CONF_TYPE_INT, CONF_RANGE, 0, 0xffff, NULL},
	{"vcodec", &lavc_param_vcodec, CONF_TYPE_STRING, 0, 0, 0, NULL},
	{"vbitrate", &lavc_param_vbitrate, CONF_TYPE_INT, CONF_RANGE, 4, MAX_BITRATE, NULL},
	{"vratetol", &lavc_param_vrate_tolerance, CONF_TYPE_INT, CONF_RANGE, 4, MAX_BITRATE, NULL},
	{"vhq", &lavc_param_mb_decision, CONF_TYPE_FLAG, 0, 0, 1, NULL},
	{"mbd", &lavc_param_mb_decision, CONF_TYPE_INT, CONF_RANGE, 0, 9, NULL},
	{"v4mv", &lavc_param_v4mv, CONF_TYPE_FLAG, 0, 0, 1, NULL},
	{"vme", "vme has no effect, please use the corresponding codec specific option (see FFmpeg documentation) instead of vme.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
	{"vqscale", &lavc_param_vqscale, CONF_TYPE_FLOAT, CONF_RANGE, 0.0, 255.0, NULL},
	{"vqmin", &lavc_param_vqmin, CONF_TYPE_INT, CONF_RANGE, 1, 31, NULL},
	{"vqmax", &lavc_param_vqmax, CONF_TYPE_INT, CONF_RANGE, 1, 31, NULL},
        {"lmin", "Please use o=lmin=<value>*QP2LAMBDA instead of lmin.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
        {"lmax", "Please use o=lmax=<value>*QP2LAMBDA instead of lmax.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
	{"mblmin", &lavc_param_mb_lmin, CONF_TYPE_FLOAT, CONF_RANGE, 0.01, 255.0, NULL},
	{"mblmax", &lavc_param_mb_lmax, CONF_TYPE_FLOAT, CONF_RANGE, 0.01, 255.0, NULL},
	{"vqdiff", &lavc_param_vqdiff, CONF_TYPE_INT, CONF_RANGE, 1, 31, NULL},
	{"vqcomp", &lavc_param_vqcompress, CONF_TYPE_FLOAT, CONF_RANGE, 0.0, 1.0, NULL},
	{"vqblur", &lavc_param_vqblur, CONF_TYPE_FLOAT, CONF_RANGE, 0.0, 1.0, NULL},
	{"vb_qfactor", &lavc_param_vb_qfactor, CONF_TYPE_FLOAT, CONF_RANGE, -31.0, 31.0, NULL},
	{"vmax_b_frames", &lavc_param_vmax_b_frames, CONF_TYPE_INT, CONF_RANGE, 0, 16, NULL}, // FF_MAX_B_FRAMES was removed from FFmpeg. We still use its value here, so we probably limit ourselves in some cases.
	{"vpass", &lavc_param_vpass, CONF_TYPE_INT, CONF_RANGE, 0, 3, NULL},
	{"vrc_strategy", "Please use o=rc_strategy=<value> instead of vrc_strategy.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
	{"vb_strategy", &lavc_param_vb_strategy, CONF_TYPE_INT, CONF_RANGE, 0, 10, NULL},
	{"vb_qoffset", &lavc_param_vb_qoffset, CONF_TYPE_FLOAT, CONF_RANGE, 0.0, 31.0, NULL},
	{"vlelim", "Please use o=luma_elim_threshold=<value> instead of vlelim.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
	{"vcelim", "Please use o=chroma_elim_threshold=<value> instead of vcelim.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
	{"vpsize", &lavc_param_packet_size, CONF_TYPE_INT, CONF_RANGE, 0, 100000000, NULL},
	{"vstrict", &lavc_param_strict, CONF_TYPE_INT, CONF_RANGE, -99, 99, NULL},
	{"vdpart", &lavc_param_data_partitioning, CONF_TYPE_FLAG, 0, 0, 1, NULL},
	{"keyint", &lavc_param_keyint, CONF_TYPE_INT, 0, 0, 0, NULL},
	{"gray", &lavc_param_gray, CONF_TYPE_FLAG, 0, 0, AV_CODEC_FLAG_GRAY, NULL},
	{"mpeg_quant", &lavc_param_mpeg_quant, CONF_TYPE_FLAG, 0, 0, 1, NULL},
	{"vi_qfactor", &lavc_param_vi_qfactor, CONF_TYPE_FLOAT, CONF_RANGE, -31.0, 31.0, NULL},
	{"vi_qoffset", &lavc_param_vi_qoffset, CONF_TYPE_FLOAT, CONF_RANGE, 0.0, 31.0, NULL},
	{"vqsquish", "Please use o=qsquish=<value> instead of vqsquish.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
	{"vqmod_amp", "Please use o=rc_qmod_amp=<value> instead of vqmod_amp.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
	{"vqmod_freq", "Please use o=rc_qmod_freq=<value> instead of vqmod_freq.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
	{"vrc_eq", "Please use o=rc_eq=<value> instead of vrc_eq.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
	{"vrc_override", &lavc_param_rc_override_string, CONF_TYPE_STRING, 0, 0, 0, NULL},
	{"vrc_maxrate", &lavc_param_rc_max_rate, CONF_TYPE_INT, CONF_RANGE, 0, MAX_BITRATE, NULL},
	{"vrc_minrate", &lavc_param_rc_min_rate, CONF_TYPE_INT, CONF_RANGE, 0, MAX_BITRATE, NULL},
	{"vrc_buf_size", &lavc_param_rc_buffer_size, CONF_TYPE_INT, CONF_RANGE, 4, MAX_BITRATE, NULL},
	{"vrc_buf_aggressivity", "Please use o=rc_buf_aggressivity=<value> instead of vrc_buf_aggressivity.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
	{"vrc_init_cplx", "Please use o=rc_init_cplx=<value> instead of vrc_init_cplx.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
	{"vrc_init_occupancy", &lavc_param_rc_initial_buffer_occupancy, CONF_TYPE_FLOAT, CONF_RANGE, 0.0, 1.0, NULL},
        {"vfdct", &lavc_param_fdct, CONF_TYPE_INT, CONF_RANGE, 0, 10, NULL},
	{"aspect", &lavc_param_aspect, CONF_TYPE_STRING, 0, 0, 0, NULL},
	{"autoaspect", &lavc_param_autoaspect, CONF_TYPE_FLAG, 0, 0, 1, NULL},
	{"lumi_mask", &lavc_param_lumi_masking, CONF_TYPE_FLOAT, CONF_RANGE, -1.0, 1.0, NULL},
	{"tcplx_mask", &lavc_param_temporal_cplx_masking, CONF_TYPE_FLOAT, CONF_RANGE, -1.0, 1.0, NULL},
	{"scplx_mask", &lavc_param_spatial_cplx_masking, CONF_TYPE_FLOAT, CONF_RANGE, -1.0, 1.0, NULL},
	{"p_mask", &lavc_param_p_masking, CONF_TYPE_FLOAT, CONF_RANGE, -1.0, 1.0, NULL},
	{"naq", "Please use o=mpv_flags=+naq instead of naq.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
	{"dark_mask", &lavc_param_dark_masking, CONF_TYPE_FLOAT, CONF_RANGE, -1.0, 1.0, NULL},
	{"ildct", &lavc_param_interlaced_dct, CONF_TYPE_FLAG, 0, 0, 1, NULL},
        {"idct", &lavc_param_idct, CONF_TYPE_INT, CONF_RANGE, 0, 20, NULL},
        {"pred", &lavc_param_prediction_method, CONF_TYPE_INT, CONF_RANGE, 0, 20, NULL},
        {"format", &lavc_param_format, CONF_TYPE_IMGFMT, 0, 0, 0, NULL},
        {"debug", &lavc_param_debug, CONF_TYPE_INT, CONF_RANGE, 0, 100000000, NULL},
        {"psnr", &lavc_param_psnr, CONF_TYPE_FLAG, 0, 0, AV_CODEC_FLAG_PSNR, NULL},
        {"precmp", &lavc_param_me_pre_cmp, CONF_TYPE_INT, CONF_RANGE, 0, 2000, NULL},
        {"cmp", &lavc_param_me_cmp, CONF_TYPE_INT, CONF_RANGE, 0, 2000, NULL},
        {"subcmp", &lavc_param_me_sub_cmp, CONF_TYPE_INT, CONF_RANGE, 0, 2000, NULL},
        {"mbcmp", &lavc_param_mb_cmp, CONF_TYPE_INT, CONF_RANGE, 0, 2000, NULL},
        {"skipcmp", &lavc_param_skip_cmp, CONF_TYPE_INT, CONF_RANGE, 0, 2000, NULL},
#ifdef FF_CMP_VSAD
        {"ildctcmp", &lavc_param_ildct_cmp, CONF_TYPE_INT, CONF_RANGE, 0, 2000, NULL},
#endif
        {"bit_exact", &lavc_param_bit_exact, CONF_TYPE_FLAG, 0, 0, AV_CODEC_FLAG_BITEXACT, NULL},
        {"predia", &lavc_param_pre_dia_size, CONF_TYPE_INT, CONF_RANGE, -2000, 2000, NULL},
        {"dia", &lavc_param_dia_size, CONF_TYPE_INT, CONF_RANGE, -2000, 2000, NULL},
	{"qpel", &lavc_param_qpel, CONF_TYPE_FLAG, 0, 0, AV_CODEC_FLAG_QPEL, NULL},
	{"trell", &lavc_param_trell, CONF_TYPE_FLAG, 0, 0, 1, NULL},
	{"lowdelay", &lavc_param_lowdelay, CONF_TYPE_FLAG, 0, 0, AV_CODEC_FLAG_LOW_DELAY, NULL},
	{"last_pred", &lavc_param_last_pred, CONF_TYPE_INT, CONF_RANGE, 0, 2000, NULL},
	{"preme", &lavc_param_pre_me, CONF_TYPE_INT, CONF_RANGE, 0, 2000, NULL},
	{"subq", &lavc_param_me_subpel_quality, CONF_TYPE_INT, CONF_RANGE, 0, 8, NULL},
	{"me_range", &lavc_param_me_range, CONF_TYPE_INT, CONF_RANGE, 0, 16000, NULL},
	{"aic", &lavc_param_aic, CONF_TYPE_FLAG, 0, 0, AV_CODEC_FLAG_AC_PRED, NULL},
	{"umv", &lavc_param_umv, CONF_TYPE_FLAG, 0, 0, 1, NULL},
	{"aiv", &lavc_param_aiv, CONF_TYPE_FLAG, 0, 0, 1, NULL},
	{"obmc", &lavc_param_obmc, CONF_TYPE_FLAG, 0, 0, 1, NULL},
	{"loop", &lavc_param_loop, CONF_TYPE_FLAG, 0, 0, AV_CODEC_FLAG_LOOP_FILTER, NULL},
	{"ibias", "Please use o=ibias=<value> instead of ibias.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
	{"pbias", "Please use o=pbias=<value> instead of pbias.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
	{"coder", &lavc_param_coder, CONF_TYPE_INT, CONF_RANGE, 0, 10, NULL},
	{"context", &lavc_param_context, CONF_TYPE_INT, CONF_RANGE, 0, 10, NULL},
	{"intra_matrix", &lavc_param_intra_matrix, CONF_TYPE_STRING, 0, 0, 0, NULL},
	{"inter_matrix", &lavc_param_inter_matrix, CONF_TYPE_STRING, 0, 0, 0, NULL},
	{"cbp", "Please use o=mpv_flags=+cbp_rd instead of cbp.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
	{"mv0", "Please use o=mpv_flags=+mv0 instead of mv0.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
	{"nr", &lavc_param_noise_reduction, CONF_TYPE_INT, CONF_RANGE, 0, 1000000, NULL},
	{"qprd", "Please use o=mpv_flags=+qp_rd instead of qprd.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
	{"ss", &lavc_param_ss, CONF_TYPE_FLAG, 0, 0, 1, NULL},
	{"alt", &lavc_param_alt, CONF_TYPE_FLAG, 0, 0, 1, NULL},
	{"ilme", &lavc_param_ilme, CONF_TYPE_FLAG, 0, 0, AV_CODEC_FLAG_INTERLACED_ME, NULL},
	{"cgop", &lavc_param_closed_gop, CONF_TYPE_FLAG, 0, 0, AV_CODEC_FLAG_CLOSED_GOP, NULL},
	{"gmc", "Please use o=gmc=<value> instead of gmc.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
	{"dc", &lavc_param_dc_precision, CONF_TYPE_INT, CONF_RANGE, 8, 11, NULL},
	{"border_mask", "Please use o=border_mask=<value> instead of border_mask.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
	{"inter_threshold", "inter_threshold has no effect, please remove it.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
	{"sc_threshold", &lavc_param_sc_threshold, CONF_TYPE_INT, CONF_RANGE, -1000000000, 1000000000, NULL},
	{"top", &lavc_param_top, CONF_TYPE_INT, CONF_RANGE, -1, 1, NULL},
        {"qns", "Please use o=quantizer_noise_shaping=<value> instead of qns.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
        {"nssew", &lavc_param_nssew, CONF_TYPE_INT, CONF_RANGE, 0, 1000000, NULL},
	{"threads", &lavc_param_threads, CONF_TYPE_INT, CONF_RANGE, 1, 16, NULL},
	{"turbo", &lavc_param_turbo, CONF_TYPE_FLAG, 0, 0, 1, NULL},
        {"skip_threshold", &lavc_param_skip_threshold, CONF_TYPE_INT, CONF_RANGE, 0, 1000000, NULL},
        {"skip_factor", &lavc_param_skip_factor, CONF_TYPE_INT, CONF_RANGE, 0, 1000000, NULL},
        {"skip_exp", &lavc_param_skip_exp, CONF_TYPE_INT, CONF_RANGE, 0, 1000000, NULL},
	{"brd_scale", &lavc_param_brd_scale, CONF_TYPE_INT, CONF_RANGE, 0, 10, NULL},
	{"bidir_refine", &lavc_param_bidir_refine, CONF_TYPE_INT, CONF_RANGE, 0, 4, NULL},
	{"sc_factor", "sc_factor has no effect, please remove it.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
	{"vglobal", &lavc_param_video_global_header, CONF_TYPE_INT, CONF_RANGE, 0, INT_MAX, NULL},
	{"aglobal", &lavc_param_audio_global_header, CONF_TYPE_INT, CONF_RANGE, 0, INT_MAX, NULL},
	{"mv0_threshold", &lavc_param_mv0_threshold, CONF_TYPE_INT, CONF_RANGE, 0, INT_MAX, NULL},
	{"refs", &lavc_param_refs, CONF_TYPE_INT, CONF_RANGE, 1, 16, NULL},
        {"b_sensitivity", &lavc_param_b_sensitivity, CONF_TYPE_INT, CONF_RANGE, 1, INT_MAX, NULL},
	{"level", &lavc_param_level, CONF_TYPE_INT, CONF_RANGE, INT_MIN, INT_MAX, NULL},
        {"o", &lavc_param_avopt, CONF_TYPE_STRING, 0, 0, 0, NULL},
        {"audio_o", &lavc_param_audio_avopt, CONF_TYPE_STRING, 0, 0, 0, NULL},
	{NULL, NULL, 0, 0, 0, 0, NULL}
};

struct vf_priv_s {
    muxer_stream_t* mux;
    AVCodecContext *context;
    AVFrame *pic;
    int coded_picture_number;
    const AVCodec *codec;
    AVPacket *pkt;
    FILE *stats_file;
};

#define stats_file (vf->priv->stats_file)
#define mux_v (vf->priv->mux)
#define lavc_venc_context (vf->priv->context)

static int encode_frame(struct vf_instance *vf, AVFrame *pic, double pts);

static int config(struct vf_instance *vf,
        int width, int height, int d_width, int d_height,
	unsigned int flags, unsigned int outfmt){
    int size, i;
    char *p;
    AVDictionary *opts = NULL;

    mux_v->bih->biWidth=width;
    mux_v->bih->biHeight=height;
    mux_v->bih->biSizeImage=mux_v->bih->biWidth*mux_v->bih->biHeight*(mux_v->bih->biBitCount/8);

    mp_msg(MSGT_MENCODER, MSGL_INFO,"videocodec: libavcodec (%dx%d fourcc=%x [%.4s])\n",
	mux_v->bih->biWidth, mux_v->bih->biHeight, mux_v->bih->biCompression,
	    (char *)&mux_v->bih->biCompression);

    vf->priv->pic->width =
    lavc_venc_context->width = width;
    vf->priv->pic->height =
    lavc_venc_context->height = height;
    if (lavc_param_vbitrate > 16000) /* != -1 */
	lavc_venc_context->bit_rate = lavc_param_vbitrate;
    else if (lavc_param_vbitrate >= 0) /* != -1 */
	lavc_venc_context->bit_rate = lavc_param_vbitrate*1000;
    else
	lavc_venc_context->bit_rate = 800000; /* default */

    mux_v->avg_rate= lavc_venc_context->bit_rate;

    lavc_venc_context->bit_rate_tolerance= lavc_param_vrate_tolerance*1000;
    lavc_venc_context->time_base= (AVRational){mux_v->h.dwScale, mux_v->h.dwRate};
    lavc_venc_context->qmin= lavc_param_vqmin;
    lavc_venc_context->qmax= lavc_param_vqmax;
    lavc_venc_context->mb_lmin= (int)(FF_QP2LAMBDA * lavc_param_mb_lmin + 0.5);
    lavc_venc_context->mb_lmax= (int)(FF_QP2LAMBDA * lavc_param_mb_lmax + 0.5);
    lavc_venc_context->max_qdiff= lavc_param_vqdiff;
    lavc_venc_context->qcompress= lavc_param_vqcompress;
    lavc_venc_context->qblur= lavc_param_vqblur;
    lavc_venc_context->max_b_frames= lavc_param_vmax_b_frames;
    lavc_venc_context->b_quant_factor= lavc_param_vb_qfactor;
    if (lavc_param_vb_strategy)
        av_dict_set_int(&opts, "b_strategy", lavc_param_vb_strategy, 0);
    lavc_venc_context->b_quant_offset= (int)(FF_QP2LAMBDA * lavc_param_vb_qoffset + 0.5);
    if (lavc_param_packet_size)
        av_dict_set_int(&opts, "ps", lavc_param_packet_size, 0);
    lavc_venc_context->strict_std_compliance= lavc_param_strict;
    lavc_venc_context->i_quant_factor= lavc_param_vi_qfactor;
    lavc_venc_context->i_quant_offset= (int)(FF_QP2LAMBDA * lavc_param_vi_qoffset + 0.5);

    mux_v->max_rate=
    lavc_venc_context->rc_max_rate= lavc_param_rc_max_rate*1000;
    lavc_venc_context->rc_min_rate= lavc_param_rc_min_rate*1000;

    mux_v->vbv_size=
    lavc_venc_context->rc_buffer_size= lavc_param_rc_buffer_size*1000;

    lavc_venc_context->rc_initial_buffer_occupancy=
            lavc_venc_context->rc_buffer_size *
            lavc_param_rc_initial_buffer_occupancy;
    lavc_venc_context->debug= lavc_param_debug;
    lavc_venc_context->last_predictor_count= lavc_param_last_pred;
    if (lavc_param_pre_me)
        av_dict_set_int(&opts, "mepre", lavc_param_pre_me, 0);
    lavc_venc_context->me_pre_cmp= lavc_param_me_pre_cmp;
    lavc_venc_context->pre_dia_size= lavc_param_pre_dia_size;
    lavc_venc_context->me_subpel_quality= lavc_param_me_subpel_quality;
    lavc_venc_context->me_range= lavc_param_me_range;
    if (lavc_param_coder)
        av_dict_set_int(&opts, "coder", lavc_param_coder, 0);
    if (lavc_param_context)
        av_dict_set_int(&opts, "context", lavc_param_context, 0);
    if (lavc_param_sc_threshold)
        av_dict_set_int(&opts, "sc_threshold", lavc_param_sc_threshold, 0);
    if (lavc_param_noise_reduction)
        av_dict_set_int(&opts, "noise_reduction", lavc_param_noise_reduction, 0);
    lavc_venc_context->nsse_weight= lavc_param_nssew;
    if (lavc_param_skip_threshold)
        av_dict_set_int(&opts, "skip_threshold", lavc_param_skip_threshold, 0);
    if (lavc_param_skip_factor)
        av_dict_set_int(&opts, "skip_factor", lavc_param_skip_factor, 0);
    if (lavc_param_skip_exp)
        av_dict_set_int(&opts, "skip_exp", lavc_param_skip_exp, 0);
    if (lavc_param_skip_cmp != FF_CMP_DCTMAX)
        av_dict_set_int(&opts, "skip_cmp", lavc_param_skip_cmp, 0);

    if (lavc_param_intra_matrix)
    {
	char *tmp;

	lavc_venc_context->intra_matrix =
	    av_malloc(sizeof(*lavc_venc_context->intra_matrix)*64);

	i = 0;
	while ((tmp = strsep(&lavc_param_intra_matrix, ",")) && (i < 64))
	{
	    if (!tmp || (tmp && !strlen(tmp)))
		break;
	    lavc_venc_context->intra_matrix[i++] = atoi(tmp);
	}

	if (i != 64)
	    av_freep(&lavc_venc_context->intra_matrix);
	else
	    mp_msg(MSGT_MENCODER, MSGL_V, "Using user specified intra matrix\n");
    }
    if (lavc_param_inter_matrix)
    {
	char *tmp;

	lavc_venc_context->inter_matrix =
	    av_malloc(sizeof(*lavc_venc_context->inter_matrix)*64);

	i = 0;
	while ((tmp = strsep(&lavc_param_inter_matrix, ",")) && (i < 64))
	{
	    if (!tmp || (tmp && !strlen(tmp)))
		break;
	    lavc_venc_context->inter_matrix[i++] = atoi(tmp);
	}

	if (i != 64)
	    av_freep(&lavc_venc_context->inter_matrix);
	else
	    mp_msg(MSGT_MENCODER, MSGL_V, "Using user specified inter matrix\n");
    }

    p= lavc_param_rc_override_string;
    for(i=0; p; i++){
        int start, end, q;
        int e=sscanf(p, "%d,%d,%d", &start, &end, &q);
        if(e!=3){
	    mp_msg(MSGT_MENCODER,MSGL_ERR,"error parsing vrc_q\n");
            return 0;
        }
        av_reallocp_array(&lavc_venc_context->rc_override, i+1, sizeof(*lavc_venc_context->rc_override));
        lavc_venc_context->rc_override[i].start_frame= start;
        lavc_venc_context->rc_override[i].end_frame  = end;
        if(q>0){
            lavc_venc_context->rc_override[i].qscale= q;
            lavc_venc_context->rc_override[i].quality_factor= 1.0;
        }
        else{
            lavc_venc_context->rc_override[i].qscale= 0;
            lavc_venc_context->rc_override[i].quality_factor= -q/100.0;
        }
        p= strchr(p, '/');
        if(p) p++;
    }
    lavc_venc_context->rc_override_count=i;

    if (lavc_param_mpeg_quant)
        av_dict_set_int(&opts, "mpeg_quant", lavc_param_mpeg_quant, 0);

    lavc_venc_context->dct_algo= lavc_param_fdct;
    lavc_venc_context->idct_algo= lavc_param_idct;

    lavc_venc_context->lumi_masking= lavc_param_lumi_masking;
    lavc_venc_context->temporal_cplx_masking= lavc_param_temporal_cplx_masking;
    lavc_venc_context->spatial_cplx_masking= lavc_param_spatial_cplx_masking;
    lavc_venc_context->p_masking= lavc_param_p_masking;
    lavc_venc_context->dark_masking= lavc_param_dark_masking;

    if (lavc_param_aspect != NULL)
    {
	int par_width, par_height, e;
	float ratio=0;

	e= sscanf (lavc_param_aspect, "%d/%d", &par_width, &par_height);
	if(e==2){
            if(par_height)
                ratio= (float)par_width / (float)par_height;
        }else{
	    e= sscanf (lavc_param_aspect, "%f", &ratio);
	}

	if (e && ratio > 0.1 && ratio < 10.0) {
	    lavc_venc_context->sample_aspect_ratio= av_d2q(ratio * height / width, 255);
	    mp_dbg(MSGT_MENCODER, MSGL_DBG2, "sample_aspect_ratio: %d/%d\n",
                lavc_venc_context->sample_aspect_ratio.num,
                lavc_venc_context->sample_aspect_ratio.den);
	    mux_v->aspect = ratio;
	    mp_dbg(MSGT_MENCODER, MSGL_DBG2, "aspect_ratio: %f\n", ratio);
	} else {
	    mp_dbg(MSGT_MENCODER, MSGL_ERR, "aspect ratio: cannot parse \"%s\"\n", lavc_param_aspect);
	    return 0;
	}
    }
    else if (lavc_param_autoaspect) {
	lavc_venc_context->sample_aspect_ratio = av_d2q((float)d_width/d_height*height / width, 255);
	mux_v->aspect = (float)d_width/d_height;
    }

    /* keyframe interval */
    if (lavc_param_keyint >= 0) /* != -1 */
	lavc_venc_context->gop_size = lavc_param_keyint;
    else
	lavc_venc_context->gop_size = 250; /* default */

    lavc_venc_context->flags = 0;
    if (lavc_param_mb_decision)
    {
	mp_msg(MSGT_MENCODER, MSGL_INFO, MSGTR_MPCODECS_HighQualityEncodingSelected);
        lavc_venc_context->mb_decision= lavc_param_mb_decision;
    }

    lavc_venc_context->me_cmp= lavc_param_me_cmp;
    lavc_venc_context->me_sub_cmp= lavc_param_me_sub_cmp;
    lavc_venc_context->mb_cmp= lavc_param_mb_cmp;
#ifdef FF_CMP_VSAD
    lavc_venc_context->ildct_cmp= lavc_param_ildct_cmp;
#endif
    lavc_venc_context->dia_size= lavc_param_dia_size;
    lavc_venc_context->flags|= lavc_param_qpel;
    lavc_venc_context->trellis = lavc_param_trell;
    lavc_venc_context->flags|= lavc_param_lowdelay;
    lavc_venc_context->flags|= lavc_param_bit_exact;
    lavc_venc_context->flags|= lavc_param_aic;
    if (lavc_param_aiv)
        av_dict_set(&opts, "aiv", "1", 0);
    if (lavc_param_umv)
        av_dict_set(&opts, "umv", "1", 0);
    if (lavc_param_obmc)
        av_dict_set(&opts, "obmc", "1", 0);
    lavc_venc_context->flags|= lavc_param_loop;
    lavc_venc_context->flags|= lavc_param_v4mv ? AV_CODEC_FLAG_4MV : 0;
    if (lavc_param_data_partitioning)
        av_dict_set(&opts, "data_partitioning", "1", 0);
    lavc_venc_context->flags|= lavc_param_mv0;
    if (lavc_param_ss)
        av_dict_set(&opts, "structured_slices", "1", 0);
    if (lavc_param_alt)
        av_dict_set(&opts, "alternate_scan", "1", 0);
    lavc_venc_context->flags|= lavc_param_ilme;
#ifdef AV_CODEC_FLAG_CLOSED_GOP
    lavc_venc_context->flags|= lavc_param_closed_gop;
#endif
    lavc_venc_context->flags|= lavc_param_gray;

    if(lavc_param_interlaced_dct) lavc_venc_context->flags|= AV_CODEC_FLAG_INTERLACED_DCT;
    lavc_venc_context->flags|= lavc_param_psnr;
    lavc_venc_context->intra_dc_precision = lavc_param_dc_precision - 8;
    if (lavc_param_prediction_method)
        av_dict_set_int(&opts, "pred", lavc_param_prediction_method, 0);
    if (lavc_param_brd_scale)
        av_dict_set_int(&opts, "brd_scale", lavc_param_brd_scale, 0);
    lavc_venc_context->bidir_refine = lavc_param_bidir_refine;
    if((lavc_param_video_global_header&1)
       /*|| (video_global_header==0 && (oc->oformat->flags & AVFMT_GLOBALHEADER))*/){
        lavc_venc_context->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }
    if(lavc_param_video_global_header&2){
        lavc_venc_context->flags2 |= AV_CODEC_FLAG2_LOCAL_HEADER;
    }
    lavc_venc_context->mv0_threshold = lavc_param_mv0_threshold;
    lavc_venc_context->refs = lavc_param_refs;
    if (lavc_param_b_sensitivity != 40)
        av_dict_set_int(&opts, "b_sensitivity", lavc_param_b_sensitivity, 0);
    lavc_venc_context->level = lavc_param_level;

    if(lavc_param_avopt){
        if(av_dict_parse_string(&opts, lavc_param_avopt, "=", ",", 0) < 0){
            mp_msg(MSGT_MENCODER,MSGL_ERR, "Your options /%s/ look like gibberish to me pal\n", lavc_param_avopt);
            return 0;
        }
    }

    mux_v->imgfmt = lavc_param_format;
    vf->priv->pic->format =
    lavc_venc_context->pix_fmt = imgfmt2pixfmt(lavc_param_format);
    if (lavc_venc_context->pix_fmt == AV_PIX_FMT_NONE)
        return 0;

    if(!stats_file) {
    /* lavc internal 2pass bitrate control */
    switch(lavc_param_vpass){
    case 2:
    case 3:
	lavc_venc_context->flags|= AV_CODEC_FLAG_PASS2;
	stats_file= fopen(passtmpfile, "rb");
	if(stats_file==NULL){
	    mp_msg(MSGT_MENCODER,MSGL_ERR,"2pass failed: filename=%s\n", passtmpfile);
            return 0;
	}
	fseek(stats_file, 0, SEEK_END);
	size= ftell(stats_file);
	if (size < 0) {
	    mp_msg(MSGT_MENCODER,MSGL_ERR,"2pass failed: could not get size; filename=%s\n", passtmpfile);
	    return 0;
	}
	fseek(stats_file, 0, SEEK_SET);

	lavc_venc_context->stats_in= av_malloc(size + 1);
	lavc_venc_context->stats_in[size]=0;

	if(fread(lavc_venc_context->stats_in, size, 1, stats_file)<1){
	    mp_msg(MSGT_MENCODER,MSGL_ERR,"2pass failed: reading from filename=%s\n", passtmpfile);
            return 0;
	}
	if(lavc_param_vpass == 2)
	    break;
	else
	    fclose(stats_file);
	    /* fall through */
    case 1:
	lavc_venc_context->flags|= AV_CODEC_FLAG_PASS1;
	stats_file= fopen(passtmpfile, "wb");
	if(stats_file==NULL){
	    mp_msg(MSGT_MENCODER,MSGL_ERR,"2pass failed: filename=%s\n", passtmpfile);
            return 0;
	}
	if(lavc_param_turbo && (lavc_param_vpass == 1)) {
	  /* uses SAD comparison functions instead of other hungrier */
	  lavc_venc_context->me_pre_cmp = 0;
	  lavc_venc_context->me_cmp = 0;
	  lavc_venc_context->me_sub_cmp = 0;
	  lavc_venc_context->mb_cmp = 2;

	  /* Disables diamond motion estimation */
	  lavc_venc_context->pre_dia_size = 0;
	  lavc_venc_context->dia_size = 1;

	  av_dict_set(&opts, "noise_reduction", "0", 0); // nr=0
	  lavc_venc_context->mb_decision = 0; // mbd=0 ("realtime" encoding)

	  lavc_venc_context->flags &= ~AV_CODEC_FLAG_QPEL;
	  lavc_venc_context->flags &= ~AV_CODEC_FLAG_4MV;
	  lavc_venc_context->trellis = 0;
	  av_dict_set(&opts, "mpv_flags", "-mv0", 0);
	  av_dict_set(&opts, "mpv_flags", "-qp_rd-cbp_rd", 0);
	}
	break;
    }
    }

    /* fixed qscale :p */
    if (lavc_param_vqscale >= 0.0)
    {
	mp_msg(MSGT_MENCODER, MSGL_INFO, MSGTR_MPCODECS_UsingConstantQscale, lavc_param_vqscale);
	lavc_venc_context->flags |= AV_CODEC_FLAG_QSCALE;
        lavc_venc_context->global_quality=
	vf->priv->pic->quality = (int)(FF_QP2LAMBDA * lavc_param_vqscale + 0.5);
    }

    lavc_venc_context->thread_count = lavc_param_threads;
    lavc_venc_context->thread_type = FF_THREAD_FRAME | FF_THREAD_SLICE;

    if (avcodec_open2(lavc_venc_context, vf->priv->codec, &opts) != 0) {
	mp_msg(MSGT_MENCODER,MSGL_ERR,MSGTR_CantOpenCodec);
	return 0;
    }
    if (av_dict_count(opts)) {
        AVDictionaryEntry *e = NULL;
        while ((e = av_dict_get(opts, "", e, AV_DICT_IGNORE_SUFFIX)))
            mp_msg(MSGT_MENCODER,MSGL_ERR,"Unknown option %s\n", e->key);
        return 0;
    }
    av_dict_free(&opts);

    /* free second pass buffer, its not needed anymore */
    av_freep(&lavc_venc_context->stats_in);
    if(lavc_venc_context->bits_per_coded_sample)
        mux_v->bih->biBitCount= lavc_venc_context->bits_per_coded_sample;
    if(lavc_venc_context->extradata_size){
        mux_v->bih= realloc(mux_v->bih, sizeof(*mux_v->bih) + lavc_venc_context->extradata_size);
        memcpy(mux_v->bih + 1, lavc_venc_context->extradata, lavc_venc_context->extradata_size);
        mux_v->bih->biSize= sizeof(*mux_v->bih) + lavc_venc_context->extradata_size;
    }

    mux_v->decoder_delay = lavc_venc_context->max_b_frames ? 1 : 0;

    return 1;
}

static int control(struct vf_instance *vf, int request, void* data){

    switch(request){
        case VFCTRL_FLUSH_FRAMES:
            if(vf->priv->codec->capabilities & AV_CODEC_CAP_DELAY)
                while(encode_frame(vf, NULL, MP_NOPTS_VALUE) > 0);
            return CONTROL_TRUE;
        default:
            return CONTROL_UNKNOWN;
    }
}

static int query_format(struct vf_instance *vf, unsigned int fmt){
    switch(fmt){
    case IMGFMT_IYUV:
    case IMGFMT_I420:
        fmt = IMGFMT_YV12;
        break;
    }
    if(lavc_param_format == fmt)
        return VFCAP_CSP_SUPPORTED | VFCAP_ACCEPT_STRIDE;
    return 0;
}

static double psnr(double d){
    if(d==0) return INFINITY;
    return -10.0*log(d)/log(10);
}

static int put_image(struct vf_instance *vf, mp_image_t *mpi, double pts, double endpts){
    AVFrame *pic= vf->priv->pic;

    pic->data[0]=mpi->planes[0];
    pic->data[1]=mpi->planes[1];
    pic->data[2]=mpi->planes[2];
    pic->linesize[0]=mpi->stride[0];
    pic->linesize[1]=mpi->stride[1];
    pic->linesize[2]=mpi->stride[2];
    pic->pict_type = is_forced_key_frame(pts) ? AV_PICTURE_TYPE_I : 0;

    if(lavc_param_interlaced_dct){
        if((mpi->fields & MP_IMGFIELD_ORDERED) && (mpi->fields & MP_IMGFIELD_INTERLACED))
            pic->top_field_first= !!(mpi->fields & MP_IMGFIELD_TOP_FIRST);
        else
            pic->top_field_first= 1;

        if(lavc_param_top!=-1)
            pic->top_field_first= lavc_param_top;
    }

    return encode_frame(vf, pic, pts) >= 0;
}

static int encode_frame(struct vf_instance *vf, AVFrame *pic, double pts){
    const char pict_type_char[5]= {'?', 'I', 'P', 'B', 'S'};
    double dts;
    AVPacket *pkt= vf->priv->pkt;
    int res;

    if(pts == MP_NOPTS_VALUE)
        pts= lavc_venc_context->frame_number * av_q2d(lavc_venc_context->time_base);

    if(pic){
#if 0
        pic->opaque= malloc(sizeof(pts));
        memcpy(pic->opaque, &pts, sizeof(pts));
#else
        if(pts != MP_NOPTS_VALUE)
            pic->pts= floor(pts / av_q2d(lavc_venc_context->time_base) + 0.5);
        else
            pic->pts= MP_NOPTS_VALUE;
#endif
    }
    res = avcodec_send_frame(lavc_venc_context, pic);
    if (res < 0 && res != AVERROR(EAGAIN) && res != AVERROR_EOF)
        return res;
    res = avcodec_receive_packet(lavc_venc_context, pkt);
    if (res < 0 && res != AVERROR(EAGAIN))
        return res == AVERROR_EOF ? 0 : res;

    /* store stats if there are any */
    if(lavc_venc_context->stats_out && stats_file) {
        fprintf(stats_file, "%s", lavc_venc_context->stats_out);
        /* make sure we can't accidentally store the same stats twice */
        lavc_venc_context->stats_out[0] = 0;
    }

    if(res == AVERROR(EAGAIN) && lavc_param_skip_threshold==0 && lavc_param_skip_factor==0){
        ++mux_v->encoder_delay;
        return 0;
    }

    dts = pts = MP_NOPTS_VALUE;
    if (pkt->pts != AV_NOPTS_VALUE)
        pts = pkt->pts * av_q2d(lavc_venc_context->time_base);
    if (pkt->dts != AV_NOPTS_VALUE)
        dts = pkt->dts * av_q2d(lavc_venc_context->time_base);

    mux_v->buffer = pkt->data;      // use ref-counted packet
    mux_v->buffer_size = pkt->size; // update size for consistency
    muxer_write_chunk(mux_v,pkt->size,pkt->flags & AV_PKT_FLAG_KEY ?0x10:0,
                      dts, pts);

    /* store psnr / pict size / type / qscale */
    if(lavc_param_psnr){
        static FILE *fvstats=NULL;
        char filename[20];
        double f= lavc_venc_context->width*lavc_venc_context->height*255.0*255.0;
	double quality=0.0;
	uint8_t *sd = av_packet_get_side_data(pkt, AV_PKT_DATA_QUALITY_STATS, NULL);

        if(!fvstats) {
            time_t today2;
            struct tm *today;
            today2 = time(NULL);
            today = localtime(&today2);
            sprintf(filename, "psnr_%02d%02d%02d.log", today->tm_hour,
                today->tm_min, today->tm_sec);
            fvstats = fopen(filename,"w");
            if(!fvstats) {
                perror("fopen");
                lavc_param_psnr=0; // disable block
                mp_msg(MSGT_MENCODER,MSGL_ERR,"Can't open %s for writing. Check its permissions.\n",filename);
                return -1;
                /*exit(1);*/
            }
        }

	if(sd && sd[5] >= 3) {
	    uint8_t pict_type = sd[4] * (sd[4] < FF_ARRAY_ELEMS(pict_type_char));
	    uint64_t error[3] = {
	        AV_RL64(sd + 8), AV_RL64(sd + 16), AV_RL64(sd + 24)
	    };
	    quality = AV_RL32(sd) / (float)FF_QP2LAMBDA;

        fprintf(fvstats, "%6d, %2.2f, %6d, %2.2f, %2.2f, %2.2f, %2.2f %c\n",
            vf->priv->coded_picture_number,
            quality,
            pkt->size,
            psnr(error[0]/f),
            psnr(error[1]*4/f),
            psnr(error[2]*4/f),
            psnr((error[0]+error[1]+error[2])/(f*1.5)),
            pict_type_char[pict_type]
            );
	}
    }
    ++vf->priv->coded_picture_number;
    res = pkt->size;
    av_packet_unref(pkt);
    return res;
}

static void uninit(struct vf_instance *vf){

    if(lavc_param_psnr){
        double f= lavc_venc_context->width*lavc_venc_context->height*255.0*255.0;
        f*= vf->priv->coded_picture_number;

        mp_msg(MSGT_MENCODER, MSGL_INFO, "PSNR: Y:%2.2f, Cb:%2.2f, Cr:%2.2f, All:%2.2f\n",
            psnr(lavc_venc_context->error[0]/f),
            psnr(lavc_venc_context->error[1]*4/f),
            psnr(lavc_venc_context->error[2]*4/f),
            psnr((lavc_venc_context->error[0]+lavc_venc_context->error[1]+lavc_venc_context->error[2])/(f*1.5))
            );
    }

    av_packet_free(&vf->priv->pkt);

    av_freep(&lavc_venc_context->intra_matrix);
    av_freep(&lavc_venc_context->inter_matrix);

    if (lavc_venc_context->codec)
        avcodec_close(lavc_venc_context);

    if(stats_file) fclose(stats_file);

    /* free rc_override */
    av_freep(&lavc_venc_context->rc_override);

    avcodec_free_context(&vf->priv->context);
    av_frame_free(&vf->priv->pic);
    free(vf->priv);
}

//===========================================================================//

static int vf_open(vf_instance_t *vf, char* args){
    vf->uninit=uninit;
    vf->config=config;
    vf->default_caps=VFCAP_CONSTANT;
    vf->control=control;
    vf->query_format=query_format;
    vf->put_image=put_image;
    vf->priv=calloc(1, sizeof(struct vf_priv_s));
    vf->priv->mux=(muxer_stream_t*)args;

    /* XXX: hack: some of the MJPEG decoder DLL's needs exported huffman
       table, so we define a zero-table, also lavc mjpeg encoder is putting
       huffman tables into the stream, so no problem */
    if (lavc_param_vcodec && !av_strcasecmp(lavc_param_vcodec, "mjpeg"))
    {
	mux_v->bih=calloc(1, sizeof(*mux_v->bih)+28);
	mux_v->bih->biSize=sizeof(*mux_v->bih)+28;
    }
    else if (lavc_param_vcodec && (!av_strcasecmp(lavc_param_vcodec, "huffyuv")
                                || !av_strcasecmp(lavc_param_vcodec, "ffvhuff")))
    {
    /* XXX: hack: huffyuv needs to store huffman tables (allthough we dunno the size yet ...) */
	mux_v->bih=calloc(1, sizeof(*mux_v->bih)+1000);
	mux_v->bih->biSize=sizeof(*mux_v->bih)+1000;
    }
    else if (lavc_param_vcodec && !av_strcasecmp(lavc_param_vcodec, "asv1"))
    {
	mux_v->bih=calloc(1, sizeof(*mux_v->bih)+8);
	mux_v->bih->biSize=sizeof(*mux_v->bih)+8;
    }
    else if (lavc_param_vcodec && !av_strcasecmp(lavc_param_vcodec, "asv2"))
    {
	mux_v->bih=calloc(1, sizeof(*mux_v->bih)+8);
	mux_v->bih->biSize=sizeof(*mux_v->bih)+8;
    }
    else if (lavc_param_vcodec && !av_strcasecmp(lavc_param_vcodec, "wmv2"))
    {
	mux_v->bih=calloc(1, sizeof(*mux_v->bih)+4);
	mux_v->bih->biSize=sizeof(*mux_v->bih)+4;
    }
    else
    {
	mux_v->bih=calloc(1, sizeof(*mux_v->bih));
	mux_v->bih->biSize=sizeof(*mux_v->bih);
    }
    mux_v->bih->biWidth=0;
    mux_v->bih->biHeight=0;
    mux_v->bih->biPlanes=1;
    mux_v->bih->biBitCount=24;
    if (!lavc_param_vcodec)
    {
	printf("No libavcodec codec specified! It's required!\n");
	return 0;
    }

    if (!av_strcasecmp(lavc_param_vcodec, "mpeg1") || !av_strcasecmp(lavc_param_vcodec, "mpeg1video"))
	mux_v->bih->biCompression = mmioFOURCC('m', 'p', 'g', '1');
    else if (!av_strcasecmp(lavc_param_vcodec, "mpeg2") || !av_strcasecmp(lavc_param_vcodec, "mpeg2video"))
	mux_v->bih->biCompression = mmioFOURCC('m', 'p', 'g', '2');
    else if (!av_strcasecmp(lavc_param_vcodec, "h263") || !av_strcasecmp(lavc_param_vcodec, "h263p"))
	mux_v->bih->biCompression = mmioFOURCC('h', '2', '6', '3');
    else if (!av_strcasecmp(lavc_param_vcodec, "rv10"))
	mux_v->bih->biCompression = mmioFOURCC('R', 'V', '1', '0');
    else if (!av_strcasecmp(lavc_param_vcodec, "mjpeg"))
	mux_v->bih->biCompression = mmioFOURCC('M', 'J', 'P', 'G');
    else if (!av_strcasecmp(lavc_param_vcodec, "ljpeg"))
	mux_v->bih->biCompression = mmioFOURCC('L', 'J', 'P', 'G');
    else if (!av_strcasecmp(lavc_param_vcodec, "mpeg4"))
	mux_v->bih->biCompression = mmioFOURCC('F', 'M', 'P', '4');
    else if (!av_strcasecmp(lavc_param_vcodec, "msmpeg4"))
	mux_v->bih->biCompression = mmioFOURCC('d', 'i', 'v', '3');
    else if (!av_strcasecmp(lavc_param_vcodec, "msmpeg4v2"))
	mux_v->bih->biCompression = mmioFOURCC('M', 'P', '4', '2');
    else if (!av_strcasecmp(lavc_param_vcodec, "wmv1"))
	mux_v->bih->biCompression = mmioFOURCC('W', 'M', 'V', '1');
    else if (!av_strcasecmp(lavc_param_vcodec, "wmv2"))
	mux_v->bih->biCompression = mmioFOURCC('W', 'M', 'V', '2');
    else if (!av_strcasecmp(lavc_param_vcodec, "huffyuv"))
	mux_v->bih->biCompression = mmioFOURCC('H', 'F', 'Y', 'U');
    else if (!av_strcasecmp(lavc_param_vcodec, "ffvhuff"))
	mux_v->bih->biCompression = mmioFOURCC('F', 'F', 'V', 'H');
    else if (!av_strcasecmp(lavc_param_vcodec, "asv1"))
	mux_v->bih->biCompression = mmioFOURCC('A', 'S', 'V', '1');
    else if (!av_strcasecmp(lavc_param_vcodec, "asv2"))
	mux_v->bih->biCompression = mmioFOURCC('A', 'S', 'V', '2');
    else if (!av_strcasecmp(lavc_param_vcodec, "ffv1"))
	mux_v->bih->biCompression = mmioFOURCC('F', 'F', 'V', '1');
    else if (!av_strcasecmp(lavc_param_vcodec, "snow"))
	mux_v->bih->biCompression = mmioFOURCC('S', 'N', 'O', 'W');
    else if (!av_strcasecmp(lavc_param_vcodec, "flv"))
	mux_v->bih->biCompression = mmioFOURCC('F', 'L', 'V', '1');
    else if (!av_strcasecmp(lavc_param_vcodec, "dvvideo"))
	mux_v->bih->biCompression = mmioFOURCC('d', 'v', 's', 'd');
    else if (!av_strcasecmp(lavc_param_vcodec, "libx264"))
	mux_v->bih->biCompression = mmioFOURCC('h', '2', '6', '4');
    else if (!av_strcasecmp(lavc_param_vcodec, "libschroedinger"))
	mux_v->bih->biCompression = mmioFOURCC('d', 'r', 'a', 'c');
    else if (!av_strcasecmp(lavc_param_vcodec, "libdirac"))
	mux_v->bih->biCompression = mmioFOURCC('d', 'r', 'a', 'c');
    else if (!av_strcasecmp(lavc_param_vcodec, "libvpx"))
	mux_v->bih->biCompression = mmioFOURCC('V', 'P', '8', '0');
    else
	mux_v->bih->biCompression = mmioFOURCC(lavc_param_vcodec[0],
		lavc_param_vcodec[1], lavc_param_vcodec[2], lavc_param_vcodec[3]); /* FIXME!!! */

    init_avcodec();

    vf->priv->codec = avcodec_find_encoder_by_name(lavc_param_vcodec);
    if (!vf->priv->codec) {
	mp_msg(MSGT_MENCODER,MSGL_ERR,MSGTR_MissingLAVCcodec, lavc_param_vcodec);
	return 0;
    }

    vf->priv->pkt = av_packet_alloc();
    vf->priv->pic = av_frame_alloc();
    vf->priv->context = avcodec_alloc_context3(vf->priv->codec);
    vf->priv->context->codec_id = vf->priv->codec->id;

    return 1;
}

const vf_info_t ve_info_lavc = {
    "libavcodec encoder",
    "lavc",
    "A'rpi, Alex, Michael",
    "for internal use by mencoder",
    vf_open
};

//===========================================================================//
