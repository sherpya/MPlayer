/*
 * MPlayer GUI for Win32
 * Copyright (C) 2003 Sascha Sommer <saschasommer@freenet.de>
 * Copyright (C) 2006 Erik Augustson <erik_27can@yahoo.com>
 * Copyright (C) 2006 Gianluigi Tiesi <sherpya@netfarm.it>
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
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include "path.h"
#include "mplayer.h"
#include "mp_msg.h"
#include "help_mp.h"
#include "m_config.h"
#include "m_option.h"
#include "parser-cfg.h"
#include "libvo/video_out.h"
#include "osdep/priority.h"
#include "mixer.h"
#include "gui/ui/ui.h"
#include "gui/interface.h"
#include "gui.h"

static const char gui_configuration[] =  "gui.conf";

/* params */
int   gtkAONorm = FALSE;
int   gtkAOExtraStereo = FALSE;
float gtkAOExtraStereoMul = 1.0f;
int   gtkCacheOn = FALSE;
int   gtkCacheSize = 2048;
int   gtkAutoSyncOn = FALSE;
int   gtkAutoSync = 0;

int video_window = TRUE;
int console = FALSE;

int gui_save_pos = TRUE;
int gui_main_pos_x = -2;
int gui_main_pos_y = -2;
int gui_video_pos_x = -1;
int gui_video_pos_y = -1;

m_config_t *gui_conf;
static const m_option_t gui_opts[] =
{
    {   "priority", &proc_priority, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {   "vo_driver", &video_driver_list, CONF_TYPE_STRING_LIST, 0, 0, 0, NULL },
    {   "v_framedrop", &frame_dropping, CONF_TYPE_INT, CONF_RANGE, 0, 2, NULL },
    {   "vo_doublebuffering", &vo_doublebuffering, CONF_TYPE_FLAG, 0, 0, 1, NULL },
    {   "vo_direct_render", &vo_directrendering, CONF_TYPE_FLAG, 0, 0, 1, NULL },
    {   "ao_driver", &audio_driver_list, CONF_TYPE_STRING_LIST, 0, 0, 0, NULL },
    {   "ao_volnorm", &gtkAONorm, CONF_TYPE_FLAG, 0, 0, 1, NULL },
    {   "softvol", &soft_vol, CONF_TYPE_FLAG, 0, 0, 1, NULL },
    {   "ao_extra_stereo", &gtkAOExtraStereo, CONF_TYPE_FLAG, 0, 0, 1, NULL },
    {   "ao_extra_stereo_coefficient", &gtkAOExtraStereoMul, CONF_TYPE_FLOAT, CONF_RANGE, -10, 10, NULL },
    {   "delay", &audio_delay, CONF_TYPE_FLOAT, CONF_RANGE, -100.0, 100.0, NULL},
    {   "osd_level", &osd_level, CONF_TYPE_INT, CONF_RANGE, 0, 3, NULL },
    {   "cache", &gtkCacheOn, CONF_TYPE_FLAG, 0, 0, 1, NULL },
    {   "cache_size", &gtkCacheSize, CONF_TYPE_INT, CONF_RANGE, 32, 0x7fffffff, NULL },
    {   "autosync", &gtkAutoSyncOn, CONF_TYPE_FLAG, 0, 0, 1, NULL },
    {   "autosync_size", &gtkAutoSync, CONF_TYPE_INT, CONF_RANGE, 0, 10000, NULL },
    {   "gui_skin", &skinName, CONF_TYPE_STRING, 0, 0, 0, NULL },
    {   "gui_main_pos_x", &gui_main_pos_x, CONF_TYPE_INT, 0, 0, 0, NULL },
    {   "gui_main_pos_y", &gui_main_pos_y, CONF_TYPE_INT, 0, 0, 0, NULL },
    {   "gui_sub_pos_x", &gui_video_pos_x, CONF_TYPE_INT, 0, 0, 0, NULL },
    {   "gui_sub_pos_y", &gui_video_pos_y, CONF_TYPE_INT, 0, 0, 0, NULL },
    {   "sub_window", &video_window, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {   "console", &console, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {   "idle", &player_idle_mode, CONF_TYPE_FLAG, CONF_GLOBAL, 0, 1, NULL},
    {   NULL, NULL, 0, 0, 0, 0, NULL }
};

int cfg_gui_include(m_option_t *conf, const char *filename)
{
    (void)conf;

    return m_config_parse_config_file(gui_conf, filename, 0);
}

void cfg_read(void)
{
    char *cfg = get_path(gui_configuration);

    player_idle_mode = TRUE;   // GUI is in idle mode by default

    /* read configuration */
    mp_msg(MSGT_GPLAYER, MSGL_V, "[GUI] [cfg] reading config file: %s\n", cfg);
    gui_conf = m_config_new();
    m_config_register_options(gui_conf, gui_opts);
    if (m_config_parse_config_file(gui_conf, cfg, 1) < 0)
        mp_msg(MSGT_GPLAYER, MSGL_ERR, MSGTR_GUI_WIN32_ConfigFileError);
    free(cfg);
}

void cfg_write(void)
{
    char *cfg = get_path(gui_configuration);
    FILE *f;
    int i;

    /* save configuration */
    if ((f = fopen(cfg, "wt+")))
    {
        for (i=0; gui_opts[i].name; i++)
        {
            char *v = m_option_print(&gui_opts[i], gui_opts[i].p);
            if(v == (char *)-1) {
                mp_msg(MSGT_GPLAYER, MSGL_WARN, MSGTR_GUI_MSG_UnableToSaveOption, gui_opts[i].name);
                v = NULL;
            }
            if(v)
            {
                char delim[] = "\"";

                if (!strchr(v, ' ')) *delim = 0;

                fprintf(f, "%s=%s%s%s\n", gui_opts[i].name, delim, v, delim);
                free(v);
            }
        }
        fclose(f);
    }
    free(cfg);
}
