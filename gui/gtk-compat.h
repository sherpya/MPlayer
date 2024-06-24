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

#if !GTK_CHECK_VERSION(2,8,14)
/* a bug: unlabeled buttons do not show the image */
#define gtk_button_set_image(button, image) do { gtk_button_set_label(button, ""); gtk_button_set_image(button, image); } while (0)
#endif

#if !GTK_CHECK_VERSION(2,14,0)
#define gtk_widget_get_window(widget) widget->window
#define gtk_adjustment_set_upper(adjustment, value) adjustment->upper = value
#endif

#if !GTK_CHECK_VERSION(2,18,0)
#define gtk_widget_get_sensitive(widget) GTK_WIDGET_SENSITIVE(widget)
#endif

#if !GTK_CHECK_VERSION(2,22,0)
#define GDK_KEY_BackSpace GDK_BackSpace
#define GDK_KEY_Escape GDK_Escape
#define GDK_KEY_Return GDK_Return
#endif

#if GTK_CHECK_VERSION(2,24,0)
#define gtk_combo_box_entry_new_text gtk_combo_box_text_new_with_entry
#define gtk_combo_box_append_text(combo_box, text) gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), text)
#endif

#endif
