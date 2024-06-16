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

#include <glob.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "fileselect.h"
#include "dialog.h"
#include "icons.h"
#include "preferences.h"
#include "tools.h"
#include "pixmaps/dir.xpm"
#include "pixmaps/file.xpm"
#include "pixmaps/file_audio.xpm"
#include "pixmaps/file_audio_track.xpm"
#include "pixmaps/file_font.xpm"
#include "pixmaps/file_image.xpm"
#include "pixmaps/file_playlist.xpm"
#include "pixmaps/file_subtitle.xpm"
#include "pixmaps/file_video.xpm"
#include "gui/gtk-compat.h"
#include "gui/interface.h"
#include "gui/app/app.h"
#include "gui/app/cfg.h"
#include "gui/app/gui.h"
#include "gui/ui/actions.h"
#include "gui/util/list.h"
#include "gui/util/mem.h"
#include "gui/util/string.h"

#define CFG_OLD_FILESELECT
#include "gui/app/cfg-old.c"

#include "config.h"
#include "help_mp.h"
#include "mpcommon.h"
#include "mplayer.h"
#include "libavutil/avstring.h"
#include "libavutil/common.h"
#include "stream/stream.h"

#ifdef __linux__
char *get_current_dir_name(void);
#else
#include <limits.h>
#define get_current_dir_name() getcwd(NULL, PATH_MAX)
#endif

const char *fsSelectedFile;
const char *fsThisDir = ".";
const gchar *fsFilter = "*";

int fsType;
static gboolean fsMedium;

static gint fsCurrFNameListSelected, fsLastFNameListSelected;

char * const fsVideoAudioFilterNames[][2] = {
    { MSGTR_GUI_FilterVideoAvi,           "*.avi"                                                                                                                                                                                                                                                                                                                                                                                                                              },
    { MSGTR_GUI_FilterVideoDivX,          "*.divx"                                                                                                                                                                                                                                                                                                                                                                                                                             },
    { MSGTR_GUI_FilterVideoFlash,         "*.flv"                                                                                                                                                                                                                                                                                                                                                                                                                              },
    { MSGTR_GUI_FilterFileMP3,            "*.mp3"                                                                                                                                                                                                                                                                                                                                                                                                                              },
    { MSGTR_GUI_FilterFileMP4,            "*.aac,*.f4a,*.f4b,*.f4v,*.m4a,*.m4b,*.m4v,*.mp4"                                                                                                                                                                                                                                                                                                                                                                                    },
    { MSGTR_GUI_FilterFileMPEG,           "*.m1v,*.m2v,*.mp2,*.mpe,*.mpeg,*.mpg,*.mpga,*.vdr,*.vob"                                                                                                                                                                                                                                                                                                                                                                            },
    { MSGTR_GUI_FilterFileMP2TS,          "*.bdm,*.bdmv,*.clpi,*.cpi,*.m2t,*.m2ts,*.mpl,*.mpls,*.mts,*.rec,*.trp,*.ts,*.ty"                                                                                                                                                                                                                                                                                                                                                    },
    { MSGTR_GUI_FilterMediumMatroska,     "*.mka,*.mkv"                                                                                                                                                                                                                                                                                                                                                                                                                        },
    { MSGTR_GUI_FilterMediumOgg,          "*.oga,*.ogg,*.ogm,*.ogv,*.ogx,*.spx"                                                                                                                                                                                                                                                                                                                                                                                                },
    { MSGTR_GUI_FilterMediumQuickTime,    "*.moov,*.mov,*.qt,*.qtvr"                                                                                                                                                                                                                                                                                                                                                                                                           },
    { MSGTR_GUI_FilterMediumRealNetworks, "*.ra,*.rm,*.rmvb,*.rv"                                                                                                                                                                                                                                                                                                                                                                                                              },
    { MSGTR_GUI_FilterImageCue,           "*.cue"                                                                                                                                                                                                                                                                                                                                                                                                                              },
    { MSGTR_GUI_FilterFileWav,            "*.wav"                                                                                                                                                                                                                                                                                                                                                                                                                              },
    { MSGTR_GUI_FilterMediumWindows,      "*.asf,*.wma,*.wmv"                                                                                                                                                                                                                                                                                                                                                                                                                  },
    { MSGTR_GUI_FilterFilePlaylist,       "*.asx,*.m3u,*.m3u8,*.m4u,*.mxu,*.nsc,*.pls,*.ram,*.smi,*.smil,*.sml,*.vlc,*.wax,*.wmx,*.wvx"                                                                                                                                                                                                                                                                                                                                        },
    { MSGTR_GUI_FilterAudioVideo,         "*.aac,*.ac3,*.ape,*.au,*.avi,*.divx,*.flac,*.flv,*.m2v,*.m4a,*.m4v,*.mjpg,*.mkv,*.mov,*.mp3,*.mp4,*.mpe,*.mpeg,*.mpg,*.ogg,*.rec,*.rm,*.ts,*.vob,*.wav,*.webm,*.wma,*.wmv"                                                                                                                                                                                                                                                          },
    { MSGTR_GUI_FilterAudioAll,           "*.aac,*.ac3,*.aif,*.aifc,*.aiff,*.amr,*.ape,*.au,*.awb,*.cdg,*.dsf,*.f4a,*.f4b,*.flac,*.m4a,*.m4b,*.mka,*.mp+,*.mp2,*.mp3,*.mpc,*.mpga,*.mpp,*.nsa,*.oga,*.ogg,*.pcm,*.qcp,*.ra,*.snd,*.spx,*.tak,*.voc,*.vqf,*.w64,*.wav,*.wma,*.wv,*.wvp"                                                                                                                                                                                         },
    { MSGTR_GUI_FilterVideoAll,           "*.264,*.3g2,*.3ga,*.3gp,*.3gp2,*.3gpp,*.3gpp2,*.apng,*.asf,*.avi,*.bdm,*.bdmv,*.bik,*.clpi,*.cpi,*.cpk,*.divx,*.dv,*.f4v,*.flc,*.fli,*.flv,*.m1v,*.m2t,*.m2ts,*.m2v,*.m4v,*.mjpg,*.mkv,*.moov,*.mov,*.mp2,*.mp4,*.mpe,*.mpeg,*.mpg,*.mpl,*.mpls,*.mts,*.mxf,*.nsv,*.nuv,*.ogg,*.ogm,*.ogv,*.ogx,*.pva,*.qt,*.qtvr,*.rec,*.rm,*.rmvb,*.roq,*.rv,*.smk,*.spl,*.str,*.swf,*.trp,*.ts,*.ty,*.vdr,*.viv,*.vivo,*.vob,*.webm,*.wmv,*.y4m" },
    { MSGTR_GUI_FilterFileAll,            "*"                                                                                                                                                                                                                                                                                                                                                                                                                                  },
    { NULL,                               NULL                                                                                                                                                                                                                                                                                                                                                                                                                                 }
};
int fsLastVideoAudioFilterSelected = -1;

char * const fsSubtitleFilterNames[][2] = {
    { "AQTitle (*.aqt)",                   "*.aqt"                                                      },
    { "Advanced SubStation Alpha (*.ass)", "*.ass"                                                      },
    { "JACOSub (*.jss) ",                  "*.jss"                                                      },
    { "RealText (*.rt) ",                  "*.rt"                                                       },
    { "SAMI (*.smi)",                      "*.smi"                                                      },
    { "SubRip (*.srt)",                    "*.srt"                                                      },
    { "SubStation Alpha (*.ssa)",          "*.ssa"                                                      },
    { "MicroDVD (*.sub)",                  "*.sub"                                                      },
    { "Text (*.txt)",                      "*.txt"                                                      },
    { MSGTR_GUI_FilterSubtitleUTF8,        "*.utf,*.utf-8,*.utf8"                                       },
    { MSGTR_GUI_FilterSubtitleAll,         "*.aqt,*.ass,*.jss,*.rt,*.smi,*.srt,*.ssa,*.sub,*.txt,*.utf" },
    { MSGTR_GUI_FilterFileAll,             "*"                                                          },
    { NULL,                                NULL                                                         }
};
int fsLastSubtitleFilterSelected = -1;

char * const fsAudioFileNames[][2] = {
    { MSGTR_GUI_FilterAudioDolbyPCM, "*.ac3,*.pcm"                                                                          },
    { MSGTR_GUI_FilterAudioMPEG,     "*.aac,*.f4a,*.m4a,*.mp2,*.mp3,*.mpga"                                                 },
    { MSGTR_GUI_FilterAudioMatroska, "*.mka"                                                                                },
    { MSGTR_GUI_FilterAudioOgg,      "*.oga,*.ogg,*.spx"                                                                    },
    { MSGTR_GUI_FilterAudioWav,      "*.wav"                                                                                },
    { MSGTR_GUI_FilterAudioWma,      "*.wma"                                                                                },
    { MSGTR_GUI_FilterAudioAll,      "*.aac,*.ac3,*.f4a,*.m4a,*.mka,*.mp2,*.mp3,*.mpga,*.oga,*.ogg,*.pcm,*.spx,*.wav,*.wma" },
    { MSGTR_GUI_FilterFileAll,       "*"                                                                                    },
    { NULL,                          NULL                                                                                   }
};
int fsLastAudioFilterSelected = -1;

char * const fsFontFileNames[][2] = {
#ifdef CONFIG_FREETYPE
    { MSGTR_GUI_FilterFontTTF,    "*.ttf"       },
    { MSGTR_GUI_FilterFontType1,  "*.pfb"       },
    { MSGTR_GUI_FilterFontAll,    "*.pfb,*.ttf" },
#else
    { MSGTR_GUI_FilterFontBitmap, "*.desc"      },
#endif
    { MSGTR_GUI_FilterFileAll,    "*"           },
    { NULL,                       NULL          }
};
int fsLastFontFilterSelected = -1;

char * const fsImageFilterNames[][2] = {
    { MSGTR_GUI_FilterImageAudio, "*.cue"       },
#ifdef CONFIG_LIBCDIO
    { MSGTR_GUI_FilterImageCD,    "*.cue"       },
    { MSGTR_GUI_FilterImageVCD,   "*.cue"       },
#endif
#ifdef CONFIG_DVDREAD
    { MSGTR_GUI_FilterImageDVD,   "*.ifo,*.iso" },
#endif
    { NULL,                       NULL          }
};
int fsLastImageFilterSelected = -1;

GtkWidget *fsFileNamesList;
GtkWidget *fsFNameList;
GtkWidget *FileSelector;
GdkColormap *fsColorMap;
GtkWidget *fsOk;
GtkWidget *fsUp;
GtkWidget *fsCancel;
GtkWidget *fsPathCombo;
GSList *fsPathList;
GHashTable *fsPathTable;
GtkWidget *fsFilterCombo;

GtkStyle *style;
GdkPixmap *dpixmap;
GdkBitmap *dmask;

static void fs_PersistantHistory(char *directory)
{
    unsigned int i;
    char *entry;

    if (!directory)
        return;

    for (i = 0; i < FF_ARRAY_ELEMS(fsHistory); i++)
        if (gstrcmp(fsHistory[i], directory) == 0) {
            entry = fsHistory[i];
            break;
        }

    if (i == FF_ARRAY_ELEMS(fsHistory)) {
        entry = strdup(directory);
        free(fsHistory[--i]);
    }

    for (; i; i--)
        fsHistory[i] = fsHistory[i - 1];

    fsHistory[0] = entry;
}

static gchar *get_current_dir_name_utf8(void)
{
    char *dir;
    gchar *utf8dir;

    dir     = get_current_dir_name();
    utf8dir = g_filename_display_name(dir);
    free(dir);

    return utf8dir;
}

static GSList *fs_AddPath(GSList *list, gpointer data, GtkPositionType pos)
{
    if (!g_slist_find_custom(list, data, (GCompareFunc)strcmp)) {
        if (pos == GTK_POS_TOP)
            list = g_slist_prepend(list, data);
        else
            list = g_slist_append(list, data);
    }

    return list;
}

static void fs_AddPathUtf8(const char *name, GtkPositionType pos)
{
    gchar *utf8name;

    utf8name   = g_filename_display_name(name);
    fsPathList = fs_AddPath(fsPathList, utf8name, pos);
    g_hash_table_insert(fsPathTable, strdup(utf8name), strdup(name));
}

static void clist_append_fname(GtkWidget *list, char *fname,
                               GdkPixmap *pixmap, GdkPixmap *mask)
{
    gint pos;
    gchar *str[2];

    str[0] = NULL;
    str[1] = g_filename_display_name(fname);
    pos    = gtk_clist_append(GTK_CLIST(list), str);
    gtk_clist_set_row_data_full(GTK_CLIST(list), pos, strdup(fname), free);
    gtk_clist_set_pixmap(GTK_CLIST(list), pos, 0, pixmap, mask);
    g_free(str[1]);
}

static void fs_get_pixmap(const char *ext, GdkPixmap **pixmap, GdkBitmap **mask)
{
    static GdkPixmap *apixmap, *atpixmap, *avpixmap, *fpixmap, *ipixmap, *ppixmap, *spixmap, *vpixmap;
    static GdkBitmap *amask, *atmask, *avmask, *fmask, *imask, *pmask, *smask, *vmask;
    size_t len = 0;
    char *p;
    int i;

    if (ext)
        len = strlen(ext);

    switch (fsType) {
    case FILESELECT_SUBTITLE:
        if (!spixmap)
            spixmap = gdk_pixmap_colormap_create_from_xpm_d(gtk_widget_get_window(FileSelector), fsColorMap, &smask, &style->bg[GTK_STATE_NORMAL], (gchar **)file_subtitle_xpm);
        *pixmap = spixmap;
        *mask   = smask;
        break;

    case FILESELECT_AUDIO_TRACK:
        if (!atpixmap)
            atpixmap = gdk_pixmap_colormap_create_from_xpm_d(gtk_widget_get_window(FileSelector), fsColorMap, &atmask, &style->bg[GTK_STATE_NORMAL], (gchar **)file_audio_track_xpm);
        *pixmap = atpixmap;
        *mask   = atmask;
        break;

    case FILESELECT_FONT:
        if (!fpixmap)
            fpixmap = gdk_pixmap_colormap_create_from_xpm_d(gtk_widget_get_window(FileSelector), fsColorMap, &fmask, &style->bg[GTK_STATE_NORMAL], (gchar **)file_font_xpm);
        *pixmap = fpixmap;
        *mask   = fmask;
        break;

    case FILESELECT_IMAGE:
        if (!ipixmap)
            ipixmap = gdk_pixmap_colormap_create_from_xpm_d(gtk_widget_get_window(FileSelector), fsColorMap, &imask, &style->bg[GTK_STATE_NORMAL], (gchar **)file_image_xpm);
        *pixmap = ipixmap;
        *mask   = imask;
        break;

    default:

        *pixmap = NULL;

        for (i = 0; ext && fsVideoAudioFilterNames[i][0]; i++) {
            if (strcmp(MSGTR_GUI_FilterVideoAll, fsVideoAudioFilterNames[i][0]) == 0) {
                if ((p = strstr(fsVideoAudioFilterNames[i][1], ext)) && (p[len] == ',' || p[len] == 0)) {
                    if (!vpixmap)
                        vpixmap = gdk_pixmap_colormap_create_from_xpm_d(gtk_widget_get_window(FileSelector), fsColorMap, &vmask, &style->bg[GTK_STATE_NORMAL], (gchar **)file_video_xpm);
                    *pixmap = vpixmap;
                    *mask   = vmask;
                    break;
                }
            }

            if (strcmp(MSGTR_GUI_FilterAudioAll, fsVideoAudioFilterNames[i][0]) == 0) {
                if ((p = strstr(fsVideoAudioFilterNames[i][1], ext)) && (p[len] == ',' || p[len] == 0)) {
                    if (!apixmap)
                        apixmap = gdk_pixmap_colormap_create_from_xpm_d(gtk_widget_get_window(FileSelector), fsColorMap, &amask, &style->bg[GTK_STATE_NORMAL], (gchar **)file_audio_xpm);
                    *pixmap = apixmap;
                    *mask   = amask;
                    break;
                }
            }

            if (strcmp(MSGTR_GUI_FilterFilePlaylist, fsVideoAudioFilterNames[i][0]) == 0) {
                if ((p = strstr(fsVideoAudioFilterNames[i][1], ext)) && (p[len] == ',' || p[len] == 0)) {
                    if (!ppixmap)
                        ppixmap = gdk_pixmap_colormap_create_from_xpm_d(gtk_widget_get_window(FileSelector), fsColorMap, &pmask, &style->bg[GTK_STATE_NORMAL], (gchar **)file_playlist_xpm);
                    *pixmap = ppixmap;
                    *mask   = pmask;
                    break;
                }
            }

            if (strcmp(MSGTR_GUI_FilterImageCue, fsVideoAudioFilterNames[i][0]) == 0) {
                if ((p = strstr(fsVideoAudioFilterNames[i][1], ext)) && (p[len] == ',' || p[len] == 0)) {
                    if (!ipixmap)
                        ipixmap = gdk_pixmap_colormap_create_from_xpm_d(gtk_widget_get_window(FileSelector), fsColorMap, &imask, &style->bg[GTK_STATE_NORMAL], (gchar **)file_image_xpm);
                    *pixmap = ipixmap;
                    *mask   = imask;
                    break;
                }
            }
        }

        for (i = 0; !*pixmap && ext && fsSubtitleFilterNames[i][0]; i++) {
            if ((p = strstr(fsSubtitleFilterNames[i][1], ext)) && (p[len] == ',' || p[len] == 0)) {
                if (!spixmap)
                    spixmap = gdk_pixmap_colormap_create_from_xpm_d(gtk_widget_get_window(FileSelector), fsColorMap, &smask, &style->bg[GTK_STATE_NORMAL], (gchar **)file_subtitle_xpm);
                *pixmap = spixmap;
                *mask   = smask;
                break;
            }
        }

        for (i = 0; !*pixmap && ext && fsImageFilterNames[i][0]; i++) {
            if ((p = strstr(fsImageFilterNames[i][1], ext)) && (p[len] == ',' || p[len] == 0)) {
                if (!ipixmap)
                    ipixmap = gdk_pixmap_colormap_create_from_xpm_d(gtk_widget_get_window(FileSelector), fsColorMap, &imask, &style->bg[GTK_STATE_NORMAL], (gchar **)file_image_xpm);
                *pixmap = ipixmap;
                *mask   = imask;
                break;
            }
        }

        if (!*pixmap) {
            if (!avpixmap)
                avpixmap = gdk_pixmap_colormap_create_from_xpm_d(gtk_widget_get_window(FileSelector), fsColorMap, &avmask, &style->bg[GTK_STATE_NORMAL], (gchar **)file_xpm);
            *pixmap = avpixmap;
            *mask   = avmask;
        }

        break;
    }
}

static void CheckDir(GtkWidget *list)
{
    GdkPixmap *fpixmap;
    GdkBitmap *fmask;
    struct stat fs;
    unsigned int i, j, fn;
    glob_t gg;
    gchar *filter, **fext;

    if (!fsFilter[0])
        return;

    gtk_widget_hide(list);
    gtk_clist_clear(GTK_CLIST(list));

    clist_append_fname(list, ".", dpixmap, dmask);
    clist_append_fname(list, "..", dpixmap, dmask);

    glob("*", 0, NULL, &gg);

    for (i = 0; i < gg.gl_pathc; i++) {
        if ((stat(gg.gl_pathv[i], &fs) != 0) || !S_ISDIR(fs.st_mode))
            continue;

        clist_append_fname(list, gg.gl_pathv[i], dpixmap, dmask);
    }

    for (fn = 1, i = 0; fsFilter[i]; i++)
        if (fsFilter[i] == ',')
            fn++;

    filter = g_strdup(fsFilter);
    fext   = calloc(fn, sizeof(gchar *));

    if (filter && fext) {
        for (j = 0, i = 0; filter[i]; i++) {
            if (filter[i] == '.')
                fext[j] = filter + i;

            if (filter[i] == ',') {
                filter[i] = 0;
                j++;
            }
        }

        for (i = 0; i < gg.gl_pathc; i++) {
            char *ext;

            if ((stat(gg.gl_pathv[i], &fs) != 0) || S_ISDIR(fs.st_mode))
                continue;

            ext = strrchr(gg.gl_pathv[i], '.');

            if (ext || !fext[0]) {
                for (j = 0; j < fn; j++) {
                    if (fext[j] == NULL || av_strcasecmp(fext[j], ext) == 0) {
                        fs_get_pixmap(ext, &fpixmap, &fmask);
                        clist_append_fname(list, gg.gl_pathv[i], fpixmap, fmask);
                        break;
                    }
                }
            }
        }
    }

    free(fext);
    g_free(filter);
    globfree(&gg);

    gtk_clist_set_column_width(GTK_CLIST(list), 0, 17);
    gtk_clist_set_row_height(GTK_CLIST(list), 17);
    gtk_widget_show(list);
}

/* ----------------------------------------------- */

static void fs_fsFilterCombo_changed(GtkEditable *editable,
                                     gpointer user_data)
{
    const char *str;
    int i;

    (void)editable;

    str = gtk_entry_get_text(gtkEntry(user_data));

    switch (fsType) {
    case FILESELECT_VIDEO_AUDIO:

        for (i = 0; fsVideoAudioFilterNames[i][0]; i++)
            if (strcmp(str, _(fsVideoAudioFilterNames[i][0])) == 0) {
                fsFilter = fsVideoAudioFilterNames[i][1];
                fsLastVideoAudioFilterSelected = i;
                break;
            }

        break;

    case FILESELECT_SUBTITLE:

        for (i = 0; fsSubtitleFilterNames[i][0]; i++)
            if (strcmp(str, _(fsSubtitleFilterNames[i][0])) == 0) {
                fsFilter = fsSubtitleFilterNames[i][1];
                fsLastSubtitleFilterSelected = i;
                break;
            }

        break;

    case FILESELECT_AUDIO_TRACK:

        for (i = 0; fsAudioFileNames[i][0]; i++)
            if (strcmp(str, _(fsAudioFileNames[i][0])) == 0) {
                fsFilter = fsAudioFileNames[i][1];
                fsLastAudioFilterSelected = i;
                break;
            }

        break;

    case FILESELECT_FONT:

        for (i = 0; fsFontFileNames[i][0]; i++)
            if (strcmp(str, _(fsFontFileNames[i][0])) == 0) {
                fsFilter = fsFontFileNames[i][1];
                fsLastFontFilterSelected = i;
                break;
            }

        break;

    case FILESELECT_IMAGE:

        for (i = 0; fsImageFilterNames[i][0]; i++)
            if (strcmp(str, _(fsImageFilterNames[i][0])) == 0) {
                fsFilter = fsImageFilterNames[i][1];
                fsLastImageFilterSelected = i;
                break;
            }

        break;

    default:

        return;
    }

    CheckDir(fsFNameList);
}

static void fs_fsPathCombo_changed(GtkEditable *editable,
                                   gpointer user_data)
{
    const unsigned char *str;
    gchar *dirname;

    (void)editable;

    str     = gtk_entry_get_text(gtkEntry(user_data));
    dirname = g_hash_table_lookup(fsPathTable, str);

    if (chdir(dirname ? (const unsigned char *)dirname : str) != -1)
        CheckDir(fsFNameList);
}

static gboolean fs_fsFNameList_event(GtkWidget *widget,
                                     GdkEvent *event,
                                     gpointer user_data)
{
    GdkEventButton *bevent;
    gint row, col;

    (void)user_data;

    bevent = (GdkEventButton *)event;

    if (event->type == GDK_BUTTON_RELEASE && bevent->button == 2) {
        if (gtk_clist_get_selection_info(GTK_CLIST(widget), bevent->x, bevent->y, &row, &col)) {
            fsSelectedFile = gtk_clist_get_row_data(GTK_CLIST(widget), row);
            gtk_button_clicked(GTK_BUTTON(fsOk));

            return TRUE;
        }
    }

    return FALSE;
}

static void fs_fsFNameList_select_row(GtkCList *clist, gint row, gint column,
                                      GdkEvent *event, gpointer user_data)
{
    (void)column;
    (void)user_data;

    fsCurrFNameListSelected = row;
    fsSelectedFile = gtk_clist_get_row_data(clist, row);

    if (event && event->type == GDK_BUTTON_PRESS)
        gtk_button_clicked(GTK_BUTTON(fsOk));
}

static void fs_Up_released(GtkButton *button, gpointer user_data)
{
    gchar *utf8dir;

    (void)button;
    (void)user_data;

    chdir("..");
    CheckDir(fsFNameList);
    utf8dir = get_current_dir_name_utf8();
    gtk_entry_set_text(gtkEntry(fsPathCombo), utf8dir);
    g_free(utf8dir);
    fsSelectedFile = fsThisDir;

    return;
}

static void fs_Cancel_released(GtkButton *button, gpointer user_data)
{
    (void)button;
    (void)user_data;

    if (fsMedium)
        fsLastFNameListSelected = fsCurrFNameListSelected;

    gtk_widget_destroy(FileSelector);
}

static void fs_Ok_released(GtkButton *button, gpointer user_data)
{
    char *fsSelectedDirectory;
    int type = STREAMTYPE_FILE, ev = evPlay;
    gchar *selected;

    (void)button;
    (void)user_data;

    if (!fsSelectedFile)
        fsSelectedFile = ".";

    if (chdir(fsSelectedFile) == 0) {
        gchar *utf8dir;

        CheckDir(fsFNameList);
        utf8dir = get_current_dir_name_utf8();
        gtk_entry_set_text(gtkEntry(fsPathCombo), utf8dir);
        g_free(utf8dir);
        gtk_widget_grab_focus(fsFNameList);
        fsSelectedFile = fsThisDir;

        return;
    }

    fsSelectedDirectory = get_current_dir_name();

    switch (fsType) {
    case FILESELECT_VIDEO_AUDIO:

        if (strcmp(fsVideoAudioFilterNames[fsLastVideoAudioFilterSelected][0], MSGTR_GUI_FilterFilePlaylist) == 0)
            type = STREAMTYPE_PLAYLIST;
audio:
        uiSetFile(fsSelectedDirectory, fsSelectedFile, type);
        selected = g_strconcat(fsSelectedDirectory, "/", fsSelectedFile, NULL);

        if (selected) {
            listMgr(PLAYLIST_DELETE, 0);
            add_to_gui_playlist(selected, PLAYLIST_ITEM_APPEND);
            g_free(selected);
        }

        guiInfo.MediumChanged = GUI_MEDIUM_NEW;
        sub_fps = 0;
        fs_PersistantHistory(fsSelectedDirectory);
        break;

    case FILESELECT_SUBTITLE:
        setddup(&guiInfo.SubtitleFilename, fsSelectedDirectory, fsSelectedFile);
        break;

    case FILESELECT_AUDIO_TRACK:
        setddup(&guiInfo.AudioFilename, fsSelectedDirectory, fsSelectedFile);
        break;

    case FILESELECT_FONT:
        setddup(&font_name, fsSelectedDirectory, fsSelectedFile);
        mplayer(MPLAYER_LOAD_FONT, 0, 0);

        if (Preferences)
            gtk_entry_set_text(GTK_ENTRY(prEFontName), font_name);

        break;

    case FILESELECT_IMAGE:

        if (strcmp(fsImageFilterNames[fsLastImageFilterSelected][0], MSGTR_GUI_FilterImageAudio) == 0) {
            uiLoadPlay = True;
            goto audio;
        }

        if (strcmp(fsImageFilterNames[fsLastImageFilterSelected][0], MSGTR_GUI_FilterImageCD) == 0)
            ev = evPlayCD;
        else if (strcmp(fsImageFilterNames[fsLastImageFilterSelected][0], MSGTR_GUI_FilterImageVCD) == 0)
            ev = evPlayVCD;
        else if (strcmp(fsImageFilterNames[fsLastImageFilterSelected][0], MSGTR_GUI_FilterImageDVD) == 0)
            ev = evPlayDVD;

        if (ev == evPlayDVD && gstrcmp(strrchr(fsSelectedFile, '.'), ".ifo") == 0)
            fsSelectedFile = "";

        uiUnsetFile();
        setddup(&guiInfo.ImageFilename, fsSelectedDirectory, fsSelectedFile);
        fs_PersistantHistory(fsSelectedDirectory);
        uiLoadPlay = True;
        break;
    }

    free(fsSelectedDirectory);

    fs_Cancel_released(NULL, NULL);

    if (fsType == FILESELECT_FONT)
        return;

    if (uiLoadPlay) {
        uiLoadPlay = False;
        uiEvent(ev, 0);
    } else
        gui(GUI_SET_STATE, (void *)GUI_STOP);

    if (fsType == FILESELECT_SUBTITLE)
        mplayerLoadSubtitle(guiInfo.SubtitleFilename);
}

/**
 * @brief Handle the escape, return and backspace key depending on the
 *        @a widget it has been pressed on.
 *
 * @param widget object which received the signal
 * @param event GdkEventKey which triggered the signal
 * @param user_data user data set when the signal handler was connected
 *
 * @return TRUE to stop other handlers from being invoked for the event or
 *         FALSE to propagate the event further
 */
static gboolean fs_key_release_event(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    (void)user_data;

    switch (event->key.keyval) {
    case GDK_KEY_Return:

        if (GTK_IS_SCROLLED_WINDOW(widget))
            gtk_button_clicked(GTK_BUTTON(fsOk));
        else if (GTK_IS_COMBO_BOX(widget))
            gtk_widget_grab_focus(fsFNameList);

        break;

    case GDK_KEY_BackSpace:

        if (GTK_IS_SCROLLED_WINDOW(widget)) {
            gtk_button_clicked(GTK_BUTTON(fsUp));
            gtk_widget_grab_focus(fsFNameList);
        }

        break;
    }

    return FALSE;
}

static void fs_PathClear(void)
{
    g_hash_table_destroy(fsPathTable);

WARN_OFF(cast_function_type)
    g_slist_foreach(fsPathList, (GFunc)g_free, NULL);  // deliberate cast between incompatible function types
WARN_ON
    g_slist_free(fsPathList);
    fsPathList = NULL;
}

static gboolean fs_Delete(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    (void)widget;
    (void)event;
    (void)user_data;

    fs_Cancel_released(NULL, NULL); // press cancel

    return TRUE;
}

static void fs_Destroy(void)
{
    fs_PathClear();
    gtk_widget_destroyed(FileSelector, &FileSelector);
}

static GtkWidget *CreateFileSelect(void)
{
    gint x, y;
    GtkAccelGroup *accel_group;
    GtkWidget *vbox4;
    GtkWidget *hbox4;
    GtkWidget *vseparator1;
    GtkWidget *hbox6;
    GtkWidget *fsFNameListWindow;
    GtkWidget *hbuttonbox3;

    GtkWidget *up;
    GdkPixbuf *pixbuf;

    accel_group = gtk_accel_group_new();

    FileSelector = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(FileSelector, 512, 440);
    gtk_window_set_position(GTK_WINDOW(FileSelector), GTK_WIN_POS_CENTER);
    gtk_window_get_position(GTK_WINDOW(FileSelector), &x, &y);
    gtk_window_move(GTK_WINDOW(FileSelector), x, y * 5 / 6);
    gtk_window_set_wmclass(GTK_WINDOW(FileSelector), "FileSelector", MPlayer);
    fsColorMap = gdk_colormap_get_system();

    gtk_widget_realize(FileSelector);
    gtkAddIcon(FileSelector);

    style   = gtk_widget_get_style(FileSelector);
    dpixmap = gdk_pixmap_colormap_create_from_xpm_d(gtk_widget_get_window(FileSelector), fsColorMap, &dmask, &style->bg[GTK_STATE_NORMAL], (gchar **)dir_xpm);

    vbox4 = gtkAddVBox(gtkAddDialogFrame(FileSelector), 0);
    hbox4 = gtkAddHBox(vbox4, 1);

    fsPathCombo = gtkAddCombo(hbox4);

    vseparator1 = gtk_vseparator_new();
    gtk_widget_show(vseparator1);
    gtk_box_pack_start(GTK_BOX(hbox4), vseparator1, FALSE, TRUE, 0);

    pixbuf = gdk_pixbuf_new_from_inline(-1, dir_up_png, FALSE, NULL);
    up     = gtk_image_new_from_pixbuf(pixbuf);
    g_object_unref(pixbuf);

    fsUp = gtk_button_new();
    gtk_button_set_image(GTK_BUTTON(fsUp), up);
    gtk_widget_show(fsUp);
    gtk_box_pack_start(GTK_BOX(hbox4), fsUp, FALSE, FALSE, 0);
    gtk_widget_set_size_request(fsUp, 60, -1);

    gtkAddHSeparator(vbox4);

    hbox6 = gtkAddHBox(NULL, 0);
    gtk_box_pack_start(GTK_BOX(vbox4), hbox6, TRUE, TRUE, 0);

    fsFNameListWindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_show(fsFNameListWindow);
    gtk_box_pack_start(GTK_BOX(hbox6), fsFNameListWindow, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(fsFNameListWindow), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    fsFNameList = gtk_clist_new(2);
    gtk_container_add(GTK_CONTAINER(fsFNameListWindow), fsFNameList);
    gtk_clist_set_column_width(GTK_CLIST(fsFNameList), 0, 80);
    gtk_clist_set_selection_mode(GTK_CLIST(fsFNameList), GTK_SELECTION_BROWSE);
    gtk_clist_column_titles_hide(GTK_CLIST(fsFNameList));
    gtk_clist_set_shadow_type(GTK_CLIST(fsFNameList), GTK_SHADOW_ETCHED_OUT);

    gtkAddHSeparator(vbox4);

    fsFilterCombo = gtkAddCombo(NULL);
    g_object_ref(fsFilterCombo);
    g_object_set_data_full(G_OBJECT(FileSelector), "fsFilterCombo", fsFilterCombo, (GDestroyNotify)g_object_unref);
    gtk_box_pack_start(GTK_BOX(vbox4), fsFilterCombo, FALSE, FALSE, 0);
    gtkEntrySetEditable(fsFilterCombo, FALSE);

    gtkAddHSeparator(vbox4);

    hbuttonbox3 = gtkAddHButtonBox(vbox4);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(hbuttonbox3), GTK_BUTTONBOX_END);
    gtk_box_set_spacing(GTK_BOX(hbuttonbox3), 10);

    fsOk     = gtkAddButton(MSGTR_GUI_Ok, hbuttonbox3);
    fsCancel = gtkAddButton(MSGTR_GUI_Cancel, hbuttonbox3);

    gtk_widget_add_accelerator(fsCancel, "clicked", accel_group, GDK_KEY_Escape, 0, GTK_ACCEL_VISIBLE);

    g_signal_connect(G_OBJECT(FileSelector), "delete-event", G_CALLBACK(fs_Delete), NULL);
    g_signal_connect(G_OBJECT(FileSelector), "destroy", G_CALLBACK(fs_Destroy), NULL);
    g_signal_connect(G_OBJECT(fsFNameListWindow), "key-release-event", G_CALLBACK(fs_key_release_event), NULL);

    g_signal_connect(G_OBJECT(fsFilterCombo), "changed", G_CALLBACK(fs_fsFilterCombo_changed), fsFilterCombo);
    g_signal_connect(G_OBJECT(fsFilterCombo), "key-release-event", G_CALLBACK(fs_key_release_event), NULL);
    g_signal_connect(G_OBJECT(fsPathCombo), "changed", G_CALLBACK(fs_fsPathCombo_changed), fsPathCombo);
    g_signal_connect(G_OBJECT(fsPathCombo), "key-release-event", G_CALLBACK(fs_key_release_event), NULL);
    g_signal_connect(G_OBJECT(fsUp), "clicked", G_CALLBACK(fs_Up_released), fsFNameList);
    g_signal_connect(G_OBJECT(fsOk), "clicked", G_CALLBACK(fs_Ok_released), NULL);
    g_signal_connect(G_OBJECT(fsCancel), "clicked", G_CALLBACK(fs_Cancel_released), NULL);
    g_signal_connect(G_OBJECT(fsFNameList), "select-row", G_CALLBACK(fs_fsFNameList_select_row), NULL);
    g_signal_connect(G_OBJECT(fsFNameList), "event", G_CALLBACK(fs_fsFNameList_event), NULL);

    gtk_window_add_accel_group(GTK_WINDOW(FileSelector), accel_group);

    return FileSelector;
}

void ShowFileSelector(int type)
{
    int i, k, c = 1;
    char *filepath = NULL, *dir = NULL;
    const gchar *fname;
    struct stat f;
    GSList *list;

    if (FileSelector)
        gtkRaise(FileSelector);
    else
        FileSelector = CreateFileSelect();

    fsType   = type;
    fsMedium = (fsType == FILESELECT_VIDEO_AUDIO || fsType == FILESELECT_SUBTITLE || fsType == FILESELECT_AUDIO_TRACK || fsType == FILESELECT_IMAGE);

    gtk_list_store_clear(GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(fsFilterCombo))));

    switch (type) {
    case FILESELECT_VIDEO_AUDIO:
        gtk_window_set_title(GTK_WINDOW(FileSelector), MSGTR_GUI_SelectFile);

        for (i = 0; fsVideoAudioFilterNames[i][0]; i++)
            if ((strcmp(fsVideoAudioFilterNames[i][0], MSGTR_GUI_FilterFilePlaylist) != 0) || allow_playlist_parsing)
                gtk_combo_box_append_text(GTK_COMBO_BOX(fsFilterCombo), _(fsVideoAudioFilterNames[i][0]));

        k = fsLastVideoAudioFilterSelected;
        gtk_entry_set_text(gtkEntry(fsFilterCombo), _(fsVideoAudioFilterNames[k >= 0 ? k : i - 4][0]));
        break;

    case FILESELECT_SUBTITLE:
        gtk_window_set_title(GTK_WINDOW(FileSelector), MSGTR_GUI_SelectSubtitle);

        for (i = 0; fsSubtitleFilterNames[i][0]; i++)
            gtk_combo_box_append_text(GTK_COMBO_BOX(fsFilterCombo), _(fsSubtitleFilterNames[i][0]));

        k = fsLastSubtitleFilterSelected;
        gtk_entry_set_text(gtkEntry(fsFilterCombo), _(fsSubtitleFilterNames[k >= 0 ? k : i - 2][0]));
        filepath = guiInfo.SubtitleFilename;
        break;

    case FILESELECT_AUDIO_TRACK:
        gtk_window_set_title(GTK_WINDOW(FileSelector), MSGTR_GUI_SelectAudioFile);

        for (i = 0; fsAudioFileNames[i][0]; i++)
            gtk_combo_box_append_text(GTK_COMBO_BOX(fsFilterCombo), _(fsAudioFileNames[i][0]));

        k = fsLastAudioFilterSelected;
        gtk_entry_set_text(gtkEntry(fsFilterCombo), _(fsAudioFileNames[k >= 0 ? k : i - 2][0]));
        filepath = guiInfo.AudioFilename;
        break;

    case FILESELECT_FONT:
        gtk_window_set_title(GTK_WINDOW(FileSelector), MSGTR_GUI_SelectFont);

        for (i = 0; fsFontFileNames[i][0]; i++)
            gtk_combo_box_append_text(GTK_COMBO_BOX(fsFilterCombo), _(fsFontFileNames[i][0]));

        k = fsLastFontFilterSelected;
        gtk_entry_set_text(gtkEntry(fsFilterCombo), _(fsFontFileNames[k >= 0 ? k : i - 2][0]));
        filepath = font_name;
        break;

    case FILESELECT_IMAGE:
        gtk_window_set_title(GTK_WINDOW(FileSelector), MSGTR_GUI_SelectImage);

        for (i = 0; fsImageFilterNames[i][0]; i++)
            gtk_combo_box_append_text(GTK_COMBO_BOX(fsFilterCombo), _(fsImageFilterNames[i][0]));

        k = fsLastImageFilterSelected;
        gtk_entry_set_text(gtkEntry(fsFilterCombo), _(fsImageFilterNames[k >= 0 ? k : 0][0]));
        break;
    }

    if (filepath && filepath[0] && !strstr(filepath, "://")) {
        dir = strdup(filepath);

        do {
            char *c = strrchr(dir, '/');

            if ((stat(dir, &f) == 0) && S_ISDIR(f.st_mode))
                break;

            if (c)
                *c = 0;
        } while (strrchr(dir, '/'));

        if (!dir[0])
            nfree(dir);
    }

    if (fsPathList)
        fs_PathClear();

    fsPathTable = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);

    if (fsMedium) {
        unsigned int i;

        for (i = 0; i < FF_ARRAY_ELEMS(fsHistory); i++)
            if (fsHistory[i]) {
                fname = cfg_old_filename_from_utf8(fsHistory[i]);
                fs_AddPathUtf8(fname, GTK_POS_BOTTOM);

                if (c)
                    c = gstrcmp(dir, fname);
            }
    }

    if (c && dir)
        fs_AddPathUtf8(dir, GTK_POS_TOP);

    free(dir);
    fname = getenv("HOME");

    if (fname)
        fs_AddPathUtf8(fname, GTK_POS_BOTTOM);
    else
        fsPathList = fs_AddPath(fsPathList, g_strdup("/home"), GTK_POS_BOTTOM);

    if (stat("/media", &f) == 0)
        fsPathList = fs_AddPath(fsPathList, g_strdup("/media"), GTK_POS_BOTTOM);

    if (stat("/mnt", &f) == 0)
        fsPathList = fs_AddPath(fsPathList, g_strdup("/mnt"), GTK_POS_BOTTOM);

    fsPathList = fs_AddPath(fsPathList, g_strdup("/"), GTK_POS_BOTTOM);

    list = fsPathList;
    gtk_list_store_clear(GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(fsPathCombo))));

    while (list) {
        gtk_combo_box_append_text(GTK_COMBO_BOX(fsPathCombo), list->data);
        list = list->next;
    }

    gtk_combo_box_set_active(GTK_COMBO_BOX(fsPathCombo), 0);

    gtk_widget_grab_focus(fsFNameList);

    if (fsMedium) {
        if (fsLastFNameListSelected + 1 > ((GtkCList *)fsFNameList)->rows)
            fsLastFNameListSelected = 0;

        ((GtkCList *)fsFNameList)->focus_row = fsLastFNameListSelected;
        gtk_clist_select_row(GTK_CLIST(fsFNameList), fsLastFNameListSelected, 1);
    }

    gtk_widget_show(FileSelector);
}
