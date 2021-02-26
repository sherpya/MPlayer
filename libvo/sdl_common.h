/*
 * common SDL routines
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

#ifndef MPLAYER_SDL_COMMON_H
#define MPLAYER_SDL_COMMON_H

#include "config.h"
#include <stdint.h>
#ifdef CONFIG_SDL_SDL_H
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

#if SDL_VERSION_ATLEAST(2, 0, 0)
#define SDL_FULLSCREEN SDL_WINDOW_FULLSCREEN_DESKTOP
#define SDL_OPENGL SDL_WINDOW_OPENGL
#define SDL_NOFRAME SDL_WINDOW_BORDERLESS
#define SDL_RESIZABLE SDL_WINDOW_RESIZABLE
#else
#define SDL_Window void
#endif

extern SDL_Window *sdl_window;

int vo_sdl_init(void);
void vo_sdl_uninit(void);
int vo_sdl_config(int w, int h, int flags, const char *title);
void vo_sdl_fullscreen(void);
#if SDL_VERSION_ATLEAST(2, 0, 0)
int
#else
SDL_Surface *
#endif
sdl_set_mode(int bpp, uint32_t flags);
int sdl_default_handle_event(SDL_Event *event);

#endif /* MPLAYER_SDL_COMMON_H */
