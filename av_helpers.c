/*
 * Generic libav* helpers
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

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/intreadwrite.h"
#include "mp_msg.h"
#include "av_helpers.h"
#include "libaf/reorder_ch.h"

int avcodec_initialized;
int avformat_initialized;

static void mp_msp_av_log_callback(void *ptr, int level, const char *fmt,
                                   va_list vl)
{
    static int print_prefix=1;
    AVClass *avc= ptr ? *(AVClass **)ptr : NULL;
    int type= MSGT_FIXME;
    int mp_level;

    switch(level){
    case AV_LOG_VERBOSE: mp_level = MSGL_V ; break;
    case AV_LOG_DEBUG:  mp_level= MSGL_V   ; break;
    case AV_LOG_INFO :  mp_level= MSGL_INFO; break;
    case AV_LOG_ERROR:  mp_level= MSGL_ERR ; break;
    default          :  mp_level= level > AV_LOG_DEBUG ? MSGL_DBG2 : MSGL_ERR; break;
    }

    if (ptr && !avc)
        mp_msg(MSGT_DECVIDEO, MSGL_ERR, "libav* called av_log with context containing a broken AVClass!\n");
    if (avc) {
        if(!strcmp(avc->class_name, "AVCodecContext")){
            AVCodecContext *s= ptr;
            if(s->codec){
                if(s->codec->type == AVMEDIA_TYPE_AUDIO){
                    if(s->codec->decode)
                        type= MSGT_DECAUDIO;
                }else if(s->codec->type == AVMEDIA_TYPE_VIDEO){
                    if(s->codec->decode)
                        type= MSGT_DECVIDEO;
                }
                //FIXME subtitles, encoders (what msgt for them? there is no appropriate ...)
            }
        }else if(!strcmp(avc->class_name, "AVFormatContext")){
            AVFormatContext *s= ptr;
            if(s->iformat)
                type= MSGT_DEMUXER;
            else if(s->oformat)
                type= MSGT_MUXER;
        }
    }

    if (!mp_msg_test(type, mp_level)) return;

    if(print_prefix && avc) {
        mp_msg(type, mp_level, "[%s @ %p]", avc->item_name(ptr), avc);
    }

    print_prefix= strchr(fmt, '\n') != NULL;
    mp_msg_va(type, mp_level, fmt, vl);
}

static void show_av_version(int type, const char *name,
                            int header_ver, int ver, const char *conf)
{
#ifdef CONFIG_FFMPEG_SO
#define FFMPEG_TYPE "external"
#else
#define FFMPEG_TYPE "internal"
#endif
    mp_msg(type, MSGL_INFO, "%s version %d.%d.%d (" FFMPEG_TYPE ")\n",
           name, ver >> 16, (ver >> 8) & 0xFF, ver & 0xFF);
    if (header_ver != ver)
        mp_msg(type, MSGL_INFO, "Mismatching header version %d.%d.%d\n",
               header_ver >> 16, (header_ver >> 8) & 0xFF, header_ver & 0xFF);
    mp_msg(type, MSGL_V, "Configuration: %s\n", conf);
}

void init_avcodec(void)
{
    if (!avcodec_initialized) {
        show_av_version(MSGT_DECVIDEO, "libavcodec", LIBAVCODEC_VERSION_INT,
                        avcodec_version(), avcodec_configuration());
        avcodec_initialized = 1;
        av_log_set_callback(mp_msp_av_log_callback);
    }
}

void init_avformat(void)
{
    if (!avformat_initialized) {
        show_av_version(MSGT_DEMUX, "libavformat", LIBAVFORMAT_VERSION_INT,
                        avformat_version(), avformat_configuration());
        avformat_initialized = 1;
        av_log_set_callback(mp_msp_av_log_callback);
    }
}

int lavc_encode_audio(AVCodecContext *ctx, void *src, int src_len, void *dst, int dst_len)
{
    void *orig_src = src;
    int bps = av_get_bytes_per_sample(ctx->sample_fmt);
    int planar = ctx->channels > 1 && av_sample_fmt_is_planar(ctx->sample_fmt);
    int isac3 = ctx->codec->id == AV_CODEC_ID_AC3;
    int n;
    int got;
    AVPacket pkt;
    AVFrame *frame = av_frame_alloc();
    if ((ctx->channels == 6 || ctx->channels == 5) &&
        (isac3 || !strcmp(ctx->codec->name,"libfaac"))) {
        reorder_channel_nch(src, AF_CHANNEL_LAYOUT_MPLAYER_DEFAULT,
                            isac3 ? AF_CHANNEL_LAYOUT_LAVC_DEFAULT : AF_CHANNEL_LAYOUT_AAC_DEFAULT,
                            ctx->channels,
                            src_len / bps, bps);
    }
    frame->nb_samples = src_len / ctx->channels / bps;
    if (planar) {
        // TODO: this is horribly inefficient.
        int ch;
        src = av_mallocz(src_len);
        for (ch = 0; ch < ctx->channels; ch++) {
            uint8_t *tmps = (uint8_t *)orig_src + ch*bps;
            uint8_t *tmpd = (uint8_t *)src + ch*src_len/ctx->channels;
            int s;
            for (s = 0; s < frame->nb_samples; s++) {
                memcpy(tmpd, tmps, bps);
                tmps += ctx->channels * bps;
                tmpd += bps;
            }
        }
    }
    frame->format = ctx->sample_fmt;
    frame->channels = ctx->channels;
    n = avcodec_fill_audio_frame(frame, ctx->channels, ctx->sample_fmt, src, src_len, 1);
    if (n < 0) return 0;
    n = avcodec_send_frame(ctx, frame);
    av_init_packet(&pkt);
    got = avcodec_receive_packet(ctx, &pkt);
    av_frame_free(&frame);
    if (planar) av_free(src);
    if (n < 0) return n;
    if (got >= 0) {
        int size = pkt.size;
        if (size > dst_len) return -1;
        memcpy(dst, pkt.data, size);
        av_packet_unref(&pkt);
        return size;
    }
    return 0;
}

#define MERGE_MARKER 0x8c4d9d108e25e9feULL

static void bytestream_put_buffer(uint8_t **b, const uint8_t *src, unsigned int size)
{
    memcpy(*b, src, size);
    *b += size;
}

int mp_packet_merge_side_data(AVPacket *pkt){
    if(pkt->side_data_elems){
        AVBufferRef *buf;
        int i;
        uint8_t *p;
        uint64_t size= pkt->size + 8LL + AV_INPUT_BUFFER_PADDING_SIZE;
        AVPacket old= *pkt;
        for (i=0; i<old.side_data_elems; i++) {
            size += old.side_data[i].size + 5LL;
        }
        if (size > INT_MAX)
            return AVERROR(EINVAL);
        buf = av_buffer_alloc(size);
        if (!buf)
            return AVERROR(ENOMEM);
        pkt->buf = buf;
        pkt->data = p = buf->data;
        pkt->size = size - AV_INPUT_BUFFER_PADDING_SIZE;
        bytestream_put_buffer(&p, old.data, old.size);
        for (i=old.side_data_elems-1; i>=0; i--) {
            bytestream_put_buffer(&p, old.side_data[i].data, old.side_data[i].size);
            AV_WB32(p, old.side_data[i].size);
            p += 4;
            *p++ = old.side_data[i].type | ((i==old.side_data_elems-1)*128);
        }
        AV_WB64(p, MERGE_MARKER);
        p += 8;
        memset(p, 0, AV_INPUT_BUFFER_PADDING_SIZE);
        av_packet_unref(&old);
        pkt->side_data_elems = 0;
        pkt->side_data = NULL;
        return 1;
    }
    return 0;
}

int mp_packet_split_side_data(AVPacket *pkt){
    if (!pkt->side_data_elems && pkt->size >12 && AV_RB64(pkt->data + pkt->size - 8) == MERGE_MARKER){
        int i;
        unsigned int size;
        uint8_t *p;

        p = pkt->data + pkt->size - 8 - 5;
        for (i=1; ; i++){
            size = AV_RB32(p);
            if (size>INT_MAX - 5 || p - pkt->data < size)
                return 0;
            if (p[4]&128)
                break;
            if (p - pkt->data < size + 5)
                return 0;
            p-= size+5;
        }

        if (i > AV_PKT_DATA_NB)
            return AVERROR(ERANGE);

        pkt->side_data = av_malloc_array(i, sizeof(*pkt->side_data));
        if (!pkt->side_data)
            return AVERROR(ENOMEM);

        p= pkt->data + pkt->size - 8 - 5;
        for (i=0; ; i++){
            size= AV_RB32(p);
            pkt->side_data[i].data = av_mallocz(size + AV_INPUT_BUFFER_PADDING_SIZE);
            pkt->side_data[i].size = size;
            pkt->side_data[i].type = p[4]&127;
            if (!pkt->side_data[i].data)
                return AVERROR(ENOMEM);
            memcpy(pkt->side_data[i].data, p-size, size);
            pkt->size -= size + 5;
            if(p[4]&128)
                break;
            p-= size+5;
        }
        pkt->size -= 8;
        pkt->side_data_elems = i+1;
        return 1;
    }
    return 0;
}
