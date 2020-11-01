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

#ifndef MPLAYER_CFG_COMMON_H
#define MPLAYER_CFG_COMMON_H

#include <sys/types.h>

#include "libmpcodecs/ad.h"
#include "libmpcodecs/dec_audio.h"
#include "libmpcodecs/dec_video.h"
#include "libmpcodecs/vd.h"
#include "libmpcodecs/vf_scale.h"
#include "libmpdemux/demux_audio.h"
#include "libmpdemux/demux_mpg.h"
#include "libmpdemux/demux_ts.h"
#include "libmpdemux/demux_viv.h"
#include "libmpdemux/demuxer.h"
#include "libmpdemux/mf.h"
#include "sub/sub.h"
#include "sub/unrar_exec.h"
#include "osdep/priority.h"
#include "stream/cdd.h"
#include "stream/network.h"
#include "stream/pvr.h"
#include "stream/stream.h"
#include "stream/stream_radio.h"
#include "stream/tcp.h"
#include "stream/tv.h"
#include "stream/udp.h"
#include "codec-cfg.h"
#include "config.h"
#include "m_config.h"
#include "m_option.h"
#include "mp_msg.h"
#include "mpcommon.h"
#ifdef CONFIG_POSTPROC
#include "libpostproc/postprocess.h"
#endif


#ifdef CONFIG_RADIO
const m_option_t radioopts_conf[]={
    {"device", &stream_radio_defaults.device, CONF_TYPE_STRING, 0, 0 ,0, NULL},
    {"driver", &stream_radio_defaults.driver, CONF_TYPE_STRING, 0, 0 ,0, NULL},
#ifdef RADIO_BSDBT848_HDR
    {"freq_min", &stream_radio_defaults.freq_min, CONF_TYPE_FLOAT, 0, 0 ,0, NULL},
    {"freq_max", &stream_radio_defaults.freq_max, CONF_TYPE_FLOAT, 0, 0 ,0, NULL},
#endif
    {"channels", &stream_radio_defaults.channels, CONF_TYPE_STRING_LIST, 0, 0 ,0, NULL},
    {"volume", &stream_radio_defaults.volume, CONF_TYPE_INT, CONF_RANGE, 0 ,100, NULL},
    {"adevice", &stream_radio_defaults.adevice, CONF_TYPE_STRING, 0, 0 ,0, NULL},
    {"arate", &stream_radio_defaults.arate, CONF_TYPE_INT, CONF_MIN, 0 ,0, NULL},
    {"achannels", &stream_radio_defaults.achannels, CONF_TYPE_INT, CONF_MIN, 0 ,0, NULL},
    {NULL, NULL, 0, 0, 0, 0, NULL}
};
#endif /* CONFIG_RADIO */

#ifdef CONFIG_TV
const m_option_t tvopts_conf[]={
    {"on", "-tv on has been removed, use tv:// instead.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
    {"immediatemode", &stream_tv_defaults.immediate, CONF_TYPE_INT, CONF_RANGE, 0, 1, NULL},
    {"noaudio", &stream_tv_defaults.noaudio, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"audiorate", &stream_tv_defaults.audiorate, CONF_TYPE_INT, 0, 0, 0, NULL},
    {"driver", &stream_tv_defaults.driver, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"device", &stream_tv_defaults.device, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"freq", &stream_tv_defaults.freq, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"channel", &stream_tv_defaults.channel, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"chanlist", &stream_tv_defaults.chanlist, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"norm", &stream_tv_defaults.norm, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"automute", &stream_tv_defaults.automute, CONF_TYPE_INT, CONF_RANGE, 0, 255, NULL},
#if defined(CONFIG_TV_V4L2) || defined(CONFIG_TV_DSHOW)
    {"normid", &stream_tv_defaults.normid, CONF_TYPE_INT, 0, 0, 0, NULL},
#endif
    {"width", &stream_tv_defaults.width, CONF_TYPE_INT, 0, 0, 4096, NULL},
    {"height", &stream_tv_defaults.height, CONF_TYPE_INT, 0, 0, 4096, NULL},
    {"input", &stream_tv_defaults.input, CONF_TYPE_INT, 0, 0, 20, NULL},
    {"outfmt", &stream_tv_defaults.outfmt, CONF_TYPE_IMGFMT, 0, 0, 0, NULL},
    {"fps", &stream_tv_defaults.fps, CONF_TYPE_FLOAT, 0, 0, 100.0, NULL},
    {"channels", &stream_tv_defaults.channels, CONF_TYPE_STRING_LIST, 0, 0, 0, NULL},
    {"brightness", &stream_tv_defaults.brightness, CONF_TYPE_INT, CONF_RANGE, -100, 100, NULL},
    {"contrast", &stream_tv_defaults.contrast, CONF_TYPE_INT, CONF_RANGE, -100, 100, NULL},
    {"hue", &stream_tv_defaults.hue, CONF_TYPE_INT, CONF_RANGE, -100, 100, NULL},
    {"saturation", &stream_tv_defaults.saturation, CONF_TYPE_INT, CONF_RANGE, -100, 100, NULL},
    {"gain", &stream_tv_defaults.gain, CONF_TYPE_INT, CONF_RANGE, -1, 100, NULL},
#if defined(CONFIG_TV_V4L) || defined(CONFIG_TV_V4L2) || defined(CONFIG_TV_DSHOW)
    {"buffersize", &stream_tv_defaults.buffer_size, CONF_TYPE_INT, CONF_RANGE, 16, 1024, NULL},
    {"amode", &stream_tv_defaults.amode, CONF_TYPE_INT, CONF_RANGE, 0, 3, NULL},
    {"volume", &stream_tv_defaults.volume, CONF_TYPE_INT, CONF_RANGE, 0, 65535, NULL},
#endif
#if defined(CONFIG_TV_V4L) || defined(CONFIG_TV_V4L2)
    {"bass", &stream_tv_defaults.bass, CONF_TYPE_INT, CONF_RANGE, 0, 65535, NULL},
    {"treble", &stream_tv_defaults.treble, CONF_TYPE_INT, CONF_RANGE, 0, 65535, NULL},
    {"balance", &stream_tv_defaults.balance, CONF_TYPE_INT, CONF_RANGE, 0, 65535, NULL},
    {"forcechan", &stream_tv_defaults.forcechan, CONF_TYPE_INT, CONF_RANGE, 1, 2, NULL},
    {"forceaudio", &stream_tv_defaults.force_audio, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"buffersize", &stream_tv_defaults.buffer_size, CONF_TYPE_INT, CONF_RANGE, 16, 1024, NULL},
    {"mjpeg", &stream_tv_defaults.mjpeg, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"decimation", &stream_tv_defaults.decimation, CONF_TYPE_INT, CONF_RANGE, 1, 4, NULL},
    {"quality", &stream_tv_defaults.quality, CONF_TYPE_INT, CONF_RANGE, 0, 100, NULL},
#ifdef CONFIG_ALSA
    {"alsa", &stream_tv_defaults.alsa, CONF_TYPE_FLAG, 0, 0, 1, NULL},
#endif /* CONFIG_ALSA */
#endif /* defined(CONFIG_TV_V4L) || defined(CONFIG_TV_V4L2) */
    {"adevice", &stream_tv_defaults.adevice, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"tdevice", &stream_tv_defaults.teletext.device, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"tpage", &stream_tv_defaults.teletext.page, CONF_TYPE_INT, CONF_RANGE, 100, 899, NULL},
    {"tformat", &stream_tv_defaults.teletext.format, CONF_TYPE_INT, CONF_RANGE, 0, 3, NULL},
    {"tlang", &stream_tv_defaults.teletext.lang, CONF_TYPE_INT, CONF_RANGE, -1, 0x7f, NULL},
    {"audioid", &stream_tv_defaults.audio_id, CONF_TYPE_INT, CONF_RANGE, 0, 9, NULL},
#ifdef CONFIG_TV_DSHOW
    {"hidden_video_renderer", &stream_tv_defaults.hidden_video_renderer, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"nohidden_video_renderer", &stream_tv_defaults.hidden_video_renderer, CONF_TYPE_FLAG, 0, 0, 0, NULL},
    {"hidden_vp_renderer", &stream_tv_defaults.hidden_vp_renderer, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"nohidden_vp_renderer", &stream_tv_defaults.hidden_vp_renderer, CONF_TYPE_FLAG, 0, 0, 0, NULL},
    {"system_clock", &stream_tv_defaults.system_clock, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"nosystem_clock", &stream_tv_defaults.system_clock, CONF_TYPE_FLAG, 0, 0, 0, NULL},
    {"normalize_audio_chunks", &stream_tv_defaults.normalize_audio_chunks, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"nonormalize_audio_chunks", &stream_tv_defaults.normalize_audio_chunks, CONF_TYPE_FLAG, 0, 0, 0, NULL},
#endif
    {NULL, NULL, 0, 0, 0, 0, NULL}
};
#endif /* CONFIG_TV */

#ifdef CONFIG_PVR
const m_option_t pvropts_conf[]={
    {"aspect", &pvr_param_aspect_ratio, CONF_TYPE_INT, 0, 1, 4, NULL},
    {"arate", &pvr_param_sample_rate, CONF_TYPE_INT, 0, 32000, 48000, NULL},
    {"alayer", &pvr_param_audio_layer, CONF_TYPE_INT, 0, 1, 5, NULL},
    {"abitrate", &pvr_param_audio_bitrate, CONF_TYPE_INT, 0, 32, 448, NULL},
    {"amode", &pvr_param_audio_mode, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"vbitrate", &pvr_param_bitrate, CONF_TYPE_INT, 0, 0, 0, NULL},
    {"vmode", &pvr_param_bitrate_mode, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"vpeak", &pvr_param_bitrate_peak, CONF_TYPE_INT, 0, 0, 0, NULL},
    {"fmt", &pvr_param_stream_type, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {NULL, NULL, 0, 0, 0, 0, NULL}
};
#endif /* CONFIG_PVR */

const m_option_t scaler_filter_conf[]={
    {"lgb", &sws_lum_gblur, CONF_TYPE_FLOAT, 0, 0, 100.0, NULL},
    {"cgb", &sws_chr_gblur, CONF_TYPE_FLOAT, 0, 0, 100.0, NULL},
    {"cvs", &sws_chr_vshift, CONF_TYPE_INT, 0, 0, 0, NULL},
    {"chs", &sws_chr_hshift, CONF_TYPE_INT, 0, 0, 0, NULL},
    {"ls", &sws_lum_sharpen, CONF_TYPE_FLOAT, 0, -100.0, 100.0, NULL},
    {"cs", &sws_chr_sharpen, CONF_TYPE_FLOAT, 0, -100.0, 100.0, NULL},
    {NULL, NULL, 0, 0, 0, 0, NULL}
};

const m_option_t vivoopts_conf[]={
    {"version", &vivo_param_version, CONF_TYPE_INT, 0, 0, 0, NULL},
    /* audio options */
    {"acodec", &vivo_param_acodec, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"abitrate", &vivo_param_abitrate, CONF_TYPE_INT, 0, 0, 0, NULL},
    {"samplerate", &vivo_param_samplerate, CONF_TYPE_INT, 0, 0, 0, NULL},
    {"bytesperblock", &vivo_param_bytesperblock, CONF_TYPE_INT, 0, 0, 0, NULL},
    /* video options */
    {"width", &vivo_param_width, CONF_TYPE_INT, 0, 0, 0, NULL},
    {"height", &vivo_param_height, CONF_TYPE_INT, 0, 0, 0, NULL},
    {"vformat", &vivo_param_vformat, CONF_TYPE_INT, 0, 0, 0, NULL},
    {NULL, NULL, 0, 0, 0, 0, NULL}
};

const m_option_t mfopts_conf[]={
    {"on", "-mf on has been removed, use mf:// instead.\n", CONF_TYPE_PRINT, 0, 0, 1, NULL},
    {"w", &mf_w, CONF_TYPE_INT, 0, 0, 0, NULL},
    {"h", &mf_h, CONF_TYPE_INT, 0, 0, 0, NULL},
    {"fps", &mf_fps, CONF_TYPE_DOUBLE, 0, 0, 0, NULL},
    {"type", &mf_type, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {NULL, NULL, 0, 0, 0, 0, NULL}
};

#include "libaf/af.h"
const m_option_t audio_filter_conf[]={
    {"list", &af_cfg.list, CONF_TYPE_STRING_LIST, 0, 0, 0, NULL},
    {"force", &af_cfg.force, CONF_TYPE_INT, CONF_RANGE, 0, 7, NULL},
    {NULL, NULL, 0, 0, 0, 0, NULL}
};

const m_option_t msgl_config[]={
    { "all", &mp_msg_level_all, CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL},

    { "global", &mp_msg_levels[MSGT_GLOBAL], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "cplayer", &mp_msg_levels[MSGT_CPLAYER], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "gplayer", &mp_msg_levels[MSGT_GPLAYER], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "vo", &mp_msg_levels[MSGT_VO], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "ao", &mp_msg_levels[MSGT_AO], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "demuxer", &mp_msg_levels[MSGT_DEMUXER], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "ds", &mp_msg_levels[MSGT_DS], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "demux", &mp_msg_levels[MSGT_DEMUX], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "header", &mp_msg_levels[MSGT_HEADER], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "avsync", &mp_msg_levels[MSGT_AVSYNC], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "autoq", &mp_msg_levels[MSGT_AUTOQ], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "cfgparser", &mp_msg_levels[MSGT_CFGPARSER], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "decaudio", &mp_msg_levels[MSGT_DECAUDIO], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "decvideo", &mp_msg_levels[MSGT_DECVIDEO], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "seek", &mp_msg_levels[MSGT_SEEK], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "win32", &mp_msg_levels[MSGT_WIN32], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "open", &mp_msg_levels[MSGT_OPEN], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "dvd", &mp_msg_levels[MSGT_DVD], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "parsees", &mp_msg_levels[MSGT_PARSEES], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "lirc", &mp_msg_levels[MSGT_LIRC], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "stream", &mp_msg_levels[MSGT_STREAM], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "cache", &mp_msg_levels[MSGT_CACHE], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "mencoder", &mp_msg_levels[MSGT_MENCODER], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "xacodec", &mp_msg_levels[MSGT_XACODEC], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "tv", &mp_msg_levels[MSGT_TV], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "radio", &mp_msg_levels[MSGT_RADIO], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "osdep", &mp_msg_levels[MSGT_OSDEP], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "spudec", &mp_msg_levels[MSGT_SPUDEC], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "playtree", &mp_msg_levels[MSGT_PLAYTREE], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "input", &mp_msg_levels[MSGT_INPUT], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "vfilter", &mp_msg_levels[MSGT_VFILTER], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "osd", &mp_msg_levels[MSGT_OSD], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "network", &mp_msg_levels[MSGT_NETWORK], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "cpudetect", &mp_msg_levels[MSGT_CPUDETECT], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "codeccfg", &mp_msg_levels[MSGT_CODECCFG], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "sws", &mp_msg_levels[MSGT_SWS], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "vobsub", &mp_msg_levels[MSGT_VOBSUB], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "subreader", &mp_msg_levels[MSGT_SUBREADER], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "afilter", &mp_msg_levels[MSGT_AFILTER], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "netst", &mp_msg_levels[MSGT_NETST], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "muxer", &mp_msg_levels[MSGT_MUXER], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "osd-menu", &mp_msg_levels[MSGT_OSD_MENU], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "identify", &mp_msg_levels[MSGT_IDENTIFY], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "ass", &mp_msg_levels[MSGT_ASS], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "statusline", &mp_msg_levels[MSGT_STATUSLINE], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    { "fixme", &mp_msg_levels[MSGT_FIXME], CONF_TYPE_INT, CONF_RANGE, -1, 9, NULL },
    {"help", "Available msg modules:\n"
    "   global     - common player errors/information\n"
    "   cplayer    - console player (mplayer.c)\n"
    "   gplayer    - gui player\n"
    "   vo         - libvo\n"
    "   ao         - libao\n"
    "   demuxer    - demuxer.c (general stuff)\n"
    "   ds         - demux stream (add/read packet etc)\n"
    "   demux      - fileformat-specific stuff (demux_*.c)\n"
    "   header     - fileformat-specific header (*header.c)\n"
    "   avsync     - mplayer.c timer stuff\n"
    "   autoq      - mplayer.c auto-quality stuff\n"
    "   cfgparser  - cfgparser.c\n"
    "   decaudio   - av decoder\n"
    "   decvideo\n"
    "   seek       - seeking code\n"
    "   win32      - win32 dll stuff\n"
    "   open       - open.c (stream opening)\n"
    "   dvd        - open.c (DVD init/read/seek)\n"
    "   parsees    - parse_es.c (mpeg stream parser)\n"
    "   lirc       - lirc_mp.c and input lirc driver\n"
    "   stream     - stream.c\n"
    "   cache      - cache2.c\n"
    "   mencoder\n"
    "   xacodec    - XAnim codecs\n"
    "   tv         - TV input subsystem\n"
    "   osdep      - OS-dependent parts\n"
    "   spudec     - spudec.c\n"
    "   playtree   - Playtree handling (playtree.c, playtreeparser.c)\n"
    "   input\n"
    "   vfilter\n"
    "   osd\n"
    "   network\n"
    "   cpudetect\n"
    "   codeccfg\n"
    "   sws\n"
    "   vobsub\n"
    "   subreader\n"
    "   osd-menu   - OSD menu messages\n"
    "   afilter    - Audio filter messages\n"
    "   netst      - Netstream\n"
    "   muxer      - muxer layer\n"
    "   identify   - identify output\n"
    "   ass        - libass messages\n"
    "   statusline - playback/encoding status line\n"
    "   fixme      - messages not yet fixed to map to module\n"
    "\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
    {NULL, NULL, 0, 0, 0, 0, NULL}

};

const m_option_t common_opts[] = {
// ------------------------- common options --------------------
    {"quiet", &quiet, CONF_TYPE_FLAG, CONF_GLOBAL, 0, 1, NULL},
    {"noquiet", &quiet, CONF_TYPE_FLAG, CONF_GLOBAL, 1, 0, NULL},
    {"really-quiet", &verbose, CONF_TYPE_FLAG, CONF_GLOBAL|CONF_PRE_PARSE, 0, -10, NULL},
    {"v", cfg_inc_verbose, CONF_TYPE_FUNC, CONF_GLOBAL|CONF_NOSAVE, 0, 0, NULL},
    {"msglevel", msgl_config, CONF_TYPE_SUBCONFIG, CONF_GLOBAL, 0, 0, NULL},
    {"msgcolor", &mp_msg_color, CONF_TYPE_FLAG, CONF_GLOBAL, 0, 1, NULL},
    {"nomsgcolor", &mp_msg_color, CONF_TYPE_FLAG, CONF_GLOBAL, 1, 0, NULL},
    {"msgmodule", &mp_msg_module, CONF_TYPE_FLAG, CONF_GLOBAL, 0, 1, NULL},
    {"nomsgmodule", &mp_msg_module, CONF_TYPE_FLAG, CONF_GLOBAL, 1, 0, NULL},
#ifdef CONFIG_ICONV
    {"msgcharset", &mp_msg_charset, CONF_TYPE_STRING, CONF_GLOBAL, 0, 0, NULL},
#endif
    {"include", cfg_include, CONF_TYPE_FUNC_PARAM_IMMEDIATE, CONF_NOSAVE, 0, 0, NULL},
#ifdef CONFIG_PRIORITY
    {"priority", &proc_priority, CONF_TYPE_STRING, 0, 0, 0, NULL},
#endif
    {"codecpath", &codec_path, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"noconfig", noconfig_opts, CONF_TYPE_SUBCONFIG, CONF_GLOBAL|CONF_NOCFG|CONF_PRE_PARSE, 0, 0, NULL},

// ------------------------- stream options --------------------

    {"chapter", parse_chapter_range, CONF_TYPE_FUNC_PARAM, 0, 0, 0, NULL},
#ifdef CONFIG_STREAM_CACHE
    {"cache", &stream_cache_size, CONF_TYPE_INT, CONF_RANGE, 32, 0x7fffffff, NULL},
    {"nocache", &stream_cache_size, CONF_TYPE_FLAG, 0, 1, 0, NULL},
    {"cache-min", &stream_cache_min_percent, CONF_TYPE_FLOAT, CONF_RANGE, 0, 99, NULL},
    {"cache-seek-min", &stream_cache_seek_min_percent, CONF_TYPE_FLOAT, CONF_RANGE, 0, 99, NULL},
#else
    {"cache", "MPlayer was compiled without cache2 support.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
#endif /* CONFIG_STREAM_CACHE */
    {"vcd", "-vcd N has been removed, use vcd://N instead.\n", CONF_TYPE_PRINT, CONF_NOCFG ,0,0, NULL},
    {"cuefile", "-cuefile has been removed, use cue://filename:N where N is the track number.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
    {"cdrom-device", &cdrom_device, CONF_TYPE_STRING, 0, 0, 0, NULL},
#ifdef CONFIG_DVDREAD
    {"dvd-device", &dvd_device,  CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"dvd-speed", &dvd_speed, CONF_TYPE_INT, 0, 0, 0, NULL},
    {"dvd", "-dvd N has been removed, use dvd://N instead.\n" , CONF_TYPE_PRINT, 0, 0, 0, NULL},
    {"dvdangle", &dvd_angle, CONF_TYPE_INT, CONF_RANGE, 1, 99, NULL},
#else
    {"dvd-device", "MPlayer was compiled without libdvdread support.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
    {"dvd-speed", "MPlayer was compiled without libdvdread support.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
    {"dvd", "MPlayer was compiled without libdvdread support.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
#endif /* CONFIG_DVDREAD */
    {"bluray-device",  &bluray_device,  CONF_TYPE_STRING, 0,          0,  0, NULL},
#ifdef CONFIG_LIBBLURAY
    {"bluray-angle",   &bluray_angle,   CONF_TYPE_INT,    CONF_RANGE, 0, 999, NULL},
    {"bluray-chapter", "The -bluray-chapter option was broken and thus removed, use -chapter instead.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
#else
    {"bluray-angle",   "MPlayer was compiled without libbluray support.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
#endif /* CONFIG_LIBBLURAY */
    {"alang", &audio_lang, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"slang", &dvdsub_lang, CONF_TYPE_STRING, 0, 0, 0, NULL},

    {"dvdauth", "libcss is obsolete. Try libdvdread instead.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
    {"dvdkey", "libcss is obsolete. Try libdvdread instead.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
    {"csslib", "libcss is obsolete. Try libdvdread instead.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},

#ifdef CONFIG_NETWORKING
    {"user", &network_username, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"passwd", &network_password, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"bandwidth", &network_bandwidth, CONF_TYPE_INT, CONF_MIN, 0, 0, NULL},
    {"http-header-fields", &network_http_header_fields, CONF_TYPE_STRING_LIST, 0, 0, 0, NULL},
    {"user-agent", &network_useragent, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"referrer", &network_referrer, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"cookies", &network_cookies_enabled, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"nocookies", &network_cookies_enabled, CONF_TYPE_FLAG, 0, 1, 0, NULL},
    {"cookies-file", &cookies_file, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"prefer-ipv4", &network_prefer_ipv4, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"ipv4-only-proxy", &network_ipv4_only_proxy, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"reuse-socket", &reuse_socket, CONF_TYPE_FLAG, CONF_GLOBAL, 0, 1, NULL},
    {"noreuse-socket", &reuse_socket, CONF_TYPE_FLAG, CONF_GLOBAL, 1, 0, NULL},
#ifdef HAVE_AF_INET6
    {"prefer-ipv6", &network_prefer_ipv4, CONF_TYPE_FLAG, 0, 1, 0, NULL},
#else
    {"prefer-ipv6", "MPlayer was compiled without IPv6 support.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
#endif /* HAVE_AF_INET6 */

#else
    {"user", "MPlayer was compiled without streaming (network) support.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
    {"passwd", "MPlayer was compiled without streaming (network) support.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
    {"bandwidth", "MPlayer was compiled without streaming (network) support.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
    {"user-agent", "MPlayer was compiled without streaming (network) support.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
#endif /* CONFIG_NETWORKING */

#ifdef CONFIG_LIVE555
    {"sdp", "-sdp has been removed, use sdp://file instead.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
#endif /* CONFIG_LIVE555 */
    {"rtsp-stream-over-http", &rtsp_transport_http, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"rtsp-stream-over-tcp", &rtsp_transport_tcp, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"rtsp-stream-over-sctp", &rtsp_transport_sctp, CONF_TYPE_FLAG, 0, 0, 1, NULL},
#ifdef CONFIG_NETWORKING
    {"rtsp-port", &rtsp_port, CONF_TYPE_INT, CONF_RANGE, -1, 65535, NULL},
    {"rtsp-destination", &rtsp_destination, CONF_TYPE_STRING, CONF_MIN, 0, 0, NULL},
#else
    {"rtsp-port", "MPlayer was compiled without networking support.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
    {"rtsp-destination", "MPlayer was compiled without networking support.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
#endif /* CONFIG_NETWORKING */

// ------------------------- demuxer options --------------------

    // number of frames to play/convert
    {"frames", &play_n_frames_mf, CONF_TYPE_INT, CONF_MIN, 0, 0, NULL},

    // seek to byte/seconds position
    {"sb", &seek_to_byte, CONF_TYPE_POSITION, CONF_MIN, 0, 0, NULL},
    {"ss", &seek_to_sec, CONF_TYPE_TIME, 0, 0, 0, NULL},

    // stop at given position
    {"endpos", &end_at, CONF_TYPE_TIME_SIZE, 0, 0, 0, NULL},

    {"edl", &edl_filename,  CONF_TYPE_STRING, 0, 0, 0, NULL},

    // force non-interleaved mode
    {"ni", &force_ni, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"noni", &force_ni, CONF_TYPE_FLAG, 0, 1, 0, NULL},

    // AVI and Ogg only: (re)build index at startup
    {"noidx", &index_mode, CONF_TYPE_FLAG, 0, -1, 0, NULL},
    {"idx", &index_mode, CONF_TYPE_FLAG, 0, -1, 1, NULL},
    {"forceidx", &index_mode, CONF_TYPE_FLAG, 0, -1, 2, NULL},
    {"saveidx", &index_file_save, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"loadidx", &index_file_load, CONF_TYPE_STRING, 0, 0, 0, NULL},

    // select audio/video/subtitle stream
    {"aid", &audio_id, CONF_TYPE_INT, CONF_RANGE, -2, 8190, NULL},
    {"ausid", &audio_substream_id, CONF_TYPE_INT, 0, 0, 0, NULL},
    {"vid", &video_id, CONF_TYPE_INT, CONF_RANGE, -2, 8190, NULL},
    {"sid", &dvdsub_id, CONF_TYPE_INT, CONF_RANGE, -2, 8190, NULL},
    {"nosub", &dvdsub_id, CONF_TYPE_FLAG, 0, -1, -2, NULL},
    {"novideo", &video_id, CONF_TYPE_FLAG, 0, -1, -2, NULL},

    { "hr-mp3-seek", &hr_mp3_seek, CONF_TYPE_FLAG, 0, 0, 1, NULL },
    { "nohr-mp3-seek", &hr_mp3_seek, CONF_TYPE_FLAG, 0, 1, 0, NULL},

    { "rawaudio", &demux_rawaudio_opts, CONF_TYPE_SUBCONFIG, 0, 0, 0, NULL},
    { "rawvideo", &demux_rawvideo_opts, CONF_TYPE_SUBCONFIG, 0, 0, 0, NULL},

#ifdef CONFIG_CDDA
    { "cdda", &cdda_opts, CONF_TYPE_SUBCONFIG, 0, 0, 0, NULL},
#endif

    // demuxer.c - select audio/sub file/demuxer
    { "audiofile", &audio_stream, CONF_TYPE_STRING, 0, 0, 0, NULL },
    { "audiofile-cache", &audio_stream_cache, CONF_TYPE_INT, CONF_RANGE, 50, 65536, NULL},
    { "subfile", &sub_stream, CONF_TYPE_STRING, 0, 0, 0, NULL },
    { "demuxer", &demuxer_name, CONF_TYPE_STRING, 0, 0, 0, NULL },
    { "audio-demuxer", &audio_demuxer_name, CONF_TYPE_STRING, 0, 0, 0, NULL },
    { "sub-demuxer", &sub_demuxer_name, CONF_TYPE_STRING, 0, 0, 0, NULL },
    { "extbased", &extension_parsing, CONF_TYPE_FLAG, 0, 0, 1, NULL },
    { "noextbased", &extension_parsing, CONF_TYPE_FLAG, 0, 1, 0, NULL },

    {"mf", mfopts_conf, CONF_TYPE_SUBCONFIG, 0,0,0, NULL},
#ifdef CONFIG_RADIO
    {"radio", radioopts_conf, CONF_TYPE_SUBCONFIG, 0, 0, 0, NULL},
#else
    {"radio", "MPlayer was compiled without Radio interface support.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
#endif /* CONFIG_RADIO */
#ifdef CONFIG_TV
    {"tv", tvopts_conf, CONF_TYPE_SUBCONFIG, 0, 0, 0, NULL},
#else
    {"tv", "MPlayer was compiled without TV interface support.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
#endif /* CONFIG_TV */
#ifdef CONFIG_PVR
    {"pvr", pvropts_conf, CONF_TYPE_SUBCONFIG, 0, 0, 0, NULL},
#else
    {"pvr", "MPlayer was compiled without V4L2/PVR interface support.\n", CONF_TYPE_PRINT, 0, 0, 0, NULL},
#endif /* CONFIG_PVR */
    {"vivo", vivoopts_conf, CONF_TYPE_SUBCONFIG, 0, 0, 0, NULL},
#ifdef CONFIG_DVBIN
    {"dvbin", dvbin_opts_conf, CONF_TYPE_SUBCONFIG, 0, 0, 0, NULL},
#endif

// ------------------------- a-v sync options --------------------

    // AVI specific: A-V sync mode (bps vs. interleaving)
    {"bps", &pts_from_bps, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"nobps", &pts_from_bps, CONF_TYPE_FLAG, 0, 1, 0, NULL},

    // set A-V sync correction speed (0=disables it):
    {"mc", &default_max_pts_correction, CONF_TYPE_FLOAT, CONF_RANGE, 0, 100, NULL},

    // force video/audio rate:
    {"fps", &force_fps, CONF_TYPE_DOUBLE, CONF_MIN, 0, 0, NULL},
    {"srate", &force_srate, CONF_TYPE_INT, CONF_RANGE, 1000, 8*48000, NULL},
    {"channels", &audio_output_channels, CONF_TYPE_INT, CONF_RANGE, 1, 8, NULL},
    {"format", &audio_output_format, CONF_TYPE_AFMT, 0, 0, 0, NULL},
    {"speed", &playback_speed, CONF_TYPE_FLOAT, CONF_RANGE, 0.01, 100.0, NULL},

    // set a-v distance
    {"delay", &audio_delay, CONF_TYPE_FLOAT, CONF_RANGE, -100.0, 100.0, NULL},

    // ignore header-specified delay (dwStart)
    {"ignore-start", &ignore_start, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"noignore-start", &ignore_start, CONF_TYPE_FLAG, 0, 1, 0, NULL},

    {"a52drc", &drc_level, CONF_TYPE_FLOAT, CONF_RANGE, 0, 2, NULL},

// ------------------------- codec/vfilter options --------------------

    // MP3-only: select stereo/left/right
#ifdef CONFIG_FAKE_MONO
    {"stereo", &fakemono, CONF_TYPE_INT, CONF_RANGE, 0, 2, NULL},
#endif

    // disable audio
    {"sound", &audio_id, CONF_TYPE_FLAG, 0, -2, -1, NULL},
    {"nosound", &audio_id, CONF_TYPE_FLAG, 0, -1, -2, NULL},

    {"af*", &af_cfg.list, CONF_TYPE_STRING_LIST, 0, 0, 0, NULL},
    {"af-adv", audio_filter_conf, CONF_TYPE_SUBCONFIG, 0, 0, 0, NULL},

    {"vop", "-vop has been removed, use -vf instead.\n", CONF_TYPE_PRINT, CONF_NOCFG ,0,0, NULL},
    {"vf*", &vf_settings, CONF_TYPE_OBJ_SETTINGS_LIST, 0, 0, 0, &vf_obj_list},
    // select audio/video codec (by name) or codec family (by number):
    {"afm", &audio_fm_list, CONF_TYPE_STRING_LIST, 0, 0, 0, NULL},
    {"vfm", &video_fm_list, CONF_TYPE_STRING_LIST, 0, 0, 0, NULL},
    {"ac", &audio_codec_list, CONF_TYPE_STRING_LIST, 0, 0, 0, NULL},
    {"vc", &video_codec_list, CONF_TYPE_STRING_LIST, 0, 0, 0, NULL},

    // postprocessing:
#ifdef CONFIG_POSTPROC
    {"pp", &divx_quality, CONF_TYPE_INT, 0, 0, 0, NULL},
    {"pphelp", pp_help, CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
#endif

    // scaling:
    {"sws", &sws_flags, CONF_TYPE_INT, 0, 0, 2, NULL},
    {"swsopts", &sws_opts, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"ssf", scaler_filter_conf, CONF_TYPE_SUBCONFIG, 0, 0, 0, NULL},
    {"zoom", &softzoom, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"nozoom", &softzoom, CONF_TYPE_FLAG, 0, 1, 0, NULL},
    {"aspect", &movie_aspect, CONF_TYPE_FLOAT, CONF_RANGE, 0.01, 10.0, NULL},
    {"noaspect", &movie_aspect, CONF_TYPE_FLAG, 0, 0, 0, NULL},
    {"xy", &screen_size_xy, CONF_TYPE_FLOAT, CONF_RANGE, 0.001, 4096, NULL},

    {"flip", &flip, CONF_TYPE_FLAG, 0, -1, 1, NULL},
    {"noflip", &flip, CONF_TYPE_FLAG, 0, -1, 0, NULL},
    {"tsfastparse", "-tsfastparse is no longer a valid option.\n", CONF_TYPE_PRINT, CONF_NOCFG ,0,0, NULL
},
    {"tsprog", &ts_prog, CONF_TYPE_INT, CONF_RANGE, 0, 65534, NULL},
    {"tsprobe", &ts_probe, CONF_TYPE_POSITION, 0, 0, TS_MAX_PROBE_SIZE, NULL},
    {"psprobe", &ps_probe, CONF_TYPE_POSITION, 0, 0, TS_MAX_PROBE_SIZE, NULL},
    {"tskeepbroken", &ts_keep_broken, CONF_TYPE_FLAG, 0, 0, 1, NULL},

    // draw by slices or whole frame (useful with libmpeg2/libavcodec)
    {"slices", &vd_use_slices, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"noslices", &vd_use_slices, CONF_TYPE_FLAG, 0, 1, 0, NULL},
    {"field-dominance", &field_dominance, CONF_TYPE_INT, CONF_RANGE, -1, 1, NULL},

#ifdef CONFIG_FFMPEG
    {"lavdopts", lavc_decode_opts_conf, CONF_TYPE_SUBCONFIG, 0, 0, 0, NULL},
    {"lavfdopts",  lavfdopts_conf, CONF_TYPE_SUBCONFIG, CONF_GLOBAL, 0, 0, NULL},
    {"lavfstreamopts",  &lavfstreamopts, CONF_TYPE_STRING, CONF_GLOBAL, 0, 0, NULL},
#endif
#ifdef CONFIG_XVID4
    {"xvidopts", xvid_dec_opts, CONF_TYPE_SUBCONFIG, 0, 0, 0, NULL},
#endif
    {"codecs-file", &codecs_file, CONF_TYPE_STRING, 0, 0, 0, NULL},
// ------------------------- subtitles options --------------------

    {"sub", &sub_name, CONF_TYPE_STRING_LIST, 0, 0, 0, NULL},
    {"sub-paths", &sub_paths, CONF_TYPE_STRING_LIST, 0, 0, 0, NULL},
#ifdef CONFIG_FRIBIDI
    {"fribidi-charset", &fribidi_charset, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"flip-hebrew", &flip_hebrew, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"noflip-hebrew", &flip_hebrew, CONF_TYPE_FLAG, 0, 1, 0, NULL},
    {"flip-hebrew-commas", &fribidi_flip_commas, CONF_TYPE_FLAG, 0, 1, 0, NULL},
    {"noflip-hebrew-commas", &fribidi_flip_commas, CONF_TYPE_FLAG, 0, 0, 1, NULL},
#else
    {"fribidi-charset", "MPlayer was compiled without FriBiDi support.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
    {"flip-hebrew", "MPlayer was compiled without FriBiDi support.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
    {"noflip-hebrew", "MPlayer was compiled without FriBiDi support.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
    {"flip-hebrew-commas", "MPlayer was compiled without FriBiDi support.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
    {"noflip-hebrew-commas", "MPlayer was compiled without FriBiDi support.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
#endif /* CONFIG_FRIBIDI */
#ifdef CONFIG_ICONV
    {"subcp", &sub_cp, CONF_TYPE_STRING, 0, 0, 0, NULL},
#endif
    {"subdelay", &sub_delay, CONF_TYPE_FLOAT, 0, 0.0, 10.0, NULL},
    {"subfps", &sub_fps, CONF_TYPE_FLOAT, 0, 0.0, 10.0, NULL},
    {"autosub", &sub_auto, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"noautosub", &sub_auto, CONF_TYPE_FLAG, 0, 1, 0, NULL},
    {"unicode", &sub_unicode, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"nounicode", &sub_unicode, CONF_TYPE_FLAG, 0, 1, 0, NULL},
    {"utf8", &sub_utf8, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"noutf8", &sub_utf8, CONF_TYPE_FLAG, 0, 1, 0, NULL},
    {"forcedsubsonly", &forced_subs_only, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"vobsub", &vobsub_name, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"vobsubid", &vobsub_id, CONF_TYPE_INT, CONF_RANGE, 0, 31, NULL},
#ifdef CONFIG_UNRAR_EXEC
    {"unrarexec", &unrar_executable, CONF_TYPE_STRING, 0, 0, 0, NULL},
#endif
    // specify IFO file for VOBSUB subtitle
    {"ifo", &spudec_ifo, CONF_TYPE_STRING, 0, 0, 0, NULL},
    // enable Closed Captioning display
    {"subcc", &subcc_enabled, CONF_TYPE_INT, CONF_RANGE, 0, 8, NULL},
    {"nosubcc", &subcc_enabled, CONF_TYPE_FLAG, 0, 1, 0, NULL},
    {"overlapsub", &suboverlap_enabled, CONF_TYPE_FLAG, 0, 0, 2, NULL},
    {"nooverlapsub", &suboverlap_enabled, CONF_TYPE_FLAG, 0, 0, 0, NULL},
    {"sub-bg-color", &sub_bg_color, CONF_TYPE_INT, CONF_RANGE, 0, 255, NULL},
    {"sub-bg-alpha", &sub_bg_alpha, CONF_TYPE_INT, CONF_RANGE, 0, 255, NULL},
    {"sub-no-text-pp", &sub_no_text_pp, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"sub-fuzziness", &sub_match_fuzziness, CONF_TYPE_INT, CONF_RANGE, 0, 2, NULL},
    {"font", &font_name, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"subfont", &sub_font_name, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"ffactor", &font_factor, CONF_TYPE_FLOAT, CONF_RANGE, 0.0, 10.0, NULL},
    {"subpos", &sub_pos, CONF_TYPE_INT, CONF_RANGE, 0, 150, NULL},
    {"subalign", &sub_alignment, CONF_TYPE_INT, CONF_RANGE, 0, 2, NULL},
    {"subwidth", &sub_width_p, CONF_TYPE_INT, CONF_RANGE, 10, 100, NULL},
    {"spualign", &spu_alignment, CONF_TYPE_INT, CONF_RANGE, -1, 2, NULL},
    {"spuaa", &spu_aamode, CONF_TYPE_INT, CONF_RANGE, 0, 31, NULL},
    {"spugauss", &spu_gaussvar, CONF_TYPE_FLOAT, CONF_RANGE, 0.0, 3.0, NULL},
    {"subfont-encoding", &subtitle_font_encoding, CONF_TYPE_STRING, 0, 0, 0, NULL},
#ifdef CONFIG_FREETYPE
    {"subfont-text-scale", &text_font_scale_factor, CONF_TYPE_FLOAT, CONF_RANGE, 0, 100, NULL},
    {"subfont-osd-scale", &osd_font_scale_factor, CONF_TYPE_FLOAT, CONF_RANGE, 0, 100, NULL},
    {"subfont-blur", &subtitle_font_radius, CONF_TYPE_FLOAT, CONF_RANGE, 0, 8, NULL},
    {"subfont-outline", &subtitle_font_thickness, CONF_TYPE_FLOAT, CONF_RANGE, 0, 8, NULL},
    {"subfont-autoscale", &subtitle_autoscale, CONF_TYPE_INT, CONF_RANGE, 0, 3, NULL},
#endif
#ifdef CONFIG_ASS
    {"ass", &ass_enabled, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"noass", &ass_enabled, CONF_TYPE_FLAG, 0, 1, 0, NULL},
    {"ass-font-scale", &ass_font_scale, CONF_TYPE_FLOAT, CONF_RANGE, 0, 100, NULL},
    {"ass-line-spacing", &ass_line_spacing, CONF_TYPE_FLOAT, CONF_RANGE, -1000, 1000, NULL},
    {"ass-top-margin", &ass_top_margin, CONF_TYPE_INT, CONF_RANGE, 0, 2000, NULL},
    {"ass-bottom-margin", &ass_bottom_margin, CONF_TYPE_INT, CONF_RANGE, 0, 2000, NULL},
    {"ass-use-margins", &ass_use_margins, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"noass-use-margins", &ass_use_margins, CONF_TYPE_FLAG, 0, 1, 0, NULL},
    {"embeddedfonts", &extract_embedded_fonts, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"noembeddedfonts", &extract_embedded_fonts, CONF_TYPE_FLAG, 0, 1, 0, NULL},
    {"ass-force-style", &ass_force_style_list, CONF_TYPE_STRING_LIST, 0, 0, 0, NULL},
    {"ass-color", &ass_color, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"ass-border-color", &ass_border_color, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"ass-styles", &ass_styles_file, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"ass-hinting", &ass_hinting, CONF_TYPE_INT, CONF_RANGE, 0, 7, NULL},
#endif
#ifdef CONFIG_FONTCONFIG
    {"fontconfig", &font_fontconfig, CONF_TYPE_FLAG, 0, -1, 1, NULL},
    {"nofontconfig", &font_fontconfig, CONF_TYPE_FLAG, 0, 1, -1, NULL},
#else
    {"fontconfig", "MPlayer was compiled without fontconfig support.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
    {"nofontconfig", "MPlayer was compiled without fontconfig support.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
#endif /* CONFIG_FONTCONFIG */
    {NULL, NULL, 0, 0, 0, 0, NULL}
};

#endif /* MPLAYER_CFG_COMMON_H */
