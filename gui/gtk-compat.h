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
 * @brief GTK compatibility definitions
 */

#ifndef GTK_COMPAT_H
#define GTK_COMPAT_H

#include <gtk/gtk.h>

#if !GTK_CHECK_VERSION(2,6,0)
#define gtk_button_set_image(button, image) gtk_button_set_label(button, MSGTR_GUI_ ## image)
#endif

#if !GTK_CHECK_VERSION(2,22,0)
#define GDK_KEY_BackSpace GDK_BackSpace
#define GDK_KEY_Escape GDK_Escape
#define GDK_KEY_Return GDK_Return
#endif

#endif
