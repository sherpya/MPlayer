/*
 * Skeleton of function spudec_process_controll() is from xine sources.
 * Further works:
 * LGB,... (yeah, try to improve it and insert your name here! ;-)
 *
 * Kim Minh Kaplan
 * implement fragments reassembly, RLE decoding.
 * read brightness from the IFO.
 *
 * For information on SPU format see <URL:http://sam.zoy.org/doc/dvd/subtitles/>
 * and <URL:http://members.aol.com/mpucoder/DVD/spu.html>
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

#include "config.h"
#include "libavutil/avstring.h"
#include "mp_msg.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "sub.h"
#include "libvo/video_out.h"
#include "spudec.h"
#include "vobsub.h"
#include "libavutil/avutil.h"
#include "libavutil/common.h"
#include "libavutil/intreadwrite.h"
#include "libswscale/swscale.h"

/* Valid values for spu_aamode:
   0: none (fastest, most ugly)
   1: approximate
   2: full (slowest)
   3: bilinear (similiar to vobsub, fast and not too bad)
   4: uses swscaler gaussian (this is the only one that looks good)
 */

int spu_aamode = 3;
int spu_alignment = -1;
float spu_gaussvar = 1.0;

typedef struct spu_packet_t packet_t;
struct spu_packet_t {
  int is_decoded;
  unsigned char *packet;
  int data_len;
  unsigned int palette[4];
  unsigned int alpha[4];
  unsigned int control_start;	/* index of start of control data */
  unsigned int current_nibble[2]; /* next data nibble (4 bits) to be
                                     processed (for RLE decoding) for
                                     even and odd lines */
  int deinterlace_oddness;	/* 0 or 1, index into current_nibble */
  unsigned int start_col;
  unsigned int start_row;
  unsigned int width, height, stride;
  unsigned int start_pts, end_pts;
  packet_t *next;
};

struct palette_crop_cache {
  int valid;
  uint32_t palette;
  int sx, sy, ex, ey;
  int result;
};

typedef struct {
  packet_t *queue_head;
  packet_t *queue_tail;
  unsigned int global_palette[16];
  unsigned int orig_frame_width, orig_frame_height;
  unsigned char* packet;
  size_t packet_reserve;	/* size of the memory pointed to by packet */
  unsigned int packet_offset;	/* end of the currently assembled fragment */
  unsigned int packet_size;	/* size of the packet once all fragments are assembled */
  int packet_pts;		/* PTS for this packet */
  unsigned int palette[4];
  unsigned int alpha[4];
  unsigned int cuspal[4];
  unsigned int custom;
  unsigned int now_pts;
  unsigned int start_pts, end_pts;
  unsigned int start_col;
  unsigned int start_row;
  unsigned int width, height, stride;
  size_t image_size;		/* Size of the image buffer */
  unsigned char *image;		/* Grayscale value */
  unsigned char *aimage;	/* Alpha value */
  unsigned int pal_start_col, pal_start_row;
  unsigned int pal_width, pal_height;
  unsigned char *pal_image;	/* palette entry value */
  unsigned int scaled_frame_width, scaled_frame_height;
  unsigned int scaled_start_col, scaled_start_row;
  unsigned int scaled_width, scaled_height, scaled_stride;
  size_t scaled_image_size;
  unsigned char *scaled_image;
  unsigned char *scaled_aimage;
  int auto_palette; /* 1 if we lack a palette and must use an heuristic. */
  int font_start_level;  /* Darkest value used for the computed font */
  const vo_functions_t *hw_spu;
  int spu_changed;
  unsigned int forced_subs_only;     /* flag: 0=display all subtitle, !0 display only forced subtitles */
  unsigned int is_forced_sub;         /* true if current subtitle is a forced subtitle */

  struct palette_crop_cache palette_crop_cache;
} spudec_handle_t;

static void spudec_queue_packet(spudec_handle_t *this, packet_t *packet)
{
  if (this->queue_head == NULL)
    this->queue_head = packet;
  else
    this->queue_tail->next = packet;
  this->queue_tail = packet;
}

static packet_t *spudec_dequeue_packet(spudec_handle_t *this)
{
  packet_t *retval = this->queue_head;

  this->queue_head = retval->next;
  if (this->queue_head == NULL)
    this->queue_tail = NULL;

  return retval;
}

static void spudec_free_packet(packet_t *packet)
{
  av_freep(&packet->packet);
  free(packet);
}

static inline unsigned int get_be16(const unsigned char *p)
{
  return (p[0] << 8) + p[1];
}

static inline unsigned int get_be24(const unsigned char *p)
{
  return (get_be16(p) << 8) + p[2];
}

static void next_line(packet_t *packet)
{
  if (packet->current_nibble[packet->deinterlace_oddness] % 2)
    packet->current_nibble[packet->deinterlace_oddness]++;
  packet->deinterlace_oddness = (packet->deinterlace_oddness + 1) % 2;
}

static inline unsigned char get_nibble(packet_t *packet)
{
  unsigned char nib;
  unsigned int *nibblep = packet->current_nibble + packet->deinterlace_oddness;
  if (*nibblep / 2 >= packet->control_start) {
    mp_msg(MSGT_SPUDEC,MSGL_WARN, "SPUdec: ERROR: get_nibble past end of packet\n");
    return 0;
  }
  nib = packet->packet[*nibblep / 2];
  if (*nibblep % 2)
    nib &= 0xf;
  else
    nib >>= 4;
  ++*nibblep;
  return nib;
}

/* Cut the sub to visible part */
static inline void spudec_cut_image(spudec_handle_t *this)
{
  unsigned int fy, ly;
  unsigned int first_y, last_y;

  if (this->stride == 0 || this->height == 0) {
    return;
  }

  for (fy = 0; fy < this->image_size && !this->aimage[fy]; fy++);
  for (ly = this->stride * this->height-1; ly && !this->aimage[ly]; ly--);
  first_y = fy / this->stride;
  last_y = ly / this->stride;
  //printf("first_y: %d, last_y: %d\n", first_y, last_y);
  this->start_row += first_y;

  // Some subtitles trigger this condition
  if (last_y + 1 > first_y ) {
	  this->height = last_y - first_y +1;
  } else {
	  this->height = 0;
	  return;
  }

//  printf("new h %d new start %d (sz %d st %d)---\n\n", this->height, this->start_row, this->image_size, this->stride);

  if (first_y > 0) {
    memmove(this->image,  this->image  + this->stride * first_y, this->stride * this->height);
    memmove(this->aimage, this->aimage + this->stride * first_y, this->stride * this->height);
  }
}


static int spudec_alloc_image(spudec_handle_t *this, int stride, int height)
{
  if (this->width > stride) // just a safeguard
    this->width = stride;
  this->stride = stride;
  this->height = height;
  if (this->image_size < this->stride * this->height) {
    if (this->image != NULL) {
      av_freep(&this->image);
      this->image = NULL;
      av_freep(&this->pal_image);
      this->pal_image = NULL;
      this->image_size = 0;
      this->pal_width = this->pal_height  = 0;
    }
    this->image = av_malloc(2 * this->stride * this->height);
    if (this->image) {
      this->image_size = this->stride * this->height;
      this->aimage = this->image + this->image_size;
      // use stride here as well to simplify reallocation checks
      this->pal_image = av_malloc(this->stride * this->height);
    }
  }
  return this->image != NULL;
}

/**
 * \param pal palette in MPlayer-style gray-alpha values, i.e.
 *            alpha == 0 means transparent, 1 fully opaque,
 *            gray value <= 256 - alpha.
 */
static void pal2gray_alpha(const uint16_t *pal,
                           const uint8_t *src, int src_stride,
                           uint8_t *dst, uint8_t *dsta,
                           int dst_stride, int w, int h, int skip_stride)
{
  int x, y;
  for (y = 0; y < h; y++) {
    for (x = 0; x < w; x++) {
      uint16_t pixel = pal[src[x]];
      *dst++  = pixel;
      *dsta++ = pixel >> 8;
    }
    if (skip_stride) {
      dst += dst_stride - w;
      dsta += dst_stride - w;
    } else for (; x < dst_stride; x++)
      *dsta++ = *dst++ = 0;
    src += src_stride;
  }
}

static int apply_palette_crop(spudec_handle_t *this,
                              unsigned crop_x, unsigned crop_y,
                              unsigned crop_w, unsigned crop_h)
{
  int i;
  uint8_t *src;
  uint16_t pal[4];
  unsigned stride = (crop_w + 15) & ~15;
  int ret = 1;
  if (crop_x > this->pal_width || crop_y > this->pal_height ||
      crop_w > this->pal_width - crop_x || crop_h > this->pal_width - crop_y ||
      crop_w > 0x8000 || crop_h > 0x8000 ||
      stride * crop_h  > this->image_size) {
    // this might be an actual error or just signal that
    // the highlight should be removed.
    this->width = 0;
    this->height = 0;
    ret = 0;
    goto out;
  }
  for (i = 0; i < 4; ++i) {
    int color;
    int alpha = this->alpha[i];
    // extend 4 -> 8 bit
    alpha |= alpha << 4;
    if (this->custom && (this->cuspal[i] >> 31) != 0)
      alpha = 0;
    color = this->custom ? this->cuspal[i] :
            this->global_palette[this->palette[i]];
    color = (color >> 16) & 0xff;
    // convert to MPlayer-style gray/alpha palette
    color = FFMIN(color, alpha);
    pal[i] = (-alpha << 8) | color;
  }
  src = this->pal_image + crop_y * this->pal_width + crop_x;
  pal2gray_alpha(pal, src, this->pal_width,
                 this->image, this->aimage, stride,
                 crop_w, crop_h, 0);
  this->width  = crop_w;
  this->height = crop_h;
  this->stride = stride;
  this->start_col = this->pal_start_col + crop_x;
  this->start_row = this->pal_start_row + crop_y;
  spudec_cut_image(this);

out:
  // reset scaled image
  this->scaled_frame_width = 0;
  this->scaled_frame_height = 0;
  this->palette_crop_cache.valid = 0;
  return ret;
}

int spudec_apply_palette_crop(void *this, uint32_t palette,
                              int sx, int sy, int ex, int ey)
{
  spudec_handle_t *spu = this;
  struct palette_crop_cache *c = &spu->palette_crop_cache;
  if (c->valid && c->palette == palette &&
      c->sx == sx && c->sy == sy && c->ex == ex && c->ey == ey)
    return c->result;
  spu->palette[0] = (palette >> 28) & 0xf;
  spu->palette[1] = (palette >> 24) & 0xf;
  spu->palette[2] = (palette >> 20) & 0xf;
  spu->palette[3] = (palette >> 16) & 0xf;
  spu->alpha[0]   = (palette >> 12) & 0xf;
  spu->alpha[1]   = (palette >>  8) & 0xf;
  spu->alpha[2]   = (palette >>  4) & 0xf;
  spu->alpha[3]   =  palette        & 0xf;
  spu->spu_changed = 1;
  c->result = apply_palette_crop(spu,
                                 sx - spu->pal_start_col, sy - spu->pal_start_row,
                                 ex - sx, ey - sy);
  c->palette = palette;
  c->sx = sx; c->sy = sy;
  c->ex = ex; c->ey = ey;
  c->valid = 1;
  return c->result;
}

static void spudec_process_data(spudec_handle_t *this, packet_t *packet)
{
  unsigned int limit0, limit1, x, y;
  uint8_t *dst;

  if (!spudec_alloc_image(this, packet->stride, packet->height))
    return;

  this->pal_start_col = packet->start_col;
  this->pal_start_row = packet->start_row;
  this->pal_height = packet->height;
  this->pal_width  = packet->width;
  this->stride = packet->stride;
  memcpy(this->palette, packet->palette, sizeof(this->palette));
  memcpy(this->alpha,   packet->alpha,   sizeof(this->alpha));

  limit0 = packet->current_nibble[1];
  limit1 = 2*packet->control_start;
  if (packet->current_nibble[0] > packet->current_nibble[1]) {
    limit0 = limit1;
    limit1 = packet->current_nibble[0];
  }
  x = 0;
  y = 0;
  dst = this->pal_image;
  while (packet->current_nibble[0] < limit0
	 && packet->current_nibble[1] < limit1
	 && y < this->pal_height) {
    unsigned int len, color;
    unsigned int rle = 0;
    rle = get_nibble(packet);
    if (rle < 0x04) {
      if (rle == 0) {
	rle = (rle << 4) | get_nibble(packet);
	if (rle < 0x04)
	  rle = (rle << 4) | get_nibble(packet);
      }
      rle = (rle << 4) | get_nibble(packet);
    }
    color = 3 - (rle & 0x3);
    len = rle >> 2;
    x += len;
    if (len == 0 || x >= this->pal_width) {
      len += this->pal_width - x;
      next_line(packet);
      x = 0;
      ++y;
    }
    memset(dst, color, len);
    dst += len;
  }
  apply_palette_crop(this, 0, 0, this->pal_width, this->pal_height);
}


/*
  This function tries to create a usable palette.
  It determines how many non-transparent colors are used, and assigns different
gray scale values to each color.
  I tested it with four streams and even got something readable. Half of the
times I got black characters with white around and half the reverse.
*/
static void compute_palette(spudec_handle_t *this, packet_t *packet)
{
  int used[16],i,cused,start,step,color;

  memset(used, 0, sizeof(used));
  for (i=0; i<4; i++)
    if (packet->alpha[i]) /* !Transparent? */
       used[packet->palette[i]] = 1;
  for (cused=0, i=0; i<16; i++)
    if (used[i]) cused++;
  if (!cused) return;
  if (cused == 1) {
    start = 0x80;
    step = 0;
  } else {
    start = this->font_start_level;
    step = (0xF0-this->font_start_level)/(cused-1);
  }
  memset(used, 0, sizeof(used));
  for (i=0; i<4; i++) {
    color = packet->palette[i];
    if (packet->alpha[i] && !used[color]) { /* not assigned? */
       used[color] = 1;
       this->global_palette[color] = start<<16;
       start += step;
    }
  }
}

static void spudec_process_control(spudec_handle_t *this, int pts100)
{
  int a,b,c,d; /* Temporary vars */
  unsigned int date, type;
  unsigned int off;
  unsigned int start_off = 0;
  unsigned int next_off;
  unsigned int start_pts = 0;
  unsigned int end_pts = 0;
  unsigned int current_nibble[2] = {0, 0};
  unsigned int control_start;
  unsigned int display = 0;
  unsigned int start_col = 0;
  unsigned int end_col = 0;
  unsigned int start_row = 0;
  unsigned int end_row = 0;
  unsigned int width = 0;
  unsigned int height = 0;
  unsigned int stride = 0;

  control_start = get_be16(this->packet + 2);
  next_off = control_start;
  while (start_off != next_off) {
    start_off = next_off;
    date = get_be16(this->packet + start_off) * 1024;
    next_off = get_be16(this->packet + start_off + 2);
    mp_msg(MSGT_SPUDEC,MSGL_DBG2, "date=%d\n", date);
    off = start_off + 4;
    for (type = this->packet[off++]; type != 0xff; type = this->packet[off++]) {
      mp_msg(MSGT_SPUDEC,MSGL_DBG2, "cmd=%d  ",type);
      switch(type) {
      case 0x00:
	/* Menu ID, 1 byte */
	mp_msg(MSGT_SPUDEC,MSGL_DBG2,"Menu ID\n");
        /* shouldn't a Menu ID type force display start? */
	start_pts = pts100 < 0 && -pts100 >= date ? 0 : pts100 + date;
	end_pts = UINT_MAX;
	display = 1;
	this->is_forced_sub=~0; // current subtitle is forced
	break;
      case 0x01:
	/* Start display */
	mp_msg(MSGT_SPUDEC,MSGL_DBG2,"Start display!\n");
	start_pts = pts100 < 0 && -pts100 >= date ? 0 : pts100 + date;
	end_pts = UINT_MAX;
	display = 1;
	this->is_forced_sub=0;
	break;
      case 0x02:
	/* Stop display */
	mp_msg(MSGT_SPUDEC,MSGL_DBG2,"Stop display!\n");
	end_pts = pts100 < 0 && -pts100 >= date ? 0 : pts100 + date;
	break;
      case 0x03:
	/* Palette */
	this->palette[0] = this->packet[off] >> 4;
	this->palette[1] = this->packet[off] & 0xf;
	this->palette[2] = this->packet[off + 1] >> 4;
	this->palette[3] = this->packet[off + 1] & 0xf;
	mp_msg(MSGT_SPUDEC,MSGL_DBG2,"Palette %d, %d, %d, %d\n",
	       this->palette[0], this->palette[1], this->palette[2], this->palette[3]);
	off+=2;
	break;
      case 0x04:
	/* Alpha */
	a = this->packet[off] >> 4;
	b = this->packet[off] & 0xf;
	c = this->packet[off + 1] >> 4;
	d = this->packet[off + 1] & 0xf;
	// Note: some DVDs change these values to create a fade-in/fade-out effect
	// We can not handle this, so just keep the highest value during the display time.
	if (display) {
		a = FFMAX(a, this->alpha[0]);
		b = FFMAX(b, this->alpha[1]);
		c = FFMAX(c, this->alpha[2]);
		d = FFMAX(d, this->alpha[3]);
	}
	this->alpha[0] = a;
	this->alpha[1] = b;
	this->alpha[2] = c;
	this->alpha[3] = d;
	mp_msg(MSGT_SPUDEC,MSGL_DBG2,"Alpha %d, %d, %d, %d\n",
	       this->alpha[0], this->alpha[1], this->alpha[2], this->alpha[3]);
	off+=2;
	break;
      case 0x05:
	/* Co-ords */
	a = get_be24(this->packet + off);
	b = get_be24(this->packet + off + 3);
	start_col = a >> 12;
	end_col = a & 0xfff;
	width = (end_col < start_col) ? 0 : end_col - start_col + 1;
	stride = (width + 15) & ~15; /* Kludge: draw_alpha needs width multiple of 16 */
	start_row = b >> 12;
	end_row = b & 0xfff;
	height = (end_row < start_row) ? 0 : end_row - start_row /* + 1 */;
	mp_msg(MSGT_SPUDEC,MSGL_DBG2,"Coords  col: %d - %d  row: %d - %d  (%dx%d)\n",
	       start_col, end_col, start_row, end_row,
	       width, height);
	off+=6;
	break;
      case 0x06:
	/* Graphic lines */
	current_nibble[0] = 2 * get_be16(this->packet + off);
	current_nibble[1] = 2 * get_be16(this->packet + off + 2);
	mp_msg(MSGT_SPUDEC,MSGL_DBG2,"Graphic offset 1: %d  offset 2: %d\n",
	       current_nibble[0] / 2, current_nibble[1] / 2);
	off+=4;
	break;
      default:
	mp_msg(MSGT_SPUDEC,MSGL_WARN,"spudec: Error determining control type 0x%02x.  Skipping %d bytes.\n",
	       type, next_off - off);
	goto next_control;
      }
    }
  next_control:
    if (!display)
      continue;
    if (end_pts == UINT_MAX && start_off != next_off) {
      end_pts = get_be16(this->packet + next_off) * 1024;
      end_pts = 1 - pts100 >= end_pts ? 0 : pts100 + end_pts - 1;
    }
    if (end_pts > 0) {
      packet_t *packet = calloc(1, sizeof(packet_t));
      int i;
      packet->start_pts = start_pts;
      packet->end_pts = end_pts;
      packet->current_nibble[0] = current_nibble[0];
      packet->current_nibble[1] = current_nibble[1];
      packet->start_row = start_row;
      packet->start_col = start_col;
      packet->width = width;
      packet->height = height;
      packet->stride = stride;
      packet->control_start = control_start;
      for (i=0; i<4; i++) {
	packet->alpha[i] = this->alpha[i];
	packet->palette[i] = this->palette[i];
      }
      packet->packet = av_malloc(this->packet_size);
      memcpy(packet->packet, this->packet, this->packet_size);
      spudec_queue_packet(this, packet);
    }
  }
}

static void spudec_decode(spudec_handle_t *this, int pts100)
{
  if (!this->hw_spu)
    spudec_process_control(this, pts100);
  else if (pts100 >= 0) {
    static vo_mpegpes_t packet = { NULL, 0, 0x20, 0 };
    static vo_mpegpes_t *pkg=&packet;
    packet.data = this->packet;
    packet.size = this->packet_size;
    packet.timestamp = pts100;
    this->hw_spu->draw_frame((uint8_t**)&pkg);
  }
}

int spudec_changed(void * this)
{
    spudec_handle_t * spu = this;
    return spu->spu_changed || spu->now_pts > spu->end_pts;
}

void spudec_assemble(void *this, unsigned char *packet, unsigned int len, int pts100)
{
  spudec_handle_t *spu = this;
//  spudec_heartbeat(this, pts100);
  if (len < 2) {
      mp_msg(MSGT_SPUDEC,MSGL_WARN,"SPUasm: packet too short\n");
      return;
  }
  spu->packet_pts = pts100;
  if (spu->packet_offset == 0) {
    unsigned int len2 = get_be16(packet);
    // Start new fragment
    if (spu->packet_reserve < len2) {
      free(spu->packet);
      spu->packet = malloc(len2);
      spu->packet_reserve = spu->packet != NULL ? len2 : 0;
    }
    if (spu->packet != NULL) {
      spu->packet_size = len2;
      if (len > len2) {
	mp_msg(MSGT_SPUDEC,MSGL_WARN,"SPUasm: invalid frag len / len2: %d / %d \n", len, len2);
	return;
      }
      memcpy(spu->packet, packet, len);
      spu->packet_offset = len;
      spu->packet_pts = pts100;
    }
  } else {
    // Continue current fragment
    if (spu->packet_size < spu->packet_offset + len){
      mp_msg(MSGT_SPUDEC,MSGL_WARN,"SPUasm: invalid fragment\n");
      spu->packet_size = spu->packet_offset = 0;
      return;
    } else {
      memcpy(spu->packet + spu->packet_offset, packet, len);
      spu->packet_offset += len;
    }
  }
#if 1
  // check if we have a complete packet (unfortunatelly packet_size is bad
  // for some disks)
  // [cb] packet_size is padded to be even -> may be one byte too long
  if ((spu->packet_offset == spu->packet_size) ||
      ((spu->packet_offset + 1) == spu->packet_size)){
    unsigned int x=0,y;
    while(x+4<=spu->packet_offset){
      y=get_be16(spu->packet+x+2); // next control pointer
      mp_msg(MSGT_SPUDEC,MSGL_DBG2,"SPUtest: x=%d y=%d off=%d size=%d\n",x,y,spu->packet_offset,spu->packet_size);
      if(x>=4 && x==y){		// if it points to self - we're done!
        // we got it!
	mp_msg(MSGT_SPUDEC,MSGL_DBG2,"SPUgot: off=%d  size=%d \n",spu->packet_offset,spu->packet_size);
	spudec_decode(spu, pts100);
	spu->packet_offset = 0;
	break;
      }
      if(y<=x || y>=spu->packet_size){ // invalid?
	mp_msg(MSGT_SPUDEC,MSGL_WARN,"SPUtest: broken packet!!!!! y=%d < x=%d\n",y,x);
        spu->packet_size = spu->packet_offset = 0;
        break;
      }
      x=y;
    }
    // [cb] packet is done; start new packet
    spu->packet_offset = 0;
  }
#else
  if (spu->packet_offset == spu->packet_size) {
    spudec_decode(spu, pts100);
    spu->packet_offset = 0;
  }
#endif
}

void spudec_reset(void *this)	// called after seek
{
  spudec_handle_t *spu = this;
  while (spu->queue_head)
    spudec_free_packet(spudec_dequeue_packet(spu));
  spu->now_pts = 0;
  spu->end_pts = 0;
  spu->packet_size = spu->packet_offset = 0;
}

void spudec_heartbeat(void *this, unsigned int pts100)
{
  spudec_handle_t *spu = this;
  spu->now_pts = pts100;

  // TODO: detect and handle broken timestamps (e.g. due to wrapping)
  while (spu->queue_head != NULL && pts100 >= spu->queue_head->start_pts) {
    packet_t *packet = spudec_dequeue_packet(spu);
    spu->start_pts = packet->start_pts;
    spu->end_pts = packet->end_pts;
    if (packet->is_decoded) {
      av_freep(&spu->image);
      spu->image_size = packet->data_len;
      spu->image      = packet->packet;
      spu->aimage     = packet->packet + packet->stride * packet->height;
      packet->packet  = NULL;
      spu->width      = packet->width;
      spu->height     = packet->height;
      spu->stride     = packet->stride;
      spu->start_col  = packet->start_col;
      spu->start_row  = packet->start_row;

      // reset scaled image
      spu->scaled_frame_width = 0;
      spu->scaled_frame_height = 0;
    } else {
      if (spu->auto_palette)
        compute_palette(spu, packet);
      spudec_process_data(spu, packet);
    }
    spudec_free_packet(packet);
    spu->spu_changed = 1;
  }
}

int spudec_visible(void *this){
    spudec_handle_t *spu = this;
    int ret=(spu->start_pts <= spu->now_pts &&
	     spu->now_pts < spu->end_pts &&
	     spu->height > 0);
//    printf("spu visible: %d  \n",ret);
    return ret;
}

void spudec_set_forced_subs_only(void * const this, const unsigned int flag)
{
  if(this){
      ((spudec_handle_t *)this)->forced_subs_only=flag;
      mp_msg(MSGT_SPUDEC,MSGL_DBG2,"SPU: Display only forced subs now %s\n", flag ? "enabled": "disabled");
  }
}

void spudec_draw(void *this, void (*draw_alpha)(int x0,int y0, int w,int h, unsigned char* src, unsigned char *srca, int stride))
{
    spudec_handle_t *spu = this;
    if (spudec_visible(spu))
    {
	draw_alpha(spu->start_col, spu->start_row, spu->width, spu->height,
		   spu->image, spu->aimage, spu->stride);
	spu->spu_changed = 0;
    }
}

static void validate_dimensions(spudec_handle_t *spu, unsigned dxs, unsigned dys)
{
    if (spu->orig_frame_width == 0 || spu->orig_frame_height == 0) {
        spu->width  = FFMIN(spu->width,  dxs);
        spu->height = FFMIN(spu->height, dys);
        spu->start_col = FFMIN(spu->start_col, dxs - spu->width);
        spu->start_row = FFMIN(spu->start_row, dys - spu->height);
        return;
    }
    spu->orig_frame_width  = FFMAX(spu->orig_frame_width,  spu->start_col + spu->width);
    spu->orig_frame_height = FFMAX(spu->orig_frame_height, spu->start_row + spu->height);
}

/* calc the bbox for spudec subs */
void spudec_calc_bbox(void *me, unsigned int dxs, unsigned int dys, unsigned int* bbox)
{
  spudec_handle_t *spu = me;
  validate_dimensions(spu, dxs, dys);
  if (spu->orig_frame_width == 0 || spu->orig_frame_height == 0
  || (spu->orig_frame_width == dxs && spu->orig_frame_height == dys)) {
    // unscaled
    bbox[0] = spu->start_col;
    bbox[1] = spu->start_col + spu->width;
    bbox[2] = spu->start_row;
    bbox[3] = spu->start_row + spu->height;
  }
  else {
    // scaled
    unsigned int scalex = 0x100 * dxs / spu->orig_frame_width;
    unsigned int scaley = 0x100 * dys / spu->orig_frame_height;
    bbox[0] = spu->start_col * scalex / 0x100;
    bbox[1] = spu->start_col * scalex / 0x100 + spu->width * scalex / 0x100;
    switch (spu_alignment) {
    case 0:
      bbox[3] = dys*sub_pos/100 + spu->height * scaley / 0x100;
      if (bbox[3] > dys) bbox[3] = dys;
      bbox[2] = bbox[3] - spu->height * scaley / 0x100;
      break;
    case 1:
      if (sub_pos < 50) {
        bbox[2] = dys*sub_pos/100 - spu->height * scaley / 0x200;
        bbox[3] = bbox[2] + spu->height;
      } else {
        bbox[3] = dys*sub_pos/100 + spu->height * scaley / 0x200;
        if (bbox[3] > dys) bbox[3] = dys;
        bbox[2] = bbox[3] - spu->height * scaley / 0x100;
      }
      break;
    case 2:
      bbox[2] = dys*sub_pos/100 - spu->height * scaley / 0x100;
      bbox[3] = bbox[2] + spu->height;
      break;
    default: /* -1 */
      bbox[2] = spu->start_row * scaley / 0x100;
      bbox[3] = spu->start_row * scaley / 0x100 + spu->height * scaley / 0x100;
      break;
    }
  }
}
/* transform mplayer's alpha value into an opacity value that is linear */
static inline int canon_alpha(int alpha)
{
  return (uint8_t)-alpha;
}

typedef struct {
  unsigned position;
  unsigned left_up;
  unsigned right_down;
}scale_pixel;


static void scale_table(unsigned int start_src, unsigned int start_tar, unsigned int end_src, unsigned int end_tar, scale_pixel * table)
{
  unsigned int t;
  unsigned int delta_src = end_src - start_src;
  unsigned int delta_tar = end_tar - start_tar;
  int src = 0;
  int src_step;
  if (delta_src == 0 || delta_tar == 0) {
    return;
  }
  src_step = (delta_src << 16) / delta_tar >>1;
  for (t = 0; t<=delta_tar; src += (src_step << 1), t++){
    table[t].position= FFMIN(src >> 16, end_src - 1);
    table[t].right_down = src & 0xffff;
    table[t].left_up = 0x10000 - table[t].right_down;
  }
}

/* bilinear scale, similar to vobsub's code */
static void scale_image(int x, int y, scale_pixel* table_x, scale_pixel* table_y, spudec_handle_t * spu)
{
  int alpha[4];
  int color[4];
  unsigned int scale[4];
  int base = table_y[y].position * spu->stride + table_x[x].position;
  int scaled = y * spu->scaled_stride + x;
  alpha[0] = canon_alpha(spu->aimage[base]);
  alpha[1] = canon_alpha(spu->aimage[base + 1]);
  alpha[2] = canon_alpha(spu->aimage[base + spu->stride]);
  alpha[3] = canon_alpha(spu->aimage[base + spu->stride + 1]);
  color[0] = spu->image[base];
  color[1] = spu->image[base + 1];
  color[2] = spu->image[base + spu->stride];
  color[3] = spu->image[base + spu->stride + 1];
  scale[0] = (table_x[x].left_up * table_y[y].left_up >> 16) * alpha[0];
  if (table_y[y].left_up == 0x10000) // necessary to avoid overflow-case
    scale[0] = table_x[x].left_up * alpha[0];
  scale[1] = (table_x[x].right_down * table_y[y].left_up >>16) * alpha[1];
  scale[2] = (table_x[x].left_up * table_y[y].right_down >> 16) * alpha[2];
  scale[3] = (table_x[x].right_down * table_y[y].right_down >> 16) * alpha[3];
  spu->scaled_image[scaled] = (color[0] * scale[0] + color[1] * scale[1] + color[2] * scale[2] + color[3] * scale[3])>>24;
  spu->scaled_aimage[scaled] = (scale[0] + scale[1] + scale[2] + scale[3]) >> 16;
  if (spu->scaled_aimage[scaled]){
    // ensure that MPlayer's simplified alpha-blending can not overflow
    spu->scaled_image[scaled] = FFMIN(spu->scaled_image[scaled], spu->scaled_aimage[scaled]);
    // convert to MPlayer-style alpha
    spu->scaled_aimage[scaled] = -spu->scaled_aimage[scaled];
  }
}

static void sws_spu_image(unsigned char *d1, unsigned char *d2, int dw, int dh,
                          int ds, const unsigned char* s1, unsigned char* s2,
                          int sw, int sh, int ss)
{
	struct SwsContext *ctx;
	static SwsFilter filter;
	static int firsttime = 1;
	static float oldvar;
	int i;

	if (!firsttime && oldvar != spu_gaussvar) sws_freeVec(filter.lumH);
	if (firsttime) {
		filter.lumH = filter.lumV =
			filter.chrH = filter.chrV = sws_getGaussianVec(spu_gaussvar, 3.0);
		sws_normalizeVec(filter.lumH, 1.0);
		firsttime = 0;
		oldvar = spu_gaussvar;
	}

	ctx=sws_getContext(sw, sh, AV_PIX_FMT_GRAY8, dw, dh, AV_PIX_FMT_GRAY8, SWS_GAUSS, &filter, NULL, NULL);
	sws_scale(ctx,&s1,&ss,0,sh,&d1,&ds);
	for (i=ss*sh-1; i>=0; i--) s2[i] = -s2[i];
	sws_scale(ctx,&s2,&ss,0,sh,&d2,&ds);
	for (i=ds*dh-1; i>=0; i--) d2[i] = -d2[i];
	sws_freeContext(ctx);
}

void spudec_draw_scaled(void *me, unsigned int dxs, unsigned int dys, void (*draw_alpha)(int x0,int y0, int w,int h, unsigned char* src, unsigned char *srca, int stride))
{
  spudec_handle_t *spu = me;
  scale_pixel *table_x;
  scale_pixel *table_y;

  if (spudec_visible(spu)) {

    // check if only forced subtitles are requested
    if( (spu->forced_subs_only) && !(spu->is_forced_sub) ){
	return;
    }

    validate_dimensions(spu, dxs, dys);
    if (!(spu_aamode&16) && (spu->orig_frame_width == 0 || spu->orig_frame_height == 0
	|| (spu->orig_frame_width == dxs && spu->orig_frame_height == dys))) {
      spudec_draw(spu, draw_alpha);
    }
    else {
      if (spu->scaled_frame_width != dxs || spu->scaled_frame_height != dys) {	/* Resizing is needed */
	/* scaled_x = scalex * x / 0x100
	   scaled_y = scaley * y / 0x100
	   order of operations is important because of rounding. */
	unsigned int scalex = 0x100 * dxs / spu->orig_frame_width;
	unsigned int scaley = 0x100 * dys / spu->orig_frame_height;
	spu->scaled_start_col = spu->start_col * scalex / 0x100;
	spu->scaled_start_row = spu->start_row * scaley / 0x100;
	spu->scaled_width = spu->width * scalex / 0x100;
	spu->scaled_height = spu->height * scaley / 0x100;
	/* Kludge: draw_alpha needs width multiple of 16 */
	spu->scaled_stride = (spu->scaled_width + 15) & ~15;
	if (spu->scaled_image_size < spu->scaled_stride * spu->scaled_height) {
	  if (spu->scaled_image) {
	    av_freep(&spu->scaled_image);
	    spu->scaled_image_size = 0;
	  }
	  spu->scaled_image = av_malloc(2 * spu->scaled_stride * spu->scaled_height);
	  if (spu->scaled_image) {
	    spu->scaled_image_size = spu->scaled_stride * spu->scaled_height;
	    spu->scaled_aimage = spu->scaled_image + spu->scaled_image_size;
	  }
	}
	if (spu->scaled_image) {
	  unsigned int x, y;
	  // needs to be 0-initialized because draw_alpha draws always a
	  // multiple of 8 pixels. TODO: optimize
	  if (spu->scaled_width & 7)
	    memset(spu->scaled_image, 0, 2 * spu->scaled_image_size);
	  if (spu->scaled_width <= 1 || spu->scaled_height <= 1) {
	    goto nothing_to_do;
	  }
	  switch(spu_aamode&15) {
	  case 4:
	  sws_spu_image(spu->scaled_image, spu->scaled_aimage,
		  spu->scaled_width, spu->scaled_height, spu->scaled_stride,
		  spu->image, spu->aimage, spu->width, spu->height, spu->stride);
	  break;
	  case 3:
	  table_x = calloc(spu->scaled_width, sizeof(scale_pixel));
	  table_y = calloc(spu->scaled_height, sizeof(scale_pixel));
	  if (!table_x || !table_y) {
	    mp_msg(MSGT_SPUDEC, MSGL_FATAL, "Fatal: spudec_draw_scaled: calloc failed\n");
	    free(table_x);
	    table_x = NULL;
	    free(table_y);
	    table_y = NULL;
	    break;
	  }
	  scale_table(0, 0, spu->width - 1, spu->scaled_width - 1, table_x);
	  scale_table(0, 0, spu->height - 1, spu->scaled_height - 1, table_y);
	  for (y = 0; y < spu->scaled_height; y++)
	    for (x = 0; x < spu->scaled_width; x++)
	      scale_image(x, y, table_x, table_y, spu);
	  free(table_x);
	  free(table_y);
	  break;
	  case 0:
	  /* no antialiasing */
	  for (y = 0; y < spu->scaled_height; ++y) {
	    int unscaled_y = y * 0x100 / scaley;
	    int strides = spu->stride * unscaled_y;
	    int scaled_strides = spu->scaled_stride * y;
	    for (x = 0; x < spu->scaled_width; ++x) {
	      int unscaled_x = x * 0x100 / scalex;
	      spu->scaled_image[scaled_strides + x] = spu->image[strides + unscaled_x];
	      spu->scaled_aimage[scaled_strides + x] = spu->aimage[strides + unscaled_x];
	    }
	  }
	  break;
	  case 1:
	  {
	    /* Intermediate antialiasing. */
	    for (y = 0; y < spu->scaled_height; ++y) {
	      const unsigned int unscaled_top = y * spu->orig_frame_height / dys;
	      unsigned int unscaled_bottom = (y + 1) * spu->orig_frame_height / dys;
	      if (unscaled_bottom >= spu->height)
		unscaled_bottom = spu->height - 1;
	      for (x = 0; x < spu->scaled_width; ++x) {
		const unsigned int unscaled_left = x * spu->orig_frame_width / dxs;
		unsigned int unscaled_right = (x + 1) * spu->orig_frame_width / dxs;
		unsigned int color = 0;
		unsigned int alpha = 0;
		unsigned int walkx, walky;
		unsigned int base, tmp;
		if (unscaled_right >= spu->width)
		  unscaled_right = spu->width - 1;
		for (walky = unscaled_top; walky <= unscaled_bottom; ++walky)
		  for (walkx = unscaled_left; walkx <= unscaled_right; ++walkx) {
		    base = walky * spu->stride + walkx;
		    tmp = canon_alpha(spu->aimage[base]);
		    alpha += tmp;
		    color += tmp * spu->image[base];
		  }
		base = y * spu->scaled_stride + x;
		spu->scaled_image[base] = alpha ? color / alpha : 0;
		spu->scaled_aimage[base] =
		  alpha * (1 + unscaled_bottom - unscaled_top) * (1 + unscaled_right - unscaled_left);
		/* spu->scaled_aimage[base] =
		  alpha * dxs * dys / spu->orig_frame_width / spu->orig_frame_height; */
		if (spu->scaled_aimage[base]) {
		  spu->scaled_aimage[base] = 256 - spu->scaled_aimage[base];
		  if (spu->scaled_aimage[base] + spu->scaled_image[base] > 255)
		    spu->scaled_image[base] = 256 - spu->scaled_aimage[base];
		}
	      }
	    }
	  }
	  break;
	  case 2:
	  {
	    /* Best antialiasing.  Very slow. */
	    /* Any pixel (x, y) represents pixels from the original
	       rectangular region comprised between the columns
	       unscaled_y and unscaled_y + 0x100 / scaley and the rows
	       unscaled_x and unscaled_x + 0x100 / scalex

	       The original rectangular region that the scaled pixel
	       represents is cut in 9 rectangular areas like this:

	       +---+-----------------+---+
	       | 1 |        2        | 3 |
	       +---+-----------------+---+
	       |   |                 |   |
	       | 4 |        5        | 6 |
	       |   |                 |   |
	       +---+-----------------+---+
	       | 7 |        8        | 9 |
	       +---+-----------------+---+

	       The width of the left column is at most one pixel and
	       it is never null and its right column is at a pixel
	       boundary.  The height of the top row is at most one
	       pixel it is never null and its bottom row is at a
	       pixel boundary. The width and height of region 5 are
	       integral values.  The width of the right column is
	       what remains and is less than one pixel.  The height
	       of the bottom row is what remains and is less than
	       one pixel.

	       The row above 1, 2, 3 is unscaled_y.  The row between
	       1, 2, 3 and 4, 5, 6 is top_low_row.  The row between 4,
	       5, 6 and 7, 8, 9 is (unsigned int)unscaled_y_bottom.
	       The row beneath 7, 8, 9 is unscaled_y_bottom.

	       The column left of 1, 4, 7 is unscaled_x.  The column
	       between 1, 4, 7 and 2, 5, 8 is left_right_column.  The
	       column between 2, 5, 8 and 3, 6, 9 is (unsigned
	       int)unscaled_x_right.  The column right of 3, 6, 9 is
	       unscaled_x_right. */
	    const double inv_scalex = (double) 0x100 / scalex;
	    const double inv_scaley = (double) 0x100 / scaley;
	    for (y = 0; y < spu->scaled_height; ++y) {
	      const double unscaled_y = y * inv_scaley;
	      const double unscaled_y_bottom = unscaled_y + inv_scaley;
	      const unsigned int top_low_row = FFMIN(unscaled_y_bottom, unscaled_y + 1.0);
	      const double top = top_low_row - unscaled_y;
	      const unsigned int height = unscaled_y_bottom > top_low_row
		? (unsigned int) unscaled_y_bottom - top_low_row
		: 0;
	      const double bottom = unscaled_y_bottom > top_low_row
		? unscaled_y_bottom - floor(unscaled_y_bottom)
		: 0.0;
	      for (x = 0; x < spu->scaled_width; ++x) {
		const double unscaled_x = x * inv_scalex;
		const double unscaled_x_right = unscaled_x + inv_scalex;
		const unsigned int left_right_column = FFMIN(unscaled_x_right, unscaled_x + 1.0);
		const double left = left_right_column - unscaled_x;
		const unsigned int width = unscaled_x_right > left_right_column
		  ? (unsigned int) unscaled_x_right - left_right_column
		  : 0;
		const double right = unscaled_x_right > left_right_column
		  ? unscaled_x_right - floor(unscaled_x_right)
		  : 0.0;
		double color = 0.0;
		double alpha = 0.0;
		double tmp;
		unsigned int base;
		/* Now use these informations to compute a good alpha,
                   and lightness.  The sum is on each of the 9
                   region's surface and alpha and lightness.

		  transformed alpha = sum(surface * alpha) / sum(surface)
		  transformed color = sum(surface * alpha * color) / sum(surface * alpha)
		*/
		/* 1: top left part */
		base = spu->stride * (unsigned int) unscaled_y;
		tmp = left * top * canon_alpha(spu->aimage[base + (unsigned int) unscaled_x]);
		alpha += tmp;
		color += tmp * spu->image[base + (unsigned int) unscaled_x];
		/* 2: top center part */
		if (width > 0) {
		  unsigned int walkx;
		  for (walkx = left_right_column; walkx < (unsigned int) unscaled_x_right; ++walkx) {
		    base = spu->stride * (unsigned int) unscaled_y + walkx;
		    tmp = /* 1.0 * */ top * canon_alpha(spu->aimage[base]);
		    alpha += tmp;
		    color += tmp * spu->image[base];
		  }
		}
		/* 3: top right part */
		if (right > 0.0) {
		  base = spu->stride * (unsigned int) unscaled_y + (unsigned int) unscaled_x_right;
		  tmp = right * top * canon_alpha(spu->aimage[base]);
		  alpha += tmp;
		  color += tmp * spu->image[base];
		}
		/* 4: center left part */
		if (height > 0) {
		  unsigned int walky;
		  for (walky = top_low_row; walky < (unsigned int) unscaled_y_bottom; ++walky) {
		    base = spu->stride * walky + (unsigned int) unscaled_x;
		    tmp = left /* * 1.0 */ * canon_alpha(spu->aimage[base]);
		    alpha += tmp;
		    color += tmp * spu->image[base];
		  }
		}
		/* 5: center part */
		if (width > 0 && height > 0) {
		  unsigned int walky;
		  for (walky = top_low_row; walky < (unsigned int) unscaled_y_bottom; ++walky) {
		    unsigned int walkx;
		    base = spu->stride * walky;
		    for (walkx = left_right_column; walkx < (unsigned int) unscaled_x_right; ++walkx) {
		      tmp = /* 1.0 * 1.0 * */ canon_alpha(spu->aimage[base + walkx]);
		      alpha += tmp;
		      color += tmp * spu->image[base + walkx];
		    }
		  }
		}
		/* 6: center right part */
		if (right > 0.0 && height > 0) {
		  unsigned int walky;
		  for (walky = top_low_row; walky < (unsigned int) unscaled_y_bottom; ++walky) {
		    base = spu->stride * walky + (unsigned int) unscaled_x_right;
		    tmp = right /* * 1.0 */ * canon_alpha(spu->aimage[base]);
		    alpha += tmp;
		    color += tmp * spu->image[base];
		  }
		}
		/* 7: bottom left part */
		if (bottom > 0.0) {
		  base = spu->stride * (unsigned int) unscaled_y_bottom + (unsigned int) unscaled_x;
		  tmp = left * bottom * canon_alpha(spu->aimage[base]);
		  alpha += tmp;
		  color += tmp * spu->image[base];
		}
		/* 8: bottom center part */
		if (width > 0 && bottom > 0.0) {
		  unsigned int walkx;
		  base = spu->stride * (unsigned int) unscaled_y_bottom;
		  for (walkx = left_right_column; walkx < (unsigned int) unscaled_x_right; ++walkx) {
		    tmp = /* 1.0 * */ bottom * canon_alpha(spu->aimage[base + walkx]);
		    alpha += tmp;
		    color += tmp * spu->image[base + walkx];
		  }
		}
		/* 9: bottom right part */
		if (right > 0.0 && bottom > 0.0) {
		  base = spu->stride * (unsigned int) unscaled_y_bottom + (unsigned int) unscaled_x_right;
		  tmp = right * bottom * canon_alpha(spu->aimage[base]);
		  alpha += tmp;
		  color += tmp * spu->image[base];
		}
		/* Finally mix these transparency and brightness information suitably */
		base = spu->scaled_stride * y + x;
		spu->scaled_image[base] = alpha > 0 ? color / alpha : 0;
		spu->scaled_aimage[base] = alpha * scalex * scaley / 0x10000;
		if (spu->scaled_aimage[base]) {
		  spu->scaled_aimage[base] = 256 - spu->scaled_aimage[base];
		  if (spu->scaled_aimage[base] + spu->scaled_image[base] > 255)
		    spu->scaled_image[base] = 256 - spu->scaled_aimage[base];
		}
	      }
	    }
	  }
	  }
nothing_to_do:
	  /* Kludge: draw_alpha needs width multiple of 8. */
	  if (spu->scaled_width < spu->scaled_stride)
	    for (y = 0; y < spu->scaled_height; ++y) {
	      memset(spu->scaled_aimage + y * spu->scaled_stride + spu->scaled_width, 0,
		     spu->scaled_stride - spu->scaled_width);
	    }
	  spu->scaled_frame_width = dxs;
	  spu->scaled_frame_height = dys;
	}
      }
      if (spu->scaled_image){
        switch (spu_alignment) {
        case 0:
          spu->scaled_start_row = dys*sub_pos/100;
	  if (spu->scaled_start_row + spu->scaled_height > dys)
	    spu->scaled_start_row = dys - spu->scaled_height;
	  break;
	case 1:
          spu->scaled_start_row = dys*sub_pos/100 - spu->scaled_height/2;
	  if (sub_pos >= 50 && spu->scaled_start_row + spu->scaled_height > dys)
	      spu->scaled_start_row = dys - spu->scaled_height;
	  break;
        case 2:
          spu->scaled_start_row = dys*sub_pos/100 - spu->scaled_height;
	  break;
	}
	draw_alpha(spu->scaled_start_col, spu->scaled_start_row, spu->scaled_width, spu->scaled_height,
		   spu->scaled_image, spu->scaled_aimage, spu->scaled_stride);
	spu->spu_changed = 0;
      }
    }
  }
  else
  {
    mp_msg(MSGT_SPUDEC,MSGL_DBG2,"SPU not displayed: start_pts=%d  end_pts=%d  now_pts=%d\n",
        spu->start_pts, spu->end_pts, spu->now_pts);
  }
}

void spudec_update_palette(void * this, unsigned int *palette)
{
  spudec_handle_t *spu = this;
  if (spu && palette) {
    memcpy(spu->global_palette, palette, sizeof(spu->global_palette));
    if(spu->hw_spu)
      spu->hw_spu->control(VOCTRL_SET_SPU_PALETTE,spu->global_palette);
  }
}

void spudec_set_font_factor(void * this, double factor)
{
  spudec_handle_t *spu = this;
  spu->font_start_level = (int)(0xF0-(0xE0*factor));
}

static void spudec_parse_extradata(spudec_handle_t *this,
                                   uint8_t *extradata, int extradata_len)
{
  uint8_t *buffer, *ptr;
  unsigned int *pal = this->global_palette, *cuspal = this->cuspal;
  unsigned int tridx;
  int i;

  if (extradata_len == 16*4) {
    for (i=0; i<16; i++)
      pal[i] = AV_RB32(extradata + i*4);
    this->auto_palette = 0;
    return;
  }

  if (!(ptr = buffer = malloc(extradata_len+1)))
    return;
  memcpy(buffer, extradata, extradata_len);
  buffer[extradata_len] = 0;

  do {
    if (*ptr == '#')
        continue;
    if (!strncmp(ptr, "size: ", 6))
        sscanf(ptr + 6, "%dx%d", &this->orig_frame_width, &this->orig_frame_height);
    if (!strncmp(ptr, "palette: ", 9) &&
        sscanf(ptr + 9, "%x, %x, %x, %x, %x, %x, %x, %x, "
                        "%x, %x, %x, %x, %x, %x, %x, %x",
               &pal[ 0], &pal[ 1], &pal[ 2], &pal[ 3],
               &pal[ 4], &pal[ 5], &pal[ 6], &pal[ 7],
               &pal[ 8], &pal[ 9], &pal[10], &pal[11],
               &pal[12], &pal[13], &pal[14], &pal[15]) == 16) {
      for (i=0; i<16; i++)
        pal[i] = vobsub_palette_to_yuv(pal[i]);
      this->auto_palette = 0;
    }
    if (!av_strncasecmp(ptr, "forced subs: on", 15))
      this->forced_subs_only = 1;
    if (!strncmp(ptr, "custom colors: ON, tridx: ", 26) &&
        sscanf(ptr + 26, "%x, colors: %x, %x, %x, %x",
               &tridx, cuspal+0, cuspal+1, cuspal+2, cuspal+3) == 5) {
      for (i=0; i<4; i++) {
        cuspal[i] = vobsub_rgb_to_yuv(cuspal[i]);
        if (tridx & (1 << (12-4*i)))
          cuspal[i] |= 1 << 31;
      }
      this->custom = 1;
    }
  } while ((ptr=strchr(ptr,'\n')) && *++ptr);

  free(buffer);
}

void *spudec_new_scaled(unsigned int *palette, unsigned int frame_width, unsigned int frame_height, uint8_t *extradata, int extradata_len)
{
  spudec_handle_t *this = calloc(1, sizeof(spudec_handle_t));
  if (this){
    this->orig_frame_height = frame_height;
    this->orig_frame_width  = frame_width;
    // set up palette:
    if (palette)
      memcpy(this->global_palette, palette, sizeof(this->global_palette));
    else
      this->auto_palette = 1;
    if (extradata)
      spudec_parse_extradata(this, extradata, extradata_len);
    /* XXX Although the video frame is some size, the SPU frame is
       always maximum size i.e. 720 wide and 576 or 480 high */
    // For HD files in MKV the VobSub resolution can be higher though,
    // see largeres_vobsub.mkv
    if (this->orig_frame_width <= 720 && this->orig_frame_height <= 576) {
      this->orig_frame_width = 720;
      if (this->orig_frame_height == 480 || this->orig_frame_height == 240)
        this->orig_frame_height = 480;
      else
        this->orig_frame_height = 576;
    }
  }
  else
    mp_msg(MSGT_SPUDEC,MSGL_FATAL, "FATAL: spudec_init: calloc");
  return this;
}

void *spudec_new(unsigned int *palette)
{
    return spudec_new_scaled(palette, 0, 0, NULL, 0);
}

void spudec_free(void *this)
{
  spudec_handle_t *spu = this;
  if (spu) {
    while (spu->queue_head)
      spudec_free_packet(spudec_dequeue_packet(spu));
    free(spu->packet);
    spu->packet = NULL;
    av_freep(&spu->scaled_image);
    spu->scaled_image = NULL;
    av_freep(&spu->image);
    spu->image = NULL;
    spu->aimage = NULL;
    av_freep(&spu->pal_image);
    spu->pal_image = NULL;
    spu->image_size = 0;
    spu->pal_width = spu->pal_height  = 0;
    free(spu);
  }
}

void spudec_set_hw_spu(void *this, const vo_functions_t *hw_spu)
{
  spudec_handle_t *spu = this;
  if (!spu)
    return;
  spu->hw_spu = hw_spu;
  hw_spu->control(VOCTRL_SET_SPU_PALETTE,spu->global_palette);
}

#define MP_NOPTS_VALUE (-1LL<<63) //both int64_t and double should be able to represent this exactly

packet_t *spudec_packet_create(int x, int y, int w, int h)
{
  packet_t *packet;
  int stride = (w + 15) & ~15;
  if ((unsigned)w >= 0x8000 || (unsigned)h > 0x4000)
    return NULL;
  packet = calloc(1, sizeof(packet_t));
  packet->is_decoded = 1;
  packet->width = w;
  packet->height = h;
  packet->stride = stride;
  packet->start_col = x;
  packet->start_row = y;
  packet->data_len = 2 * stride * h;
  if (packet->data_len) { // size 0 is a special "clear" packet
    packet->packet = av_malloc(packet->data_len);
    if (!packet->packet) {
      free(packet);
      packet = NULL;
    }
  }
  return packet;
}

void spudec_packet_clear(packet_t *packet)
{
  /* clear alpha and value, as value is premultiplied */
  memset(packet->packet, 0, packet->data_len);
}

void spudec_packet_fill(packet_t *packet,
                        const uint8_t *pal_img, int pal_stride,
                        const void *palette,
                        int x, int y, int w, int h)
{
  const uint32_t *pal = palette;
  uint8_t *img  = packet->packet + x + y * packet->stride;
  uint8_t *aimg = img + packet->stride * packet->height;
  int i;
  uint16_t g8a8_pal[256];

  for (i = 0; i < 256; i++) {
      uint32_t pixel = pal[i];
      int alpha = pixel >> 24;
      int gray = (((pixel & 0x000000ff) >>  0) +
                  ((pixel & 0x0000ff00) >>  7) +
                  ((pixel & 0x00ff0000) >> 16)) >> 2;
      gray = FFMIN(gray, alpha);
      g8a8_pal[i] = (-alpha << 8) | gray;
  }
  pal2gray_alpha(g8a8_pal, pal_img, pal_stride,
                 img, aimg, packet->stride, w, h, 1);
}

void spudec_packet_send(void *spu, packet_t *packet, double pts, double endpts)
{
  packet->start_pts = 0;
  packet->end_pts = 0x7fffffff;
  // Note: valid timestamps from e.g. DVB subtitles can
  // overflow. Just ignoring the overflow seems to work
  // best for now, but should be fixed properly at some point.
  if (pts != MP_NOPTS_VALUE)
    packet->start_pts = pts * 90000;
  if (endpts != MP_NOPTS_VALUE)
    packet->end_pts = endpts * 90000;
  spudec_queue_packet(spu, packet);
}

/**
 * palette must contain at least 256 32-bit entries, otherwise crashes
 * are possible
 */
void spudec_set_paletted(void *spu, const uint8_t *pal_img, int pal_stride,
                         const void *palette,
                         int x, int y, int w, int h,
                         double pts, double endpts)
{
  packet_t *packet = spudec_packet_create(x, y, w, h);
  if (!packet)
      return;
  if (packet->data_len) // size 0 is a special "clear" packet
    spudec_packet_fill(packet, pal_img, pal_stride, palette, 0, 0, w, h);
  spudec_packet_send(spu, packet, pts, endpts);
}
