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

#ifndef MPLAYER_OSX_COMMON_H
#define MPLAYER_OSX_COMMON_H

struct MPGLContext;

int convert_key(unsigned key, unsigned charcode);
void change_movie_aspect(float new_aspect);
void config_movie_aspect(float config_aspect);
void osx_foreground_hack(void);

extern void (*vo_osx_redraw_func)(void);
int vo_osx_init(void);
int vo_osx_config(uint32_t, uint32_t, uint32_t);
void vo_osx_uninit(void);
void vo_osx_ontop(void);
void vo_osx_fullscreen(void);
int vo_osx_check_events(void);
void vo_osx_update_xinerama_info(void);
void vo_osx_swap_buffers(struct MPGLContext *);

#endif /* MPLAYER_OSX_COMMON_H */
