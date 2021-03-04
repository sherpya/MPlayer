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

/**
 * @file
 * @brief Internal declarations and definitions
 */

#ifndef MPLAYER_GUI_GUI_H
#define MPLAYER_GUI_GUI_H

#include "config.h"

#ifdef ENABLE_NLS
#include <libintl.h>
#include <locale.h>
#define GETTEXT_PACKAGE "mplayer"
#define _(string) dgettext(GETTEXT_PACKAGE, string)
#else
#define _(string) string
#endif

/// Name of the program the GUI utilizes
#define MPlayer "MPlayer"

//@{
/// Boolean symbolic constant
#if !defined(True) && !defined(False)
#define True  1
#define False 0
#endif
//@}

/// A pseudo stream type indicating not to change the current one
#define SAME_STREAMTYPE (STREAMTYPE_DUMMY - 1)

/// StreamTypes that are permitted in a playlist
#define isPlaylistStreamtype (guiInfo.StreamType == STREAMTYPE_FILE || guiInfo.StreamType == STREAMTYPE_STREAM)

/// StreamTypes that permit seeking
#define isSeekableStreamtype (guiInfo.StreamType != STREAMTYPE_STREAM && guiInfo.StreamType != STREAMTYPE_TV && guiInfo.StreamType != STREAMTYPE_DVB)

/// Check whether @a x/y is inside the rectangle given by @a top @a x/y and @a bottom @a x/y.
#define isInside(x, y, tx, ty, bx, by) ((x) > (tx) && (y) > (ty) && (x) < (bx) && (y) < (by))

/// Check whether #guiItem @a item has a button (and thus a pressed state).
#define hasButton(item) (item.type == itButton || item.type == itHPotmeter || item.type == itVPotmeter || item.type == itRPotmeter)

/// Check whether #guiItem @a item utilizes member 'value'
#define hasValue(item) (item.type == itHPotmeter || item.type == itVPotmeter || item.type == itRPotmeter || item.type == itPimage)

#endif /* MPLAYER_GUI_GUI_H */
