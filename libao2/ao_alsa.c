/*
 * ALSA 0.9.x-1.x audio output driver
 *
 * Copyright (C) 2004 Alex Beregszaszi
 *
 * modified for real ALSA 0.9.0 support by Zsolt Barat <joy@streamminister.de>
 * additional AC-3 passthrough support by Andy Lo A Foe <andy@alsaplayer.org>
 * 08/22/2002 iec958-init rewritten and merged with common init, zsolt
 * 04/13/2004 merged with ao_alsa1.x, fixes provided by Jindrich Makovicka
 * 04/25/2004 printfs converted to mp_msg, Zsolt.
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

#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <alloca.h>
#define ALSA_PCM_NEW_HW_PARAMS_API
#define ALSA_PCM_NEW_SW_PARAMS_API
#include <alsa/asoundlib.h>

#include "config.h"
#include "subopt-helper.h"
#include "mixer.h"
#include "mp_msg.h"
#include "help_mp.h"
#include "audio_out.h"
#include "audio_out_internal.h"
#include "libaf/af_format.h"

static const ao_info_t info =
{
    "ALSA-0.9.x-1.x audio output",
    "alsa",
    "Alex Beregszaszi, Zsolt Barat <joy@streamminister.de>",
    "under development"
};

LIBAO_EXTERN(alsa)

static snd_pcm_t *alsa_handler;
static snd_pcm_format_t alsa_format;
static snd_pcm_hw_params_t *alsa_hwparams;
static snd_pcm_sw_params_t *alsa_swparams;

static size_t bytes_per_sample;

static int alsa_can_pause;
static int prepause_space;

#define ALSA_DEVICE_SIZE 256

static void alsa_error_handler(const char *file, int line, const char *function,
			       int err, const char *format, ...)
{
  char tmp[0xc00];
  va_list va;

  va_start(va, format);
  vsnprintf(tmp, sizeof tmp, format, va);
  va_end(va);

  if (err)
    mp_msg(MSGT_AO, MSGL_ERR, "[AO_ALSA] alsa-lib: %s:%i:(%s) %s: %s\n",
	   file, line, function, tmp, snd_strerror(err));
  else
    mp_msg(MSGT_AO, MSGL_ERR, "[AO_ALSA] alsa-lib: %s:%i:(%s) %s\n",
	   file, line, function, tmp);
}

/* to set/get/query special features/parameters */
static int control(int cmd, void *arg)
{
  switch(cmd) {
  case AOCONTROL_QUERY_FORMAT:
    return CONTROL_TRUE;
  case AOCONTROL_GET_VOLUME:
  case AOCONTROL_SET_VOLUME:
    {
      ao_control_vol_t *vol = (ao_control_vol_t *)arg;

      int err;
      snd_mixer_t *handle;
      snd_mixer_elem_t *elem;
      snd_mixer_selem_id_t *sid;

      char *mix_name = "PCM";
      char *card = "default";
      int mix_index = 0;

      long pmin, pmax;
      long get_vol, set_vol;
      float f_multi;

      if(AF_FORMAT_IS_IEC61937(ao_data.format))
	return CONTROL_TRUE;

      if(mixer_channel) {
	 char *test_mix_index;

	 mix_name = strdup(mixer_channel);
	 if ((test_mix_index = strchr(mix_name, ','))){
		*test_mix_index = 0;
		test_mix_index++;
		mix_index = strtol(test_mix_index, &test_mix_index, 0);

		if (*test_mix_index){
		  mp_msg(MSGT_AO,MSGL_ERR,
		    MSGTR_AO_ALSA_InvalidMixerIndexDefaultingToZero);
		  mix_index = 0 ;
		}
	 }
      }
      if(mixer_device) card = mixer_device;

      //allocate simple id
      snd_mixer_selem_id_alloca(&sid);

      //sets simple-mixer index and name
      snd_mixer_selem_id_set_index(sid, mix_index);
      snd_mixer_selem_id_set_name(sid, mix_name);

      if (mixer_channel) {
	free(mix_name);
	mix_name = NULL;
      }

      if ((err = snd_mixer_open(&handle, 0)) < 0) {
	mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_MixerOpenError, snd_strerror(err));
	return CONTROL_ERROR;
      }

      if ((err = snd_mixer_attach(handle, card)) < 0) {
	mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_MixerAttachError,
	       card, snd_strerror(err));
	snd_mixer_close(handle);
	return CONTROL_ERROR;
      }

      if ((err = snd_mixer_selem_register(handle, NULL, NULL)) < 0) {
	mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_MixerRegisterError, snd_strerror(err));
	snd_mixer_close(handle);
	return CONTROL_ERROR;
      }
      err = snd_mixer_load(handle);
      if (err < 0) {
	mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_MixerLoadError, snd_strerror(err));
	snd_mixer_close(handle);
	return CONTROL_ERROR;
      }

      elem = snd_mixer_find_selem(handle, sid);
      if (!elem && !mixer_channel) {
        // if nothing specified just try picking the first
        elem = snd_mixer_first_elem(handle);
        if (elem)
            mp_msg(MSGT_AO, MSGL_V, "[AO_ALSA] Auto-selected element %s\n", snd_mixer_selem_get_name(elem));
      }
      if (!elem) {
	mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_UnableToFindSimpleControl,
	       snd_mixer_selem_id_get_name(sid), snd_mixer_selem_id_get_index(sid));
        mp_msg(MSGT_AO, MSGL_INFO, "[AO_ALSA] Available elements for device:\n");
        for (elem = snd_mixer_first_elem(handle); elem; elem = snd_mixer_elem_next(elem))
            mp_msg(MSGT_AO, MSGL_INFO, "[AO_ALSA]  %s\n", snd_mixer_selem_get_name(elem));
        mp_msg(MSGT_AO, MSGL_INFO, "\n");
	snd_mixer_close(handle);
	return CONTROL_ERROR;
	}

      if (!snd_mixer_selem_has_playback_volume(elem) &&
          !snd_mixer_selem_has_playback_volume_joined(elem))
      {
          if (!snd_mixer_selem_has_playback_switch(elem))
              return CONTROL_FALSE;
          if (cmd == AOCONTROL_GET_VOLUME) {
              int v;
              snd_mixer_selem_get_playback_switch(elem, SND_MIXER_SCHN_FRONT_LEFT, &v);
              vol->left = v ? 100 : 0;
              snd_mixer_selem_get_playback_switch(elem, SND_MIXER_SCHN_FRONT_RIGHT, &v);
              vol->right = v ? 100 : 0;
              return CONTROL_TRUE;
          }
          // special case: only mute supported
          if (vol->left != 0 && vol->left != 100 ||
              vol->right != 0 && vol->right != 100)
              return CONTROL_FALSE;
          if (snd_mixer_selem_has_playback_switch_joined(elem)) {
              if (vol->left != vol->right)
                  return CONTROL_FALSE;
          } else {
              snd_mixer_selem_set_playback_switch(elem, SND_MIXER_SCHN_FRONT_RIGHT, vol->right != 0);
          }
          snd_mixer_selem_set_playback_switch(elem, SND_MIXER_SCHN_FRONT_LEFT, vol->left != 0);
          return CONTROL_TRUE;
      }

      snd_mixer_selem_get_playback_volume_range(elem,&pmin,&pmax);
      f_multi = (100 / (float)(pmax - pmin));

      if (cmd == AOCONTROL_SET_VOLUME) {

	set_vol = vol->left / f_multi + pmin + 0.5;

	//setting channels
	if ((err = snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, set_vol)) < 0) {
	  mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_ErrorSettingLeftChannel,
		 snd_strerror(err));
	  snd_mixer_close(handle);
	  return CONTROL_ERROR;
	}
	mp_msg(MSGT_AO,MSGL_DBG2,"left=%li, ", set_vol);

	set_vol = vol->right / f_multi + pmin + 0.5;

	if ((err = snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_RIGHT, set_vol)) < 0) {
	  mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_ErrorSettingRightChannel,
		 snd_strerror(err));
	  snd_mixer_close(handle);
	  return CONTROL_ERROR;
	}
	mp_msg(MSGT_AO,MSGL_DBG2,"right=%li, pmin=%li, pmax=%li, mult=%f\n",
	       set_vol, pmin, pmax, f_multi);

	if (snd_mixer_selem_has_playback_switch(elem)) {
	  int lmute = (vol->left == 0.0);
	  int rmute = (vol->right == 0.0);
	  if (snd_mixer_selem_has_playback_switch_joined(elem)) {
	    lmute = rmute = lmute && rmute;
	  } else {
	    snd_mixer_selem_set_playback_switch(elem, SND_MIXER_SCHN_FRONT_RIGHT, !rmute);
	  }
	  snd_mixer_selem_set_playback_switch(elem, SND_MIXER_SCHN_FRONT_LEFT, !lmute);
	}
      }
      else {
	snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &get_vol);
	vol->left = (get_vol - pmin) * f_multi;
	snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_RIGHT, &get_vol);
	vol->right = (get_vol - pmin) * f_multi;

	mp_msg(MSGT_AO,MSGL_DBG2,"left=%f, right=%f\n",vol->left,vol->right);
      }
      snd_mixer_close(handle);
      return CONTROL_OK;
    }

  } //end switch
  return CONTROL_UNKNOWN;
}

static void parse_device (char *dest, const char *src, int len)
{
  char *tmp;
  memmove(dest, src, len);
  dest[len] = 0;
  while ((tmp = strchr(dest, '.')))
    tmp[0] = ',';
  while ((tmp = strchr(dest, '=')))
    tmp[0] = ':';
}

static void print_help (void)
{
  mp_msg (MSGT_AO, MSGL_FATAL,
           MSGTR_AO_ALSA_CommandlineHelp);
}

static int str_maxlen(void *strp) {
  strarg_t *str = strp;
  return str->len <= ALSA_DEVICE_SIZE;
}

static int try_open_device(const char *device, int open_mode, int try_ac3)
{
  int err, len;
  char *ac3_device, *args;

  if (try_ac3) {
    /* to set the non-audio bit, use AES0=6
     * 6 == IEC958_AES0_NONAUDIO | IEC958_AES0_PRO_EMPHASIS_NONE */
    len = strlen(device);
    ac3_device = malloc(len + 7 + 1);
    if (!ac3_device)
      return -ENOMEM;
    strcpy(ac3_device, device);
    args = strchr(ac3_device, ':');
    if (!args) {
      /* no existing parameters: add it behind device name */
      strcat(ac3_device, ":AES0=6");
    } else {
      do
	++args;
      while (isspace(*args));
      if (*args == '\0') {
	/* ":" but no parameters */
	strcat(ac3_device, "AES0=6");
      } else if (*args != '{') {
	/* a simple list of parameters: add it at the end of the list */
	strcat(ac3_device, ",AES0=6");
      } else {
	/* parameters in config syntax: add it inside the { } block */
	do
	  --len;
	while (len > 0 && isspace(ac3_device[len]));
	if (ac3_device[len] == '}')
	  strcpy(ac3_device + len, " AES0=6}");
      }
    }
    err = snd_pcm_open(&alsa_handler, ac3_device, SND_PCM_STREAM_PLAYBACK,
		       open_mode);
    free(ac3_device);
    if (err >= 0)
      return err;
  }
  return snd_pcm_open(&alsa_handler, device, SND_PCM_STREAM_PLAYBACK, open_mode);
}

/*
    open & setup audio device
    return: 1=success 0=fail
*/
static int init(int rate_hz, int channels, int format, int flags)
{
    unsigned int alsa_buffer_time = 500000; /* 0.5 s */
    unsigned int alsa_fragcount = 16;
    int err;
    int block;
    strarg_t device;
    snd_pcm_uframes_t chunk_size;
    snd_pcm_uframes_t bufsize;
    snd_pcm_uframes_t boundary;
    const opt_t subopts[] = {
      {"block", OPT_ARG_BOOL, &block, NULL},
      {"device", OPT_ARG_STR, &device, str_maxlen},
      {NULL}
    };

    char alsa_device[ALSA_DEVICE_SIZE + 1];
    // make sure alsa_device is null-terminated even when using strncpy etc.
    memset(alsa_device, 0, ALSA_DEVICE_SIZE + 1);

    mp_msg(MSGT_AO,MSGL_V,"alsa-init: requested format: %d Hz, %d channels, %x\n", rate_hz,
	channels, format);
    alsa_handler = NULL;
#if SND_LIB_VERSION >= 0x010005
    mp_msg(MSGT_AO,MSGL_V,"alsa-init: using ALSA %s\n", snd_asoundlib_version());
#else
    mp_msg(MSGT_AO,MSGL_V,"alsa-init: compiled for ALSA-%s\n", SND_LIB_VERSION_STR);
#endif

    snd_lib_error_set_handler(alsa_error_handler);

    ao_data.samplerate = rate_hz;
    ao_data.format = format;
    ao_data.channels = channels;

    switch (format)
      {
      case AF_FORMAT_S8:
	alsa_format = SND_PCM_FORMAT_S8;
	break;
      case AF_FORMAT_U8:
	alsa_format = SND_PCM_FORMAT_U8;
	break;
      case AF_FORMAT_U16_LE:
	alsa_format = SND_PCM_FORMAT_U16_LE;
	break;
      case AF_FORMAT_U16_BE:
	alsa_format = SND_PCM_FORMAT_U16_BE;
	break;
      case AF_FORMAT_AC3_LE:
      case AF_FORMAT_S16_LE:
      case AF_FORMAT_IEC61937_LE:
	alsa_format = SND_PCM_FORMAT_S16_LE;
	break;
      case AF_FORMAT_AC3_BE:
      case AF_FORMAT_S16_BE:
      case AF_FORMAT_IEC61937_BE:
	alsa_format = SND_PCM_FORMAT_S16_BE;
	break;
      case AF_FORMAT_U32_LE:
	alsa_format = SND_PCM_FORMAT_U32_LE;
	break;
      case AF_FORMAT_U32_BE:
	alsa_format = SND_PCM_FORMAT_U32_BE;
	break;
      case AF_FORMAT_S32_LE:
	alsa_format = SND_PCM_FORMAT_S32_LE;
	break;
      case AF_FORMAT_S32_BE:
	alsa_format = SND_PCM_FORMAT_S32_BE;
	break;
      case AF_FORMAT_U24_LE:
	alsa_format = SND_PCM_FORMAT_U24_3LE;
	break;
      case AF_FORMAT_U24_BE:
	alsa_format = SND_PCM_FORMAT_U24_3BE;
	break;
      case AF_FORMAT_S24_LE:
	alsa_format = SND_PCM_FORMAT_S24_3LE;
	break;
      case AF_FORMAT_S24_BE:
	alsa_format = SND_PCM_FORMAT_S24_3BE;
	break;
      case AF_FORMAT_FLOAT_LE:
	alsa_format = SND_PCM_FORMAT_FLOAT_LE;
	break;
      case AF_FORMAT_FLOAT_BE:
	alsa_format = SND_PCM_FORMAT_FLOAT_BE;
	break;
      case AF_FORMAT_MU_LAW:
	alsa_format = SND_PCM_FORMAT_MU_LAW;
	break;
      case AF_FORMAT_A_LAW:
	alsa_format = SND_PCM_FORMAT_A_LAW;
	break;

      default:
	alsa_format = SND_PCM_FORMAT_MPEG; //? default should be -1
	break;
      }

    //subdevice parsing
    // set defaults
    block = 1;
    /* switch for spdif
     * sets opening sequence for SPDIF
     * sets also the playback and other switches 'on the fly'
     * while opening the abstract alias for the spdif subdevice
     * 'iec958'
     */
    if (AF_FORMAT_IS_IEC61937(format)) {
	device.str = "iec958";
	mp_msg(MSGT_AO,MSGL_V,"alsa-spdif-init: playing AC3/iec61937/iec958, %i channels\n", channels);
    }
  else
        /* in any case for multichannel playback we should select
         * appropriate device
         */
        switch (channels) {
	case 1:
	case 2:
	  device.str = "default";
	  mp_msg(MSGT_AO,MSGL_V,"alsa-init: setup for 1/2 channel(s)\n");
	  break;
	case 4:
	  if (alsa_format == SND_PCM_FORMAT_FLOAT_LE)
	    // hack - use the converter plugin
	    device.str = "plug:surround40";
	  else
	    device.str = "surround40";
	  mp_msg(MSGT_AO,MSGL_V,"alsa-init: device set to surround40\n");
	  break;
	case 6:
	  if (alsa_format == SND_PCM_FORMAT_FLOAT_LE)
	    device.str = "plug:surround51";
	  else
	    device.str = "surround51";
	  mp_msg(MSGT_AO,MSGL_V,"alsa-init: device set to surround51\n");
	  break;
	case 8:
	  if (alsa_format == SND_PCM_FORMAT_FLOAT_LE)
	    device.str = "plug:surround71";
	  else
	    device.str = "surround71";
	  mp_msg(MSGT_AO,MSGL_V,"alsa-init: device set to surround71\n");
	  break;
	default:
	  device.str = "default";
	  mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_ChannelsNotSupported,channels);
        }
    device.len = strlen(device.str);
    if (subopt_parse(ao_subdevice, subopts) != 0) {
        print_help();
        return 0;
    }
    parse_device(alsa_device, device.str, device.len);

    mp_msg(MSGT_AO,MSGL_V,"alsa-init: using device %s\n", alsa_device);

    if (!alsa_handler) {
      int open_mode = block ? 0 : SND_PCM_NONBLOCK;
      int isac3 =  AF_FORMAT_IS_IEC61937(format);
      //modes = 0, SND_PCM_NONBLOCK, SND_PCM_ASYNC
      mp_msg(MSGT_AO,MSGL_V,"alsa-init: opening device in %sblocking mode\n", block ? "" : "non-");
      if ((err = try_open_device(alsa_device, open_mode, isac3)) < 0)
	{
	  if (err != -EBUSY && !block) {
	    mp_msg(MSGT_AO,MSGL_INFO,MSGTR_AO_ALSA_OpenInNonblockModeFailed);
	    if ((err = try_open_device(alsa_device, 0, isac3)) < 0) {
	      mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_PlaybackOpenError, snd_strerror(err));
	      return 0;
	    }
	  } else {
	    mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_PlaybackOpenError, snd_strerror(err));
	    return 0;
	  }
	}

      if ((err = snd_pcm_nonblock(alsa_handler, 0)) < 0) {
         mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_ErrorSetBlockMode, snd_strerror(err));
      } else {
	mp_msg(MSGT_AO,MSGL_V,"alsa-init: device reopened in blocking mode\n");
      }

      snd_pcm_hw_params_alloca(&alsa_hwparams);
      snd_pcm_sw_params_alloca(&alsa_swparams);

      // setting hw-parameters
      if ((err = snd_pcm_hw_params_any(alsa_handler, alsa_hwparams)) < 0)
	{
	  mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_UnableToGetInitialParameters,
		 snd_strerror(err));
	  return 0;
	}

      err = snd_pcm_hw_params_set_access(alsa_handler, alsa_hwparams,
					 SND_PCM_ACCESS_RW_INTERLEAVED);
      if (err < 0) {
	mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_UnableToSetAccessType,
	       snd_strerror(err));
	return 0;
      }

      /* workaround for nonsupported formats
	 sets default format to S16_LE if the given formats aren't supported */
      if ((err = snd_pcm_hw_params_test_format(alsa_handler, alsa_hwparams,
                                             alsa_format)) < 0)
      {
         mp_msg(MSGT_AO,MSGL_INFO,
		MSGTR_AO_ALSA_FormatNotSupportedByHardware, af_fmt2str_short(format));
         alsa_format = SND_PCM_FORMAT_S16_LE;
         if (AF_FORMAT_IS_AC3(ao_data.format))
           ao_data.format = AF_FORMAT_AC3_LE;
         else if (AF_FORMAT_IS_IEC61937(ao_data.format))
           ao_data.format = AF_FORMAT_IEC61937_LE;
         else
         ao_data.format = AF_FORMAT_S16_LE;
      }

      if ((err = snd_pcm_hw_params_set_format(alsa_handler, alsa_hwparams,
					      alsa_format)) < 0)
	{
	  mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_UnableToSetFormat,
		 snd_strerror(err));
	  return 0;
	}

      if ((err = snd_pcm_hw_params_set_channels_near(alsa_handler, alsa_hwparams,
						     &ao_data.channels)) < 0)
	{
	  mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_UnableToSetChannels,
		 snd_strerror(err));
	  return 0;
	}

      /* workaround for buggy rate plugin (should be fixed in ALSA 1.0.11)
         prefer our own resampler, since that allows users to choose the resampler,
         even per file if desired */
#if SND_LIB_VERSION >= 0x010009
      if ((err = snd_pcm_hw_params_set_rate_resample(alsa_handler, alsa_hwparams,
						     0)) < 0)
	{
	  mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_UnableToDisableResampling,
		 snd_strerror(err));
	  return 0;
	}
#endif

      if ((err = snd_pcm_hw_params_set_rate_near(alsa_handler, alsa_hwparams,
						 &ao_data.samplerate, NULL)) < 0)
        {
	  mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_UnableToSetSamplerate2,
		 snd_strerror(err));
	  return 0;
        }

      bytes_per_sample = af_fmt2bits(ao_data.format) / 8;
      bytes_per_sample *= ao_data.channels;
      ao_data.bps = ao_data.samplerate * bytes_per_sample;

	if ((err = snd_pcm_hw_params_set_buffer_time_near(alsa_handler, alsa_hwparams,
							  &alsa_buffer_time, NULL)) < 0)
	  {
	    mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_UnableToSetBufferTimeNear,
		   snd_strerror(err));
	    return 0;
	  }

	if ((err = snd_pcm_hw_params_set_periods_near(alsa_handler, alsa_hwparams,
						      &alsa_fragcount, NULL)) < 0) {
	  mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_UnableToSetPeriods,
		 snd_strerror(err));
	  return 0;
	}

      /* finally install hardware parameters */
      if ((err = snd_pcm_hw_params(alsa_handler, alsa_hwparams)) < 0)
	{
	  mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_UnableToSetHwParameters,
		 snd_strerror(err));
	  return 0;
	}
      // end setting hw-params


      // gets buffersize for control
      if ((err = snd_pcm_hw_params_get_buffer_size(alsa_hwparams, &bufsize)) < 0)
	{
	  mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_UnableToGetBufferSize, snd_strerror(err));
	  return 0;
	}
      else {
	ao_data.buffersize = bufsize * bytes_per_sample;
	  mp_msg(MSGT_AO,MSGL_V,"alsa-init: got buffersize=%i\n", ao_data.buffersize);
      }

      if ((err = snd_pcm_hw_params_get_period_size(alsa_hwparams, &chunk_size, NULL)) < 0) {
	mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_UnableToGetPeriodSize, snd_strerror(err));
	return 0;
      } else {
	mp_msg(MSGT_AO,MSGL_V,"alsa-init: got period size %li\n", chunk_size);
      }
      ao_data.outburst = chunk_size * bytes_per_sample;

      /* setting software parameters */
      if ((err = snd_pcm_sw_params_current(alsa_handler, alsa_swparams)) < 0) {
	mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_UnableToGetSwParameters,
	       snd_strerror(err));
	return 0;
      }
#if SND_LIB_VERSION >= 0x000901
      if ((err = snd_pcm_sw_params_get_boundary(alsa_swparams, &boundary)) < 0) {
	mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_UnableToGetBoundary,
	       snd_strerror(err));
	return 0;
      }
#else
      boundary = 0x7fffffff;
#endif
      /* start playing when one period has been written */
      if ((err = snd_pcm_sw_params_set_start_threshold(alsa_handler, alsa_swparams, chunk_size)) < 0) {
	mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_UnableToSetStartThreshold,
	       snd_strerror(err));
	return 0;
      }
      /* disable underrun reporting */
      if ((err = snd_pcm_sw_params_set_stop_threshold(alsa_handler, alsa_swparams, boundary)) < 0) {
	mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_UnableToSetStopThreshold,
	       snd_strerror(err));
	return 0;
      }
#if SND_LIB_VERSION >= 0x000901
      /* play silence when there is an underrun */
      if ((err = snd_pcm_sw_params_set_silence_size(alsa_handler, alsa_swparams, boundary)) < 0) {
	mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_UnableToSetSilenceSize,
	       snd_strerror(err));
	return 0;
      }
#endif
      if ((err = snd_pcm_sw_params(alsa_handler, alsa_swparams)) < 0) {
	mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_UnableToGetSwParameters,
	       snd_strerror(err));
	return 0;
      }
      /* end setting sw-params */

      mp_msg(MSGT_AO,MSGL_V,"alsa: %d Hz/%d channels/%d bpf/%d bytes buffer/%s\n",
	     ao_data.samplerate, ao_data.channels, (int)bytes_per_sample, ao_data.buffersize,
	     snd_pcm_format_description(alsa_format));

    } // end switch alsa_handler (spdif)
    alsa_can_pause = snd_pcm_hw_params_can_pause(alsa_hwparams);
    return 1;
} // end init


/* close audio device */
static void uninit(int immed)
{

  if (alsa_handler) {
    int err;

    if (!immed)
      snd_pcm_drain(alsa_handler);

    if ((err = snd_pcm_close(alsa_handler)) < 0)
      {
	mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_PcmCloseError, snd_strerror(err));
	return;
      }
    else {
      alsa_handler = NULL;
      mp_msg(MSGT_AO,MSGL_V,"alsa-uninit: pcm closed\n");
    }
  }
  else {
    mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_NoHandlerDefined);
  }
  snd_config_update_free_global();
}

static void audio_pause(void)
{
    int err;

    if (alsa_can_pause) {
        if ((err = snd_pcm_pause(alsa_handler, 1)) < 0)
        {
            mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_PcmPauseError, snd_strerror(err));
            return;
        }
          mp_msg(MSGT_AO,MSGL_V,"alsa-pause: pause supported by hardware\n");
    } else {
        prepause_space = get_space();
        if ((err = snd_pcm_drop(alsa_handler)) < 0)
        {
            mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_PcmDropError, snd_strerror(err));
            return;
        }
    }
}

static void audio_resume(void)
{
    int err;

    if (snd_pcm_state(alsa_handler) == SND_PCM_STATE_SUSPENDED) {
        mp_msg(MSGT_AO,MSGL_INFO,MSGTR_AO_ALSA_PcmInSuspendModeTryingResume);
        while ((err = snd_pcm_resume(alsa_handler)) == -EAGAIN) sleep(1);
    }
    if (alsa_can_pause) {
        if ((err = snd_pcm_pause(alsa_handler, 0)) < 0)
        {
            mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_PcmResumeError, snd_strerror(err));
            return;
        }
          mp_msg(MSGT_AO,MSGL_V,"alsa-resume: resume supported by hardware\n");
    } else {
        if ((err = snd_pcm_prepare(alsa_handler)) < 0)
        {
           mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_PcmPrepareError, snd_strerror(err));
            return;
        }
        mp_ao_resume_refill(&audio_out_alsa, prepause_space);
    }
}

/* stop playing and empty buffers (for seeking/pause) */
static void reset(void)
{
    int err;

    if ((err = snd_pcm_drop(alsa_handler)) < 0)
    {
	mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_PcmPrepareError, snd_strerror(err));
	return;
    }
    if ((err = snd_pcm_prepare(alsa_handler)) < 0)
    {
	mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_PcmPrepareError, snd_strerror(err));
	return;
    }
    return;
}

/*
    plays 'len' bytes of 'data'
    returns: number of bytes played
    modified last at 29.06.02 by jp
    thanxs for marius <marius@rospot.com> for giving us the light ;)
*/

static int play(void* data, int len, int flags)
{
  int num_frames;
  snd_pcm_sframes_t res = 0;
  if (!(flags & AOPLAY_FINAL_CHUNK))
      len = len / ao_data.outburst * ao_data.outburst;
  num_frames = len / bytes_per_sample;

  //mp_msg(MSGT_AO,MSGL_ERR,"alsa-play: frames=%i, len=%i\n",num_frames,len);

  if (!alsa_handler) {
    mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_DeviceConfigurationError);
    return 0;
  }

  if (num_frames == 0)
    return 0;

  do {
    res = snd_pcm_writei(alsa_handler, data, num_frames);

      if (res == -EINTR) {
	/* nothing to do */
	res = 0;
      }
      else if (res == -ESTRPIPE) {	/* suspend */
	mp_msg(MSGT_AO,MSGL_INFO,MSGTR_AO_ALSA_PcmInSuspendModeTryingResume);
	while ((res = snd_pcm_resume(alsa_handler)) == -EAGAIN)
	  sleep(1);
      }
      if (res < 0) {
	mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_WriteError, snd_strerror(res));
	mp_msg(MSGT_AO,MSGL_INFO,MSGTR_AO_ALSA_TryingToResetSoundcard);
	if ((res = snd_pcm_prepare(alsa_handler)) < 0) {
	  mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_PcmPrepareError, snd_strerror(res));
	  break;
	}
	res = 0;
      }
  } while (res == 0);

  return res < 0 ? 0 : res * bytes_per_sample;
}

/* how many byes are free in the buffer */
static int get_space(void)
{
    snd_pcm_status_t *status;
    int ret;

    snd_pcm_status_alloca(&status);

    if ((ret = snd_pcm_status(alsa_handler, status)) < 0)
    {
	mp_msg(MSGT_AO,MSGL_ERR,MSGTR_AO_ALSA_CannotGetPcmStatus, snd_strerror(ret));
	return 0;
    }

    ret = snd_pcm_status_get_avail(status) * bytes_per_sample;
    if (ret > ao_data.buffersize)  // Buffer underrun?
	ret = ao_data.buffersize;
    return ret;
}

/* delay in seconds between first and last sample in buffer */
static float get_delay(void)
{
  if (alsa_handler) {
    snd_pcm_sframes_t delay;

    if (snd_pcm_delay(alsa_handler, &delay) < 0)
      return 0;

    if (delay < 0) {
      /* underrun - move the application pointer forward to catch up */
#if SND_LIB_VERSION >= 0x000901 /* snd_pcm_forward() exists since 0.9.0rc8 */
      snd_pcm_forward(alsa_handler, -delay);
#endif
      delay = 0;
    }
    return (float)delay / (float)ao_data.samplerate;
  } else {
    return 0;
  }
}
