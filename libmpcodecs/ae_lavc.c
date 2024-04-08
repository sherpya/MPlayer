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
#include <inttypes.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include "config.h"
#include "m_option.h"
#include "mp_msg.h"
#include "libmpdemux/aviheader.h"
#include "libmpdemux/ms_hdr.h"
#include "stream/stream.h"
#include "libmpdemux/muxer.h"
#include "ae_lavc.h"
#include "av_helpers.h"
#include "ve.h"
#include "help_mp.h"
#include "av_opts.h"
#include "libaf/af_format.h"
#include "libaf/reorder_ch.h"
#include "libavcodec/avcodec.h"
#include "libavutil/intreadwrite.h"
#include "libavformat/avformat.h"
#include "libmpdemux/mp_taglists.h"
#include "fmt-conversion.h"

static AVCodec        *lavc_acodec;
static AVCodecContext *lavc_actx;
static int compressed_frame_size = 0;

static int bind_lavc(audio_encoder_t *encoder, muxer_stream_t *mux_a)
{
	mux_a->wf = malloc(sizeof(WAVEFORMATEX)+lavc_actx->extradata_size+256);
	mux_a->wf->wFormatTag = lavc_param_atag;
	mux_a->wf->nChannels = lavc_actx->ch_layout.nb_channels;
	mux_a->wf->nSamplesPerSec = lavc_actx->sample_rate;
	mux_a->wf->nAvgBytesPerSec = (lavc_actx->bit_rate / 8);
        mux_a->avg_rate= lavc_actx->bit_rate;
	mux_a->h.dwRate = mux_a->wf->nAvgBytesPerSec;
	if(lavc_actx->block_align)
		mux_a->h.dwSampleSize = mux_a->h.dwScale = lavc_actx->block_align;
	else
	{
		mux_a->h.dwScale = (mux_a->wf->nAvgBytesPerSec * lavc_actx->frame_size)/ mux_a->wf->nSamplesPerSec; /* for cbr */

		if ((mux_a->wf->nAvgBytesPerSec *
			lavc_actx->frame_size) % mux_a->wf->nSamplesPerSec)
		{
			mux_a->h.dwScale = lavc_actx->frame_size;
			mux_a->h.dwRate = lavc_actx->sample_rate;
			mux_a->h.dwSampleSize = 0; // Blocksize not constant
		}
		else
			mux_a->h.dwSampleSize = 0;
	}
        if(mux_a->h.dwSampleSize)
                mux_a->wf->nBlockAlign = mux_a->h.dwSampleSize;
        else
                mux_a->wf->nBlockAlign = 1;
	mux_a->h.dwSuggestedBufferSize = (encoder->params.audio_preload*mux_a->wf->nAvgBytesPerSec)/1000;
	mux_a->h.dwSuggestedBufferSize -= mux_a->h.dwSuggestedBufferSize % mux_a->wf->nBlockAlign;

	switch(lavc_param_atag)
	{
		case 0x11: /* imaadpcm */
			mux_a->wf->wBitsPerSample = 4;
			mux_a->wf->cbSize = 2;
			AV_WL16(mux_a->wf+1, lavc_actx->frame_size);
			break;
		case 0x55: /* mp3 */
			mux_a->wf->cbSize = 12;
			mux_a->wf->wBitsPerSample = 0; /* does not apply */
			((MPEGLAYER3WAVEFORMAT *) (mux_a->wf))->wID = 1;
			((MPEGLAYER3WAVEFORMAT *) (mux_a->wf))->fdwFlags = 2;
			((MPEGLAYER3WAVEFORMAT *) (mux_a->wf))->nBlockSize = mux_a->wf->nBlockAlign;
			((MPEGLAYER3WAVEFORMAT *) (mux_a->wf))->nFramesPerBlock = 1;
			((MPEGLAYER3WAVEFORMAT *) (mux_a->wf))->nCodecDelay = 0;
			break;
		default:
			mux_a->wf->wBitsPerSample = 0; /* Unknown */
			if (lavc_actx->extradata && (lavc_actx->extradata_size > 0))
			{
				memcpy(mux_a->wf+1, lavc_actx->extradata, lavc_actx->extradata_size);
				mux_a->wf->cbSize = lavc_actx->extradata_size;
			}
			else
				mux_a->wf->cbSize = 0;
			break;
	}

	// Fix allocation
	mux_a->wf = realloc(mux_a->wf, sizeof(WAVEFORMATEX)+mux_a->wf->cbSize);

	encoder->min_buffer_size = mux_a->h.dwSuggestedBufferSize;
	encoder->max_buffer_size = mux_a->h.dwSuggestedBufferSize*2;

	return 1;
}

static int encode_lavc(audio_encoder_t *encoder, uint8_t *dest, void *src, int size, int max_size)
{
	int n;
	n = lavc_encode_audio(lavc_actx, src, size, dest, max_size);
	compressed_frame_size = n < 0 ? 0 : n;
	return compressed_frame_size;
}


static int close_lavc(audio_encoder_t *encoder)
{
	compressed_frame_size = 0;
	return 1;
}

static int get_frame_size(audio_encoder_t *encoder)
{
        int sz = compressed_frame_size;
        compressed_frame_size = 0;
        return sz;
}


int mpae_init_lavc(audio_encoder_t *encoder)
{
	encoder->params.samples_per_frame = encoder->params.sample_rate;
	encoder->params.bitrate = encoder->params.sample_rate * encoder->params.channels * 2 * 8;

	if(!lavc_param_acodec)
	{
		mp_msg(MSGT_MENCODER, MSGL_FATAL, MSGTR_NoLavcAudioCodecName);
		return 0;
	}

	init_avcodec();

	lavc_acodec = avcodec_find_encoder_by_name(lavc_param_acodec);
	if (!lavc_acodec)
	{
		mp_msg(MSGT_MENCODER, MSGL_FATAL, MSGTR_LavcAudioCodecNotFound, lavc_param_acodec);
		return 0;
	}
	if(lavc_param_atag == 0)
	{
		lavc_param_atag = mp_codec_id2tag(lavc_acodec->id, 0, 1);
		if(!lavc_param_atag)
		{
			mp_msg(MSGT_MENCODER, MSGL_FATAL, "Couldn't find wav tag for specified codec, exit\n");
			return 0;
		}
	}

	lavc_actx = avcodec_alloc_context3(lavc_acodec);
	if(lavc_actx == NULL)
	{
		mp_msg(MSGT_MENCODER, MSGL_FATAL, MSGTR_CouldntAllocateLavcContext);
		return 0;
	}

	lavc_actx->codec_id = lavc_acodec->id;
	// put sample parameters
	lavc_actx->sample_fmt = AV_SAMPLE_FMT_S16;
	if (lavc_acodec->sample_fmts) {
		const enum AVSampleFormat *fmts;
		lavc_actx->sample_fmt = lavc_acodec->sample_fmts[0]; // fallback to first format
		for (fmts = lavc_acodec->sample_fmts; *fmts != AV_SAMPLE_FMT_NONE; fmts++) {
			if (samplefmt2affmt(av_get_packed_sample_fmt(*fmts)) == encoder->params.sample_format) { // preferred format found
				lavc_actx->sample_fmt = *fmts;
				break;
			}
		}
	}
	encoder->input_format = samplefmt2affmt(av_get_packed_sample_fmt(lavc_actx->sample_fmt));
	if (encoder->input_format == AF_FORMAT_UNKNOWN) {
            mp_msg(MSGT_MENCODER,MSGL_ERR, "Audio encoder requires unknown or unsupported input format\n");
            return 0;
	}
	lavc_actx->ch_layout.nb_channels = encoder->params.channels;
	lavc_actx->sample_rate = encoder->params.sample_rate;
	lavc_actx->time_base.num = 1;
	lavc_actx->time_base.den = encoder->params.sample_rate;
        if(lavc_param_abitrate<1000)
                lavc_actx->bit_rate = encoder->params.bitrate = lavc_param_abitrate * 1000;
        else
                lavc_actx->bit_rate = encoder->params.bitrate = lavc_param_abitrate;
        if(lavc_param_audio_avopt){
            if(parse_avopts(lavc_actx, lavc_param_audio_avopt) < 0){
                mp_msg(MSGT_MENCODER,MSGL_ERR, "Your options /%s/ look like gibberish to me pal\n", lavc_param_audio_avopt);
                return 0;
            }
        }


	/*
	* Special case for adpcm_ima_wav.
	* The bitrate is only dependent on samplerate.
	* We have to known frame_size and block_align in advance,
	* so I just copied the code from libavcodec/adpcm.c
	*
	* However, ms adpcm_ima_wav uses a block_align of 2048,
	* lavc defaults to 1024
	*/
	if(lavc_param_atag == 0x11) {
		int blkalign = 2048;
		int framesize = (blkalign - 4 * lavc_actx->ch_layout.nb_channels) * 8 / (4 * lavc_actx->ch_layout.nb_channels) + 1;
		lavc_actx->bit_rate = lavc_actx->sample_rate*8*blkalign/framesize;
	}
        if((lavc_param_audio_global_header&1)
        /*|| (video_global_header==0 && (oc->oformat->flags & AVFMT_GLOBALHEADER))*/){
                lavc_actx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
        }
        if(lavc_param_audio_global_header&2){
                lavc_actx->flags2 |= AV_CODEC_FLAG2_LOCAL_HEADER;
        }

	if(avcodec_open2(lavc_actx, lavc_acodec, NULL) < 0)
	{
		mp_msg(MSGT_MENCODER, MSGL_FATAL, MSGTR_CouldntOpenCodec, lavc_param_acodec, lavc_param_abitrate);
		return 0;
	}

	if(lavc_param_atag == 0x11) {
		lavc_actx->block_align = 2048;
		lavc_actx->frame_size = (lavc_actx->block_align - 4 * lavc_actx->ch_layout.nb_channels) * 8 / (4 * lavc_actx->ch_layout.nb_channels) + 1;
	}

	encoder->decode_buffer_size = lavc_actx->frame_size *
	                              av_get_bytes_per_sample(lavc_actx->sample_fmt) *
	                              encoder->params.channels;
	while (encoder->decode_buffer_size < 1024) encoder->decode_buffer_size *= 2;
	encoder->bind = bind_lavc;
	encoder->get_frame_size = get_frame_size;
	encoder->encode = encode_lavc;
	encoder->close = close_lavc;

	return 1;
}
