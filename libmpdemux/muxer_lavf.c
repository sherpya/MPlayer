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
#include <sys/types.h>
#include <inttypes.h>
#include <limits.h>
#include "config.h"
#include "mp_msg.h"
#include "fmt-conversion.h"
#include "help_mp.h"
#include "mencoder.h"
#include "aviheader.h"
#include "ms_hdr.h"
#include "av_opts.h"
#include "av_helpers.h"

#include "stream/stream.h"
#include "muxer.h"
#include "demuxer.h"
#include "stheader.h"
#include "m_option.h"
#include "libavformat/avformat.h"
#include "libavutil/avstring.h"

#include "mp_taglists.h"

#define BIO_BUFFER_SIZE 32768

typedef struct {
	//AVInputFormat *avif;
	AVFormatContext *oc;
	AVIOContext *pb;
	int audio_streams;
	int video_streams;
	int64_t last_pts;
	uint8_t buffer[BIO_BUFFER_SIZE];
} muxer_priv_t;

typedef struct {
	int64_t last_pts;
	AVStream *avstream;
} muxer_stream_priv_t;

static char *conf_format = NULL;
static int mux_rate= 0;
static int mux_packet_size= 0;
static float mux_preload= 0.5;
static float mux_max_delay= 0.7;
static char *mux_avopt = NULL;

const m_option_t lavfopts_conf[] = {
	{"format", &(conf_format), CONF_TYPE_STRING, 0, 0, 0, NULL},
	{"muxrate", &mux_rate, CONF_TYPE_INT, CONF_RANGE, 0, INT_MAX, NULL},
	{"packetsize", &mux_packet_size, CONF_TYPE_INT, CONF_RANGE, 0, INT_MAX, NULL},
	{"preload", &mux_preload, CONF_TYPE_FLOAT, CONF_RANGE, 0, INT_MAX, NULL},
	{"delay", &mux_max_delay, CONF_TYPE_FLOAT, CONF_RANGE, 0, INT_MAX, NULL},
        {"o", &mux_avopt, CONF_TYPE_STRING, 0, 0, 0, NULL},

	{NULL, NULL, 0, 0, 0, 0, NULL}
};

static int mp_write(void *opaque, uint8_t *buf, int size)
{
	muxer_t *muxer = opaque;
	return stream_write_buffer(muxer->stream, buf, size);
}

static int64_t mp_seek(void *opaque, int64_t pos, int whence)
{
	muxer_t *muxer = opaque;
	if(whence == SEEK_CUR)
	{
		off_t cur = stream_tell(muxer->stream);
		if(cur == -1)
			return -1;
		pos += cur;
	}
	else if(whence == SEEK_END)
	{
		uint64_t size=0;
		if(stream_control(muxer->stream, STREAM_CTRL_GET_SIZE, &size) == STREAM_UNSUPPORTED || size < pos)
			return -1;
		pos = size - pos;
	}
	mp_msg(MSGT_MUXER, MSGL_DBG2, "SEEK %"PRIu64"\n", (int64_t)pos);
	if(!stream_seek(muxer->stream, pos))
		return -1;
	return 0;
}


static muxer_stream_t* lavf_new_stream(muxer_t *muxer, int type)
{
	muxer_priv_t *priv = muxer->priv;
	muxer_stream_t *stream;
	muxer_stream_priv_t *spriv;
	AVCodecParameters *ctx;

	if(type != MUXER_TYPE_VIDEO && type != MUXER_TYPE_AUDIO)
	{
		mp_msg(MSGT_MUXER, MSGL_ERR, "UNKNOWN TYPE %d\n", type);
		return NULL;
	}

	stream = calloc(1, sizeof(muxer_stream_t));
	if(!stream)
	{
		mp_msg(MSGT_MUXER, MSGL_ERR, "Could not allocate muxer_stream, EXIT.\n");
		return NULL;
	}
	muxer->streams[muxer->avih.dwStreams] = stream;
	stream->b_buffer = malloc(2048);
	if(!stream->b_buffer)
	{
		mp_msg(MSGT_MUXER, MSGL_ERR, "Could not allocate b_buffer, EXIT.\n");
		free(stream);
		return NULL;
	}
	stream->b_buffer_size = 2048;
	stream->b_buffer_ptr = 0;
	stream->b_buffer_len = 0;

	spriv = calloc(1, sizeof(muxer_stream_priv_t));
	if(!spriv)
	{
		free(stream);
		return NULL;
	}
	stream->priv = spriv;

	spriv->avstream = avformat_new_stream(priv->oc, NULL);
	if(!spriv->avstream)
	{
		mp_msg(MSGT_MUXER, MSGL_ERR, "Could not allocate avstream, EXIT.\n");
		return NULL;
	}
	spriv->avstream->id = 1;

	ctx = spriv->avstream->codecpar;
	ctx->codec_id = AV_CODEC_ID_NONE;
	switch(type)
	{
		case MUXER_TYPE_VIDEO:
			ctx->codec_type = AVMEDIA_TYPE_VIDEO;
			break;
		case MUXER_TYPE_AUDIO:
			ctx->codec_type = AVMEDIA_TYPE_AUDIO;
			break;
	}

	muxer->avih.dwStreams++;
	stream->muxer = muxer;
	stream->type = type;
	mp_msg(MSGT_MUXER, MSGL_V, "ALLOCATED STREAM N. %d, type=%d\n", muxer->avih.dwStreams, type);
	return stream;
}


static void fix_parameters(muxer_stream_t *stream)
{
	muxer_stream_priv_t *spriv = stream->priv;
	AVCodecParameters *ctx = spriv->avstream->codecpar;

        ctx->bit_rate= stream->avg_rate;

	if(stream->type == MUXER_TYPE_AUDIO)
	{
		if (!ctx->bit_rate)
		    ctx->bit_rate = stream->wf->nAvgBytesPerSec * 8;
		ctx->codec_id = mp_tag2codec_id(stream->wf->wFormatTag, 1);
#if 0 //breaks aac in mov at least
		ctx->codec_tag = codec_get_wav_tag(ctx->codec_id);
#endif
		mp_msg(MSGT_MUXER, MSGL_INFO, "AUDIO CODEC ID: %x, TAG: %x\n", ctx->codec_id, (uint32_t) ctx->codec_tag);
		ctx->sample_rate = stream->wf->nSamplesPerSec;
//                mp_msg(MSGT_MUXER, MSGL_INFO, "stream->h.dwSampleSize: %d\n", stream->h.dwSampleSize);
		ctx->channels = stream->wf->nChannels;
                if(stream->h.dwRate && (stream->h.dwScale * (int64_t)ctx->sample_rate) % stream->h.dwRate == 0)
                    ctx->frame_size= (stream->h.dwScale * (int64_t)ctx->sample_rate) / stream->h.dwRate;
                mp_msg(MSGT_MUXER, MSGL_V, "MUXER_LAVF(audio stream) frame_size: %d, scale: %u, sps: %u, rate: %u, ctx->block_align = stream->wf->nBlockAlign; %d=%d stream->wf->nAvgBytesPerSec:%d\n",
			ctx->frame_size, stream->h.dwScale, ctx->sample_rate, stream->h.dwRate,
			ctx->block_align, stream->wf->nBlockAlign, stream->wf->nAvgBytesPerSec);
		ctx->block_align = stream->h.dwSampleSize;
		if(stream->wf+1 && stream->wf->cbSize)
		{
			ctx->extradata = av_malloc(stream->wf->cbSize);
			if(ctx->extradata != NULL)
			{
				ctx->extradata_size = stream->wf->cbSize;
				memcpy(ctx->extradata, stream->wf+1, ctx->extradata_size);
			}
			else
				mp_msg(MSGT_MUXER, MSGL_ERR, "MUXER_LAVF(audio stream) error! Could not allocate %d bytes for extradata.\n",
					stream->wf->cbSize);
		}
	}
	else if(stream->type == MUXER_TYPE_VIDEO)
	{
		ctx->codec_id = mp_tag2codec_id(stream->bih->biCompression, 0);
                if(ctx->codec_id <= 0 || force_fourcc)
                    ctx->codec_tag= stream->bih->biCompression;
		mp_msg(MSGT_MUXER, MSGL_INFO, "VIDEO CODEC ID: %d\n", ctx->codec_id);
		if (stream->imgfmt)
		    ctx->format = imgfmt2pixfmt(stream->imgfmt);
		ctx->width = stream->bih->biWidth;
		ctx->height = stream->bih->biHeight;
		ctx->bit_rate = 800000;
		spriv->avstream->time_base.den = stream->h.dwRate;
		spriv->avstream->time_base.num = stream->h.dwScale;
		if (stream->aspect)
			ctx->sample_aspect_ratio =
			spriv->avstream->sample_aspect_ratio = av_d2q(stream->aspect * ctx->height / ctx->width, 255);

		if(stream->bih->biSize > sizeof(*stream->bih))
		{
			ctx->extradata_size = stream->bih->biSize - sizeof(*stream->bih);
			ctx->extradata = av_malloc(ctx->extradata_size);
			if(ctx->extradata != NULL)
				memcpy(ctx->extradata, stream->bih+1, ctx->extradata_size);
			else
			{
				mp_msg(MSGT_MUXER, MSGL_ERR, "MUXER_LAVF(video stream) error! Could not allocate %d bytes for extradata.\n",
					ctx->extradata_size);
				ctx->extradata_size = 0;
			}
		}
	}
}

static void write_chunk(muxer_stream_t *stream, size_t len, unsigned int flags, double dts, double pts)
{
	muxer_t *muxer = stream->muxer;
	muxer_priv_t *priv = muxer->priv;
	muxer_stream_priv_t *spriv = stream->priv;
	AVPacket pkt;

	if(len)
	{
	av_init_packet(&pkt);
	pkt.size = len;
	pkt.stream_index= spriv->avstream->index;
	pkt.data = stream->buffer;

	if(flags & AVIIF_KEYFRAME)
		pkt.flags |= AV_PKT_FLAG_KEY;
	else
		pkt.flags = 0;

	pkt.dts = (dts / av_q2d(priv->oc->streams[pkt.stream_index]->time_base) + 0.5);
	pkt.pts = (pts / av_q2d(priv->oc->streams[pkt.stream_index]->time_base) + 0.5);
//fprintf(stderr, "%Ld %Ld id:%d tb:%f %f\n", pkt.dts, pkt.pts, pkt.stream_index, av_q2d(priv->oc->streams[pkt.stream_index]->time_base), stream->timer);

	if(av_interleaved_write_frame(priv->oc, &pkt) != 0) //av_write_frame(priv->oc, &pkt)
	{
		mp_msg(MSGT_MUXER, MSGL_ERR, "Error while writing frame.\n");
	}
	}

	return;
}


static void write_header(muxer_t *muxer)
{
	muxer_priv_t *priv = muxer->priv;
	AVDictionary *opts = NULL;
	char tmpstr[50];

	mp_msg(MSGT_MUXER, MSGL_INFO, MSGTR_WritingHeader);
	if (mux_rate) {
		snprintf(tmpstr, sizeof(tmpstr), "%i", mux_rate);
		av_dict_set(&opts, "muxrate", tmpstr, 0);
	}
	if (mux_preload) {
		snprintf(tmpstr, sizeof(tmpstr), "%i", (int)(mux_preload * AV_TIME_BASE));
		av_dict_set(&opts, "preload", tmpstr, 0);
	}
	avformat_write_header(priv->oc, &opts);
	av_dict_free(&opts);
	muxer->cont_write_header = NULL;
}


static void write_trailer(muxer_t *muxer)
{
	int i;
	muxer_priv_t *priv = muxer->priv;

	mp_msg(MSGT_MUXER, MSGL_INFO, MSGTR_WritingTrailer);
	av_write_trailer(priv->oc);
	for(i = 0; i < priv->oc->nb_streams; i++)
	{
		av_freep(&(priv->oc->streams[i]));
	}

	av_freep(&priv->oc->pb);

	av_free(priv->oc);
}

static void list_formats(void) {
	void *i = NULL;
	const AVOutputFormat *fmt;
	mp_msg(MSGT_DEMUX, MSGL_INFO, "Available lavf output formats:\n");
	while ((fmt = av_muxer_iterate(&i)))
		mp_msg(MSGT_DEMUX, MSGL_INFO, "%15s : %s\n", fmt->name, fmt->long_name);
}

int muxer_init_muxer_lavf(muxer_t *muxer)
{
	muxer_priv_t *priv;
	AVOutputFormat *fmt = NULL;

	init_avformat();

	if (conf_format && strcmp(conf_format, "help") == 0) {
		list_formats();
		return 0;
	}

	mp_msg(MSGT_MUXER, MSGL_WARN, "** MUXER_LAVF *****************************************************************\n");
	mp_msg(MSGT_MUXER, MSGL_WARN,
"REMEMBER: MEncoder's libavformat muxing is presently broken and can generate\n"
"INCORRECT files in the presence of B-frames. Moreover, due to bugs MPlayer\n"
"will play these INCORRECT files as if nothing were wrong!\n"
"*******************************************************************************\n");

	priv = calloc(1, sizeof(muxer_priv_t));
	if(priv == NULL)
		return 0;

	priv->oc = avformat_alloc_context();
	if(!priv->oc)
	{
		mp_msg(MSGT_MUXER, MSGL_FATAL, "Could not get format context.\n");
		goto fail;
	}

	if(conf_format)
		fmt = av_guess_format(conf_format, NULL, NULL);
	if(! fmt)
		fmt = av_guess_format(NULL, out_filename, NULL);
	if(! fmt)
	{
		mp_msg(MSGT_MUXER, MSGL_FATAL, "Cannot get specified format.\n");
		goto fail;
	}
	if (fmt->flags & AVFMT_NOFILE) {
            const char *src = out_filename;
            if (!strncmp(out_filename, "ffmpeg://dummy://", 17)) src += 17;
            else if (!strncmp(out_filename, "ffmpeg://", 9)) src += 9;
            priv->oc->url = av_strdup(src);
	}
	priv->oc->oformat = fmt;


	priv->oc->packet_size= mux_packet_size;
        priv->oc->max_delay= (int)(mux_max_delay*AV_TIME_BASE);
        if (info_name)
            av_dict_set(&priv->oc->metadata, "title",     info_name,      0);
        if (info_artist)
            av_dict_set(&priv->oc->metadata, "author",    info_artist,    0);
        if (info_genre)
            av_dict_set(&priv->oc->metadata, "genre",     info_genre,     0);
        if (info_copyright)
            av_dict_set(&priv->oc->metadata, "copyright", info_copyright, 0);
        if (info_comment)
            av_dict_set(&priv->oc->metadata, "comment",   info_comment,   0);

        if(mux_avopt){
            if(parse_avopts(priv->oc, mux_avopt) < 0){
                mp_msg(MSGT_MUXER,MSGL_ERR, "Your options /%s/ look like gibberish to me pal.\n", mux_avopt);
                goto fail;
            }
        }

	priv->oc->pb = avio_alloc_context(priv->buffer, BIO_BUFFER_SIZE, 1, muxer, NULL, mp_write, mp_seek);
	if ((muxer->stream->flags & MP_STREAM_SEEK) != MP_STREAM_SEEK)
            priv->oc->pb->seekable = 0;

	muxer->priv = priv;
	muxer->cont_new_stream = &lavf_new_stream;
	muxer->cont_write_chunk = &write_chunk;
	muxer->cont_write_header = &write_header;
	muxer->cont_write_index = &write_trailer;
	muxer->fix_stream_parameters = &fix_parameters;
	mp_msg(MSGT_MUXER, MSGL_INFO, "OK, exit.\n");
	return 1;

fail:
	free(priv);
	return 0;
}
