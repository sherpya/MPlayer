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

#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interface.h"
#include "app/app.h"
#include "app/cfg.h"
#include "app/gui.h"
#include "dialog/dialog.h"
#include "skin/skin.h"
#include "ui/actions.h"
#include "ui/ui.h"
#include "util/list.h"
#include "util/mem.h"
#include "util/misc.h"
#include "util/string.h"
#include "wm/ws.h"
#include "wm/wsxdnd.h"

#include "access_mpcontext.h"
#include "codec-cfg.h"
#include "config.h"
#include "help_mp.h"
#include "mixer.h"
#include "mp_msg.h"
#include "mpcommon.h"
#include "mplayer.h"
#include "path.h"
#include "input/input.h"
#include "libaf/equalizer.h"
#include "libavutil/common.h"
#include "libmpcodecs/ad.h"
#include "libmpcodecs/dec_audio.h"
#include "libmpcodecs/dec_video.h"
#include "libmpcodecs/vd.h"
#include "libmpcodecs/vf.h"
#include "libmpdemux/demuxer.h"
#include "libvo/video_out.h"
#include "libvo/x11_common.h"
#include "osdep/timer.h"
#include "stream/stream.h"
#ifdef CONFIG_DVDREAD
#include "stream/stream_dvd.h"
#endif
#include "sub/font_load.h"
#include "sub/sub.h"
#include "sub/subreader.h"

/**
 * @brief Initialize interface data.
 */
guiInterface_t guiInfo = {
    .StreamType   = STREAMTYPE_DUMMY,
    .Volume       = 50.0f,
    .Balance      = 50.0f,
    .LastVolume   = -1.0f,
    .PlaylistNext = True
};

static int skin;
static int current_volume;
static int guiInitialized;
static int orig_fontconfig;
static struct {
    int changed;
    char *name;
} orig_demuxer;

/**
 * @brief Set option 'fontconfig' depending on #font_name.
 */
static void set_fontconfig(void)
{
    font_fontconfig = (font_name && strchr(font_name, '/') ? -1 : orig_fontconfig);
}

/**
 * @brief Add a video filter
 *        (or change the parameter/value pairs of an existing one).
 *
 * @param vf video filter to be added or changed
 * @param argvf pointer to an array of (new) parameter/value pairs
 */
static void add_vf(const char *vf, const char * const *argvf)
{
    if (vf_settings) {
        int i = 0;

        while (vf_settings[i].name) {
            if (strcmp(vf_settings[i].name, vf) == 0)
                break;

            i++;
        }

        if (vf_settings[i].name) {
            listFree(&vf_settings[i].attribs);
            vf_settings[i].attribs = listDup(argvf);
        } else {
            void *settings = realloc(vf_settings, (i + 2) * sizeof(*vf_settings));

            if (!settings)
                return;

            vf_settings = settings;
            vf_settings[i].name    = strdup(vf);
            vf_settings[i].attribs = listDup(argvf);
            memset(&vf_settings[i + 1], 0, sizeof(*vf_settings));
        }
    } else {
        vf_settings = calloc(2, sizeof(*vf_settings));

        if (!vf_settings)
            return;

        vf_settings[0].name    = strdup(vf);
        vf_settings[0].attribs = listDup(argvf);
    }

    mp_msg(MSGT_GPLAYER, MSGL_INFO, _(MSGTR_GUI_MSG_AddingVideoFilter), vf);
}

/**
 * @brief Get a video filter's array of parameter/value pairs.
 *
 * @param vf video filter in question
 *
 * @return pointer to the array of parameter/value pairs
 */
static char **get_vf(const char *vf)
{
    char **attribs = NULL;

    if (vf_settings) {
        int i = 0;

        while (vf_settings[i].name) {
            if (strcmp(vf_settings[i].name, vf) == 0) {
                attribs = vf_settings[i].attribs;
                break;
            }

            i++;
        }
    }

    return attribs;
}

/**
 * @brief Remove a video filter.
 *
 * @param vf video filter to be removed
 */
static void remove_vf(char *vf)
{
    if (vf_settings) {
        int i = 0;

        while (vf_settings[i].name) {
            if (strcmp(vf_settings[i].name, vf) == 0) {
                int j;

                mp_msg(MSGT_GPLAYER, MSGL_INFO, _(MSGTR_GUI_MSG_RemovingVideoFilter), vf);

                free(vf_settings[i].name);
                listFree(&vf_settings[i].attribs);

                j = i + 1;

                while (vf_settings[j].name)
                    j++;

                memmove(&vf_settings[i], &vf_settings[i + 1], (j - i) * sizeof(*vf_settings));

                break;
            }

            i++;
        }
    }
}

/**
 * @brief Reset the audio, video and subtitles stream IDs.
 */
void reset_stream_ids(void)
{
    audio_id  = -1;
    video_id  = -1;
    dvdsub_id = -1;
    vobsub_id = -1;
}

/* MPlayer -> GUI */

/**
 * @brief Initialize and start the GUI.
 */
void guiInit(void)
{
    char **argvf;
    int ret;
    plItem *playlist;

    mp_msg(MSGT_GPLAYER, MSGL_V, "GUI init.\n");

#ifdef ENABLE_NLS
    setlocale(LC_MESSAGES, "");
    bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);
#endif

    /* check options */

    if (!cdrom_device)
        cdrom_device = strdup(DEFAULT_CDROM_DEVICE);
    if (!dvd_device)
        dvd_device = strdup(DEFAULT_DVD_DEVICE);
#ifdef CONFIG_DXR3
    if (!gtkDXR3Device)
        gtkDXR3Device = strdup("/dev/em8300-0");
#endif

    if (stream_cache_size > 0) {
        gtkCacheOn   = True;
        gtkCacheSize = stream_cache_size;
    } else if (stream_cache_size == 0)
        gtkCacheOn = False;

    if (autosync && (autosync != gtkAutoSync)) {
        gtkAutoSyncOn = True;
        gtkAutoSync   = autosync;
    }

    gtkASS.enabled       = ass_enabled;
    gtkASS.use_margins   = ass_use_margins;
    gtkASS.top_margin    = ass_top_margin;
    gtkASS.bottom_margin = ass_bottom_margin;

    argvf = get_vf("rotate");
    guiInfo.Rotation = (argvf && argvf[1] ? atoi(argvf[1]) : -1);

    /* initialize graphical user interfaces */

    wsInit(mDisplay);
    gtkInit(mDisplayName);

    /* load skin */

    skinDirInHome = get_path("skins");
    skinDirInData = MPLAYER_DATADIR "/skins";

    mp_msg(MSGT_GPLAYER, MSGL_DBG2, "[interface] skin directory #1: %s\n", skinDirInHome);
    mp_msg(MSGT_GPLAYER, MSGL_DBG2, "[interface] skin directory #2: %s\n", skinDirInData);

    skin = (skinName != NULL);

    if (!skin)
        skinName = strdup("default");

    ret = skinRead(skinName);

    if (ret == -1 && strcmp(skinName, "default") != 0) {
        mp_msg(MSGT_GPLAYER, MSGL_WARN, _(MSGTR_GUI_MSG_SkinCfgSelectedNotFound), skinName);

        setdup(&skinName, "default");
        ret = skinRead(skinName);
    }

    switch (ret) {
    case -1:
        if (skin) {
            gmp_msg(MSGT_GPLAYER, MSGL_FATAL, _(MSGTR_GUI_MSG_SkinCfgNotFound), skinName);
            mplayer(MPLAYER_EXIT_GUI, EXIT_ERROR, 0);
        } else {
            if (skinRead("Noskin") != 0)
                mplayer(MPLAYER_EXIT_GUI, EXIT_ERROR, 0);

            gtkMessageBox(MSGBOX_WARNING | MSGBOX_WAIT, _(MSGTR_GUI_MSG_NoSkinInstalled));
            setdup(&skinName, "");
            break;
        }

    case -2:
        gmp_msg(MSGT_GPLAYER, MSGL_FATAL, _(MSGTR_GUI_MSG_SkinCfgError), skinName);
        mplayer(MPLAYER_EXIT_GUI, EXIT_ERROR, 0);
    }

    /* initialize windows */

    if (gui_save_pos) {
        if (gui_main_pos_x != -3)
            guiApp.main.x = gui_main_pos_x;
        if (gui_main_pos_y != -3)
            guiApp.main.y = gui_main_pos_y;
        if (gui_video_pos_x != -3)
            guiApp.video.x = gui_video_pos_x;
        if (gui_video_pos_y != -3)
            guiApp.video.y = gui_video_pos_y;
    }

    if (WinID > 0) {
        guiApp.videoWindow.Parent = WinID;
        guiApp.video.x = 0;
        guiApp.video.y = 0;
    }

    if (guiWinID >= 0)
        guiApp.mainWindow.Parent = guiWinID;

    uiMainInit();      // main window must be first!
    uiVideoInit();     // video window must be second!
    uiPlaybarInit();
    uiMenuInit();

    WinID = (Window)guiApp.videoWindow.WindowID;

    btnValue(evSetVolume, &guiInfo.Volume);
    btnValue(evSetBalance, &guiInfo.Balance);
    btnValue(evSetMoviePosition, &guiInfo.Position);

    // skin demands usage of current volume
    if (guiInfo.Volume < 0.0f) {
        guiInfo.Volume = 0.0f;
        current_volume = True;
    }

    if (guiInfo.Position)
        uiEvent(evSetMoviePosition, guiInfo.Position);

    wsWindowVisibility(&guiApp.mainWindow, wsShowWindow);

    if (gtkShowVideoWindow) {
        wsWindowVisibility(&guiApp.videoWindow, wsShowWindow);

        guiInfo.VideoWindow = True;

        if (fullscreen)
            uiFullScreen();
    } else
        wsWindowBackground(&guiApp.videoWindow, 0, 0, 0);

    if (fullscreen) {
        guiApp.videoWindow.isFullScreen = True;
        btnSet(evFullScreen, btnPressed);
    }

    guiInfo.Playing = GUI_STOP;

    playlist = listMgr(PLAYLIST_ITEM_GET_CURR, 0);

    if (playlist && !filename) {
        uiSetFile(playlist->path, playlist->name, STREAMTYPE_FILE);
        guiInfo.Tracks = (uintptr_t)listMgr(PLAYLIST_ITEM_GET_POS, 0);
        guiInfo.Track  = 1;
        filename       = NULL; // don't start playing
    }

    orig_fontconfig = font_fontconfig;
    set_fontconfig();

    guiInitialized = True;
}

/**
 * @brief Stop and finalize the GUI.
 */
void guiDone(void)
{
    if (guiInitialized) {
        if (gui_save_pos) {
            gui_main_pos_x  = guiApp.mainWindow.X;
            gui_main_pos_y  = guiApp.mainWindow.Y;
            gui_video_pos_x = guiApp.videoWindow.X;
            gui_video_pos_y = guiApp.videoWindow.Y;
        }

        ass_enabled       = gtkASS.enabled;
        ass_use_margins   = gtkASS.use_margins;
        ass_top_margin    = gtkASS.top_margin;
        ass_bottom_margin = gtkASS.bottom_margin;

        cfg_write();

        if (guiApp.menuIsPresent)
            uiMenuDone();
        if (guiApp.playbarIsPresent)
            uiPlaybarDone();

        uiVideoDone();
        uiMainDone();

        wsDone();
    }

    uiUnsetFile();
    listMgr(PLAYLIST_DELETE, 0);
    listMgr(URLLIST_DELETE, 0);
    appFreeStruct();
    free(guiIcon.collection);

    if (gui_conf) {
        m_config_free(gui_conf);
        gui_conf = NULL;
    }

    mp_msg(MSGT_GPLAYER, MSGL_V, "GUI done.\n");
}

/**
 * @brief Issue a command to the GUI.
 *
 * @note The GUI is controlled by giving it commands.
 *
 * @param what command to be performed
 * @param data pointer to data needed for the command
 *
 * @return #True (ok) or #False (error)
 */
int gui(int what, void *data)
{
    static float last_balance    = -1.0f;
    static int last_playlistnext = True;
#ifdef CONFIG_DVDREAD
    dvd_priv_t *dvd;
#endif
    int idata = (intptr_t)data, msg, state, replay_gain;
    stream_t *stream = NULL;
    sh_audio_t *sh_audio;
    mixer_t *mixer;
    float l, r, b;
    plItem *next = NULL;

    switch (what) {
    case GUI_SET_CONTEXT:

        guiInfo.mpcontext = data;
        break;

    case GUI_SET_STATE:

        switch (idata) {
        case GUI_STOP:
        case GUI_PLAY:
// if ( !gtkShowVideoWindow ) wsWindowVisibility( &guiApp.videoWindow,wsHideWindow );
        case GUI_PAUSE:
            guiInfo.Playing = idata;
            break;
        }

        uiState();
        break;

    case GUI_REDRAW:

        uiEvent(ivRedraw, 0);

        if (!guiInfo.Playing || !guiInfo.VideoWindow)
            wsEvents();
        /* else it's handled by the vo driver calling GUI_HANDLE_X_EVENT */

        wsMouseAutohide();
        gtkEvents();

        if (uiCueCheckNext(&last_playlistnext) == False)
            uiCueSetTitle();

        break;

    case GUI_RUN_COMMAND:

        mp_msg(MSGT_GPLAYER, MSGL_DBG2, "[interface] GUI_RUN_COMMAND: %d\n", idata);

        switch (idata) {
        case MP_CMD_VO_FULLSCREEN:
            uiEvent(evFullScreen, True);
            break;

        case MP_CMD_PLAY_TREE_STEP:
            uiEvent(evNext, 0);
            break;

        case -MP_CMD_PLAY_TREE_STEP:
            uiEvent(evPrev, 0);
            break;

        case MP_CMD_STOP:
            uiEvent(evStop, 0);
            break;

        case MP_CMD_QUIT:
            uiEvent(evExit, 0);
            break;
        }

        break;

    case GUI_RUN_MESSAGE:

        mp_msg(MSGT_GPLAYER, MSGL_DBG2, "[interface] GUI_RUN_MESSAGE: %s\n", (const char *)data);
        msg = appFindMessage((const char *)data);

        if ((msg == evMenu) || appFindItem(msg))
            uiEvent(msg, 0);

        break;

    case GUI_PREPARE:

        uiEvent(ivRedraw, True);
        wsMouseVisibility(&guiApp.videoWindow, wsHideMouseCursor);
        usec_sleep(20000);
        wsEvents();

        if (guiInfo.MediumChanged == GUI_MEDIUM_NEW) {
            reset_stream_ids();
            stream_cache_size = -1;
            autosync  = 0;
            force_fps = 0;
        }

        if (guiInfo.Filename && (gstrcmp(strrchr(guiInfo.Filename, '.'), ".cue") == 0))
            guiInfo.StreamType = STREAMTYPE_BINCUE;

        switch (guiInfo.StreamType) {
        case STREAMTYPE_FILE:
        case STREAMTYPE_STREAM:
            filename = guiInfo.Filename;
            break;

        case STREAMTYPE_CDDA:
        {
            char tmp[512];

            snprintf(tmp, sizeof(tmp), "cdda://%d%s%s", guiInfo.Track, guiInfo.ImageFilename ? "/" : "", guiInfo.ImageFilename ? guiInfo.ImageFilename : "");
            uiSetFile(NULL, tmp, SAME_STREAMTYPE);
        }
        break;

        case STREAMTYPE_VCD:
        {
            char tmp[512];

            snprintf(tmp, sizeof(tmp), "vcd://%d%s%s", guiInfo.Track, guiInfo.ImageFilename ? "/" : "", guiInfo.ImageFilename ? guiInfo.ImageFilename : "");
            uiSetFile(NULL, tmp, SAME_STREAMTYPE);
        }
        break;

        case STREAMTYPE_DVD:
        {
            char tmp[512];

            snprintf(tmp, sizeof(tmp), "dvd://%d%s%s", guiInfo.Track, guiInfo.ImageFilename ? "/" : "", guiInfo.ImageFilename ? guiInfo.ImageFilename : "");
            uiSetFile(NULL, tmp, SAME_STREAMTYPE);
        }
#ifdef CONFIG_DVDREAD
            dvd_chapter = guiInfo.Chapter;
            dvd_angle   = guiInfo.Angle;
#endif
            break;

        case STREAMTYPE_TV:
        case STREAMTYPE_DVB:
        {
            char tmp[512];

            sprintf(tmp, "%s://", guiTV[gui_tv_digital].SchemeName);
            uiSetFile(NULL, tmp, SAME_STREAMTYPE);
        }
        break;

        case STREAMTYPE_BINCUE:
        {
            char tmp[512], *fname, *colon;

            fname = guiInfo.Filename;

            if (strncmp(guiInfo.Filename, "cue://", 6) == 0)
                fname += 6;

            if (fname == guiInfo.Filename) {   // no scheme, so check for playlist
                plItem **playlist = cue_playlist(guiInfo.Filename);

                if (playlist) {
                    plItem *curr = listMgr(PLAYLIST_ITEM_GET_CURR, 0);

                    while (*playlist) {
                        if (*(playlist + 1))
                            (*playlist)->stop = (*(playlist + 1))->start;
                        else
                            (*playlist)->stop = INT_MAX;

                        listMgr(PLAYLIST_ITEM_INSERT, *playlist);
                        playlist++;
                    }

                    listMgr(PLAYLIST_ITEM_SET_CURR, curr);
                    listMgr(PLAYLIST_ITEM_DEL_CURR, 0);

                    guiInfo.StreamType = STREAMTYPE_FILE;
                }
            }

            if (guiInfo.StreamType == STREAMTYPE_BINCUE) {
                colon = strrchr(fname, ':');

                if (colon)
                    *colon = 0;

                snprintf(tmp, sizeof(tmp), "cue://%s:%d", fname, guiInfo.Track);
                uiSetFile(NULL, tmp, SAME_STREAMTYPE);
            } else {
                next = listMgr(PLAYLIST_ITEM_GET_CURR, 0);
                uiSetFileFromPlaylist(next);
            }
        }
        break;
        }

        /* video opts */

        if (!video_driver_list) {
            int i = 0;

            while (video_out_drivers[i++]) {
                if (video_out_drivers[i - 1]->control(VOCTRL_GUISUPPORT, NULL) == VO_TRUE) {
                    listSet(&video_driver_list, video_out_drivers[i - 1]->info->short_name);
                    break;
                }
            }
        }

        if (!video_driver_list && !video_driver_list[0]) {
            gmp_msg(MSGT_GPLAYER, MSGL_FATAL, _(MSGTR_GUI_MSG_VideoOutError));
            mplayer(MPLAYER_EXIT_GUI, EXIT_ERROR, 0);
        }

        {
            int i = 0;

            guiInfo.VideoWindow = False;

            while (video_out_drivers[i++]) {
                if ((video_driver_list && !gstrcmp(video_driver_list[0], video_out_drivers[i - 1]->info->short_name)) && (video_out_drivers[i - 1]->control(VOCTRL_GUISUPPORT, NULL) == VO_TRUE)) {
                    guiInfo.VideoWindow = True;
                    break;
                }
            }
        }

        if (video_driver_list && !gstrcmp(video_driver_list[0], "dxr3"))
            if (guiInfo.StreamType != STREAMTYPE_DVD && guiInfo.StreamType != STREAMTYPE_VCD)
                if (gtkVfLAVC)
                    add_vf("lavc", NULL);

        if (gtkVfPP)
            add_vf("pp", NULL);

        switch (guiInfo.Rotation) {
            static const char *argvf[] = { "_oldargs_", NULL, NULL };

        case -1:
            remove_vf("rotate");
            remove_vf("flip");
            remove_vf("mirror");
            break;

        case 1:
            argvf[1] = "1";
            add_vf("rotate", argvf);
            remove_vf("flip");
            remove_vf("mirror");
            break;

        case 2:
            argvf[1] = "2";
            add_vf("rotate", argvf);
            remove_vf("flip");
            remove_vf("mirror");
            break;

        case 8:
            remove_vf("rotate");
            add_vf("flip", NULL);
            add_vf("mirror", NULL);
            break;
        }

        /* audio opts */

// if ( ao_plugin_cfg.plugin_list ) { free( ao_plugin_cfg.plugin_list ); ao_plugin_cfg.plugin_list=NULL; }
        if (gtkAONorm)
            listRepl(&af_cfg.list, "volnorm", "volnorm");

        if (gtkEnableAudioEqualizer)
            listRepl(&af_cfg.list, "equalizer", "equalizer");

        if (gtkAOExtraStereo) {
            char *name;

            name = malloc(12 + 20 + 1);
            snprintf(name, 12 + 20, "extrastereo=%f", gtkAOExtraStereoMul);
            name[12 + 20] = 0;
            listRepl(&af_cfg.list, "extrastereo", name);
            free(name);
        }

        if (audio_driver_list && !gstrncmp(audio_driver_list[0], "oss", 3)) {
            mixer_device  = gtkAOOSSMixer;
            mixer_channel = gtkAOOSSMixerChannel;

            if (gtkAOOSSDevice) {
                char *tmp;

                tmp = calloc(1, strlen(gtkAOOSSDevice) + 7);
                sprintf(tmp, "oss:%s", gtkAOOSSDevice);
                listSet(&audio_driver_list, tmp);
                free(tmp);
            }
        }

        if (audio_driver_list && !gstrncmp(audio_driver_list[0], "alsa", 4)) {
            mixer_device  = gtkAOALSAMixer;
            mixer_channel = gtkAOALSAMixerChannel;

            if (gtkAOALSADevice) {
                char *tmp;

                tmp = calloc(1, strlen(gtkAOALSADevice) + 14);
                sprintf(tmp, "alsa:device=%s", gtkAOALSADevice);
                listSet(&audio_driver_list, tmp);
                free(tmp);
            }
        }

        if (audio_driver_list && !gstrncmp(audio_driver_list[0], "sdl", 3)) {
            if (gtkAOSDLDriver) {
                char *tmp;

                tmp = calloc(1, strlen(gtkAOSDLDriver) + 10);
                sprintf(tmp, "sdl:%s", gtkAOSDLDriver);
                listSet(&audio_driver_list, tmp);
                free(tmp);
            }
        }

        if (audio_driver_list && !gstrncmp(audio_driver_list[0], "esd", 3)) {
            if (gtkAOESDDevice) {
                char *tmp;

                tmp = calloc(1, strlen(gtkAOESDDevice) + 10);
                sprintf(tmp, "esd:%s", gtkAOESDDevice);
                listSet(&audio_driver_list, tmp);
                free(tmp);
            }
        }

        /* subtitle */

        stream_dump_type = 0;

        if (gtkSubDumpMPSub)
            stream_dump_type = 4;

        if (gtkSubDumpSrt)
            stream_dump_type = 6;

        gtkSubDumpMPSub = gtkSubDumpSrt = False;

        /* misc */

        if (gtkCacheOn)
            stream_cache_size = gtkCacheSize;

        if (gtkAutoSyncOn)
            autosync = gtkAutoSync;

        if (guiInfo.AudioFilename)
            audio_stream = gstrdup(guiInfo.AudioFilename);
        else if (guiInfo.MediumChanged == GUI_MEDIUM_NEW)
            nfree(audio_stream);

// audio_stream = NULL;

        guiInfo.MediumChanged = False;

        ass_enabled       = gtkASS.enabled;
        ass_use_margins   = gtkASS.use_margins;
        ass_top_margin    = gtkASS.top_margin;
        ass_bottom_margin = gtkASS.bottom_margin;

        break;

    case GUI_SET_STREAM:

        if (guiInfo.StreamType == STREAMTYPE_PLAYLIST)
            guiInfo.mpcontext->file_format = DEMUXER_TYPE_PLAYLIST;
        else {
            stream = data;
            guiInfo.StreamType = stream->type;
        }

        if (orig_demuxer.changed) {
            free(demuxer_name);
            demuxer_name = orig_demuxer.name;
            orig_demuxer.changed = False;
        }

        switch (guiInfo.StreamType) {
        case STREAMTYPE_FILE:
        case STREAMTYPE_STREAM:
            guiInfo.Tracks = (uintptr_t)listMgr(PLAYLIST_ITEM_GET_POS, 0);
            break;

        case STREAMTYPE_CDDA:
            guiInfo.Tracks = 0;
            stream_control(stream, STREAM_CTRL_GET_NUM_TITLES, &guiInfo.Tracks);
            if (stream_control(stream, STREAM_CTRL_GET_CURRENT_TITLE, &guiInfo.Track) == STREAM_OK)
                guiInfo.Track++;
            break;

        case STREAMTYPE_VCD:
            guiInfo.Tracks = 0;
            stream_control(stream, STREAM_CTRL_GET_NUM_TITLES, &guiInfo.Tracks);
            if (stream_control(stream, STREAM_CTRL_GET_CURRENT_TITLE, &guiInfo.Track) == STREAM_OK)
                guiInfo.Track++;
            break;

        case STREAMTYPE_DVD:
            guiInfo.Tracks = 0;
            stream_control(stream, STREAM_CTRL_GET_NUM_TITLES, &guiInfo.Tracks);
            guiInfo.Chapters = 0;
            stream_control(stream, STREAM_CTRL_GET_NUM_CHAPTERS, &guiInfo.Chapters);
            guiInfo.Angles = 0;
            stream_control(stream, STREAM_CTRL_GET_NUM_ANGLES, &guiInfo.Angles);
            if (stream_control(stream, STREAM_CTRL_GET_CURRENT_TITLE, &guiInfo.Track) == STREAM_OK)
                guiInfo.Track++;
            // guiInfo.Chapter will be set by mplayer
            guiInfo.Angle = 1;
            stream_control(stream, STREAM_CTRL_GET_ANGLE, &guiInfo.Angle);
#ifdef CONFIG_DVDREAD
            dvd = stream->priv;
            guiInfo.AudioStreams = dvd->nr_of_channels;
            memcpy(guiInfo.AudioStream, dvd->audio_streams, sizeof(dvd->audio_streams));
            guiInfo.Subtitles = dvd->nr_of_subtitles;
            memcpy(guiInfo.Subtitle, dvd->subtitles, sizeof(dvd->subtitles));
#endif
            break;

        case STREAMTYPE_TV:
        case STREAMTYPE_DVB:
            guiInfo.Tracks = guiInfo.Track = 1;
            break;

        case STREAMTYPE_BINCUE:

            guiInfo.Tracks = 0;
            stream_control(stream, STREAM_CTRL_GET_NUM_TITLES, &guiInfo.Tracks);
            if (stream_control(stream, STREAM_CTRL_GET_CURRENT_TITLE, &guiInfo.Track) == STREAM_OK)
                guiInfo.Track++;
            stream_control(stream, STREAM_CTRL_GET_NUM_ANGLES, &guiInfo.Angles);

            if (guiInfo.Angles == 0) {
                orig_demuxer.name    = demuxer_name;
                demuxer_name         = strdup("rawaudio");
                orig_demuxer.changed = True;
            }

            break;
        }

        break;

    case GUI_SET_VIDEO:

        /* video */

        guiInfo.sh_video = data;

        nfree(guiInfo.CodecName);

        if (guiInfo.sh_video)
            guiInfo.CodecName = strdup(codec_idx2str(guiInfo.sh_video->codec->name_idx));

        state = (isSeekableStreamtype ? btnReleased : btnDisabled);
        btnSet(evForward10sec, state);
        btnSet(evBackward10sec, state);
        btnSet(evForward1min, state);
        btnSet(evBackward1min, state);
        btnSet(evForward10min, state);
        btnSet(evBackward10min, state);
        btnSet(evSetMoviePosition, state);

        if (video_driver_list && !gstrcmp(video_driver_list[0], "dxr3") && (((demuxer_t *)mpctx_get_demuxer(guiInfo.mpcontext))->file_format != DEMUXER_TYPE_MPEG_PS) && !gtkVfLAVC) {
            gtkMessageBox(MSGBOX_FATAL, _(MSGTR_GUI_MSG_DXR3NeedsLavc));
            return False;
        }

        break;

    case GUI_SET_AUDIO:

        sh_audio = data;

        if (sh_audio) {
            guiInfo.AudioChannels    = sh_audio->channels;
            guiInfo.AudioPassthrough = (gstrcmp(sh_audio->ad_driver->info->short_name, "hwac3") == 0);

            if (!guiInfo.sh_video) {
                guiInfo.VideoWindow = False;
                guiInfo.VideoWidth  = 0;
                guiInfo.VideoHeight = 0;
            }
        } else {
            guiInfo.AudioChannels    = 0;
            guiInfo.AudioPassthrough = False;
        }

        if (guiInfo.AudioPassthrough)
            btnSet(evSetVolume, btnDisabled);
        if (guiInfo.AudioChannels < 2 || guiInfo.AudioPassthrough)
            btnSet(evSetBalance, btnDisabled);

        if (current_volume) {
            mixer_getvolume(mpctx_get_mixer(guiInfo.mpcontext), &l, &r);
            guiInfo.Volume = FFMAX(l, r);
            current_volume = False;
        }

        if (gtkReplayGainOn) {
            gainItem *item;
            int ReplayGain = False;

            item = listMgr(GAINLIST_ITEM_FIND, guiInfo.Filename);

            if (item) {
                replay_gain = (item->replay_gain < 0.0f ? item->replay_gain - 0.05f : item->replay_gain + 0.05f) * 10;
                ReplayGain  = True;
            }

            if (!ReplayGain)
                ReplayGain = (demux_control(mpctx_get_demuxer(guiInfo.mpcontext), DEMUXER_CTRL_GET_REPLAY_GAIN, &replay_gain) == DEMUXER_CTRL_OK);

            if (ReplayGain) {
                guiInfo.LastVolume       = guiInfo.Volume;
                guiInfo.Volume           = constrain(100.0 + (replay_gain / 10.0 + gtkReplayGainAdjustment) / 0.5);
                guiInfo.ReplayGainVolume = -1.0f;
            }
        }

        if (guiInfo.LastVolume >= 0.0f || last_balance < 0.0f)
            uiEvent(ivSetVolume, guiInfo.Volume);

        if (last_balance < 0.0f) {
            if (guiInfo.AudioChannels == 2 && !guiInfo.AudioPassthrough)
                uiEvent(ivSetBalance, guiInfo.Balance);

            last_balance = guiInfo.Balance;
        }

        if (gtkEnableAudioEqualizer) {
            equalizer_t eq;
            unsigned int i, j;

            for (i = 0; i < FF_ARRAY_ELEMS(gtkEquChannels); i++) {
                for (j = 0; j < FF_ARRAY_ELEMS(*gtkEquChannels); j++) {
                    eq.channel = i;
                    eq.band    = j;
                    eq.gain    = gtkEquChannels[i][j];
                    mplayer(MPLAYER_SET_EQUALIZER, 0, &eq);
                }
            }
        }

        if (guiInfo.Start)
            uiAbsSeek(guiInfo.Start);

        // This must be done here (in the last call from MPlayer before
        // playback starts) and not in GUI_SETUP_VIDEO_WINDOW, because
        // without video there will be no call to GUI_SETUP_VIDEO_WINDOW
        if (!guiInfo.VideoWindow) {
            wsWindowVisibility(&guiApp.videoWindow, wsHideWindow);
            btnSet(evFullScreen, fullscreen ? btnPressed : btnReleased);
        }

        break;

    case GUI_SET_VOLUME_BALANCE:

        mixer = data;

        mixer_getvolume(mixer, &l, &r);
        guiInfo.Volume = FFMAX(l, r);

        if (guiInfo.LastVolume >= 0.0f && guiInfo.ReplayGainVolume < 0.0f)
            guiInfo.ReplayGainVolume = guiInfo.Volume;

        mixer_getbalance(mixer, &b);
        guiInfo.Balance = (b + 1.0) * 50.0;   // transform -1..1 to 0..100

        if (guiInfo.Balance != last_balance) {
            uiEvent(ivSetVolume, guiInfo.Volume);
            last_balance = guiInfo.Balance;
        }

        break;

    case GUI_SETUP_VIDEO_WINDOW:

        guiInfo.VideoWidth  = vo_dwidth;
        guiInfo.VideoHeight = vo_dheight;

        if (!guiApp.videoWindow.isFullScreen || !guiApp.videoWindow.Mapped) {
            if (!guiApp.videoWindow.isFullScreen)
                wsWindowResize(&guiApp.videoWindow, guiInfo.VideoWidth, guiInfo.VideoHeight);
            if (!guiApp.videoWindow.Mapped)
                wsWindowVisibility(&guiApp.videoWindow, wsShowWindow);
        }

        if (fullscreen ^ guiApp.videoWindow.isFullScreen)
            uiEvent(evFullScreen, True);

        if (guiWinID >= 0)
            wsWindowMove(&guiApp.mainWindow, True, 0, guiInfo.VideoHeight);

        wsWindowBackground(&guiApp.videoWindow, -1, -1, -1);

        break;

    case GUI_HANDLE_X_EVENT:

        wsEvent(data);
        break;

    case GUI_END_PLAY:

        guiInfo.sh_video = NULL;

        nfree(guiInfo.AudioFilename);
        nfree(guiInfo.SubtitleFilename);

        if (guiInfo.LastVolume >= 0.0f) {
            if (guiInfo.Volume == guiInfo.ReplayGainVolume)
                uiEvent(ivSetVolume, guiInfo.LastVolume);

            guiInfo.LastVolume = -1.0f;
        }

        btnSet(evSetVolume, btnReleased);
        btnSet(evSetBalance, btnReleased);

        uiEvent(ivRedraw, True);

        if (guiInfo.Playing) {
            last_playlistnext = guiInfo.PlaylistNext;

            if (!guiInfo.PlaylistNext) {
                guiInfo.PlaylistNext = True;
                break;
            }

            if ((guiInfo.StreamType == STREAMTYPE_CDDA || guiInfo.StreamType == STREAMTYPE_BINCUE) && guiInfo.Track < guiInfo.Tracks) {
                uiNext();
                break;
            }

            next = listMgr(PLAYLIST_ITEM_GET_NEXT, 0);

            if (guiInfo.Stop && (guiInfo.ElapsedTime > guiInfo.Stop)) {
                while (next && next->stop)
                    next = listMgr(PLAYLIST_ITEM_GET_NEXT, 0);
            }
        }

        if (next) {
            uiSetFileFromPlaylist(next);
            guiInfo.MediumChanged = GUI_MEDIUM_NEW;
        } else {
            if (guiInfo.MediumChanged == GUI_MEDIUM_NEW)
                break;

            filename = NULL;

            if (isPlaylistStreamtype) {
                plItem *curr = listMgr(PLAYLIST_ITEM_GET_CURR, 0);

                if (!curr)
                    uiUnsetFile();
                else if ((curr != listMgr(PLAYLIST_GET, 0)) && guiInfo.Playing) {
                    curr = listMgr(PLAYLIST_ITEM_SET_CURR, listMgr(PLAYLIST_GET, 0));
                    uiSetFileFromPlaylist(curr);
                    guiInfo.Track = 1;
                }
            } else if (guiInfo.Playing) {
                int first;

                switch (guiInfo.StreamType) {
                case STREAMTYPE_VCD:
                    first = 2;
                    break;

                case STREAMTYPE_BINCUE:
                    first = 1 + guiInfo.Angles;
                    break;

                default:
                    first = 1;
                    break;
                }

                if (guiInfo.Track != first) {
                    uiUnsetMedia(True);
                    guiInfo.Track = first;
                }

                if (guiInfo.StreamType == STREAMTYPE_DVD) {
                    guiInfo.Chapter = 1;
                    guiInfo.Angle   = 1;
                }
            }

            guiInfo.ElapsedTime = 0;
            guiInfo.Position    = 0.0f;

            if (gtkShowVideoWindow) {
                guiInfo.VideoWindow = True;

                if (!guiApp.videoWindow.isFullScreen)
                    wsWindowResize(&guiApp.videoWindow, guiApp.video.width, guiApp.video.height);

                if (!guiApp.videoWindow.Mapped)
                    wsWindowVisibility(&guiApp.videoWindow, wsShowWindow);

                if (fullscreen ^ guiApp.videoWindow.isFullScreen)
                    uiEvent(evFullScreen, -1);
            } else {
                wsWindowVisibility(&guiApp.videoWindow, wsHideWindow);
                guiInfo.VideoWindow = False;
                btnSet(evFullScreen, fullscreen ? btnPressed : btnReleased);
            }

            gui(GUI_SET_STATE, (void *)GUI_STOP);

            wsWindowRedraw(&guiApp.videoWindow);
            wsMouseVisibility(&guiApp.videoWindow, wsShowMouseCursor);
            wsEvents();
        }

        break;
    }

    return True;
}

/**
 * @brief Initialize the GUI playlist (i.e. import files that had been given
 *        on the command line) or add files "on the fly" (i.e. replace the
 *        current one (a playlist file) by other ones (its content)).
 *
 * @param what command (#GUI_PLAYLIST_INIT or #GUI_PLAYLIST_ADD) to be performed
 * @param playtree MPlayer playtree to read from
 * @param config MPlayer config context
 * @param enqueue whether to overwrite GUI playlist (#False) or to append to it (#True)
 *
 * @return #True (ok) or #False (error)
 */
int guiPlaylist(int what, play_tree_t *playtree, m_config_t *config, int enqueue)
{
    play_tree_iter_t *pt_iter;
    const char *file;
    int added = False;
    plItem *curr;

    pt_iter = pt_iter_create(&playtree, config);

    if (!pt_iter)
        return False;

    switch (what) {
    case GUI_PLAYLIST_INIT:

        if (!enqueue)
            listMgr(PLAYLIST_DELETE, 0);

        while ((file = pt_iter_get_next_file(pt_iter)))
            if (add_to_gui_playlist(file, PLAYLIST_ITEM_APPEND))
                added = True;

        uiCurr();   // update filename
        guiInfo.PlaylistNext = True;

        if (added)
            guiInfo.Track = 1;

        if (enqueue)
            filename = NULL;            // don't start playing

        break;

    case GUI_PLAYLIST_ADD:

        curr = listMgr(PLAYLIST_ITEM_GET_CURR, 0);

        while ((file = pt_iter_get_next_file(pt_iter)))
            if (add_to_gui_playlist(file, PLAYLIST_ITEM_INSERT))
                added = True;

        if (curr)
            listMgr(PLAYLIST_ITEM_SET_CURR, curr);
        else
            listMgr(PLAYLIST_ITEM_SET_CURR, listMgr(PLAYLIST_GET, 0));

        if (curr && added)
            listMgr(PLAYLIST_ITEM_DEL_CURR, 0);

        uiCurr();   // update filename

        break;
    }

    pt_iter_destroy(&pt_iter);

    return added;
}

/* GUI -> MPlayer */

void mplayer(int what, float value, void *data)
{
    af_stream_t *afilter;
    equalizer_t *eq = (equalizer_t *)data;

    switch (what) {
    /* subtitle */

    case MPLAYER_SET_FONT_FACTOR:
        font_factor = value;
        mplayer(MPLAYER_LOAD_FONT, 0, 0);
        break;

    case MPLAYER_SET_FONT_OUTLINE:
        subtitle_font_thickness = 8.0 * value / 100.0;   // transform 0..100 to 0..8
        mplayer(MPLAYER_LOAD_FONT, 0, 0);
        break;

    case MPLAYER_SET_FONT_BLUR:
        subtitle_font_radius = 8.0 * value / 100.0;      // transform 0..100 to 0..8
        mplayer(MPLAYER_LOAD_FONT, 0, 0);
        break;

    case MPLAYER_SET_FONT_TEXTSCALE:
        text_font_scale_factor = value;
        mplayer(MPLAYER_LOAD_FONT, 0, 0);
        break;

    case MPLAYER_SET_FONT_OSDSCALE:
        osd_font_scale_factor = value;
        mplayer(MPLAYER_LOAD_FONT, 0, 0);
        break;

    case MPLAYER_SET_FONT_ENCODING:
        nfree(subtitle_font_encoding);
        subtitle_font_encoding = gstrdup(data);
        mplayer(MPLAYER_LOAD_FONT, 0, 0);
        break;

    case MPLAYER_SET_FONT_AUTOSCALE:
        subtitle_autoscale = (int)value;
        mplayer(MPLAYER_LOAD_FONT, 0, 0);
        break;

    case MPLAYER_LOAD_FONT:
#ifdef CONFIG_FREETYPE
        set_fontconfig();

        force_load_font = 1;
#else
        free_font_desc(vo_font);

        if (font_name) {
            vo_font = read_font_desc(font_name, font_factor, 0);

            if (!vo_font)
                gmp_msg(MSGT_GPLAYER, MSGL_ERR, _(MSGTR_GUI_CantLoadFont), font_name);
        } else {
            char *fname = get_path("font/font.desc");

            setdup(&font_name, fname);
            free(fname);
            vo_font = read_font_desc(font_name, font_factor, 0);

            if (!vo_font) {
                setdup(&font_name, MPLAYER_DATADIR "/font/font.desc");
                vo_font = read_font_desc(font_name, font_factor, 0);
            }
        }
#endif
        break;

    case MPLAYER_SET_SUB_ENCODING:
        nfree(sub_cp);
        sub_cp = gstrdup(data);
        break;

    case MPLAYER_SET_EXTRA_STEREO:
        gtkAOExtraStereoMul = value;
        afilter = mpctx_get_afilter(guiInfo.mpcontext);
        if (afilter)
            af_control_any_rev(afilter, AF_CONTROL_ES_MUL | AF_CONTROL_SET, &gtkAOExtraStereoMul);
        break;

    case MPLAYER_SET_PANSCAN:
    {
        mp_cmd_t *mp_cmd;

        mp_cmd     = calloc(1, sizeof(*mp_cmd));
        mp_cmd->id = MP_CMD_PANSCAN;
        ARRAY_STRCPY(mp_cmd->name, "panscan");
        mp_cmd->args[0].v.f = value;
        mp_cmd->args[1].v.i = 1;
        mp_input_queue_cmd(mp_cmd);
    }
    break;

    case MPLAYER_SET_AUTO_QUALITY:
        auto_quality = (int)value;
        break;

    /* set equalizers */

    case MPLAYER_SET_CONTRAST:
        if (guiInfo.sh_video)
            set_video_colors(guiInfo.sh_video, "contrast", value);
        break;

    case MPLAYER_SET_BRIGHTNESS:
        if (guiInfo.sh_video)
            set_video_colors(guiInfo.sh_video, "brightness", value);
        break;

    case MPLAYER_SET_HUE:
        if (guiInfo.sh_video)
            set_video_colors(guiInfo.sh_video, "hue", value);
        break;

    case MPLAYER_SET_SATURATION:
        if (guiInfo.sh_video)
            set_video_colors(guiInfo.sh_video, "saturation", value);
        break;

    case MPLAYER_SET_EQUALIZER:
    {
        af_control_ext_t tmp;

        afilter = mpctx_get_afilter(guiInfo.mpcontext);

        if (eq) {
            gtkEquChannels[eq->channel][eq->band] = eq->gain;
            tmp.ch  = eq->channel;
            tmp.arg = gtkEquChannels[eq->channel];

            if (afilter)
                af_control_any_rev(afilter, AF_CONTROL_EQUALIZER_GAIN | AF_CONTROL_SET, &tmp);
        } else {
            unsigned int i;

            memset(gtkEquChannels, 0, sizeof(gtkEquChannels));

            if (afilter) {
                for (i = 0; i < FF_ARRAY_ELEMS(gtkEquChannels); i++) {
                    tmp.ch  = i;
                    tmp.arg = gtkEquChannels[i];
                    af_control_any_rev(afilter, AF_CONTROL_EQUALIZER_GAIN | AF_CONTROL_SET, &tmp);
                }
            }
        }

        break;
    }

    case MPLAYER_EXIT_GUI:
        exit_player_with_rc((enum exit_reason)value, (enum exit_reason)value >= EXIT_ERROR);
        break;
    }
}

void mplayerLoadSubtitle(const char *name)
{
    if (subdata) {
        mp_msg(MSGT_GPLAYER, MSGL_INFO, _(MSGTR_GUI_MSG_RemovingSubtitle));

        sub_free(subdata);
        subdata = NULL;
    }

    if (name) {
        mp_msg(MSGT_GPLAYER, MSGL_INFO, _(MSGTR_GUI_MSG_LoadingSubtitle), name);

        subdata = sub_read_file(name, guiInfo.sh_video ? guiInfo.sh_video->fps : 25);

        if (!subdata) {
            gmp_msg(MSGT_GPLAYER, MSGL_ERR, _(MSGTR_GUI_CantLoadSub), name);
            return;
        }
    }

    update_set_of_subtitles();
}

// NOTE TO MYSELF: This function is nonsense.
//                 MPlayer should pass messages to the GUI
//                 which must decide then which message has
//                 to be shown (MSGL_FATAL, for example).
//                 But with this function it is at least
//                 possible to show GUI's very critical or
//                 abort messages.
void gmp_msg(int mod, int lev, const char *format, ...)
{
    char msg[512];
    va_list va;

    va_start(va, format);
    vsnprintf(msg, sizeof(msg), format, va);
    va_end(va);

    mp_msg(mod, lev, "%s", msg);

    if (mp_msg_test(mod, lev)) {
        wsEvents();
        gtkMessageBox(MSGBOX_FATAL, msg);
    }
}
