/*
 * KEY code definitions for GyS-TermIO v2.0
 *
 * copyright (C) 1999 A'rpi/ESP-team
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

#ifndef MPLAYER_KEYCODES_H
#define MPLAYER_KEYCODES_H

#define KEY_BASE 0x1000000

enum {
    KEY_BACKSPACE = 8,
    KEY_TAB = 9,
    KEY_ENTER = 13,
    KEY_ESC = 27,
    KEY_DELETE = 127,
    /* Control keys */
    KEY_INSERT = KEY_BASE,
    KEY_HOME,
    KEY_END,
    KEY_PAGE_UP,
    KEY_PAGE_DOWN,
    /* Cursor movement */
    KEY_RIGHT = KEY_BASE + 16,
    KEY_LEFT,
    KEY_DOWN,
    KEY_UP,
    /* Keypad keys */
    KEY_KP0 = KEY_BASE + 32,
    KEY_KP1,
    KEY_KP2,
    KEY_KP3,
    KEY_KP4,
    KEY_KP5,
    KEY_KP6,
    KEY_KP7,
    KEY_KP8,
    KEY_KP9,
    KEY_KPDEC,
    KEY_KPINS,
    KEY_KPDEL,
    KEY_KPENTER,
    /* Key combination keys */
    KEY_CTRL = KEY_BASE + 48,
    /* Function keys */
    KEY_F = KEY_BASE + 64,
    /* Multimedia keyboard/remote keys */
    KEY_POWER = KEY_BASE+384,
    KEY_MENU,
    KEY_PLAY,
    KEY_PAUSE,
    KEY_PLAYPAUSE,
    KEY_STOP,
    KEY_FORWARD,
    KEY_REWIND,
    KEY_NEXT,
    KEY_PREV,
    KEY_VOLUME_UP,
    KEY_VOLUME_DOWN,
    KEY_MUTE,
    /* Special internal/virtual keys */
    KEY_CLOSE_WIN = KEY_BASE + 0x1000,
};

/* Control keys short name */
#define KEY_BS KEY_BACKSPACE
#define KEY_DEL KEY_DELETE
#define KEY_INS KEY_INSERT
#define KEY_PGUP KEY_PAGE_UP
#define KEY_PGDOWN KEY_PAGE_DOWN
#define KEY_PGDWN KEY_PAGE_DOWN

/* Ctrl + Key */
#define CTRL_A      (0x01)
#define CTRL_B      (0x02)
#define CTRL_C      (0x03)
#define CTRL_D      (0x04)
#define CTRL_E      (0x05)
#define CTRL_F      (0x06)
#define CTRL_G      (0x07)
/* #define CTRL_H      (0x08) */ /* KEY_BACKSPACE */
/* #define CTRL_I      (0x09) */ /* KEY_TAB */
#define CTRL_J      (0x0a)
#define CTRL_K      (0x0b)
/* #define CTRL_M      (0x0c) */ /* KEY_ENTER */
#define CTRL_N      (0x0e)
#define CTRL_O      (0x0f)
#define CTRL_P      (0x10)
#define CTRL_Q      (0x11)
#define CTRL_L      (0x12)
#define CTRL_V      (0x16)
#define CTRL_W      (0x17)
#define CTRL_R      (0x18)
#define CTRL_S      (0x19)
#define CTRL_Z      (0x1a)
#define CTRL_SO     (0x1b)
#define CTRL_BSL    (0x1c)
#define CTRL_SC     (0x1d)
#define CTRL_T      (0x20)
#define CTRL_U      (0x21)
#define CTRL_Y      (0x25)

#endif /* MPLAYER_KEYCODES_H */
