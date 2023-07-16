/*
 * Copyright (C) 2004 Michael Niedermayer <michaelni@gmx.at>
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

// #include <stdio.h>
#include <stdlib.h>
// #include <unistd.h>
#include <limits.h>

#include "config.h"
#include "mp_msg.h"
#include "help_mp.h"
#include "av_opts.h"
#include "av_helpers.h"

#include "stream/stream.h"
#include "aviprint.h"
#include "demuxer.h"
#include "stheader.h"
#include "m_option.h"
#include "sub/sub.h"

#include "libavformat/avformat.h"
#include "libavformat/avio.h"
#include "libavutil/avutil.h"
#include "libavutil/avstring.h"
#include "libavutil/display.h"
#include "libavutil/mathematics.h"
#include "libavutil/opt.h"
#include "libavutil/replaygain.h"

#include "mp_taglists.h"

#define INITIAL_PROBE_SIZE STREAM_BUFFER_MIN
#define SMALL_MAX_PROBE_SIZE (32 * 1024)
#define PROBE_BUF_SIZE (2*1024*1024)

static unsigned int opt_probesize = 0;
static unsigned int opt_analyzeduration = 0;
static char *opt_format;
static char *opt_cryptokey;
static char *opt_avopt = NULL;

const m_option_t lavfdopts_conf[] = {
	{"probesize", &(opt_probesize), CONF_TYPE_INT, CONF_RANGE, 32, INT_MAX, NULL},
	{"format",    &(opt_format),    CONF_TYPE_STRING,       0,  0,       0, NULL},
	{"analyzeduration",    &(opt_analyzeduration),    CONF_TYPE_INT,       CONF_RANGE,  0,       INT_MAX, NULL},
	{"cryptokey", &(opt_cryptokey), CONF_TYPE_STRING,       0,  0,       0, NULL},
        {"o",                  &opt_avopt,                CONF_TYPE_STRING,    0,           0,             0, NULL},
	{NULL, NULL, 0, 0, 0, 0, NULL}
};

#define BIO_BUFFER_SIZE 32768

typedef struct lavf_priv {
    const AVInputFormat *avif;
    AVFormatContext *avfc;
    AVIOContext *pb;
    int audio_streams;
    int video_streams;
    int sub_streams;
    int64_t last_pts;
    int astreams[MAX_A_STREAMS];
    int vstreams[MAX_V_STREAMS];
    int sstreams[MAX_S_STREAMS];
    int cur_program;
    int nb_streams_last;
    int use_lavf_netstream;
    int r_gain;
}lavf_priv_t;

static int mp_read(void *opaque, uint8_t *buf, int size) {
    demuxer_t *demuxer = opaque;
    stream_t *stream = demuxer->stream;
    int ret;

    ret=stream_read(stream, buf, size);
    if (!ret && stream->eof)
      ret = AVERROR_EOF;

    mp_msg(MSGT_HEADER,MSGL_DBG2,"%d=mp_read(%p, %p, %d), pos: %"PRId64", eof:%d\n",
           ret, stream, buf, size, stream_tell(stream), stream->eof);
    return ret;
}

static int64_t mp_seek(void *opaque, int64_t pos, int whence) {
    demuxer_t *demuxer = opaque;
    stream_t *stream = demuxer->stream;
    int64_t current_pos;
    mp_msg(MSGT_HEADER,MSGL_DBG2,"mp_seek(%p, %"PRId64", %d)\n", stream, pos, whence);
    if(whence == SEEK_CUR)
        pos +=stream_tell(stream);
    else if(whence == SEEK_END && stream->end_pos > 0)
        pos += stream->end_pos;
    else if(whence == SEEK_SET)
        pos += stream->start_pos;
    else if(whence == AVSEEK_SIZE && stream->end_pos > 0) {
        uint64_t size = 0;
        stream_control(stream, STREAM_CTRL_GET_SIZE, &size);
        if (size > stream->end_pos)
            stream->end_pos = size;
        return stream->end_pos - stream->start_pos;
    } else
        return -1;

    if(pos<0)
        return -1;
    current_pos = stream_tell(stream);
    if(stream_seek(stream, pos)==0) {
        stream_reset(stream);
        stream_seek(stream, current_pos);
        return -1;
    }

    return pos - stream->start_pos;
}

static int64_t mp_read_seek(void *opaque, int stream_idx, int64_t ts, int flags) {
    demuxer_t *demuxer = opaque;
    stream_t *stream = demuxer->stream;
    lavf_priv_t *priv = demuxer->priv;
    AVStream *st = priv->avfc->streams[stream_idx];
    int ret;
    double pts;

    pts = (double)ts * st->time_base.num / st->time_base.den;
    ret = stream_control(stream, STREAM_CTRL_SEEK_TO_TIME, &pts);
    if (ret < 0)
        ret = AVERROR(ENOSYS);
    return ret;
}

static void list_formats(void) {
    void *i = 0;
    const AVInputFormat *fmt;
    mp_msg(MSGT_DEMUX, MSGL_INFO, "Available lavf input formats:\n");
    while ((fmt = av_demuxer_iterate(&i)))
        mp_msg(MSGT_DEMUX, MSGL_INFO, "%15s : %s\n", fmt->name, fmt->long_name);
}

static int lavf_check_file(demuxer_t *demuxer){
    AVProbeData avpd = { 0 };
    lavf_priv_t *priv;
    int probe_data_size = 0;
    int read_size = INITIAL_PROBE_SIZE;
    int score;

    if(!demuxer->priv) {
        demuxer->priv=calloc(sizeof(lavf_priv_t),1);
        ((lavf_priv_t *)demuxer->priv)->r_gain = INT32_MIN;
    }
    priv= demuxer->priv;

    init_avformat();

    if (opt_format) {
        if (strcmp(opt_format, "help") == 0) {
           list_formats();
           return 0;
        }
        priv->avif= av_find_input_format(opt_format);
        if (!priv->avif) {
            mp_msg(MSGT_DEMUX,MSGL_FATAL,"Unknown lavf format %s\n", opt_format);
            return 0;
        }
        mp_msg(MSGT_DEMUX,MSGL_INFO,"Forced lavf %s demuxer\n", priv->avif->long_name);
        return DEMUXER_TYPE_LAVF;
    }

    avpd.buf = av_mallocz(FFMAX(BIO_BUFFER_SIZE, PROBE_BUF_SIZE) +
                          AV_INPUT_BUFFER_PADDING_SIZE);
    do {
        read_size = stream_read(demuxer->stream, avpd.buf + probe_data_size, read_size);
        if(read_size < 0) {
            av_free(avpd.buf);
            return 0;
        }
        probe_data_size += read_size;
        avpd.filename= demuxer->stream->url;
        if (!avpd.filename) {
            mp_msg(MSGT_DEMUX, MSGL_WARN, "Stream url is not set!\n");
            avpd.filename = "";
        }
        if (!strncmp(avpd.filename, "ffmpeg://", 9))
            avpd.filename += 9;
        avpd.buf_size= probe_data_size;

        score = 0;
        priv->avif= av_probe_input_format2(&avpd, probe_data_size > 0, &score);
        read_size = FFMIN(2*read_size, PROBE_BUF_SIZE - probe_data_size);
    } while ((demuxer->desc->type != DEMUXER_TYPE_LAVF_PREFERRED ||
              probe_data_size < SMALL_MAX_PROBE_SIZE) &&
             score <= AVPROBE_SCORE_MAX / 4 &&
             read_size > 0 && probe_data_size < PROBE_BUF_SIZE);
    av_free(avpd.buf);

    if(!priv->avif){
        mp_msg(MSGT_HEADER,MSGL_V,"LAVF_check: no clue about this gibberish!\n");
        return 0;
    }else{
        mp_msg(MSGT_HEADER,MSGL_V,"LAVF_check: %s\n", priv->avif->long_name);
        if (!strcmp(priv->avif->name, "hls,applehttp")) {
            mp_msg(MSGT_HEADER,MSGL_V,"LAVF: network streaming with lavf\n");
            avformat_network_init();
            priv->use_lavf_netstream  = 1;
        }
    }

    return DEMUXER_TYPE_LAVF;
}

/* Before adding anything to this list please stop and consider why.
 * There are two good reasons
 * 1) to reduce startup time when streaming these file types
 * 2) workarounds around bugs in our native demuxers that are not reasonable to
 *    fix
 * For the case 2) that means the issue should be understood well
 * enough to be able to decide that a fix is not reasonable.
 */
static const char * const preferred_list[] = {
    "cdxl",
    "dxa",
    "flv",
    "flac",
    "gxf",
    "nut",
    "nuv",
    "matroska,webm",
    "mov,mp4,m4a,3gp,3g2,mj2",
    "mpc",
    "mpc8",
    "mxf",
    "ogg",
    "pva",
    "qcp",
    "swf",
    "vqf",
    "w64",
    "wv",
    "yuv4mpegpipe",
    NULL
};

static int lavf_check_preferred_file(demuxer_t *demuxer){
    if (lavf_check_file(demuxer)) {
        const char * const *p = preferred_list;
        lavf_priv_t *priv = demuxer->priv;
        while (*p) {
            if (strcmp(*p, priv->avif->name) == 0)
                return DEMUXER_TYPE_LAVF_PREFERRED;
            p++;
        }
    }
    return 0;
}

static uint8_t char2int(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0;
}

static void parse_cryptokey(AVFormatContext *avfc, const char *str) {
    int len = strlen(str) / 2;
    uint8_t *key = av_mallocz(len);
    int i;
    avfc->keylen = len;
    avfc->key = key;
    for (i = 0; i < len; i++, str += 2)
        *key++ = (char2int(str[0]) << 4) | char2int(str[1]);
}

static void handle_stream(demuxer_t *demuxer, AVFormatContext *avfc, int i) {
    lavf_priv_t *priv= demuxer->priv;
    AVStream *st= avfc->streams[i];
    AVCodecParameters *codec= st->codecpar;
    char *stream_type = NULL;
    int stream_id;
    AVDictionaryEntry *lang = av_dict_get(st->metadata, "language", NULL, 0);
    AVDictionaryEntry *title= av_dict_get(st->metadata, "title",    NULL, 0);
    int g;

    switch(codec->codec_type){
        case AVMEDIA_TYPE_AUDIO:{
            WAVEFORMATEX *wf;
            sh_audio_t* sh_audio;
            sh_audio = new_sh_audio_aid(demuxer, i, priv->audio_streams, lang ? lang->value : NULL);
            if(!sh_audio)
                break;
            stream_type = "audio";
            priv->astreams[priv->audio_streams] = i;
            wf= calloc(sizeof(*wf) + codec->extradata_size, 1);
            codec->codec_tag = mp_codec_id2tag(codec->codec_id, codec->codec_tag, 1);
            wf->wFormatTag= codec->codec_tag;
            wf->nChannels= codec->channels;
            sh_audio->channel_layout = codec->channel_layout;
            wf->nSamplesPerSec= codec->sample_rate;
            wf->nAvgBytesPerSec= codec->bit_rate/8;
            wf->nBlockAlign= codec->block_align ? codec->block_align : 1;
            wf->wBitsPerSample= codec->bits_per_coded_sample;
            wf->cbSize= codec->extradata_size;
            if(codec->extradata_size)
                memcpy(wf + 1, codec->extradata, codec->extradata_size);
            sh_audio->wf= wf;
            sh_audio->audio.dwSampleSize= codec->block_align;
            if(codec->frame_size && codec->sample_rate){
                sh_audio->audio.dwScale=codec->frame_size;
                sh_audio->audio.dwRate= codec->sample_rate;
            }else{
                sh_audio->audio.dwScale= codec->block_align ? codec->block_align*8 : 8;
                sh_audio->audio.dwRate = codec->bit_rate;
            }
            g= av_gcd(sh_audio->audio.dwScale, sh_audio->audio.dwRate);
            sh_audio->audio.dwScale /= g;
            sh_audio->audio.dwRate  /= g;
//          printf("sca:%d rat:%d fs:%d sr:%d ba:%d\n", sh_audio->audio.dwScale, sh_audio->audio.dwRate, codec->frame_size, codec->sample_rate, codec->block_align);
            sh_audio->format= codec->codec_tag;
            sh_audio->channels= codec->channels;
            sh_audio->samplerate= codec->sample_rate;
            sh_audio->i_bps= codec->bit_rate/8;
            switch (codec->codec_id) {
                case AV_CODEC_ID_PCM_S8:
                case AV_CODEC_ID_PCM_U8:
                    sh_audio->samplesize = 1;
                    break;
                case AV_CODEC_ID_PCM_S16LE:
                case AV_CODEC_ID_PCM_S16BE:
                case AV_CODEC_ID_PCM_U16LE:
                case AV_CODEC_ID_PCM_U16BE:
                    sh_audio->samplesize = 2;
                    break;
                case AV_CODEC_ID_PCM_ALAW:
                    sh_audio->format = 0x6;
                    break;
                case AV_CODEC_ID_PCM_MULAW:
                    sh_audio->format = 0x7;
                    break;
            }
            if (title && title->value)
                mp_msg(MSGT_IDENTIFY, MSGL_INFO, "ID_AID_%d_NAME=%s\n", priv->audio_streams, title->value);
            if (st->disposition & AV_DISPOSITION_DEFAULT)
              sh_audio->default_track = 1;
            if(mp_msg_test(MSGT_HEADER,MSGL_V) ) print_wave_header(sh_audio->wf, MSGL_V);
            // select the first audio stream if auto-selection is requested
            if (demuxer->audio->id == -1) {
                demuxer->audio->id = i;
                demuxer->audio->sh= demuxer->a_streams[i];
            }
            if (demuxer->audio->id != i)
                st->discard= AVDISCARD_ALL;
            if (priv->audio_streams == 0) {
                size_t rg_size;
                AVReplayGain *rg = (AVReplayGain*)av_stream_get_side_data(st, AV_PKT_DATA_REPLAYGAIN, &rg_size);
                if (rg && rg_size >= sizeof(*rg)) {
                    priv->r_gain = rg->track_gain / 10000;
                }
            } else
                priv->r_gain = INT32_MIN;
            stream_id = priv->audio_streams++;
            break;
        }
        case AVMEDIA_TYPE_VIDEO:{
            AVDictionaryEntry *rot = av_dict_get(st->metadata, "rotate",   NULL, 0);
            const int32_t *disp_matrix = av_stream_get_side_data(st, AV_PKT_DATA_DISPLAYMATRIX, NULL);
            sh_video_t* sh_video;
            BITMAPINFOHEADER *bih;
            sh_video=new_sh_video_vid(demuxer, i, priv->video_streams);
            if(!sh_video) break;
            stream_type = "video";
            priv->vstreams[priv->video_streams] = i;
            if (codec->extradata_size >= 9 &&
                !memcmp(codec->extradata + codec->extradata_size - 9, "BottomUp", 9))
            {
                codec->extradata_size -= 9;
                sh_video->flipped_input ^= 1;
            }
            // always reserve space for palette
            sh_video->bih_size = sizeof(*bih) + codec->extradata_size + 1024;
            bih=calloc(sh_video->bih_size,1);

            if (codec->codec_id == AV_CODEC_ID_RAWVIDEO) {
                if (codec->bits_per_coded_sample && codec->bits_per_coded_sample > 0 &&
                    codec->codec_tag == MKTAG('r', 'a', 'w', 32))
                    codec->codec_tag = 0;
                switch (codec->format) {
                    case AV_PIX_FMT_RGB24:
                        codec->codec_tag= MKTAG(24, 'B', 'G', 'R');
                        break;
                    case AV_PIX_FMT_BGR24:
                        codec->codec_tag= MKTAG(24, 'R', 'G', 'B');
                        break;
                }
            }
            codec->codec_tag = mp_codec_id2tag(codec->codec_id, codec->codec_tag, 0);
            bih->biSize= sizeof(*bih) + codec->extradata_size;
            bih->biWidth= codec->width;
            bih->biHeight= codec->height;
            bih->biPlanes= 1;
            bih->biBitCount= codec->bits_per_coded_sample;
            bih->biSizeImage = bih->biWidth * bih->biHeight * bih->biBitCount/8;
            bih->biCompression= codec->codec_tag;
            sh_video->bih= bih;
            sh_video->disp_w= codec->width;
            sh_video->disp_h= codec->height;
            if (st->time_base.den) { /* if container has time_base, use that */
                sh_video->video.dwRate= st->time_base.den;
                sh_video->video.dwScale= st->time_base.num;
#if 0
            } else {
                // not available in latest FFmpeg API, ok to just remove?
                sh_video->video.dwRate= st->codec->time_base.den;
                sh_video->video.dwScale= st->codec->time_base.num;
#endif
            }
            sh_video->fps=av_q2d(st->r_frame_rate);
            sh_video->frametime=1/av_q2d(st->r_frame_rate);
            sh_video->format=bih->biCompression;
            if(st->sample_aspect_ratio.num)
                sh_video->original_aspect = codec->width * st->sample_aspect_ratio.num
                         / (float)(codec->height * st->sample_aspect_ratio.den);
            else
                sh_video->original_aspect = codec->width * codec->sample_aspect_ratio.num
                       / (float)(codec->height * codec->sample_aspect_ratio.den);
            sh_video->i_bps=codec->bit_rate/8;
            if (title && title->value)
                mp_msg(MSGT_IDENTIFY, MSGL_INFO, "ID_VID_%d_NAME=%s\n", priv->video_streams, title->value);
            if (st->disposition & AV_DISPOSITION_DEFAULT)
                sh_video->default_track = 1;
            if (rot && rot->value)
                mp_msg(MSGT_IDENTIFY, MSGL_INFO, "ID_VID_%d_ROTATE=%s\n", priv->video_streams, rot->value);
            else if (disp_matrix)
                mp_msg(MSGT_IDENTIFY, MSGL_INFO, "ID_VID_%d_ROTATE=%f\n", priv->video_streams, (float)av_display_rotation_get(disp_matrix));
            mp_msg(MSGT_DEMUX,MSGL_DBG2,"stream aspect= %d*%d/(%d*%d)\n",
                codec->width, st->sample_aspect_ratio.num,
                codec->height, st->sample_aspect_ratio.den);
            mp_msg(MSGT_DEMUX,MSGL_DBG2,"codec aspect= %d*%d/(%d*%d)\n",
                codec->width, codec->sample_aspect_ratio.num,
                codec->height, codec->sample_aspect_ratio.den);

            if(codec->extradata_size)
                memcpy(sh_video->bih + 1, codec->extradata, codec->extradata_size);
            if( mp_msg_test(MSGT_HEADER,MSGL_V) ) print_video_header(sh_video->bih, MSGL_V);
            /*
                short biPlanes;
                int  biXPelsPerMeter;
                int  biYPelsPerMeter;
                int biClrUsed;
                int biClrImportant;
            */
            // select the first video stream if auto-selection is requested
            if(demuxer->video->id == -1) {
                demuxer->video->id = i;
                demuxer->video->sh= demuxer->v_streams[i];
            }
            if(demuxer->video->id != i)
                st->discard= AVDISCARD_ALL;
            stream_id = priv->video_streams++;
            break;
        }
        case AVMEDIA_TYPE_SUBTITLE:{
            sh_sub_t* sh_sub;
            char type;
            if (codec->codec_id == AV_CODEC_ID_TEXT
                || codec->codec_id == AV_CODEC_ID_SUBRIP
                )
                type = 't';
            else if (codec->codec_id == AV_CODEC_ID_MOV_TEXT)
                type = 'm';
            else if (codec->codec_id == AV_CODEC_ID_SSA
                     || codec->codec_id == AV_CODEC_ID_ASS
                )
                type = 'a';
            else if (codec->codec_id == AV_CODEC_ID_DVD_SUBTITLE)
                type = 'v';
            else if (codec->codec_id == AV_CODEC_ID_XSUB)
                type = 'x';
            else if (codec->codec_id == AV_CODEC_ID_DVB_SUBTITLE)
                type = 'b';
            else if (codec->codec_id == AV_CODEC_ID_DVB_TELETEXT)
                type = 'd';
            else if (codec->codec_id == AV_CODEC_ID_HDMV_PGS_SUBTITLE)
                type = 'p';
            else if (codec->codec_id == AV_CODEC_ID_EIA_608)
                type = 'c';
            else if(codec->codec_tag == MKTAG('c', '6', '0', '8'))
                type = 'c';
            else
                break;
            sh_sub = new_sh_sub_sid(demuxer, i, priv->sub_streams, lang ? lang->value : NULL);
            if(!sh_sub) break;
            stream_type = "subtitle";
            priv->sstreams[priv->sub_streams] = i;
            sh_sub->type = type;
            if (codec->extradata_size) {
                sh_sub->extradata = malloc(codec->extradata_size);
                memcpy(sh_sub->extradata, codec->extradata, codec->extradata_size);
                sh_sub->extradata_len = codec->extradata_size;
            }
            if (title && title->value)
                mp_msg(MSGT_IDENTIFY, MSGL_INFO, "ID_SID_%d_NAME=%s\n", priv->sub_streams, title->value);
            if (st->disposition & AV_DISPOSITION_DEFAULT)
              sh_sub->default_track = 1;
            stream_id = priv->sub_streams++;
            break;
        }
        case AVMEDIA_TYPE_ATTACHMENT:{
            if (st->codecpar->codec_id == AV_CODEC_ID_TTF || st->codecpar->codec_id == AV_CODEC_ID_OTF) {
                AVDictionaryEntry *fnametag = av_dict_get(st->metadata, "filename", NULL, 0);
                AVDictionaryEntry *mimetype = av_dict_get(st->metadata, "mimetype", NULL, 0);
                demuxer_add_attachment(demuxer, fnametag ? fnametag->value : NULL,
                                       mimetype ? mimetype->value : "application/x-font",
                                       codec->extradata, codec->extradata_size);
            }
            break;
        }
        default:
            st->discard= AVDISCARD_ALL;
    }
    if (stream_type) {
        const AVCodec *avc = avcodec_find_decoder(codec->codec_id);
        const char *codec_name = avc ? avc->name : "unknown";
        if (!avc && *stream_type == 's' && demuxer->s_streams[i])
            codec_name = sh_sub_type2str(((sh_sub_t *)demuxer->s_streams[i])->type);
        mp_msg(MSGT_DEMUX, MSGL_INFO, "[lavf] stream %d: %s (%s), -%cid %d", i, stream_type, codec_name, *stream_type, stream_id);
        if (lang && lang->value && *stream_type != 'v')
            mp_msg(MSGT_DEMUX, MSGL_INFO, ", -%clang %s", *stream_type, lang->value);
        if (title && title->value)
            mp_msg(MSGT_DEMUX, MSGL_INFO, ", %s", title->value);
        mp_msg(MSGT_DEMUX, MSGL_INFO, "\n");
    }
}

static demuxer_t* demux_open_lavf(demuxer_t *demuxer){
    AVDictionary *opts = NULL;
    AVFormatContext *avfc;
    AVDictionaryEntry *t = NULL;
    lavf_priv_t *priv= demuxer->priv;
    int i;
    char mp_filename[2048]="mp:";

    stream_seek(demuxer->stream, 0);

    avfc = avformat_alloc_context();

    if (opt_cryptokey)
        parse_cryptokey(avfc, opt_cryptokey);
    if (user_correct_pts != 0)
        avfc->flags |= AVFMT_FLAG_GENPTS;
    if (index_mode == 0)
        avfc->flags |= AVFMT_FLAG_IGNIDX;

    if(opt_probesize) {
        if (av_opt_set_int(avfc, "probesize", opt_probesize, 0) < 0)
            mp_msg(MSGT_HEADER,MSGL_ERR, "demux_lavf, couldn't set option probesize to %u\n", opt_probesize);
    }
    if(opt_analyzeduration) {
        if (av_opt_set_int(avfc, "analyzeduration", opt_analyzeduration * AV_TIME_BASE, 0) < 0)
            mp_msg(MSGT_HEADER,MSGL_ERR, "demux_lavf, couldn't set option analyzeduration to %u\n", opt_analyzeduration);
    }

    if (rtsp_transport_http || rtsp_transport_tcp)
       av_dict_set(&opts, "rtsp_transport", rtsp_transport_http ? "http" : "tcp", 0);

    if(opt_avopt){
        if(av_dict_parse_string(&opts, opt_avopt, "=", ",", 0) < 0){
            mp_msg(MSGT_HEADER,MSGL_ERR, "Your options /%s/ look like gibberish to me pal\n", opt_avopt);
            return NULL;
        }
    }

    if(demuxer->stream->url) {
        if (!strncmp(demuxer->stream->url, "ffmpeg://dummy://", 17))
            av_strlcpy(mp_filename, demuxer->stream->url + 17, sizeof(mp_filename));
        else if (!strncmp(demuxer->stream->url, "ffmpeg://", 9))
            av_strlcpy(mp_filename, demuxer->stream->url + 9, sizeof(mp_filename));
        else if (!strncmp(demuxer->stream->url, "rtsp://", 7))
            av_strlcpy(mp_filename, demuxer->stream->url, sizeof(mp_filename));
        else if (priv->use_lavf_netstream)
            av_strlcpy(mp_filename, demuxer->stream->url, sizeof(mp_filename));
        else
            av_strlcat(mp_filename, demuxer->stream->url, sizeof(mp_filename));
    } else
        av_strlcat(mp_filename, "foobar.dummy", sizeof(mp_filename));

    if (!(priv->avif->flags & AVFMT_NOFILE)) {
        uint8_t *buffer = av_mallocz(BIO_BUFFER_SIZE);
        priv->pb = avio_alloc_context(buffer, BIO_BUFFER_SIZE, 0,
                                      demuxer, mp_read, NULL, mp_seek);
        priv->pb->read_seek = mp_read_seek;
        if (!demuxer->stream->end_pos || (demuxer->stream->flags & MP_STREAM_SEEK) != MP_STREAM_SEEK)
            priv->pb->seekable = 0;
        avfc->pb = priv->pb;
    }

    if(avformat_open_input(&avfc, mp_filename, priv->avif, &opts)<0){
        mp_msg(MSGT_HEADER,MSGL_ERR,"LAVF_header: av_open_input_stream() failed\n");
        return NULL;
    }
    if (av_dict_count(opts)) {
        AVDictionaryEntry *e = NULL;
        int invalid = 0;
        while ((e = av_dict_get(opts, "", e, AV_DICT_IGNORE_SUFFIX))) {
            if (strcmp(e->key, "rtsp_transport")) {
                invalid++;
                mp_msg(MSGT_HEADER,MSGL_ERR,"Unknown option %s\n", e->key);
            }
        }
        if (invalid)
            return 0;
    }
    av_dict_free(&opts);

    priv->avfc= avfc;

    if(avformat_find_stream_info(avfc, NULL) < 0){
        mp_msg(MSGT_HEADER,MSGL_ERR,"LAVF_header: av_find_stream_info() failed\n");
    }

    /* Add metadata. */
    while((t = av_dict_get(avfc->metadata, "", t, AV_DICT_IGNORE_SUFFIX)))
        demux_info_add(demuxer, t->key, t->value);

    for(i=0; i < avfc->nb_chapters; i++) {
        AVChapter *c = avfc->chapters[i];
        uint64_t start = av_rescale_q(c->start, c->time_base, (AVRational){1,1000});
        uint64_t end   = av_rescale_q(c->end, c->time_base, (AVRational){1,1000});
        t = av_dict_get(c->metadata, "title", NULL, 0);
        demuxer_add_chapter(demuxer, t ? t->value : NULL, start, end);
    }

    for(i=0; i<avfc->nb_streams; i++)
        handle_stream(demuxer, avfc, i);
    priv->nb_streams_last = avfc->nb_streams;

    if(avfc->nb_programs) {
        int p;
        for (p = 0; p < avfc->nb_programs; p++) {
            AVProgram *program = avfc->programs[p];
            t = av_dict_get(program->metadata, "title", NULL, 0);
            mp_msg(MSGT_HEADER,MSGL_INFO,"LAVF: Program %d %s\n", program->id, t ? t->value : "");
            mp_msg(MSGT_IDENTIFY, MSGL_V, "PROGRAM_ID=%d\n", program->id);
        }
    }

    mp_msg(MSGT_HEADER,MSGL_V,"LAVF: %d audio and %d video streams found\n",priv->audio_streams,priv->video_streams);
    mp_msg(MSGT_HEADER,MSGL_V,"LAVF: build %d\n", LIBAVFORMAT_BUILD);
    if(!priv->audio_streams) demuxer->audio->id=-2;  // nosound
//    else if(best_audio > 0 && demuxer->audio->id == -1) demuxer->audio->id=best_audio;
    if(!priv->video_streams){
        if(!priv->audio_streams){
	    mp_msg(MSGT_HEADER,MSGL_ERR,"LAVF: no audio or video headers found - broken file?\n");
            if (!priv->sub_streams)
            return NULL;
        }
        demuxer->video->id=-2; // audio-only
    } //else if (best_video > 0 && demuxer->video->id == -1) demuxer->video->id = best_video;

    return demuxer;
}

static int demux_lavf_fill_buffer(demuxer_t *demux, demux_stream_t *dsds){
    lavf_priv_t *priv= demux->priv;
    AVPacket pkt;
    demux_packet_t *dp;
    demux_stream_t *ds;
    int id;
    double stream_pts = MP_NOPTS_VALUE;
    mp_msg(MSGT_DEMUX,MSGL_DBG2,"demux_lavf_fill_buffer()\n");

    demux->filepos=stream_tell(demux->stream);

    if(av_read_frame(priv->avfc, &pkt) < 0)
        return 0;

    // handle any new streams that might have been added
    for (id = priv->nb_streams_last; id < priv->avfc->nb_streams; id++)
        handle_stream(demux, priv->avfc, id);
    priv->nb_streams_last = priv->avfc->nb_streams;

    id= pkt.stream_index;

    if(id==demux->audio->id){
        // audio
        ds=demux->audio;
        if(!ds->sh){
            ds->sh=demux->a_streams[id];
            mp_msg(MSGT_DEMUX,MSGL_V,"Auto-selected LAVF audio ID = %d\n",ds->id);
        }
    } else if(id==demux->video->id){
        // video
        sh_video_t *sh;
        ds=demux->video;
        if(!ds->sh){
            ds->sh=demux->v_streams[id];
            mp_msg(MSGT_DEMUX,MSGL_V,"Auto-selected LAVF video ID = %d\n",ds->id);
        }
        sh = ds->sh;
        if (sh && sh->bih) {
            size_t size = 0;
            const uint8_t *pal = av_packet_get_side_data(&pkt, AV_PKT_DATA_PALETTE, &size);
            if (pal && size)
                memcpy(((uint8_t *)sh->bih) + sh->bih->biSize, pal, FFMIN(size, 1024));
        }
    } else if(id==demux->sub->id){
        // subtitle
        ds=demux->sub;
        sub_utf8=1;
    } else {
        av_packet_unref(&pkt);
        return 1;
    }

        mp_packet_merge_side_data(&pkt);
        dp=new_demux_packet(pkt.size);
        memcpy(dp->buffer, pkt.data, pkt.size);

    if(pkt.pts != AV_NOPTS_VALUE){
        dp->pts=pkt.pts * av_q2d(priv->avfc->streams[id]->time_base);
        priv->last_pts= dp->pts * AV_TIME_BASE;
        if(pkt.duration > 0)
            dp->endpts = dp->pts + pkt.duration * av_q2d(priv->avfc->streams[id]->time_base);
    }
    dp->pos=demux->filepos;
    dp->flags= !!(pkt.flags&AV_PKT_FLAG_KEY);
    if (ds == demux->video &&
        stream_control(demux->stream, STREAM_CTRL_GET_CURRENT_TIME, (void *)&stream_pts) != STREAM_UNSUPPORTED)
        dp->stream_pts = stream_pts;
    // append packet to DS stream:
    ds_add_packet(ds,dp);
    av_packet_unref(&pkt);
    return 1;
}

static void demux_seek_lavf(demuxer_t *demuxer, float rel_seek_secs, float audio_delay, int flags){
    lavf_priv_t *priv = demuxer->priv;
    int avsflags = 0;
    mp_msg(MSGT_DEMUX,MSGL_DBG2,"demux_seek_lavf(%p, %f, %f, %d)\n", demuxer, rel_seek_secs, audio_delay, flags);

    if (flags & SEEK_ABSOLUTE) {
      priv->last_pts = priv->avfc->start_time != AV_NOPTS_VALUE ?
                       priv->avfc->start_time : 0;
    }
    // This is important also for SEEK_ABSOLUTE because seeking
    // is done by dts, while start_time is relative to pts and thus
    // usually too large.
    if (rel_seek_secs <= 0) avsflags = AVSEEK_FLAG_BACKWARD;
    if (flags & SEEK_FACTOR) {
      if (priv->avfc->duration == 0 || priv->avfc->duration == AV_NOPTS_VALUE)
        return;
      priv->last_pts += rel_seek_secs * priv->avfc->duration;
    } else {
      priv->last_pts += rel_seek_secs * AV_TIME_BASE;
    }
    if (av_seek_frame(priv->avfc, -1, priv->last_pts, avsflags) < 0) {
        avsflags ^= AVSEEK_FLAG_BACKWARD;
        av_seek_frame(priv->avfc, -1, priv->last_pts, avsflags);
    }
}

static int demux_lavf_control(demuxer_t *demuxer, int cmd, void *arg)
{
    lavf_priv_t *priv = demuxer->priv;

    switch (cmd) {
        case DEMUXER_CTRL_CORRECT_PTS:
	    return DEMUXER_CTRL_OK;
        case DEMUXER_CTRL_GET_TIME_LENGTH:
	    if (priv->avfc->duration == 0 || priv->avfc->duration == AV_NOPTS_VALUE)
	        return DEMUXER_CTRL_DONTKNOW;

	    *((double *)arg) = (double)priv->avfc->duration / AV_TIME_BASE;
	    return DEMUXER_CTRL_OK;

	case DEMUXER_CTRL_GET_PERCENT_POS:
	    if (priv->avfc->duration == 0 || priv->avfc->duration == AV_NOPTS_VALUE)
	        return DEMUXER_CTRL_DONTKNOW;

	    *((int *)arg) = (int)((priv->last_pts - priv->avfc->start_time)*100 / priv->avfc->duration);
	    return DEMUXER_CTRL_OK;
	case DEMUXER_CTRL_REMAP_AUDIO_ID:
	case DEMUXER_CTRL_REMAP_SUB_ID:
	{
	    int id = *((int*)arg);
	    if (priv->avfc && id >= 0 && id < priv->avfc->nb_streams) *(int *)arg = priv->avfc->streams[id]->id;
	    return DEMUXER_CTRL_OK;
	}
	case DEMUXER_CTRL_SWITCH_AUDIO:
	case DEMUXER_CTRL_SWITCH_VIDEO:
	{
	    int id = *((int*)arg);
	    int newid = -2;
	    int i, curridx = -1;
	    int nstreams, *pstreams;
	    demux_stream_t *ds;

	    if(cmd == DEMUXER_CTRL_SWITCH_VIDEO)
	    {
	        ds = demuxer->video;
	        nstreams = priv->video_streams;
	        pstreams = priv->vstreams;
	    }
	    else
	    {
	        ds = demuxer->audio;
	        nstreams = priv->audio_streams;
	        pstreams = priv->astreams;
	    }
	    for(i = 0; i < nstreams; i++)
	    {
	        if(pstreams[i] == ds->id) //current stream id
	        {
	            curridx = i;
	            break;
	        }
	    }

            if(id == -2) { // no sound
                i = -1;
            } else if(id == -1) { // next track
                i = (curridx + 2) % (nstreams + 1) - 1;
                if (i >= 0)
                    newid = pstreams[i];
	    }
	    else // select track by id
	    {
	        if (id >= 0 && id < nstreams) {
	            i = id;
	            newid = pstreams[i];
	        }
	    }
	    if(i == curridx)
	        return DEMUXER_CTRL_NOTIMPL;
	    else
	    {
	        ds_free_packs(ds);
	        if(ds->id >= 0 && ds->id < nstreams)
	            priv->avfc->streams[ds->id]->discard = AVDISCARD_ALL;
	        *((int*)arg) = ds->id = newid;
	        if(newid >= 0)
	            priv->avfc->streams[newid]->discard = AVDISCARD_NONE;
	        return DEMUXER_CTRL_OK;
	    }
        }
        case DEMUXER_CTRL_IDENTIFY_PROGRAM:
        {
            demux_program_t *prog = arg;
            AVProgram *program;
            int p, i;
            int start;

            prog->vid = prog->aid = prog->sid = -2;	//no audio and no video by default
            if(priv->avfc->nb_programs < 1)
                return DEMUXER_CTRL_DONTKNOW;

            if(prog->progid == -1)
            {
                p = 0;
                while(p<priv->avfc->nb_programs && priv->avfc->programs[p]->id != priv->cur_program)
                    p++;
                p = (p + 1) % priv->avfc->nb_programs;
            }
            else
            {
                for(i=0; i<priv->avfc->nb_programs; i++)
                    if(priv->avfc->programs[i]->id == prog->progid)
                        break;
                if(i==priv->avfc->nb_programs)
                    return DEMUXER_CTRL_DONTKNOW;
                p = i;
            }
            start = p;
redo:
            program = priv->avfc->programs[p];
            for(i=0; i<program->nb_stream_indexes; i++)
            {
                switch(priv->avfc->streams[program->stream_index[i]]->codecpar->codec_type)
                {
                    case AVMEDIA_TYPE_VIDEO:
                        if(prog->vid == -2)
                            prog->vid = program->stream_index[i];
                        break;
                    case AVMEDIA_TYPE_AUDIO:
                        if(prog->aid == -2)
                            prog->aid = program->stream_index[i];
                        break;
                    case AVMEDIA_TYPE_SUBTITLE:
                        if(prog->sid == -2)
                            prog->sid = program->stream_index[i];
                        break;
                }
            }
            if (prog->aid >= 0 && prog->aid < MAX_A_STREAMS &&
                demuxer->a_streams[prog->aid]) {
                sh_audio_t *sh = demuxer->a_streams[prog->aid];
                prog->aid = sh->aid;
            } else
                prog->aid = -2;
            if (prog->vid >= 0 && prog->vid < MAX_V_STREAMS &&
                demuxer->v_streams[prog->vid]) {
                sh_video_t *sh = demuxer->v_streams[prog->vid];
                prog->vid = sh->vid;
            } else
                prog->vid = -2;
            if(prog->progid == -1 && prog->vid == -2 && prog->aid == -2)
            {
                p = (p + 1) % priv->avfc->nb_programs;
                if (p == start)
                    return DEMUXER_CTRL_DONTKNOW;
                goto redo;
            }
            priv->cur_program = prog->progid = program->id;
            return DEMUXER_CTRL_OK;
        }
        case DEMUXER_CTRL_GET_REPLAY_GAIN:
            if (priv->r_gain == INT32_MIN)
                return DEMUXER_CTRL_DONTKNOW;
            *((int *)arg) = priv->r_gain;
            return DEMUXER_CTRL_OK;
	default:
	    return DEMUXER_CTRL_NOTIMPL;
    }
}

static void demux_close_lavf(demuxer_t *demuxer)
{
    lavf_priv_t* priv = demuxer->priv;
    if (priv){
        if(priv->avfc)
        {
         av_freep(&priv->avfc->key);
         avformat_close_input(&priv->avfc);
        }
        if (priv->pb) av_freep(&priv->pb->buffer);
        av_freep(&priv->pb);
        free(priv); demuxer->priv= NULL;
    }
}


const demuxer_desc_t demuxer_desc_lavf = {
  "libavformat demuxer",
  "lavf",
  "libavformat",
  "Michael Niedermayer",
  "supports many formats, requires libavformat",
  DEMUXER_TYPE_LAVF,
  0, // Check after other demuxer
  lavf_check_file,
  demux_lavf_fill_buffer,
  demux_open_lavf,
  demux_close_lavf,
  demux_seek_lavf,
  demux_lavf_control
};

const demuxer_desc_t demuxer_desc_lavf_preferred = {
  "libavformat preferred demuxer",
  "lavfpref",
  "libavformat",
  "Michael Niedermayer",
  "supports many formats, requires libavformat",
  DEMUXER_TYPE_LAVF_PREFERRED,
  1,
  lavf_check_preferred_file,
  demux_lavf_fill_buffer,
  demux_open_lavf,
  demux_close_lavf,
  demux_seek_lavf,
  demux_lavf_control
};
