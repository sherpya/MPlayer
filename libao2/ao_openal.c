/*
 * OpenAL audio output driver for MPlayer
 *
 * Copyleft 2006 by Reimar Döffinger (Reimar.Doeffinger@stud.uni-karlsruhe.de)
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
 * along with MPlayer; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#ifdef OPENAL_AL_H
#include <OpenAL/alc.h>
#include <OpenAL/al.h>
#else
#include <AL/alc.h>
#include <AL/al.h>
#endif

#include "mp_msg.h"
#include "help_mp.h"

#include "audio_out.h"
#include "audio_out_internal.h"
#include "libaf/af_format.h"
#include "osdep/timer.h"
#include "subopt-helper.h"

static const ao_info_t info =
{
  "OpenAL audio output",
  "openal",
  "Reimar Döffinger <Reimar.Doeffinger@stud.uni-karlsruhe.de>",
  ""
};

LIBAO_EXTERN(openal)

#define MAX_CHANS 8
#define NUM_BUF 128
#define CHUNK_SIZE 512
static ALuint buffers[MAX_CHANS][NUM_BUF];
static ALuint sources[MAX_CHANS];

static int cur_buf[MAX_CHANS];
static int unqueue_buf[MAX_CHANS];
static int16_t *tmpbuf;
static int num_sources;


static int control(int cmd, void *arg) {
  switch (cmd) {
    case AOCONTROL_GET_VOLUME:
    case AOCONTROL_SET_VOLUME: {
      ALfloat volume;
      ao_control_vol_t *vol = (ao_control_vol_t *)arg;
      if (cmd == AOCONTROL_SET_VOLUME) {
        volume = (vol->left + vol->right) / 200.0;
        alListenerf(AL_GAIN, volume);
      }
      alGetListenerf(AL_GAIN, &volume);
      vol->left = vol->right = volume * 100;
      return CONTROL_TRUE;
    }
  }
  return CONTROL_UNKNOWN;
}

/**
 * \brief print suboption usage help
 */
static void print_help(void) {
  mp_msg(MSGT_AO, MSGL_FATAL,
          "\n-ao openal commandline help:\n"
          "Example: mplayer -ao openal\n"
          "\nOptions:\n"
        );
}

static int init(int rate, int channels, int format, int flags) {
  float position[3] = {0, 0, 0};
  float direction[6] = {0, 0, 1, 0, -1, 0};
  float sppos[MAX_CHANS][3] = {
    {-1, 0, 0.5}, {1, 0, 0.5},
    {-1, 0,  -1}, {1, 0,  -1},
    {0,  0,   1}, {0, 0, 0.1},
    {-1, 0,   0}, {1, 0,   0},
  };
  ALCdevice *dev = NULL;
  ALCcontext *ctx = NULL;
  ALCint freq = 0;
  ALCint attribs[] = {ALC_FREQUENCY, rate, 0, 0};
  int i;
  const opt_t subopts[] = {
    {NULL}
  };
  if (subopt_parse(ao_subdevice, subopts) != 0) {
    print_help();
    return 0;
  }
  if (channels > MAX_CHANS) {
    mp_msg(MSGT_AO, MSGL_FATAL, "[OpenAL] Invalid number of channels: %i\n", channels);
    goto err_out;
  }
  dev = alcOpenDevice(NULL);
  if (!dev) {
    mp_msg(MSGT_AO, MSGL_FATAL, "[OpenAL] could not open device\n");
    goto err_out;
  }
  ctx = alcCreateContext(dev, attribs);
  alcMakeContextCurrent(ctx);
  alListenerfv(AL_POSITION, position);
  alListenerfv(AL_ORIENTATION, direction);
  // play stereo using a single source - this is closer
  // to the result from other aos and what people expect.
  num_sources = channels <= 2 ? 1 : channels;
  alGenSources(num_sources, sources);
  for (i = 0; i < num_sources; i++) {
    cur_buf[i] = 0;
    unqueue_buf[i] = 0;
    alGenBuffers(NUM_BUF, buffers[i]);
    alSourcefv(sources[i], AL_POSITION, sppos[i]);
    alSource3f(sources[i], AL_VELOCITY, 0, 0, 0);
  }
  if (num_sources == 1)
    alSource3f(sources[0], AL_POSITION, 0, 0, 1);
  ao_data.channels = channels;
  alcGetIntegerv(dev, ALC_FREQUENCY, 1, &freq);
  if (alcGetError(dev) == ALC_NO_ERROR && freq)
    rate = freq;
  ao_data.samplerate = rate;
  ao_data.format = AF_FORMAT_S16_NE;
  ao_data.bps = channels * rate * 2;
  ao_data.buffersize = CHUNK_SIZE * NUM_BUF;
  ao_data.outburst = channels * CHUNK_SIZE;
  tmpbuf = malloc(CHUNK_SIZE);
  return 1;

err_out:
  return 0;
}

// close audio device
static void uninit(int immed) {
  ALCcontext *ctx = alcGetCurrentContext();
  ALCdevice *dev = alcGetContextsDevice(ctx);
  free(tmpbuf);
  if (!immed) {
    ALint state;
    alGetSourcei(sources[0], AL_SOURCE_STATE, &state);
    while (state == AL_PLAYING) {
      usec_sleep(10000);
      alGetSourcei(sources[0], AL_SOURCE_STATE, &state);
    }
  }
  reset();
  alcMakeContextCurrent(NULL);
  alcDestroyContext(ctx);
  alcCloseDevice(dev);
}

static void unqueue_buffers(void) {
  ALint p;
  int s;
  for (s = 0; s < num_sources; s++) {
    int till_wrap = NUM_BUF - unqueue_buf[s];
    alGetSourcei(sources[s], AL_BUFFERS_PROCESSED, &p);
    if (p >= till_wrap) {
      alSourceUnqueueBuffers(sources[s], till_wrap, &buffers[s][unqueue_buf[s]]);
      unqueue_buf[s] = 0;
      p -= till_wrap;
    }
    if (p) {
      alSourceUnqueueBuffers(sources[s], p, &buffers[s][unqueue_buf[s]]);
      unqueue_buf[s] += p;
    }
  }
}

/**
 * \brief stop playing and empty buffers (for seeking/pause)
 */
static void reset(void) {
  alSourceStopv(num_sources, sources);
  unqueue_buffers();
}

/**
 * \brief stop playing, keep buffers (for pause)
 */
static void audio_pause(void) {
  alSourcePausev(num_sources, sources);
}

/**
 * \brief resume playing, after audio_pause()
 */
static void audio_resume(void) {
  alSourcePlayv(num_sources, sources);
}

static int get_space(void) {
  ALint queued;
  unqueue_buffers();
  alGetSourcei(sources[0], AL_BUFFERS_QUEUED, &queued);
  queued = NUM_BUF - queued - 3;
  if (queued < 0) return 0;
  return queued * CHUNK_SIZE * num_sources;
}

/**
 * \brief write data into buffer and reset underrun flag
 */
static int play(void *data, int len, int flags) {
  ALint state;
  int i, j, k;
  int ch;
  int16_t *d = data;
  int format = ao_data.channels == 2 && num_sources == 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
  len /= num_sources * CHUNK_SIZE;
  for (i = 0; i < len; i++) {
    for (ch = 0; ch < num_sources; ch++) {
      for (j = 0, k = ch; j < CHUNK_SIZE / 2; j++, k += num_sources)
        tmpbuf[j] = d[k];
      alBufferData(buffers[ch][cur_buf[ch]], format, tmpbuf,
                     CHUNK_SIZE, ao_data.samplerate);
      alSourceQueueBuffers(sources[ch], 1, &buffers[ch][cur_buf[ch]]);
      cur_buf[ch] = (cur_buf[ch] + 1) % NUM_BUF;
    }
    d += num_sources * CHUNK_SIZE / 2;
  }
  alGetSourcei(sources[0], AL_SOURCE_STATE, &state);
  if (state != AL_PLAYING) // checked here in case of an underrun
    alSourcePlayv(num_sources, sources);
  return len * num_sources * CHUNK_SIZE;
}

static float get_delay(void) {
  ALint queued;
  unqueue_buffers();
  alGetSourcei(sources[0], AL_BUFFERS_QUEUED, &queued);
  return queued * CHUNK_SIZE / 2 / (float)ao_data.samplerate;
}
