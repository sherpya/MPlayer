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

#ifndef MPLAYER_AV_HELPERS_H
#define MPLAYER_AV_HELPERS_H

struct AVCodecContext;
struct AVFrame;
struct AVPacket;

void init_avcodec(void);
void init_avformat(void);
int lavc_encode_audio(struct AVCodecContext *ctx, void *src, int src_len, void *dst, int dst_len);
int mp_packet_merge_side_data(struct AVPacket *pkt);
int mp_packet_split_side_data(struct AVPacket *pkt);

#endif /* MPLAYER_AV_HELPERS_H */
