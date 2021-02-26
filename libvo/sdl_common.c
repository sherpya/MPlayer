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

#include "sdl_common.h"
#include "mp_msg.h"
#include "mp_fifo.h"
#include "osdep/keycodes.h"
#include "osdep/setenv.h"
#include "input/input.h"
#include "input/mouse.h"
#include "video_out.h"
#include "geometry.h"

static int old_w;
static int old_h;
static int mode_flags;
static int reinit;
SDL_Window *sdl_window;

/**
 * Update vo_screenwidth and vo_screenheight.
 *
 * This function only works with SDL since 1.2.10 and
 * even then only when called before the first
 * SDL_SetVideoMode.
 * Once there's a better way available implement an
 * update_xinerama_info function.
 */
static void get_screensize(void) {
    // Keep user-provided settings
    if (vo_screenwidth > 0 || vo_screenheight > 0) return;
#if SDL_VERSION_ATLEAST(2, 0, 0)
    SDL_DisplayMode m;
    if (SDL_GetCurrentDisplayMode(0, &m)) return;
    vo_screenwidth = m.w;
    vo_screenheight = m.h;
    // TODO: better to use a check that gets the runtime version instead?
#elif SDL_VERSION_ATLEAST(1, 2, 10)
    const SDL_VideoInfo *vi;
    vi = SDL_GetVideoInfo();
    vo_screenwidth  = vi->current_w;
    vo_screenheight = vi->current_h;
#endif
}

int vo_sdl_init(void)
{
    reinit = 0;

    if (!SDL_WasInit(SDL_INIT_VIDEO)) {
        // Unfortunately SDL_WINDOWID must be set at SDL_Init
        // and is ignored afterwards, thus it cannot work per-file.
        // Also, a value of 0 does not work for selecting the root window.
        if (WinID > 0) {
            char envstr[20];
            snprintf(envstr, sizeof(envstr), "0x%"PRIx64, WinID);
            setenv("SDL_WINDOWID", envstr, 1);
        }
        if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE) < 0)
            return 0;
        sdl_window = NULL;
    }

#if !SDL_VERSION_ATLEAST(2, 0, 0)
    // Setup Keyrepeats (500/30 are defaults)
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, 100 /*SDL_DEFAULT_REPEAT_INTERVAL*/);

    // Easiest way to get uppercase characters
    SDL_EnableUNICODE(1);

    // We don't want those in our event queue.
    SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
#endif
    SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
    SDL_EventState(SDL_USEREVENT, SDL_IGNORE);

    // Try to get a sensible default for fullscreen.
    get_screensize();

    return 1;
}

void vo_sdl_uninit(void)
{
    if (SDL_WasInit(SDL_INIT_VIDEO))
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    sdl_window = NULL;
}

int vo_sdl_config(int w, int h, int flags, const char *title)
{
#if !SDL_VERSION_ATLEAST(2, 0, 0)
    SDL_WM_SetCaption(title, NULL);
#endif
    vo_dwidth  = old_w = w;
    vo_dheight = old_h = h;
    vo_fs = !!(flags & VOFLAG_FULLSCREEN);
    if (vo_fs) {
        vo_dwidth  = vo_screenwidth;
        vo_dheight = vo_screenheight;
    }
#if SDL_VERSION_ATLEAST(2, 0, 0)
    if (!sdl_window) {
        unsigned sdlflags = SDL_OPENGL;
        if (vo_fs) flags |= SDL_FULLSCREEN;
        if (!vo_border) flags |= SDL_NOFRAME;
        sdl_window = SDL_CreateWindow(title,
                                      geometry_xy_changed ? vo_dx : SDL_WINDOWPOS_UNDEFINED,
                                      geometry_xy_changed ? vo_dy : SDL_WINDOWPOS_UNDEFINED,
                                      vo_dwidth, vo_dheight, sdlflags);
    }
#endif
    SDL_GL_SetAttribute(SDL_GL_STEREO, !!(flags & VOFLAG_STEREO));
    return 1;
}

void vo_sdl_fullscreen(void)
{
    if (vo_fs) {
        vo_dwidth  = old_w;
        vo_dheight = old_h;
    } else {
        old_w = vo_dwidth;
        old_h = vo_dheight;
        vo_dwidth  = vo_screenwidth;
        vo_dheight = vo_screenheight;
    }
    vo_fs = !vo_fs;
#if SDL_VERSION_ATLEAST(2, 0, 0)
    SDL_SetWindowFullscreen(sdl_window, vo_fs ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
#else
    sdl_set_mode(0, mode_flags);
#endif
    // on OSX at least we now need to do a full reinit.
    // TODO: this should only be set if really necessary.
    reinit = 1;
}

#if SDL_VERSION_ATLEAST(2, 0, 0)
int
#else
SDL_Surface *
#endif
sdl_set_mode(int bpp, uint32_t flags)
{
    SDL_Surface *s;
    mode_flags = flags;
#if SDL_VERSION_ATLEAST(2, 0, 0)
    if (!sdl_window) return 0;
    SDL_SetWindowSize(sdl_window, vo_dwidth, vo_dheight);
    if (geometry_xy_changed) SDL_SetWindowPosition(sdl_window, vo_dx, vo_dy);
    if (flags & SDL_OPENGL) SDL_GL_GetDrawableSize(sdl_window, &vo_dwidth, &vo_dheight);
    else SDL_GetWindowSize(sdl_window, &vo_dwidth, &vo_dheight);
    return 1;
#else
    if (vo_fs) flags |= SDL_FULLSCREEN;
    // doublebuf with opengl creates flickering
#if !defined( __AMIGAOS4__ ) && !defined( __APPLE__ )
    if (vo_doublebuffering && !(flags & SDL_OPENGL))
        flags |= SDL_DOUBLEBUF;
#endif
    if (!vo_border)
        flags |= SDL_NOFRAME;
    if (geometry_xy_changed) {
        char envstr[20];
        snprintf(envstr, sizeof(envstr), "%i,%i", vo_dx, vo_dy);
        setenv("SDL_VIDEO_WINDOW_POS", envstr, 1);
    }
    s = SDL_SetVideoMode(vo_dwidth, vo_dheight, bpp, flags);
    if (!s) {
      mp_msg(MSGT_VO, MSGL_FATAL, "SDL SetVideoMode failed: %s\n", SDL_GetError());
      return NULL;
    }
    vo_dwidth  = s->w;
    vo_dheight = s->h;
    return s;
#endif
}

static const struct mp_keymap keysym_map[] = {
    {SDLK_RETURN, KEY_ENTER}, {SDLK_ESCAPE, KEY_ESC},
    {SDLK_F1, KEY_F+1}, {SDLK_F2, KEY_F+2}, {SDLK_F3, KEY_F+3},
    {SDLK_F4, KEY_F+4}, {SDLK_F5, KEY_F+5}, {SDLK_F6, KEY_F+6},
    {SDLK_F7, KEY_F+7}, {SDLK_F8, KEY_F+8}, {SDLK_F9, KEY_F+9},
    {SDLK_F10, KEY_F+10}, {SDLK_F11, KEY_F+11}, {SDLK_F12, KEY_F+12},
    {SDLK_KP_PLUS, '+'}, {SDLK_KP_MINUS, '-'}, {SDLK_TAB, KEY_TAB},
    {SDLK_PAGEUP, KEY_PAGE_UP}, {SDLK_PAGEDOWN, KEY_PAGE_DOWN},
    {SDLK_UP, KEY_UP}, {SDLK_DOWN, KEY_DOWN},
    {SDLK_LEFT, KEY_LEFT}, {SDLK_RIGHT, KEY_RIGHT},
    {SDLK_KP_MULTIPLY, '*'}, {SDLK_KP_DIVIDE, '/'},
#if SDL_VERSION_ATLEAST(2, 0, 0)
    {SDLK_KP_0, KEY_KP0}, {SDLK_KP_1, KEY_KP1}, {SDLK_KP_2, KEY_KP2},
    {SDLK_KP_3, KEY_KP3}, {SDLK_KP_4, KEY_KP4}, {SDLK_KP_5, KEY_KP5},
    {SDLK_KP_6, KEY_KP6}, {SDLK_KP_7, KEY_KP7}, {SDLK_KP_8, KEY_KP8},
    {SDLK_KP_9, KEY_KP9},
#else
    {SDLK_KP0, KEY_KP0}, {SDLK_KP1, KEY_KP1}, {SDLK_KP2, KEY_KP2},
    {SDLK_KP3, KEY_KP3}, {SDLK_KP4, KEY_KP4}, {SDLK_KP5, KEY_KP5},
    {SDLK_KP6, KEY_KP6}, {SDLK_KP7, KEY_KP7}, {SDLK_KP8, KEY_KP8},
    {SDLK_KP9, KEY_KP9},
#endif
    {SDLK_KP_PERIOD, KEY_KPDEC}, {SDLK_KP_ENTER, KEY_KPENTER},
    {0, 0}
};

int sdl_default_handle_event(SDL_Event *event)
{
    int mpkey;
    if (!event) {
        int res = reinit ? VO_EVENT_REINIT : 0;
        reinit = 0;
        return res;
    }
    switch (event->type) {
#if SDL_VERSION_ATLEAST(2, 0, 0)
    case SDL_WINDOWEVENT:
        switch (event->window.event)
        {
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            vo_dwidth  = event->window.data1;
            vo_dheight = event->window.data2;
            return VO_EVENT_RESIZE;
        case SDL_WINDOWEVENT_EXPOSED:
            return VO_EVENT_EXPOSE;
        }
        break;
#else
    case SDL_VIDEORESIZE:
        vo_dwidth  = event->resize.w;
        vo_dheight = event->resize.h;
        return VO_EVENT_RESIZE;

    case SDL_VIDEOEXPOSE:
        return VO_EVENT_EXPOSE;
#endif

    case SDL_MOUSEMOTION:
        vo_mouse_movement(event->motion.x, event->motion.y);
        break;

    case SDL_MOUSEBUTTONDOWN:
        if (!vo_nomouse_input)
            mplayer_put_key((MOUSE_BTN0 + event->button.button - 1) | MP_KEY_DOWN);
        break;

    case SDL_MOUSEBUTTONUP:
        if (!vo_nomouse_input)
            mplayer_put_key(MOUSE_BTN0 + event->button.button - 1);
        break;

    case SDL_KEYDOWN:
        mpkey = lookup_keymap_table(keysym_map, event->key.keysym.sym);
        {
#if SDL_VERSION_ATLEAST(2, 0, 0)
        unsigned code = event->key.keysym.sym;
#else
        unsigned code = event->key.keysym.unicode;
#endif
        if (!mpkey && code > 0 && code < 128)
            mpkey = code;
        }
        if (mpkey)
            mplayer_put_key(mpkey);
        break;

    case SDL_QUIT:
        mplayer_put_key(KEY_CLOSE_WIN);
        break;
    }
    return 0;
}
