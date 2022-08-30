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

#ifndef MPLAYER_MUXER_H
#define MPLAYER_MUXER_H

#include <stdint.h>
#include <sys/types.h>
#include "aviheader.h"
#include "m_option.h"
#include "ms_hdr.h"
#include "stream/stream.h"

#define MUXER_MAX_STREAMS 16

#define MUXER_TYPE_VIDEO 0
#define MUXER_TYPE_AUDIO 1

#define MUXER_TYPE_AVI 0
#define MUXER_TYPE_MPEG 1
#define MUXER_TYPE_RAWVIDEO 2
#define MUXER_TYPE_LAVF 3
#define MUXER_TYPE_RAWAUDIO 4


typedef struct {
  // muxer data:
  int type;  // audio or video
  int id;    // stream no
  uint32_t ckid; // chunk id (00dc 01wb etc)
  double timer;
  off_t size;
  float aspect; // aspect ratio of this stream (set by ve_*.c)
  // buffering:
  unsigned char *buffer;
  unsigned int buffer_size;
  unsigned int buffer_len;
  // mpeg block buffer:
  unsigned char *b_buffer;
  unsigned int b_buffer_size;	//size of b_buffer
  unsigned int b_buffer_ptr;	//index to next data to write
  unsigned int b_buffer_len;	//len of next data to write
  // muxer frame buffer:
  unsigned int muxbuf_seen;
  // source stream:
  void* source; // sh_audio or sh_video
  int codec; // codec used for encoding. 0 means copy
  // avi stream header:
  AVIStreamHeader h;  // Rate/Scale and SampleSize must be filled by caller!
  // stream specific:
  WAVEFORMATEX *wf;
  BITMAPINFOHEADER *bih;   // in format
  int encoder_delay; // in number of frames
  int decoder_delay; // in number of frames
  int imgfmt;
  // mpeg specific:
  size_t ipb[3]; // sizes of I/P/B frames
  // muxer of that stream
  struct muxer_t *muxer;
  void *priv; // private stream specific data stored by the muxer

  int vbv_size;
  int max_rate;
  int avg_rate;
} muxer_stream_t;

typedef struct {
  uint32_t id;
  char *text;
} muxer_info_t;

typedef struct muxer_t{
  // encoding:
  MainAVIHeader avih;
  off_t movi_start;
  off_t movi_end;
  off_t file_end; // for MPEG it's system timestamp in 1/90000 s
  float audio_delay_fix;
  // index:
  AVIINDEXENTRY *idx;
  int idx_pos;
  int idx_size;
  // streams:
  int num_videos;	// for MPEG recalculations
  int num_audios;
  unsigned int sysrate;	// max rate in bytes/s
  //int num_streams;
  muxer_stream_t* def_v;  // default video stream (for general headers)
  muxer_stream_t* streams[MUXER_MAX_STREAMS];
  // muxer frame buffer:
  struct muxbuf_t * muxbuf;
  int muxbuf_num;
  int muxbuf_skip_buffer;
  // functions:
  stream_t *stream;
  void (*fix_stream_parameters)(muxer_stream_t *);
  void (*cont_write_chunk)(muxer_stream_t *,size_t,unsigned int, double dts, double pts);
  void (*cont_write_header)(struct muxer_t *);
  void (*cont_write_index)(struct muxer_t *);
  muxer_stream_t* (*cont_new_stream)(struct muxer_t *,int);
  void *priv;
} muxer_t;

/* muxer frame buffer */
typedef struct muxbuf_t {
  muxer_stream_t *stream; /* pointer back to corresponding stream */
  double dts; /* decode timestamp / time at which this packet should be feeded into the decoder */
  double pts; /* presentation timestamp / time at which the data in this packet will be presented to the user */
  unsigned char *buffer;
  size_t len;
  unsigned int flags;
} muxbuf_t;

extern char *force_fourcc;

extern const m_option_t lavfopts_conf[];
extern const m_option_t mpegopts_conf[];
extern const m_option_t nuvopts_conf[];

muxer_t *muxer_new_muxer(int type,stream_t *stream);
off_t muxer_close(muxer_t *muxer);
void muxer_free(muxer_t *muxer);
void muxer_flush(muxer_t *m);
#define muxer_new_stream(muxer,a) muxer->cont_new_stream(muxer,a)
#define muxer_stream_fix_parameters(muxer, a) muxer->fix_stream_parameters(a)
void muxer_write_chunk(muxer_stream_t *s, size_t len, unsigned int flags, double dts, double pts);
#define muxer_write_header(muxer) muxer->cont_write_header(muxer)
#define muxer_write_index(muxer) muxer->cont_write_index(muxer)

int muxer_init_muxer_avi(muxer_t *);
int muxer_init_muxer_mpeg(muxer_t *);
int muxer_init_muxer_rawvideo(muxer_t *);
int muxer_init_muxer_lavf(muxer_t *);
int muxer_init_muxer_rawaudio(muxer_t *);

#endif /* MPLAYER_MUXER_H */
