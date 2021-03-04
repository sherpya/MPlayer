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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <gdk/gdkx.h>

#include "dialog.h"
#include "about.h"
#include "equalizer.h"
#include "fileselect.h"
#include "menu.h"
#include "msgbox.h"
#include "playlist.h"
#include "preferences.h"
#include "skinbrowser.h"
#include "tools.h"
#include "url.h"
#include "gui/interface.h"
#include "gui/app/app.h"
#include "gui/app/gui.h"
#include "gui/util/string.h"
#include "gui/wm/ws.h"

#include "config.h"
#include "help_mp.h"
#include "mp_msg.h"
#include "libavutil/intreadwrite.h"
#include "osdep/timer.h"

#define THRESHOLD 128   // transparency values equal to or above this will become
                        // opaque, all values below this will become transparent

guiIcon_t guiIcon;

static const char guiIconName[] = "mplayer";

static GtkWidget *PopUpMenu;

static int gtkInitialized;

/**
 * @brief Add an icon to the #guiIcon icon structure.
 *
 * @param theme theme to load the icon from
 * @param size size of the icon to load
 * @param gdkIcon memory location to store a pointer to the created pixmap
 * @param gdkIconMask memory location to store a pointer to the created mask
 *
 * @return #True (ok) or #False (error)
 */
static int gtkLoadIcon(GtkIconTheme *theme, gint size, GdkPixmap **gdkIcon, GdkBitmap **gdkIconMask)
{
    GdkPixbuf *pixbuf;
    guchar *data;
    int csize, i;

    pixbuf = gtk_icon_theme_load_icon(theme, guiIconName, size, 0, NULL);

    if (pixbuf)
        gdk_pixbuf_render_pixmap_and_mask_for_colormap(pixbuf, gdk_colormap_get_system(), gdkIcon, gdkIconMask, THRESHOLD);

    if (pixbuf &&
        gdk_pixbuf_get_colorspace(pixbuf) == GDK_COLORSPACE_RGB &&
        gdk_pixbuf_get_n_channels(pixbuf) == 4 &&
        gdk_pixbuf_get_bits_per_sample(pixbuf) == 8) {
        csize = guiIcon.collection_size;
        guiIcon.collection_size += 2 + gdk_pixbuf_get_width(pixbuf) * gdk_pixbuf_get_height(pixbuf);

        guiIcon.collection = realloc(guiIcon.collection, guiIcon.collection_size * sizeof(*guiIcon.collection));

        if (guiIcon.collection) {
            guiIcon.collection[csize++] = gdk_pixbuf_get_width(pixbuf);
            guiIcon.collection[csize++] = gdk_pixbuf_get_height(pixbuf);

            data = gdk_pixbuf_get_pixels(pixbuf);

            for (i = csize; i < guiIcon.collection_size; data += 4, i++)
                guiIcon.collection[i] = (uint32_t)(data[3] << 24) | AV_RB24(data); // RGBA -> ARGB
        }

        g_object_unref(pixbuf);
    } else
        mp_msg(MSGT_GPLAYER, MSGL_WARN, _(MSGTR_GUI_MSG_IconError), guiIconName, size);

    /* start up GTK which realizes the pixmaps */
    gtk_main_iteration_do(FALSE);

    return (pixbuf != NULL);
}

/**
 * @brief Initialize the GTK user interface.
 *
 * @param display_name name of the X display to use or NULL (using the DISPLAY environment variable)
 */
void gtkInit(char *display_name)
{
    int argc = 0;
    char *arg[3], **argv = arg;
    char *env;
    GtkIconTheme *theme;
    GdkPixmap *gdkIcon;
    GdkBitmap *gdkIconMask;

    mp_msg(MSGT_GPLAYER, MSGL_V, "GTK init.\n");

    arg[argc++] = gmplayer;

    if (display_name) {             // MPlayer option '-display' was given
        arg[argc++] = "--display";  // Pass corresponding command line arguments to GTK,
        arg[argc++] = display_name; // to open the requested display for the GUI, too.
    }

    gtk_disable_setlocale();

    env = getenv("G_FILENAME_ENCODING");

    if ((!env && getenv("G_BROKEN_FILENAMES")) || (gstrncmp(env, "@locale", 7) == 0))
        mp_msg(MSGT_GPLAYER, MSGL_WARN, _(MSGTR_GUI_MSG_LocaleEncoding));

    gtk_init(&argc, &argv);
    wsSetErrorHandler();      // GDK has just set its own handler

    theme = gtk_icon_theme_get_default();

    if (gtkLoadIcon(theme, 16, &gdkIcon, &gdkIconMask)) {
        guiIcon.small      = GDK_PIXMAP_XID(gdkIcon);
        guiIcon.small_mask = GDK_PIXMAP_XID(gdkIconMask);
    }

    if (gtkLoadIcon(theme, 32, &gdkIcon, &gdkIconMask)) {
        guiIcon.normal      = GDK_PIXMAP_XID(gdkIcon);
        guiIcon.normal_mask = GDK_PIXMAP_XID(gdkIconMask);
    }

    gtkLoadIcon(theme, 48, &gdkIcon, &gdkIconMask);

    gtkInitialized = True;
}

/**
 * @brief Add the #guiIcon icons to a GTK window.
 *
 * @param window pointer to a GtkWindow widget
 */
void gtkAddIcon(GtkWidget *window)
{
    wsWindowIcon(gdk_display, GDK_WINDOW_XWINDOW(window->window), &guiIcon);
}

/**
 * @brief Process all pending events.
 */
void gtkEvents(void)
{
    while (gtk_events_pending())
        gtk_main_iteration();
}

/* funcs */

void gtkMessageBox(int type, const gchar *str)
{
    if (!gtkInitialized)
        return;

    ShowMessageBox(str);
    gtk_label_set_text(GTK_LABEL(gtkMessageBoxText), str);

    if (strlen(str) > 80) {
        gtk_label_set_line_wrap(GTK_LABEL(gtkMessageBoxText), TRUE);
        gtk_label_set_justify(GTK_LABEL(gtkMessageBoxText), GTK_JUSTIFY_LEFT);
        gtk_widget_set_size_request(gtkMessageBoxText, 415, -1);
    } else
        gtk_label_set_line_wrap(GTK_LABEL(gtkMessageBoxText), FALSE);

    switch (type & ~MSGBOX_WAIT) {
    case MSGBOX_FATAL:
        gtk_window_set_title(GTK_WINDOW(MessageBox), _(MSGTR_GUI_ErrorFatal));
        gtk_widget_hide(InformationImage);
        gtk_widget_hide(WarningImage);
        gtk_widget_show(ErrorImage);
        type |= MSGBOX_WAIT;
        break;

    case MSGBOX_ERROR:
        gtk_window_set_title(GTK_WINDOW(MessageBox), _(MSGTR_GUI_Error));
        gtk_widget_hide(InformationImage);
        gtk_widget_hide(WarningImage);
        gtk_widget_show(ErrorImage);
        break;

    case MSGBOX_WARNING:
        gtk_window_set_title(GTK_WINDOW(MessageBox), _(MSGTR_GUI_Warning));
        gtk_widget_hide(InformationImage);
        gtk_widget_show(WarningImage);
        gtk_widget_hide(ErrorImage);
        break;

    case MSGBOX_INFORMATION:
        gtk_window_set_title(GTK_WINDOW(MessageBox), _(MSGTR_GUI_Information));
        gtk_widget_show(InformationImage);
        gtk_widget_hide(WarningImage);
        gtk_widget_hide(ErrorImage);
        break;
    }

    gtk_widget_show(MessageBox);
    gtkSetLayer(MessageBox);

    if (type & MSGBOX_WAIT) {
        while (MessageBox) {
            gtk_main_iteration_do(FALSE);
            usec_sleep(5000);
        }
    }
}

/**
 * @brief Set the layer for a GTK window.
 *
 * @param window pointer to a GtkWindow widget
 */
void gtkSetLayer(GtkWidget *window)
{
    wsWindowLayer(gdk_display, GDK_WINDOW_XWINDOW(window->window), guiApp.videoWindow.isFullScreen);
    gtkRaise(window);
}

/**
 * @brief Activate a GTK window, i.e. raise it to the top.
 *
 * @param window pointer to a GtkWindow widget
 */
void gtkRaise(GtkWidget *window)
{
    wsWindowRaiseTop(gdk_display, GDK_WINDOW_XWINDOW(window->window));
}

static void gtkSelectInCList(GtkWidget *list, char *item)
{
    gint i;

    if ((i = gtkFindInCList(list, item)) > -1)
        gtk_clist_select_row(GTK_CLIST(list), i, 0);
}

void gtkShow(int type, char *param)
{
    switch (type) {
    case evEqualizer:
        ShowEqualizer();
        gtkSetLayer(Equalizer);
        break;

    case evSkinBrowser:
        ShowSkinBrowser();

//        gtk_clist_clear(GTK_CLIST(SkinList));
        if (FillSkinList(sbSkinDirInHome) &&
            FillSkinList(sbSkinDirInData)) {
            gtkSelectInCList(SkinList, param);
            gtk_clist_sort(GTK_CLIST(SkinList));
            gtkSetLayer(SkinBrowser);
        } else {
            gtk_widget_destroy(SkinBrowser);
            gtkMessageBox(MSGBOX_ERROR, _(MSGTR_GUI_MSG_SkinDirNotFound));
        }

        break;

    case evPreferences:
        ShowPreferences();
        break;

    case evPlaylist:
        ShowPlaylist();
        gtkSetLayer(Playlist);
        break;

    case evLoad:
        ShowFileSelector(FILESELECT_VIDEO_AUDIO);
        gtkSetLayer(FileSelector);
        break;

    case evLoadSubtitle:
        ShowFileSelector(FILESELECT_SUBTITLE);
        gtkSetLayer(FileSelector);
        break;

    case evLoadAudioFile:
        ShowFileSelector(FILESELECT_AUDIO_TRACK);
        gtkSetLayer(FileSelector);
        break;

    case evPlayImage:
        ShowFileSelector(FILESELECT_IMAGE);
        gtkSetLayer(FileSelector);
        break;

    case evAbout:
        ShowAbout();
        gtkSetLayer(About);
        break;

    case ivShowPopUpMenu:

        if (PopUpMenu) {
            gtk_widget_hide(PopUpMenu);
            gtk_widget_destroy(PopUpMenu);
        }

        PopUpMenu = CreatePopUpMenu((int)param);
        gtk_menu_popup(GTK_MENU(PopUpMenu), NULL, NULL, NULL, NULL, 0, 0);
        break;

    case ivHidePopUpMenu:

        if (PopUpMenu) {
            gtk_widget_hide(PopUpMenu);
            gtk_widget_destroy(PopUpMenu);
            PopUpMenu = NULL;
        }

        break;

    case evLoadURL:
        ShowURLDialog();
        gtkSetLayer(URLDialog);
        break;
    }
}
