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

#ifndef MPLAYER_STREAM_H
#define MPLAYER_STREAM_H

#include "config.h"
#include "m_option.h"
#include "mp_msg.h"
#include "url.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <sys/types.h>
#include <fcntl.h>

#define STREAMTYPE_DUMMY -1    // for placeholders, when the actual reading is handled in the demuxer
#define STREAMTYPE_FILE 0      // read from seekable file
#define STREAMTYPE_VCD  1      // raw mode-2 CDROM reading, 2324 bytes/sector
#define STREAMTYPE_STREAM 2    // same as FILE but no seeking (for net/stdin)
#define STREAMTYPE_DVD  3      // libdvdread
#define STREAMTYPE_MEMORY  4   // read data from memory area
#define STREAMTYPE_PLAYLIST 6  // FIXME!!! same as STREAMTYPE_FILE now
#define STREAMTYPE_DS   8      // read from a demuxer stream
#define STREAMTYPE_DVDNAV 9    // we cannot safely "seek" in this...
#define STREAMTYPE_CDDA 10     // raw audio CD reader
#define STREAMTYPE_SMB 11      // smb:// url, using libsmbclient (samba)
#define STREAMTYPE_BINCUE 12   // cd/vcd directly from bin/cue files
#define STREAMTYPE_DVB 13
#define STREAMTYPE_VSTREAM 14
#define STREAMTYPE_SDP 15
#define STREAMTYPE_PVR 16
#define STREAMTYPE_TV 17
#define STREAMTYPE_MF 18
#define STREAMTYPE_RADIO 19
#define STREAMTYPE_BLURAY 20
#define STREAMTYPE_BD 21

#define STREAM_BUFFER_MIN 2048
#define STREAM_BUFFER_SIZE (2*STREAM_BUFFER_MIN) // must be at least 2*STREAM_BUFFER_MIN
#define STREAM_MAX_SECTOR_SIZE (8*1024)

#define VCD_SECTOR_SIZE 2352
#define VCD_SECTOR_OFFS 24
#define VCD_SECTOR_DATA 2324

/// atm it will always use mode == STREAM_READ
/// streams that use the new api should check the mode at open
#define STREAM_READ  0
#define STREAM_WRITE 1
#define STREAM_APPEND 2
/// Seek flags, if not mannualy set and s->seek isn't NULL
/// MP_STREAM_SEEK is automaticly set
#define MP_STREAM_SEEK_BW  2
#define MP_STREAM_SEEK_FW  4
#define MP_STREAM_SEEK  (MP_STREAM_SEEK_BW|MP_STREAM_SEEK_FW)
/** This is a HACK for live555 that does not respect the
    separation between stream an demuxer and thus is not
    actually a stream cache can not be used */
#define STREAM_NON_CACHEABLE 8

//////////// Open return code
#define STREAM_REDIRECTED -2
/// This can't open the requested protocol (used by stream wich have a
/// * protocol when they don't know the requested protocol)
#define STREAM_UNSUPPORTED -1
#define STREAM_ERROR 0
#define STREAM_OK    1

#define MAX_STREAM_PROTOCOLS 10

#define STREAM_CTRL_RESET 0
#define STREAM_CTRL_GET_TIME_LENGTH 1
#define STREAM_CTRL_SEEK_TO_CHAPTER 2
#define STREAM_CTRL_GET_CURRENT_CHAPTER 3
#define STREAM_CTRL_GET_NUM_CHAPTERS 4
#define STREAM_CTRL_GET_CURRENT_TIME 5
#define STREAM_CTRL_SEEK_TO_TIME 6
#define STREAM_CTRL_GET_SIZE 7
#define STREAM_CTRL_GET_ASPECT_RATIO 8
#define STREAM_CTRL_GET_NUM_ANGLES 9
#define STREAM_CTRL_GET_ANGLE 10
#define STREAM_CTRL_SET_ANGLE 11
#define STREAM_CTRL_GET_NUM_TITLES 12
#define STREAM_CTRL_GET_LANG 13
#define STREAM_CTRL_GET_CURRENT_TITLE 14
#define STREAM_CTRL_GET_CURRENT_CHANNEL 15

enum stream_ctrl_type {
	stream_ctrl_audio,
	stream_ctrl_sub,
};

struct stream_lang_req {
	enum stream_ctrl_type type;
	int id;
	char buf[40];
};

typedef enum {
	streaming_stopped_e,
	streaming_playing_e
} streaming_status;

typedef struct streaming_control {
	URL_t *url;
	streaming_status status;
	int buffering;	// boolean
	unsigned int prebuffer_size;
	char *buffer;
	unsigned int buffer_size;
	unsigned int buffer_pos;
	unsigned int bandwidth;	// The downstream available
	int (*streaming_read)( int fd, char *buffer, int buffer_size, struct streaming_control *stream_ctrl );
	int (*streaming_seek)( int fd, int64_t pos, struct streaming_control *stream_ctrl );
	void *data;
} streaming_ctrl_t;

struct stream;
typedef struct stream_info_st {
  const char *info;
  const char *name;
  const char *author;
  const char *comment;
  /// mode isn't used atm (ie always READ) but it shouldn't be ignored
  /// opts is at least in it's defaults settings and may have been
  /// altered by url parsing if enabled and the options string parsing.
  int (*open)(struct stream* st, int mode, void* opts, int* file_format);
  const char* protocols[MAX_STREAM_PROTOCOLS];
  const void* opts;
  int opts_url; /* If this is 1 we will parse the url as an option string
		 * too. Otherwise options are only parsed from the
		 * options string given to open_stream_plugin */
} stream_info_t;

typedef struct stream {
  // Read
  int (*fill_buffer)(struct stream *s, char* buffer, int max_len);
  // Write
  int (*write_buffer)(struct stream *s, char* buffer, int len);
  // Seek
  int (*seek)(struct stream *s, int64_t pos);
  // Control
  // Will be later used to let streams like dvd and cdda report
  // their structure (ie tracks, chapters, etc)
  int (*control)(struct stream *s,int cmd,void* arg);
  // Close
  void (*close)(struct stream *s);

  int fd;   // file descriptor, see man open(2)
  int type; // see STREAMTYPE_*
  int flags;
  int sector_size; // sector size (seek will be aligned on this size if non 0)
  int read_chunk; // maximum amount of data to read at once to limit latency (0 for default)
  unsigned int buf_pos,buf_len;
  int64_t pos,start_pos,end_pos;
  int eof;
  int mode; //STREAM_READ or STREAM_WRITE
  unsigned int cache_pid;
  void* cache_data;
  void* priv; // used for DVD, TV, RTSP etc
  char* url;  // strdup() of filename/url
#ifdef CONFIG_NETWORKING
  streaming_ctrl_t *streaming_ctrl;
#endif
  unsigned char buffer[STREAM_BUFFER_SIZE>STREAM_MAX_SECTOR_SIZE?STREAM_BUFFER_SIZE:STREAM_MAX_SECTOR_SIZE];
  struct stream *capture_stream;
} stream_t;

#ifdef CONFIG_NETWORKING
#include "network.h"
#endif

int stream_fill_buffer(stream_t *s);
int stream_seek_long(stream_t *s, int64_t pos);
void stream_capture_do(stream_t *s);

#ifdef CONFIG_STREAM_CACHE
int stream_enable_cache(stream_t *stream,int64_t size,int64_t min,int64_t prefill);
int cache_stream_fill_buffer(stream_t *s);
int cache_stream_seek_long(stream_t *s,int64_t pos);
#else
// no cache, define wrappers:
#define cache_stream_fill_buffer(x) stream_fill_buffer(x)
#define cache_stream_seek_long(x,y) stream_seek_long(x,y)
#define stream_enable_cache(x,y,z,w) 1
#endif
int stream_write_buffer(stream_t *s, unsigned char *buf, int len);

static inline int stream_read_char(stream_t *s)
{
  return (s->buf_pos<s->buf_len)?s->buffer[s->buf_pos++]:
    (cache_stream_fill_buffer(s)?s->buffer[s->buf_pos++]:-256);
//  if(s->buf_pos<s->buf_len) return s->buffer[s->buf_pos++];
//  stream_fill_buffer(s);
//  if(s->buf_pos<s->buf_len) return s->buffer[s->buf_pos++];
//  return 0; // EOF
}

static inline unsigned int stream_read_word(stream_t *s)
{
  int x,y;
  x=stream_read_char(s);
  y=stream_read_char(s);
  return (x<<8)|y;
}

static inline unsigned int stream_read_dword(stream_t *s)
{
  unsigned int y;
  y=stream_read_char(s);
  y=(y<<8)|stream_read_char(s);
  y=(y<<8)|stream_read_char(s);
  y=(y<<8)|stream_read_char(s);
  return y;
}

#define stream_read_fourcc stream_read_dword_le

static inline unsigned int stream_read_word_le(stream_t *s)
{
  int x,y;
  x=stream_read_char(s);
  y=stream_read_char(s);
  return (y<<8)|x;
}

static inline unsigned int stream_read_dword_le(stream_t *s)
{
  unsigned int y;
  y=stream_read_char(s);
  y|=stream_read_char(s)<<8;
  y|=stream_read_char(s)<<16;
  y|=stream_read_char(s)<<24;
  return y;
}

static inline uint64_t stream_read_qword(stream_t *s)
{
  uint64_t y;
  y = stream_read_char(s);
  y=(y<<8)|stream_read_char(s);
  y=(y<<8)|stream_read_char(s);
  y=(y<<8)|stream_read_char(s);
  y=(y<<8)|stream_read_char(s);
  y=(y<<8)|stream_read_char(s);
  y=(y<<8)|stream_read_char(s);
  y=(y<<8)|stream_read_char(s);
  return y;
}

static inline uint64_t stream_read_qword_le(stream_t *s)
{
  uint64_t y;
  y = stream_read_dword_le(s);
  y|=(uint64_t)stream_read_dword_le(s)<<32;
  return y;
}

static inline unsigned int stream_read_int24(stream_t *s)
{
  unsigned int y;
  y = stream_read_char(s);
  y=(y<<8)|stream_read_char(s);
  y=(y<<8)|stream_read_char(s);
  return y;
}

static inline int stream_read(stream_t *s, char *mem, int total)
{
  int len=total;
  while(len>0){
    int x;
    x=s->buf_len-s->buf_pos;
    if(x==0){
      if(!cache_stream_fill_buffer(s)) return total-len; // EOF
      x=s->buf_len-s->buf_pos;
    }
    if(s->buf_pos>s->buf_len) mp_msg(MSGT_DEMUX, MSGL_WARN, "stream_read: WARNING! s->buf_pos>s->buf_len\n");
    if(x>len) x=len;
    memcpy(mem,&s->buffer[s->buf_pos],x);
    s->buf_pos+=x; mem+=x; len-=x;
  }
  return total;
}

uint8_t *stream_read_until(stream_t *s, uint8_t *mem, int max, uint8_t term, int utf16);
static inline uint8_t *stream_read_line(stream_t *s, uint8_t *mem,
                                        int max, int utf16)
{
  return stream_read_until(s, mem, max, '\n', utf16);
}

static inline int stream_eof(stream_t *s)
{
  return s->eof;
}

static inline int64_t stream_tell(stream_t *s)
{
  return s->pos+s->buf_pos-s->buf_len;
}

static inline int stream_seek(stream_t *s, int64_t pos)
{

  mp_dbg(MSGT_DEMUX, MSGL_DBG3, "seek to 0x%"PRIX64"\n", pos);

  if (pos < 0) {
    mp_msg(MSGT_DEMUX, MSGL_ERR,
           "Invalid seek to negative position %"PRIx64"!\n", pos);
    pos = 0;
  }
  if (s->buf_len == 0 && s->pos == pos)
    return 1;
  if(pos<s->pos){
    int64_t x=pos-(s->pos-s->buf_len);
    if(x>=0){
      s->buf_pos=x;
//      putchar('*');fflush(stdout);
      return 1;
    }
  }

  return cache_stream_seek_long(s,pos);
}

static inline int stream_skip(stream_t *s, int64_t len)
{
  if( len<0 || (len>2*STREAM_BUFFER_SIZE && (s->flags & MP_STREAM_SEEK_FW)) ) {
    // negative or big skip!
    return stream_seek(s,stream_tell(s)+len);
  }
  while(len>0){
    int x=s->buf_len-s->buf_pos;
    if(x==0){
      if(!cache_stream_fill_buffer(s)) return 0; // EOF
      x=s->buf_len-s->buf_pos;
    }
    if(x>len) x=len;
    //memcpy(mem,&s->buf[s->buf_pos],x);
    s->buf_pos+=x; len-=x;
  }
  return 1;
}

void stream_reset(stream_t *s);
int stream_control(stream_t *s, int cmd, void *arg);
stream_t* new_stream(int fd,int type);
int free_stream(stream_t *s);
stream_t* new_memory_stream(unsigned char* data,int len);
stream_t* open_stream(const char* filename,char** options,int* file_format);
stream_t* open_stream_full(const char* filename,int mode, char** options, int* file_format);
stream_t* open_output_stream(const char* filename,char** options);
/// Set the callback to be used by libstream to check for user
/// interruption during long blocking operations (cache filling, etc).
void stream_set_interrupt_callback(int (*cb)(int));
/// Call the interrupt checking callback if there is one and
/// wait for time milliseconds
int stream_check_interrupt(int time);
/// Internal read function bypassing the stream buffer
int stream_read_internal(stream_t *s, void *buf, int len);
/// Internal seek function bypassing the stream buffer
int stream_seek_internal(stream_t *s, int64_t newpos);

extern int bluray_angle;
extern int bluray_chapter;
extern int dvd_speed;
extern int dvd_chapter;
extern int dvd_last_chapter;
extern int dvd_angle;

extern char *bluray_device;
extern char * audio_stream;
extern char *cdrom_device;
extern char *dvd_device;

extern const m_option_t dvbin_opts_conf[];

extern char *rtsp_destination;
extern char *lavfstreamopts;

typedef struct {
 int id; // 0 - 31 mpeg; 128 - 159 ac3; 160 - 191 pcm
 int language;
 int type;
 int channels;
} stream_language_t;

int bluray_id_from_lang(stream_t *s, enum stream_ctrl_type type, const char *lang);

int parse_chapter_range(const m_option_t *conf, const char *range);

#ifdef __MINGW32__
wchar_t *utf8_to_wide_char(const char *utf8);
char *utf8_to_local_windows_code_page(const char *utf8);
#endif

#endif /* MPLAYER_STREAM_H */
