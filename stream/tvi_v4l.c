/*
 * Video 4 Linux input
 *
 * Copyright (C) 2001 Alex Beregszaszi
 *
 * Some ideas are based on xawtv/libng's grab-v4l.c written by
 *   Gerd Knorr <kraxel@bytesex.org>
 *
 * Multithreading, a/v sync and native ALSA support by
 *   Jindrich Makovicka <makovick@gmail.com>
 *
 * MJPEG hardware encoding support by
 *   Ivan Szanto <szivan@freemail.hu>
 *
 * CODE IS UNDER DEVELOPMENT, NO FEATURE REQUESTS PLEASE!
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

#include "config.h"

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>

/* Necessary to prevent collisions between <linux/time.h> and <sys/time.h> when V4L2 is installed. */
#define _LINUX_TIME_H

#include <linux/videodev.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#ifdef HAVE_SYS_SYSINFO_H
#include <sys/sysinfo.h>
#endif

#include "libavutil/avstring.h"
#include "mp_msg.h"
#include "libaf/af_format.h"
#include "libmpcodecs/img_format.h"
#include "libmpcodecs/dec_teletext.h"
#include "libvo/fastmemcpy.h"
#include "libvo/videodev_mjpeg.h"

#include "tv.h"

#include "audio_in.h"

static tvi_handle_t *tvi_init_v4l(tv_param_t* tv_param);

const tvi_info_t tvi_info_v4l = {
    tvi_init_v4l,
    "Video 4 Linux input",
    "v4l",
    "Alex Beregszaszi",
    "under development"
};

#define PAL_WIDTH  768
#define PAL_HEIGHT 576
#define PAL_FPS    25

#define NTSC_WIDTH  640
#define NTSC_HEIGHT 480
#define NTSC_FPS    (30000.0/1001.0)

#define MAX_AUDIO_CHANNELS      10

#define VID_BUF_SIZE_IMMEDIATE   2
#define VIDEO_AVG_BUFFER_SIZE  600

typedef struct priv {
    /* general */
    char                        *video_device;
    int                         video_fd;
    struct video_capability     capability;
    struct video_channel        *channels;
    int                         act_channel;
    struct video_tuner          tuner;

    /* video */
    struct video_picture        picture;
    int                         format;         /* output format */
    int                         width;
    int                         height;
    int                         bytesperline;
    float                       fps;

    struct video_mbuf           mbuf;
    unsigned char               *mmap;
    struct video_mmap           *buf;
    int                         nbuf;

    /* audio */
    char                        *audio_device;
    audio_in_t                  audio_in;

    int                         audio_id;
    struct video_audio          audio[MAX_AUDIO_CHANNELS];
    int                         audio_channels[MAX_AUDIO_CHANNELS];

    /* buffering stuff */
    int                         immediate_mode;

    int                         audio_buffer_size;
    int                         aud_skew_cnt;
    unsigned char               *audio_ringbuffer;
    long long                   *audio_skew_buffer;
    volatile int                audio_head;
    volatile int                audio_tail;
    volatile int                audio_cnt;
    volatile long long          audio_skew;
    volatile double             audio_skew_factor;
    volatile long long          audio_skew_measure_time;
    volatile int                audio_drop;

    int                         first;
    int                         video_buffer_size_max;
    volatile int                video_buffer_size_current;
    unsigned char               **video_ringbuffer;
    long long                   *video_timebuffer;
    long long                   *video_avg_buffer;
    int                         video_avg_ptr;
    int                         video_interval_sum;
    volatile int                video_head;
    volatile int                video_tail;
    volatile int                video_cnt;

    volatile int                shutdown;

    pthread_t                   audio_grabber_thread;
    pthread_t                   video_grabber_thread;
    pthread_mutex_t             audio_starter;
    pthread_mutex_t             skew_mutex;
    pthread_mutex_t             video_buffer_mutex;

    long long                   starttime;
    double                      audio_secs_per_block;
    long long                   audio_skew_total;
    long                        audio_recv_blocks_total;
    long                        audio_sent_blocks_total;
    long                        mjpeg_bufsize;
    char                        *vbi_dev;
    int                         vbi_fd;
    int                         vbi_bufsize;
    int                         vbi_shutdown;
    pthread_t                   vbi_grabber_thread;
    void                        *priv_vbi;

    tv_param_t                  *tv_param;
} priv_t;

#include "tvi_def.h"

static const char *device_cap2name[] = {
    "capture", "tuner", "teletext", "overlay", "chromakey", "clipping",
    "frameram", "scales", "monochrome", "subcapture", "mpeg-decoder",
    "mpeg-encoder", "mjpeg-decoder", "mjpeg-encoder", NULL
};

static const char *device_palette2name[] = {
    "-", "grey", "hi240", "rgb16", "rgb24", "rgb32", "rgb15", "yuv422",
    "yuyv", "uyvy", "yuv420", "yuv411", "raw", "yuv422p", "yuv411p",
    "yuv420p", "yuv410p"
};
#define PALETTE(x) ((x < sizeof(device_palette2name)/sizeof(char*)) ? device_palette2name[x] : "UNKNOWN")

static const struct {
    char* name;
    int normid;
    int tuner_flags;
    int tuner_mode;
    int input_norm;
    float fps;
} supported_norms[]={
    {"pal",   TV_NORM_PAL,    VIDEO_TUNER_PAL,   VIDEO_MODE_PAL,  VIDEO_MODE_PAL,  PAL_FPS },
    {"ntsc",  TV_NORM_NTSC,   VIDEO_TUNER_NTSC,  VIDEO_MODE_NTSC, VIDEO_MODE_NTSC, NTSC_FPS},
    {"secam", TV_NORM_SECAM,  VIDEO_TUNER_SECAM, VIDEO_MODE_SECAM,VIDEO_MODE_SECAM,PAL_FPS },
    {"palnc", TV_NORM_PALNC,  VIDEO_TUNER_PAL,   VIDEO_MODE_PAL,  3,               PAL_FPS },
    {"palm",  TV_NORM_PALM,   VIDEO_TUNER_NTSC,  VIDEO_MODE_NTSC, 4,               NTSC_FPS},
    {"paln",  TV_NORM_PALN,   VIDEO_TUNER_PAL,   VIDEO_MODE_PAL,  5,               PAL_FPS },
    {"ntscjp",TV_NORM_NTSCJP, VIDEO_TUNER_NTSC,  VIDEO_MODE_NTSC, 6,               NTSC_FPS},
    {"auto",  -1,             -1,                -1,              VIDEO_MODE_AUTO, -1      },
    {NULL,    -1,             -1,                -1,              -1      }
};

static const char *norm2name(int mode)
{
    int i;
    for(i=0;supported_norms[i].name; i++)
        if(supported_norms[i].input_norm==mode)
            return supported_norms[i].name;
    return "unknown";
};

static const char *audio_mode2name(int mode)
{
    switch (mode) {
    case VIDEO_SOUND_MONO:
        return "mono";
    case VIDEO_SOUND_STEREO:
        return "stereo";
    case VIDEO_SOUND_LANG1:
        return "language1";
    case VIDEO_SOUND_LANG2:
        return "language2";
    default:
        return "unknown";
    }
};

static void *audio_grabber(void *data);
static void *video_grabber(void *data);

static int palette2depth(int palette)
{
    switch(palette)
    {
        /* component */
        case VIDEO_PALETTE_RGB555:
            return 15;
        case VIDEO_PALETTE_RGB565:
            return 16;
        case VIDEO_PALETTE_RGB24:
            return 24;
        case VIDEO_PALETTE_RGB32:
            return 32;
        /* planar */
        case VIDEO_PALETTE_YUV411P:
        case VIDEO_PALETTE_YUV420P:
        case VIDEO_PALETTE_YUV410P:
            return 12;
        /* packed */
        case VIDEO_PALETTE_YUV422P:
        case VIDEO_PALETTE_YUV422:
        case VIDEO_PALETTE_YUYV:
        case VIDEO_PALETTE_UYVY:
        case VIDEO_PALETTE_YUV420:
        case VIDEO_PALETTE_YUV411:
            return 16;
    }
    return -1;
}

static int format2palette(int format)
{
    switch(format)
    {
        case IMGFMT_BGR15:
            return VIDEO_PALETTE_RGB555;
        case IMGFMT_BGR16:
            return VIDEO_PALETTE_RGB565;
        case IMGFMT_BGR24:
            return VIDEO_PALETTE_RGB24;
        case IMGFMT_BGR32:
            return VIDEO_PALETTE_RGB32;
        case IMGFMT_YV12:
        case IMGFMT_I420:
            return VIDEO_PALETTE_YUV420P;
        case IMGFMT_YUY2:
            return VIDEO_PALETTE_YUV422;
    case IMGFMT_UYVY:
       return VIDEO_PALETTE_UYVY;
    }
    return -1;
}

// sets and sanitizes audio buffer/block sizes
static void setup_audio_buffer_sizes(priv_t *priv)
{
    int bytes_per_sample = priv->audio_in.bytes_per_sample;

    // make the audio buffer at least 5 seconds long
    priv->audio_buffer_size = 1 + 5*priv->audio_in.samplerate
        *priv->audio_in.channels
        *bytes_per_sample/priv->audio_in.blocksize;
    if (priv->audio_buffer_size < 256) priv->audio_buffer_size = 256;

    // make the skew buffer at least 1 second long
    priv->aud_skew_cnt = 1 + 1*priv->audio_in.samplerate
        *priv->audio_in.channels
        *bytes_per_sample/priv->audio_in.blocksize;
    if (priv->aud_skew_cnt < 16) priv->aud_skew_cnt = 16;

    mp_msg(MSGT_TV, MSGL_V, "Audio capture - buffer %d blocks of %d bytes, skew average from %d meas.\n",
           priv->audio_buffer_size, priv->audio_in.blocksize, priv->aud_skew_cnt);
}

static tvi_handle_t *tvi_init_v4l(tv_param_t* tv_param)
{
    tvi_handle_t *h;
    priv_t *priv;

    h = tv_new_handle(sizeof(priv_t), &functions);
    if (!h)
        return NULL;

    priv = h->priv;

    /* set video device name */
    if (!tv_param->device)
        priv->video_device = strdup("/dev/video0");
    else
        priv->video_device = strdup(tv_param->device);

    /* set video device name */
    if (!tv_param->adevice)
        priv->audio_device = NULL;
    else {
        priv->audio_device = strdup(tv_param->adevice);
    }

    /* allocation failed */
    if (!priv->video_device) {
        tv_free_handle(h);
        return NULL;
    }

    priv->tv_param=tv_param;
    return h;
}

/* retrieves info about audio channels from the BTTV */
static void init_v4l_audio(priv_t *priv)
{
    int i;
    int reqmode;

    if (!priv->capability.audios) return;

    /* audio chanlist */

    mp_msg(MSGT_TV, MSGL_V, " Audio devices: %d\n", priv->capability.audios);

    mp_msg(MSGT_TV, MSGL_V, "Video capture card reports the audio setup as follows:\n");
    for (i = 0; i < priv->capability.audios; i++)
    {
        if (i >= MAX_AUDIO_CHANNELS)
        {
            mp_msg(MSGT_TV, MSGL_ERR, "no space for more audio channels (increase in source!) (%d > %d)\n",
                   i, MAX_AUDIO_CHANNELS);
            i = priv->capability.audios;
            break;
        }

        priv->audio[i].audio = i;
        if (ioctl(priv->video_fd, VIDIOCGAUDIO, &priv->audio[i]) == -1)
        {
            mp_msg(MSGT_TV, MSGL_ERR, "ioctl get audio failed: %s\n", strerror(errno));
            break;
        }

        /* mute all channels */
        priv->audio[i].flags |= VIDEO_AUDIO_MUTE;
        reqmode = -1;
        if (priv->tv_param->amode >= 0) {
            switch (priv->tv_param->amode) {
            case 0:
                reqmode = VIDEO_SOUND_MONO;
                break;
            case 1:
                reqmode = VIDEO_SOUND_STEREO;
                break;
            case 2:
                reqmode = VIDEO_SOUND_LANG1;
                break;
            case 3:
                reqmode = VIDEO_SOUND_LANG2;
                break;
            default:
                mp_msg(MSGT_TV, MSGL_ERR, "Unknown audio mode requested.\n");
                break;
            }
            if (reqmode >= 0) priv->audio[i].mode = reqmode;
        }
        ioctl(priv->video_fd, VIDIOCSAUDIO, &priv->audio[i]);

        // get the parameters back
        if (ioctl(priv->video_fd, VIDIOCGAUDIO, &priv->audio[i]) == -1)
        {
            mp_msg(MSGT_TV, MSGL_ERR, "ioctl get audio failed: %s\n", strerror(errno));
            break;
        }

        switch(priv->audio[i].mode)
        {
        case VIDEO_SOUND_MONO:
        case VIDEO_SOUND_LANG1:
        case VIDEO_SOUND_LANG2:
            priv->audio_channels[i] = 1;
            break;
        case VIDEO_SOUND_STEREO:
            priv->audio_channels[i] = 2;
            break;
        default:
            mp_msg(MSGT_TV, MSGL_ERR, "Card reports an unknown audio mode !\n");
            mp_msg(MSGT_TV, MSGL_ERR, "Trying two channel audio. Use forcechan to override.\n");
            priv->audio_channels[i] = 2;
            break;
        }

        if (reqmode >= 0 && priv->audio[i].mode != reqmode) {
            mp_msg(MSGT_TV, MSGL_ERR, "Audio mode setup warning!\n");
            mp_msg(MSGT_TV, MSGL_ERR, "Requested mode was %s, but v4l still reports %s.\n",
                   audio_mode2name(reqmode), audio_mode2name(priv->audio[i].mode));
            mp_msg(MSGT_TV, MSGL_ERR, "You may need \"forcechan\" option to force stereo/mono audio recording.\n");
        }

        /* display stuff */
        mp_msg(MSGT_TV, MSGL_V, "  %d: %s: ", priv->audio[i].audio,
               priv->audio[i].name);
        if (priv->audio[i].flags & VIDEO_AUDIO_MUTABLE) {
            mp_msg(MSGT_TV, MSGL_V, "muted=%s ",
                   (priv->audio[i].flags & VIDEO_AUDIO_MUTE) ? "yes" : "no");
        }
        mp_msg(MSGT_TV, MSGL_V, "vol=%d bass=%d treble=%d balance=%d mode=%s",
               priv->audio[i].volume, priv->audio[i].bass, priv->audio[i].treble,
               priv->audio[i].balance, audio_mode2name(priv->audio[i].mode));
        mp_msg(MSGT_TV, MSGL_V, " chan=%d\n", priv->audio_channels[i]);

        if (priv->tv_param->forcechan >= 0)
            priv->audio_channels[i] = priv->tv_param->forcechan;

        // we'll call VIDIOCSAUDIO again when starting capture
        // let's set audio mode to requested mode again for the case
        // when VIDIOCGAUDIO just cannot report the mode correctly
        if (reqmode >= 0) priv->audio[i].mode = reqmode;
    }
}

#if !defined(__LINUX_VIDEODEV2_H) && !defined(VIDIOC_QUERYCAP)
struct v4l2_capability
{
        __u8    driver[16];     /* i.e. "bttv" */
        __u8    card[32];       /* i.e. "Hauppauge WinTV" */
        __u8    bus_info[32];   /* "PCI:" + pci_dev->slot_name */
        __u32   version;        /* should use KERNEL_VERSION() */
        __u32   capabilities;   /* Device capabilities */
        __u32   reserved[4];
};

#define VIDIOC_QUERYCAP         _IOR  ('V',  0, struct v4l2_capability)
#endif

static int init(priv_t *priv)
{
    int i;

    if (priv->tv_param->immediate == 1)
        priv->tv_param->noaudio = 1;

    priv->video_ringbuffer = NULL;
    priv->video_timebuffer = NULL;
    priv->video_avg_buffer = NULL;
    priv->audio_ringbuffer = NULL;
    priv->audio_skew_buffer = NULL;

    priv->video_fd = open(priv->video_device, O_RDWR);
    mp_msg(MSGT_TV, MSGL_DBG2, "Video fd: %d, %p\n", priv->video_fd,
        priv->video_device);
    if (priv->video_fd == -1)
    {
        mp_msg(MSGT_TV, MSGL_ERR, "unable to open '%s': %s\n",
            priv->video_device, strerror(errno));
        goto err;
    }

    /* check for v4l2 */
    if (ioctl(priv->video_fd, VIDIOC_QUERYCAP, &priv->capability) == 0) {
        mp_msg(MSGT_TV, MSGL_ERR, "=================================================================\n");
        mp_msg(MSGT_TV, MSGL_ERR, " WARNING: YOU ARE USING V4L DEMUXER WITH V4L2 DRIVERS!!!\n");
        mp_msg(MSGT_TV, MSGL_ERR, " As the V4L1 compatibility layer is broken, this may not work.\n");
        mp_msg(MSGT_TV, MSGL_ERR, " If you encounter any problems, use driver=v4l2 instead.\n");
        mp_msg(MSGT_TV, MSGL_ERR, " Bugreports on driver=v4l with v4l2 drivers will be ignored.\n");
        mp_msg(MSGT_TV, MSGL_ERR, "=================================================================\n");
    }

    /* get capabilities (priv->capability is needed!) */
    if (ioctl(priv->video_fd, VIDIOCGCAP, &priv->capability) == -1)
    {
        mp_msg(MSGT_TV, MSGL_ERR, "ioctl get capabilities failed: %s\n", strerror(errno));
        goto err;
    }

    fcntl(priv->video_fd, F_SETFD, FD_CLOEXEC);

    mp_msg(MSGT_TV, MSGL_INFO, "Selected device: %s\n", priv->capability.name);
    mp_msg(MSGT_TV, MSGL_INFO, " Capabilities: ");
    for (i = 0; device_cap2name[i] != NULL; i++)
        if (priv->capability.type & (1 << i))
            mp_msg(MSGT_TV, MSGL_INFO, "%s ", device_cap2name[i]);
    mp_msg(MSGT_TV, MSGL_INFO, "\n");
    mp_msg(MSGT_TV, MSGL_INFO, " Device type: %d\n", priv->capability.type);
    mp_msg(MSGT_TV, MSGL_INFO, " Supported sizes: %dx%d => %dx%d\n",
        priv->capability.minwidth, priv->capability.minheight,
        priv->capability.maxwidth, priv->capability.maxheight);
    priv->width = priv->capability.minwidth;
    priv->height = priv->capability.minheight;

    /* somewhere here could disable priv->tv_param->mjpeg, if it is not a capability */

    /* initialize if necessary */
    if ( priv->tv_param->mjpeg )
    {
        struct mjpeg_params bparm;
        struct mjpeg_requestbuffers breq;          /* buffer requests */

        if (ioctl(priv->video_fd, MJPIOC_G_PARAMS, &bparm) < 0)
        {
            mp_msg(MSGT_TV, MSGL_ERR,
                   "  MJP: Error getting video parameters: %s\n", strerror(errno));
            goto err;
        }

        mp_msg(MSGT_TV, MSGL_INFO,
               "  MJP: previous params: x: %d, y: %d, w: %d, h: %d, decim: %d, fields: %d,\n",
               bparm.img_x, bparm.img_y, bparm.img_width, bparm.img_height,
               bparm.decimation, bparm.field_per_buff);

        mp_msg(MSGT_TV, MSGL_INFO,
               "  MJP: HorDcm: %d, VerDcm: %d, TmpDcm: %d\n",
               bparm.HorDcm, bparm.VerDcm, bparm.TmpDcm);

        bparm.input = priv->tv_param->input; /* tv */
        if (!av_strcasecmp(priv->tv_param->norm, "pal"))
            bparm.norm =  0; /* PAL */
        else if (!av_strcasecmp(priv->tv_param->norm, "ntsc"))
            bparm.norm =  1; /* NTSC */
        else if (!av_strcasecmp(priv->tv_param->norm, "secam"))
            bparm.norm =  2; /* SECAM */
        bparm.quality = priv->tv_param->quality;
        bparm.decimation = priv->tv_param->decimation;

        mp_msg(MSGT_TV, MSGL_INFO, "  MJP: setting params to decimation: %d, quality: %d\n",
               bparm.decimation, bparm.quality);

        if (ioctl(priv->video_fd, MJPIOC_S_PARAMS, &bparm) < 0)
        {
            mp_msg(MSGT_TV, MSGL_ERR,
                   "  MJP: Error setting video parameters: %s\n", strerror(errno));
            goto err;
        }

        if (ioctl(priv->video_fd, MJPIOC_G_PARAMS, &bparm) < 0)
        {
            mp_msg(MSGT_TV, MSGL_ERR,
                   "  MJP: Error getting video parameters: %s\n", strerror(errno));
            goto err;
        }

        mp_msg(MSGT_TV, MSGL_INFO,
               "  MJP: current params: x: %d, y: %d, w: %d, h: %d, decim: %d, fields: %d,\n",
               bparm.img_x, bparm.img_y, bparm.img_width, bparm.img_height,
               bparm.decimation, bparm.field_per_buff);

        mp_msg(MSGT_TV, MSGL_INFO,
               "  MJP: HorDcm: %d, VerDcm: %d, TmpDcm: %d\n",
               bparm.HorDcm, bparm.VerDcm, bparm.TmpDcm);


        breq.count = 64;
        priv -> nbuf = breq.count;
        priv->mbuf.frames = priv -> nbuf;
        priv->mjpeg_bufsize = 256*1024;
        if (priv->tv_param->buffer_size >= 0)
            priv->mjpeg_bufsize = priv->tv_param->buffer_size*1024;
        breq.size  = priv -> mjpeg_bufsize;
        if (ioctl(priv->video_fd, MJPIOC_REQBUFS,&(breq)) < 0)
        {
           mp_msg (MSGT_TV, MSGL_ERR,
              "  MJP: Error requesting video buffers: %s\n", strerror(errno));
           goto err;
        }
        mp_msg(MSGT_TV, MSGL_INFO,
               "  MJP: Got %ld buffers of size %ld KB\n",
               breq.count, breq.size/1024);

        priv -> mmap = mmap(0, breq.count * breq.size,
            PROT_READ|PROT_WRITE, MAP_SHARED, priv->video_fd, 0);
        if (priv -> mmap == MAP_FAILED)
        {
            mp_msg(MSGT_TV, MSGL_INFO,
                   "  MJP: Error mapping video buffers: %s\n", strerror(errno));
            goto err;
        }
    }

    mp_msg(MSGT_TV, MSGL_INFO, " Inputs: %d\n", priv->capability.channels);
    priv->channels = calloc(priv->capability.channels, sizeof(struct video_channel));
    if (!priv->channels)
        goto malloc_failed;
    memset(priv->channels, 0, sizeof(struct video_channel)*priv->capability.channels);
    for (i = 0; i < priv->capability.channels; i++)
    {
        priv->channels[i].channel = i;
        if (ioctl(priv->video_fd, VIDIOCGCHAN, &priv->channels[i]) == -1)
        {
            mp_msg(MSGT_TV, MSGL_ERR, "ioctl get channel failed: %s\n", strerror(errno));
            break;
        }
        mp_msg(MSGT_TV, MSGL_INFO, "  %d: %s: %s%s%s%s (tuner:%d, norm:%s)\n", i,
            priv->channels[i].name,
            (priv->channels[i].flags & VIDEO_VC_TUNER) ? "tuner " : "",
            (priv->channels[i].flags & VIDEO_VC_AUDIO) ? "audio " : "",
            (priv->channels[i].flags & VIDEO_TYPE_TV) ? "tv " : "",
            (priv->channels[i].flags & VIDEO_TYPE_CAMERA) ? "camera " : "",
            priv->channels[i].tuners,
            norm2name(priv->channels[i].norm));
    }
    priv->act_channel = 0;

    if (!(priv->capability.type & VID_TYPE_CAPTURE))
    {
        mp_msg(MSGT_TV, MSGL_ERR, "Only grabbing supported (for overlay use another program)\n");
        goto err;
    }


    /* init v4l audio even when we don't capture */
    init_v4l_audio(priv);

    if (!priv->capability.audios && !priv->tv_param->force_audio) priv->tv_param->noaudio = 1;

    /* audio init */
    if (!priv->tv_param->noaudio) {

#ifdef CONFIG_ALSA
        if (priv->tv_param->alsa)
            audio_in_init(&priv->audio_in, AUDIO_IN_ALSA);
        else
            audio_in_init(&priv->audio_in, AUDIO_IN_OSS);
#else
        audio_in_init(&priv->audio_in, AUDIO_IN_OSS);
#endif

        if (priv->audio_device) {
            audio_in_set_device(&priv->audio_in, priv->audio_device);
        }

        if (priv->tv_param->audio_id < priv->capability.audios)
            priv->audio_id = priv->tv_param->audio_id;
        else
            priv->audio_id = 0;
        audio_in_set_samplerate(&priv->audio_in, 44100);
        if (priv->capability.audios) {
            audio_in_set_channels(&priv->audio_in, priv->audio_channels[priv->audio_id]);
        } else {
            if (priv->tv_param->forcechan >= 0) {
                audio_in_set_channels(&priv->audio_in, priv->tv_param->forcechan);
            } else {
                audio_in_set_channels(&priv->audio_in, 2);
            }
        }
        if (audio_in_setup(&priv->audio_in) < 0) return 0;
        setup_audio_buffer_sizes(priv);
    }

    return 1;

malloc_failed:
    free(priv->channels);
    free(priv->buf);
err:
    if (priv->video_fd != -1)
        close(priv->video_fd);
    return 0;
}

static int uninit(priv_t *priv)
{
    unsigned long num;

    priv->vbi_shutdown=1;
    if(priv->vbi_grabber_thread)
        pthread_join(priv->vbi_grabber_thread, NULL);

    teletext_control(priv->priv_vbi,TV_VBI_CONTROL_STOP,(void*)1);
    priv->priv_vbi=NULL;

    if(priv->vbi_fd){
        close(priv->vbi_fd);
        priv->vbi_fd=0;
    }

    free(priv->vbi_dev);
    priv->vbi_dev = NULL;

    priv->shutdown = 1;

    mp_msg(MSGT_TV, MSGL_V, "Waiting for threads to finish... ");
    if (!priv->tv_param->noaudio) {
        pthread_join(priv->audio_grabber_thread, NULL);
        pthread_mutex_destroy(&priv->audio_starter);
        pthread_mutex_destroy(&priv->skew_mutex);
    }
    pthread_mutex_destroy(&priv->video_buffer_mutex);
    if(priv->video_grabber_thread)
        pthread_join(priv->video_grabber_thread, NULL);

    mp_msg(MSGT_TV, MSGL_V, "done\n");

    if (priv->capability.audios) {
        priv->audio[priv->audio_id].flags |= VIDEO_AUDIO_MUTE;
        ioctl(priv->video_fd, VIDIOCSAUDIO, &priv->audio[priv->audio_id]);
    }

    if ( priv->tv_param->mjpeg )
    {
        num = -1;
        if (ioctl(priv->video_fd, MJPIOC_QBUF_CAPT, &num) < 0)
            mp_msg(MSGT_TV, MSGL_ERR, "\n  MJP: ioctl MJPIOC_QBUF_CAPT failed: %s\n", strerror(errno));
    }
    else
    {
        // We need to munmap as close don't close mem mappings
        if(munmap(priv->mmap,priv->mbuf.size))
            mp_msg(MSGT_TV, MSGL_ERR, "Munmap failed: %s\n",strerror(errno));
    }

    if(close(priv->video_fd))
        mp_msg(MSGT_TV, MSGL_ERR, "Close tv failed: %s\n",strerror(errno));

    audio_in_uninit(&priv->audio_in);

    if (priv->video_ringbuffer) {
        int i;
        for (i = 0; i < priv->video_buffer_size_current; i++) {
            free(priv->video_ringbuffer[i]);
        }
        free(priv->video_ringbuffer);
    }

    free(priv->video_timebuffer);
    free(priv->video_avg_buffer);
    if (!priv->tv_param->noaudio) {
        free(priv->audio_ringbuffer);
        free(priv->audio_skew_buffer);
    }

    return 1;
}

static int get_capture_buffer_size(priv_t *priv)
{
    uint64_t bufsize;
    int cnt;

    if (priv->tv_param->buffer_size >= 0) {
        bufsize = priv->tv_param->buffer_size*1024*1024;
    } else {
#ifdef HAVE_SYS_SYSINFO_H
        struct sysinfo si;

        sysinfo(&si);
        bufsize = (si.freeram/2)*si.mem_unit;
        if (bufsize < 16*1024*1024)
#endif
        bufsize = 16*1024*1024;
    }

    cnt = bufsize/(priv->height*priv->bytesperline);
    if (cnt < 2) cnt = 2;

    return cnt;
}

static int vbi_init(priv_t* priv,char* device)
{
    int vbi_fd=0;
    struct video_capability cap;

    if(!device)
        return TVI_CONTROL_FALSE;

    priv->vbi_dev=strdup(device);

    vbi_fd=open(priv->vbi_dev,O_RDWR);
    if(vbi_fd<0){
        mp_msg(MSGT_TV,MSGL_ERR,"vbi: could not open device %s\n",priv->vbi_dev);
        return  TVI_CONTROL_FALSE;
    }

    if(ioctl(vbi_fd,VIDIOCGCAP,&cap)<0){
        mp_msg(MSGT_TV,MSGL_ERR,"vbi: Query capabilities failed for %s\n",priv->vbi_dev);
        close(vbi_fd);
        return  TVI_CONTROL_FALSE;
    }
    if(!cap.type & VID_TYPE_CAPTURE){
        mp_msg(MSGT_TV,MSGL_ERR,"vbi: %s is not capture device\n",priv->vbi_dev);
        close(vbi_fd);
        return  TVI_CONTROL_FALSE;
    }

    priv->vbi_fd=vbi_fd;
    mp_msg(MSGT_TV,MSGL_DBG3,"vbi: init ok\n");
    return TVI_CONTROL_TRUE;
}

static int vbi_get_props(priv_t* priv,tt_stream_props* ptsp)
{
    struct vbi_format fmt;
    int res;
    if(!priv || !ptsp)
        return TVI_CONTROL_FALSE;

    memset(&fmt,0,sizeof(struct vbi_format));
    if((res=ioctl(priv->vbi_fd,VIDIOCGVBIFMT,&fmt))<0){
        mp_msg(MSGT_TV,MSGL_ERR,"vbi_get_props: Query format failed: %x\n",res);
        return  TVI_CONTROL_FALSE;
    }

    ptsp->interlaced=(fmt.flags& VBI_INTERLACED?1:0);
    if(fmt.start[1]>0 && fmt.count[1]){
        if(fmt.start[1]>=286)
            //625
            ptsp->offset=10.2e-6*fmt.sampling_rate;
        else
            //525
            ptsp->offset=9.2e-6*fmt.sampling_rate;
    }else
        ptsp->offset=9.7e-6*fmt.sampling_rate;

    ptsp->sampling_rate=fmt.sampling_rate;
    ptsp->samples_per_line=fmt.samples_per_line,

    ptsp->count[0]=fmt.count[0];
    ptsp->count[1]=fmt.count[1];
    ptsp->bufsize = ptsp->samples_per_line * (ptsp->count[0] + ptsp->count[1]);

    mp_msg(MSGT_TV,MSGL_V,"vbi_get_props: sampling_rate=%d,offset:%d,samples_per_line: %d\n interlaced:%s, count=[%d,%d]\n",
        ptsp->sampling_rate,
        ptsp->offset,
        ptsp->samples_per_line,
        ptsp->interlaced?"Yes":"No",
        ptsp->count[0],
        ptsp->count[1]);

    return TVI_CONTROL_TRUE;
}

static void *vbi_grabber(void *data)
{
    priv_t *priv = (priv_t *) data;
    int bytes,seq,prev_seq;
    unsigned char* buf;
    tt_stream_props tsp;

    if(!priv->priv_vbi){
        mp_msg(MSGT_TV,MSGL_WARN,"vbi: vbi not initialized. stopping thread.\n");
        return NULL;
    }

    if(vbi_get_props(priv,&tsp)!=TVI_CONTROL_TRUE)
        return NULL;

    buf=malloc(tsp.bufsize);
    seq=0;
    prev_seq=0;
    mp_msg(MSGT_TV,MSGL_V,"vbi: vbi capture thread started.\n");

    while (!priv->vbi_shutdown){
        bytes=read(priv->vbi_fd,buf,tsp.bufsize);
        if(bytes<0 && errno==EINTR)
            continue;
        if (bytes!=tsp.bufsize){
            mp_msg(MSGT_TV,MSGL_WARN,"vbi: expecting bytes: %d, got: %d",tsp.bufsize,bytes);
            break;
        }
        seq=*(int*)(buf+bytes-4);
        if(seq<=1) continue;
        if (prev_seq && seq!=prev_seq+1){
            prev_seq=0;
            seq=0;
        }
        prev_seq=seq;
        teletext_control(priv->priv_vbi,TV_VBI_CONTROL_DECODE_PAGE,&buf);
        mp_msg(MSGT_TV,MSGL_DBG3,"grabber: seq:%d\n",seq);
    }
    free(buf);
    return NULL;
}

static int start(priv_t *priv)
{
    int i;
    int bytes_per_sample;
    struct video_window win;

    if (ioctl(priv->video_fd, VIDIOCGPICT, &priv->picture) == -1)
    {
        mp_msg(MSGT_TV, MSGL_ERR, "ioctl get picture failed: %s\n", strerror(errno));
        return 0;
    }

    priv->picture.palette = format2palette(priv->format);
    priv->picture.depth = palette2depth(priv->picture.palette);

    if (priv->format != IMGFMT_BGR15) {
        priv->bytesperline = priv->width * priv->picture.depth / 8;
    } else {
        priv->bytesperline = priv->width * 2;
    }

    mp_msg(MSGT_TV, MSGL_V, "Picture values:\n");
    mp_msg(MSGT_TV, MSGL_V, " Depth: %d, Palette: %s (Format: %s)\n", priv->picture.depth,
        PALETTE(priv->picture.palette), vo_format_name(priv->format));
    mp_msg(MSGT_TV, MSGL_V, " Brightness: %d, Hue: %d, Colour: %d, Contrast: %d\n",
        priv->picture.brightness, priv->picture.hue,
        priv->picture.colour, priv->picture.contrast);

    if (ioctl(priv->video_fd, VIDIOCSPICT, &priv->picture) == -1)
    {
        mp_msg(MSGT_TV, MSGL_ERR, "ioctl set picture failed: %s\n", strerror(errno));
        mp_msg(MSGT_TV, MSGL_ERR, "The 'outfmt' of '%s' is likely not supported by your card\n",
               vo_format_name(priv->format));
        return 0;
    }

    /* Set capture size */
    win.x = 0;
    win.y = 0;
    win.width = priv->width;
    win.height = priv->height;
    win.chromakey = -1;
    win.flags = 0;
    win.clipcount = 0;
    if (ioctl(priv->video_fd, VIDIOCSWIN, &win) == -1)
        mp_msg(MSGT_TV, MSGL_ERR, "ioctl set window failed: %s\n", strerror(errno));

    if ( !priv->tv_param->mjpeg )
    {
        /* map grab buffer */
        if (ioctl(priv->video_fd, VIDIOCGMBUF, &priv->mbuf) == -1)
        {
            mp_msg(MSGT_TV, MSGL_ERR, "ioctl get mbuf failed: %s\n", strerror(errno));
            return 0;
        }

        mp_msg(MSGT_TV, MSGL_V, "mbuf: size=%d, frames=%d\n",
            priv->mbuf.size, priv->mbuf.frames);
        priv->mmap = mmap(0, priv->mbuf.size, PROT_READ|PROT_WRITE,
                          MAP_SHARED, priv->video_fd, 0);
        if (priv->mmap == (unsigned char *)-1)
        {
            mp_msg(MSGT_TV, MSGL_ERR, "Unable to map memory for buffers: %s\n", strerror(errno));
            return 0;
        }
        mp_msg(MSGT_TV, MSGL_DBG2, "our buffer: %p\n", priv->mmap);

        /* num of buffers */
        priv->nbuf = priv->mbuf.frames;

        /* video buffers */
        priv->buf = calloc(priv->nbuf, sizeof(struct video_mmap));
        if (!priv->buf)
            return 0;
        memset(priv->buf, 0, priv->nbuf * sizeof(struct video_mmap));
    }

    if ( !priv->tv_param->mjpeg )
    {
        priv->nbuf = priv->mbuf.frames;
        for (i=0; i < priv->nbuf; i++)
        {
            priv->buf[i].format = priv->picture.palette;
            priv->buf[i].frame = i;
            priv->buf[i].width = priv->width;
            priv->buf[i].height = priv->height;
            mp_msg(MSGT_TV, MSGL_DBG2, "buffer: %d => %p\n", i, &priv->buf[i]);
        }
    }

#if 0
    {
        struct video_play_mode pmode;

        pmode.mode = VID_PLAY_NORMAL;
        pmode.p1 = 1;
        pmode.p2 = 0;
        if (ioctl(priv->video_fd, VIDIOCSPLAYMODE, &pmode) == -1)
        {
            mp_msg(MSGT_TV, MSGL_ERR, "ioctl set play mode failed: %s\n", strerror(errno));
//          return 0;
        }
    }
#endif

#if 0
    // initialize video capture
    if (ioctl(priv->video_fd, VIDIOCCAPTURE, &one) == -1)
    {
        mp_msg(MSGT_TV, MSGL_ERR, "FATAL: ioctl ccapture failed: %s\n", strerror(errno));
        return 0;
    }
#endif

    /* setup audio parameters */
    if (!priv->tv_param->noaudio) {
        setup_audio_buffer_sizes(priv);
        bytes_per_sample = priv->audio_in.bytes_per_sample;
        priv->audio_skew_buffer = calloc(priv->aud_skew_cnt, sizeof(long long));
        if (!priv->audio_skew_buffer) {
            mp_msg(MSGT_TV, MSGL_ERR, "cannot allocate skew buffer: %s\n", strerror(errno));
            return 0;
        }

        priv->audio_ringbuffer = calloc(priv->audio_in.blocksize, priv->audio_buffer_size);
        if (!priv->audio_ringbuffer) {
            mp_msg(MSGT_TV, MSGL_ERR, "cannot allocate audio buffer: %s\n", strerror(errno));
            return 0;
        }

        priv->audio_secs_per_block = (double)priv->audio_in.blocksize/(priv->audio_in.samplerate
                                                                    *priv->audio_in.channels
                                                                    *bytes_per_sample);
        priv->audio_head = 0;
        priv->audio_tail = 0;
        priv->audio_cnt = 0;
        priv->audio_drop = 0;
        priv->audio_skew = 0;
        priv->audio_skew_total = 0;
        priv->audio_recv_blocks_total = 0;
        priv->audio_sent_blocks_total = 0;
    }

    /* setup video parameters */
    if (priv->immediate_mode) {
        priv->video_buffer_size_max = VID_BUF_SIZE_IMMEDIATE;
    } else {
        priv->video_buffer_size_max = get_capture_buffer_size(priv);
    }
    priv->video_buffer_size_current = 0;

    if (!priv->tv_param->noaudio) {
        if (priv->video_buffer_size_max < 3.0*priv->fps*priv->audio_secs_per_block) {
            mp_msg(MSGT_TV, MSGL_ERR, "Video buffer shorter than 3 times audio frame duration.\n"
                   "You will probably experience heavy framedrops.\n");
        }
    }

    mp_msg(MSGT_TV, MSGL_V, "Using a ring buffer for maximum %d frames, %d MB total size.\n",
           priv->video_buffer_size_max,
           priv->video_buffer_size_max*priv->height*priv->bytesperline/(1024*1024));

    priv->video_ringbuffer = calloc(priv->video_buffer_size_max, sizeof(unsigned char*));
    if (!priv->video_ringbuffer) {
        mp_msg(MSGT_TV, MSGL_ERR, "cannot allocate video buffer: %s\n", strerror(errno));
        return 0;
    }
    for (i = 0; i < priv->video_buffer_size_max; i++)
        priv->video_ringbuffer[i] = NULL;

    priv->video_timebuffer = calloc(priv->video_buffer_size_max, sizeof(long long));
    if (!priv->video_timebuffer) {
        mp_msg(MSGT_TV, MSGL_ERR, "cannot allocate time buffer: %s\n", strerror(errno));
        return 0;
    }
    priv->video_avg_buffer = malloc(sizeof(long long) * VIDEO_AVG_BUFFER_SIZE);
    if (!priv->video_avg_buffer) {
        mp_msg(MSGT_TV, MSGL_ERR, "cannot allocate period buffer: %s\n", strerror(errno));
        return 0;
    }
    priv->video_interval_sum = (1e6/priv->fps)*VIDEO_AVG_BUFFER_SIZE;
    for (i = 0; i < VIDEO_AVG_BUFFER_SIZE; i++) {
        priv->video_avg_buffer[i] = 1e6/priv->fps;
    }

    priv->video_avg_ptr = 0;

    priv->video_head = 0;
    priv->video_tail = 0;
    priv->video_cnt = 0;
    priv->first = 1;

    if (priv->capability.audios) {
        /* enable audio */
        if (priv->tv_param->volume >= 0)
            priv->audio[priv->audio_id].volume = priv->tv_param->volume;
        if (priv->tv_param->bass >= 0)
            priv->audio[priv->audio_id].bass = priv->tv_param->bass;
        if (priv->tv_param->treble >= 0)
            priv->audio[priv->audio_id].treble = priv->tv_param->treble;
        if (priv->tv_param->balance >= 0)
            priv->audio[priv->audio_id].balance = priv->tv_param->balance;
        priv->audio[priv->audio_id].flags &= ~VIDEO_AUDIO_MUTE;
        mp_msg(MSGT_TV, MSGL_V, "Enabling tv audio. Requested setup is:\n");
        mp_msg(MSGT_TV, MSGL_V, "id=%d vol=%d bass=%d treble=%d balance=%d mode=%s",
               priv->audio_id,
               priv->audio[priv->audio_id].volume, priv->audio[priv->audio_id].bass, priv->audio[priv->audio_id].treble,
               priv->audio[priv->audio_id].balance, audio_mode2name(priv->audio[priv->audio_id].mode));
        mp_msg(MSGT_TV, MSGL_V, " chan=%d\n", priv->audio_channels[priv->audio_id]);
        ioctl(priv->video_fd, VIDIOCSAUDIO, &priv->audio[priv->audio_id]);
    }

    /* start vbi thread */
    if(priv->priv_vbi){
        priv->vbi_shutdown = 0;
        pthread_create(&priv->vbi_grabber_thread, NULL, vbi_grabber, priv);
    }

    /* launch capture threads */
    priv->shutdown = 0;
    if (!priv->tv_param->noaudio) {
        pthread_mutex_init(&priv->audio_starter, NULL);
        pthread_mutex_init(&priv->skew_mutex, NULL);
        pthread_mutex_lock(&priv->audio_starter);
        pthread_create(&priv->audio_grabber_thread, NULL, audio_grabber, priv);
    }
    pthread_mutex_init(&priv->video_buffer_mutex, NULL);
    /* we'll launch the video capture later, when a first request for a frame arrives */

    return 1;
}


static int control(priv_t *priv, int cmd, void *arg)
{
    mp_msg(MSGT_TV, MSGL_DBG2, "\ndebug: control(priv=%p, cmd=%d, arg=%p)\n",
        priv, cmd, arg);
    switch(cmd)
    {
        /* ========== GENERIC controls =========== */
        case TVI_CONTROL_IS_VIDEO:
        {
            if (priv->capability.type & VID_TYPE_CAPTURE)
                return TVI_CONTROL_TRUE;
            return TVI_CONTROL_FALSE;
        }
        case TVI_CONTROL_IS_AUDIO:
            if (priv->tv_param->force_audio) return TVI_CONTROL_TRUE;
            if (priv->channels[priv->act_channel].flags & VIDEO_VC_AUDIO)
            {
                return TVI_CONTROL_TRUE;
            }
            return TVI_CONTROL_FALSE;
        case TVI_CONTROL_IS_TUNER:
        {
//          if (priv->capability.type & VID_TYPE_TUNER)
            if (priv->channels[priv->act_channel].flags & VIDEO_VC_TUNER)
                return TVI_CONTROL_TRUE;
            return TVI_CONTROL_FALSE;
        }

        /* ========== VIDEO controls =========== */
        case TVI_CONTROL_VID_GET_FORMAT:
        {
            int output_fmt = -1;

            output_fmt = priv->format;
            if ( priv->tv_param->mjpeg )
            {
                mp_msg(MSGT_TV, MSGL_INFO, "  MJP: setting sh_video->format to mjpg\n");
                output_fmt = 0x47504a4d;
                output_fmt = 0x67706a6d;
                *(int *)arg = output_fmt;
                mp_msg(MSGT_TV, MSGL_V, "Output format: %s\n", "mjpg");
            }
            else
            {
                *(int *)arg = output_fmt;
                mp_msg(MSGT_TV, MSGL_V, "Output format: %s\n", vo_format_name(output_fmt));
            }
            return TVI_CONTROL_TRUE;
        }
        case TVI_CONTROL_VID_SET_FORMAT:
            priv->format = *(int *)arg;
            // !HACK! v4l uses BGR format instead of RGB
            // and we have to correct this. Fortunately,
            // tv.c reads later the format back so we
            // can persuade it to use what we want.
            if (IMGFMT_IS_RGB(priv->format)) {
                priv->format &= ~IMGFMT_RGB_MASK;
                priv->format |= IMGFMT_BGR;
            }
            return TVI_CONTROL_TRUE;
        case TVI_CONTROL_VID_GET_PLANES:
            *(int *)arg = 1; /* FIXME, also not needed at this time */
            return TVI_CONTROL_TRUE;
        case TVI_CONTROL_VID_GET_BITS:
            *(int *)arg = palette2depth(format2palette(priv->format));
            return TVI_CONTROL_TRUE;
        case TVI_CONTROL_VID_GET_WIDTH:
            *(int *)arg = priv->width;
            return TVI_CONTROL_TRUE;
        case TVI_CONTROL_VID_CHK_WIDTH:
        {
            int req_width = *(int *)arg;

            mp_msg(MSGT_TV, MSGL_V, "Requested width: %d\n", req_width);
            if ((req_width >= priv->capability.minwidth) &&
                (req_width <= priv->capability.maxwidth))
                return TVI_CONTROL_TRUE;
            return TVI_CONTROL_FALSE;
        }
        case TVI_CONTROL_VID_SET_WIDTH:
            priv->width = *(int *)arg;
            return TVI_CONTROL_TRUE;
        case TVI_CONTROL_VID_GET_HEIGHT:
            *(int *)arg = priv->height;
            return TVI_CONTROL_TRUE;
        case TVI_CONTROL_VID_CHK_HEIGHT:
        {
            int req_height = *(int *)arg;

            mp_msg(MSGT_TV, MSGL_V, "Requested height: %d\n", req_height);
            if ((req_height >= priv->capability.minheight) &&
                (req_height <= priv->capability.maxheight))
                return TVI_CONTROL_TRUE;
            return TVI_CONTROL_FALSE;
        }
        case TVI_CONTROL_VID_SET_HEIGHT:
            priv->height = *(int *)arg;
            return TVI_CONTROL_TRUE;
        case TVI_CONTROL_VID_GET_PICTURE:
            if (ioctl(priv->video_fd, VIDIOCGPICT, &priv->picture) == -1)
            {
                mp_msg(MSGT_TV, MSGL_ERR, "ioctl get picture failed: %s\n", strerror(errno));
                return TVI_CONTROL_FALSE;
            }
            return TVI_CONTROL_TRUE;
        case TVI_CONTROL_VID_SET_PICTURE:
            if (ioctl(priv->video_fd, VIDIOCSPICT, &priv->picture) == -1)
            {
                mp_msg(MSGT_TV, MSGL_ERR, "ioctl set picture failed: %s\n", strerror(errno));
                return TVI_CONTROL_FALSE;
            }
            return TVI_CONTROL_TRUE;
        case TVI_CONTROL_VID_SET_BRIGHTNESS:
            priv->picture.brightness = (327*(*(int *)arg+100)) + 68;
            return control(priv, TVI_CONTROL_VID_SET_PICTURE, 0);
        case TVI_CONTROL_VID_SET_HUE:
            priv->picture.hue = (327*(*(int *)arg+100)) + 68;
            return control(priv, TVI_CONTROL_VID_SET_PICTURE, 0);
        case TVI_CONTROL_VID_SET_SATURATION:
            priv->picture.colour = (327*(*(int *)arg+100)) + 68;
            return control(priv, TVI_CONTROL_VID_SET_PICTURE, 0);
        case TVI_CONTROL_VID_SET_CONTRAST:
            priv->picture.contrast = (327*(*(int *)arg+100)) + 68;
            return control(priv, TVI_CONTROL_VID_SET_PICTURE, 0);
        case TVI_CONTROL_VID_GET_BRIGHTNESS:
            if(!control(priv, TVI_CONTROL_VID_GET_PICTURE, 0)) return 0;
            *(int*)arg = ((int)priv->picture.brightness-68)/327-100;
            return 1;
        case TVI_CONTROL_VID_GET_HUE:
            if(!control(priv, TVI_CONTROL_VID_GET_PICTURE, 0)) return 0;
            *(int*)arg = ((int)priv->picture.hue-68)/327-100;
            return 1;
        case TVI_CONTROL_VID_GET_SATURATION:
            if(!control(priv, TVI_CONTROL_VID_GET_PICTURE, 0)) return 0;
            *(int*)arg = ((int)priv->picture.colour-68)/327-100;
            return 1;
        case TVI_CONTROL_VID_GET_CONTRAST:
            if(!control(priv, TVI_CONTROL_VID_GET_PICTURE, 0)) return 0;
            *(int*)arg = ((int)priv->picture.contrast-68)/327-100;
            return 1;
        case TVI_CONTROL_VID_GET_FPS:
            *(float *)arg=priv->fps;
            return TVI_CONTROL_TRUE;

        /* ========== TUNER controls =========== */
        case TVI_CONTROL_TUN_GET_FREQ:
        {
            unsigned long freq;

            if (ioctl(priv->video_fd, VIDIOCGFREQ, &freq) == -1)
            {
                mp_msg(MSGT_TV, MSGL_ERR, "ioctl get freq failed: %s\n", strerror(errno));
                return TVI_CONTROL_FALSE;
            }

            /* tuner uses khz not mhz ! */
//          if (priv->tuner.flags & VIDEO_TUNER_LOW)
//              freq /= 1000;
            *(unsigned long *)arg = freq;
            return TVI_CONTROL_TRUE;
        }
        case TVI_CONTROL_TUN_SET_FREQ:
        {
            /* argument is in MHz ! */
            unsigned long freq = *(unsigned long *)arg;

            if (priv->capability.audios) {
                priv->audio[priv->audio_id].flags |= VIDEO_AUDIO_MUTE;
                ioctl(priv->video_fd, VIDIOCSAUDIO, &priv->audio[priv->audio_id]);
            }

            mp_msg(MSGT_TV, MSGL_V, "requested frequency: %.3f\n", (float)freq/16);

            /* tuner uses khz not mhz ! */
//          if (priv->tuner.flags & VIDEO_TUNER_LOW)
//              freq *= 1000;
//          mp_msg(MSGT_TV, MSGL_V, " requesting from driver: freq=%.3f\n", (float)freq/16);
            if (ioctl(priv->video_fd, VIDIOCSFREQ, &freq) == -1)
            {
                mp_msg(MSGT_TV, MSGL_ERR, "ioctl set freq failed: %s\n", strerror(errno));
                return TVI_CONTROL_FALSE;
            }
            usleep(100000); // wait to suppress noise during switching

            if (priv->capability.audios) {
                priv->audio[priv->audio_id].flags &= ~VIDEO_AUDIO_MUTE;
                ioctl(priv->video_fd, VIDIOCSAUDIO, &priv->audio[priv->audio_id]);
            }

            return TVI_CONTROL_TRUE;
        }
        case TVI_CONTROL_TUN_GET_TUNER:
        {
            if (ioctl(priv->video_fd, VIDIOCGTUNER, &priv->tuner) == -1)
            {
                mp_msg(MSGT_TV, MSGL_ERR, "ioctl get tuner failed: %s\n", strerror(errno));
                return TVI_CONTROL_FALSE;
            }

            mp_msg(MSGT_TV, MSGL_V, "Tuner (%s) range: %lu -> %lu\n", priv->tuner.name,
                priv->tuner.rangelow, priv->tuner.rangehigh);
            return TVI_CONTROL_TRUE;
        }
        case TVI_CONTROL_TUN_SET_TUNER:
        {
            if (ioctl(priv->video_fd, VIDIOCSTUNER, &priv->tuner) == -1)
            {
                mp_msg(MSGT_TV, MSGL_ERR, "ioctl set tuner failed: %s\n", strerror(errno));
                return TVI_CONTROL_FALSE;
            }
            return TVI_CONTROL_TRUE;
        }
        case TVI_CONTROL_TUN_SET_NORM:
        {
            int req_mode = *(int *)arg;
            int norm_index;
            for(norm_index=0;supported_norms[norm_index].name; norm_index++)
                if(req_mode==supported_norms[norm_index].normid)
                    break;

            if(!supported_norms[norm_index].name) {
                mp_msg(MSGT_TV, MSGL_ERR, "Unknown norm!\n");
                return TVI_CONTROL_FALSE;
            }

            if (priv->channels[priv->act_channel].flags & VIDEO_VC_TUNER) {
                int prev_mode;

                control(priv, TVI_CONTROL_TUN_GET_TUNER, 0);
                if(!(priv->tuner.flags & supported_norms[norm_index].tuner_flags))
                {
                    mp_msg(MSGT_TV, MSGL_ERR, "Tuner isn't capable to set norm!\n");
                    return TVI_CONTROL_FALSE;
                }

                prev_mode = priv->tuner.mode;

                priv->tuner.mode = supported_norms[norm_index].tuner_mode;

                if (control(priv, TVI_CONTROL_TUN_SET_TUNER, &priv->tuner) != TVI_CONTROL_TRUE) {
                    // norm setting failed, but maybe it's only because it's fixed
                    if (priv->tuner.mode != prev_mode) return TVI_CONTROL_FALSE; // no it really failed
                }

            }

            priv->channels[priv->act_channel].norm = supported_norms[norm_index].input_norm;

            if (ioctl(priv->video_fd, VIDIOCSCHAN, &priv->channels[priv->act_channel]) == -1)
            {
                mp_msg(MSGT_TV, MSGL_ERR, "ioctl set chan failed: %s\n", strerror(errno));
                return TVI_CONTROL_FALSE;
            }

            if (ioctl(priv->video_fd, VIDIOCGCAP, &priv->capability) == -1) {
                mp_msg(MSGT_TV, MSGL_ERR, "ioctl get capabilities failed: %s\n", strerror(errno));
                return TVI_CONTROL_FALSE;
            }

            priv->fps = supported_norms[norm_index].fps;

            if(priv->height > priv->capability.maxheight) {
                priv->height = priv->capability.maxheight;
            }

            if(priv->width > priv->capability.maxwidth) {
                priv->width = priv->capability.maxwidth;
            }

            return TVI_CONTROL_TRUE;
        }
        case TVI_CONTROL_TUN_GET_NORM:
        {
            *(int *)arg = priv->tuner.mode;

            return TVI_CONTROL_TRUE;
        }
        case TVI_CONTROL_TUN_GET_SIGNAL:
        {
            if (ioctl(priv->video_fd, VIDIOCGTUNER, &priv->tuner) == -1)
            {
                mp_msg(MSGT_TV, MSGL_ERR, "ioctl get tuner failed: %s\n", strerror(errno));
                return TVI_CONTROL_FALSE;
            }
            *(int*)arg=100*(priv->tuner.signal>>8)/255;
            return TVI_CONTROL_TRUE;
        }

        /* ========== AUDIO controls =========== */
        case TVI_CONTROL_AUD_GET_FORMAT:
        {
            *(int *)arg = AF_FORMAT_S16_LE;
            return TVI_CONTROL_TRUE;
        }
        case TVI_CONTROL_AUD_GET_CHANNELS:
        {
            *(int *)arg = priv->audio_in.channels;
            return TVI_CONTROL_TRUE;
        }
        case TVI_CONTROL_AUD_GET_SAMPLERATE:
        {
            *(int *)arg = priv->audio_in.samplerate;
            return TVI_CONTROL_TRUE;
        }
        case TVI_CONTROL_AUD_GET_SAMPLESIZE:
        {
            *(int *)arg = priv->audio_in.bytes_per_sample;
            return TVI_CONTROL_TRUE;
        }
        case TVI_CONTROL_AUD_SET_SAMPLERATE:
        {
            if (audio_in_set_samplerate(&priv->audio_in, *(int *)arg) < 0) return TVI_CONTROL_FALSE;
            setup_audio_buffer_sizes(priv);
            return TVI_CONTROL_TRUE;
        }
        /* ========== SPECIFIC controls =========== */
        case TVI_CONTROL_SPC_GET_INPUT:
        {
            int req_chan = *(int *)arg;
            int i;

            for (i = 0; i < priv->capability.channels; i++)
            {
                if (priv->channels[i].channel == req_chan)
                    break;
            }

            priv->act_channel = i;

            if (ioctl(priv->video_fd, VIDIOCGCHAN, &priv->channels[i]) == -1)
            {
                mp_msg(MSGT_TV, MSGL_ERR, "ioctl get channel failed: %s\n", strerror(errno));
                return TVI_CONTROL_FALSE;
            }
            return TVI_CONTROL_TRUE;
        }

        case TVI_CONTROL_SPC_SET_INPUT:
        {
            struct video_channel chan;
            int req_chan = *(int *)arg;
            int i;

            if (req_chan >= priv->capability.channels)
            {
                mp_msg(MSGT_TV, MSGL_ERR, "Invalid input requested: %d, valid: 0-%d\n",
                       req_chan, priv->capability.channels - 1);
                return TVI_CONTROL_FALSE;
            }

            for (i = 0; i < priv->capability.channels; i++)
            {
                if (priv->channels[i].channel == req_chan)
                    chan = priv->channels[i];
            }

            if (ioctl(priv->video_fd, VIDIOCSCHAN, &chan) == -1)
            {
                mp_msg(MSGT_TV, MSGL_ERR, "ioctl set chan failed: %s\n", strerror(errno));
                return TVI_CONTROL_FALSE;
            }
            mp_msg(MSGT_TV, MSGL_INFO, "Using input '%s'\n", chan.name);

            priv->act_channel = i;

            /* update tuner state */
//          if (priv->capability.type & VID_TYPE_TUNER)
            if (priv->channels[priv->act_channel].flags & VIDEO_VC_TUNER)
                control(priv, TVI_CONTROL_TUN_GET_TUNER, 0);

            /* update local channel list */
            control(priv, TVI_CONTROL_SPC_GET_INPUT, &req_chan);
            return TVI_CONTROL_TRUE;
        case TVI_CONTROL_IMMEDIATE:
            priv->immediate_mode = 1;
            return TVI_CONTROL_TRUE;
        }
        case TVI_CONTROL_VBI_INIT:
        {
            void* ptr;
            tt_stream_props tsp;

            if (vbi_init(priv,*(char**)arg)!=TVI_CONTROL_TRUE)
                return TVI_CONTROL_FALSE;
            if(vbi_get_props(priv,&tsp)==TVI_CONTROL_TRUE)
            {
                ptr=&tsp;
                if(teletext_control(NULL,TV_VBI_CONTROL_START,&ptr)==VBI_CONTROL_TRUE)
                    priv->priv_vbi=ptr;
                else
                    priv->priv_vbi=NULL;
            }
            return TVI_CONTROL_TRUE;
        }
        case TVI_CONTROL_GET_VBI_PTR:
            *(void **)arg=priv->priv_vbi;
            return TVI_CONTROL_TRUE;
    }

    return TVI_CONTROL_UNKNOWN;
}

static int set_mute(priv_t* priv,int value)
{
    if (!priv->capability.audios) {
        return 0;

    if(value)
        priv->audio[priv->audio_id].flags |=VIDEO_AUDIO_MUTE;
    else
        priv->audio[priv->audio_id].flags &= ~VIDEO_AUDIO_MUTE;
    }
    if(ioctl(priv->video_fd, VIDIOCSAUDIO, &priv->audio[priv->audio_id])<0)
        return 0;
    return 1;
}

// copies a video frame
// for YV12 swaps the 2nd and 3rd plane
static inline void copy_frame(priv_t *priv, unsigned char *dest, unsigned char *source)
{
    if(priv->tv_param->automute>0){
        if (ioctl(priv->video_fd, VIDIOCGTUNER, &priv->tuner) >= 0) {
            if(priv->tv_param->automute<<8>priv->tuner.signal){
                fill_blank_frame(dest,priv->bytesperline * priv->height,priv->format);
                set_mute(priv,1);
                return;
            }
        }
        set_mute(priv,0);
    }
    // YV12 uses VIDEO_PALETTE_YUV420P, but the planes are swapped
    if (priv->format == IMGFMT_YV12) {
        fast_memcpy(dest, source, priv->width * priv->height);
        fast_memcpy(dest+priv->width * priv->height*5/4, source+priv->width * priv->height, priv->width * priv->height/4);
        fast_memcpy(dest+priv->width * priv->height, source+priv->width * priv->height*5/4, priv->width * priv->height/4);
        return;
    }

    fast_memcpy(dest, source, priv->bytesperline * priv->height);
}

// maximum skew change, in frames
#define MAX_SKEW_DELTA 0.6
static void *video_grabber(void *data)
{
#define MAXTOL (priv->nbuf)
    priv_t *priv = (priv_t*)data;
    struct timeval curtime;
    long long skew, prev_skew, xskew, interval, prev_interval;
    int frame;
    int i;
    int framecount;
    int tolerance;
    unsigned long num;

    /* start the capture process */

    if ( priv->tv_param->mjpeg )
    {
        mp_msg(MSGT_TV, MSGL_INFO, "  MJP: gonna capture ! \n");
        for (i=0; i < priv->nbuf; i++) {
            num = i;
            if (ioctl(priv->video_fd, MJPIOC_QBUF_CAPT, &num) < 0)
                mp_msg(MSGT_TV, MSGL_ERR,
                       "\n  MJP: ioctl MJPIOC_QBUF_CAPT b failed: %s\n", strerror(errno));
        }
    }
    else
    {
        for (i=0; i < priv->nbuf; i++) {
            if (ioctl(priv->video_fd, VIDIOCMCAPTURE, &priv->buf[i]) == -1)
                mp_msg(MSGT_TV, MSGL_ERR, "\nioctl mcapture failed: %s\n", strerror(errno));
        }
    }

    gettimeofday(&curtime, NULL);
    priv->starttime = (long long)1e6*curtime.tv_sec + curtime.tv_usec;
    priv->audio_skew_measure_time = 0;
    pthread_mutex_unlock(&priv->audio_starter);
    xskew = 0;
    skew = 0;
    interval = 0;

    prev_interval = 0;
    prev_skew = 0;

    tolerance = MAXTOL;

    for (framecount = 0; !priv->shutdown;)
    {
        for (i = 0; i < priv->nbuf && !priv->shutdown; i++, framecount++) {

            if (priv->immediate_mode) {
                while (priv->video_cnt == priv->video_buffer_size_max) {
                    usleep(10000);
                    if (priv->shutdown) {
                        return NULL;
                    }
                }
            }

            frame = i;

            if ( priv->tv_param->mjpeg )
            {
                while (ioctl(priv->video_fd, MJPIOC_SYNC, &priv->buf[frame].frame) < 0 &&
                       (errno == EAGAIN || errno == EINTR));
            }
            else
            {
                while (ioctl(priv->video_fd, VIDIOCSYNC, &priv->buf[frame].frame) < 0 &&
                       (errno == EAGAIN || errno == EINTR));
            }
            mp_dbg(MSGT_TV, MSGL_DBG3, "\npicture sync failed\n");

            gettimeofday(&curtime, NULL);
            if (!priv->immediate_mode) {
                interval = (long long)1e6*curtime.tv_sec + curtime.tv_usec - priv->starttime;
            } else {
                interval = (long long)1e6*framecount/priv->fps;
            }

            if (!priv->immediate_mode) {
                long long period, orig_interval;

                if (tolerance == 0) {
                    if (interval - prev_interval == 0) {
                        mp_msg(MSGT_TV, MSGL_V, "\nvideo capture thread: frame delta = 0\n");
                    } else if ((interval - prev_interval < (long long)0.85e6/priv->fps)
                               || (interval - prev_interval > (long long)1.15e6/priv->fps) ) {
                        mp_msg(MSGT_TV, MSGL_V, "\nvideo capture thread: frame delta ~ %.1f fps\n",
                               (double)1e6/(interval - prev_interval));
                    }
                }

                // correct the rate fluctuations on a small scale
                orig_interval = interval;
                period = priv->video_interval_sum/VIDEO_AVG_BUFFER_SIZE;
                if (interval - prev_interval > 105*period/100) {
                    if (tolerance > 0) {
                        mp_msg(MSGT_TV, MSGL_DBG3, "correcting timestamp\n");
                        interval = prev_interval + priv->video_interval_sum/VIDEO_AVG_BUFFER_SIZE;
                        tolerance--;
                    } else {
                        mp_msg(MSGT_TV, MSGL_DBG3, "bad - frames were dropped\n");
                        tolerance = MAXTOL;
                    }
                } else {
                    if (tolerance < MAXTOL) {
                        mp_msg(MSGT_TV, MSGL_DBG3, "fluctuation overcome\n");
                    }
                    tolerance = MAXTOL;
                }

                priv->video_interval_sum -= priv->video_avg_buffer[priv->video_avg_ptr];
                priv->video_avg_buffer[priv->video_avg_ptr++] = orig_interval-prev_interval;
                priv->video_interval_sum += orig_interval-prev_interval;
                if (priv->video_avg_ptr >= VIDEO_AVG_BUFFER_SIZE) priv->video_avg_ptr = 0;

//              fprintf(stderr, "fps: %f\n", (double)1e6*VIDEO_AVG_BUFFER_SIZE/priv->video_interval_sum);

                // interpolate the skew in time
                pthread_mutex_lock(&priv->skew_mutex);
                xskew = priv->audio_skew + (interval - priv->audio_skew_measure_time)*priv->audio_skew_factor;
                pthread_mutex_unlock(&priv->skew_mutex);
                // correct extreme skew changes to avoid (especially) moving backwards in time
                if (xskew - prev_skew > (interval - prev_interval)*MAX_SKEW_DELTA) {
                    skew = prev_skew + (interval - prev_interval)*MAX_SKEW_DELTA;
                } else if (xskew - prev_skew < -(interval - prev_interval)*MAX_SKEW_DELTA) {
                    skew = prev_skew - (interval - prev_interval)*MAX_SKEW_DELTA;
                } else {
                    skew = xskew;
                }
            }

            mp_msg(MSGT_TV, MSGL_DBG3, "\nfps = %f, interval = %f, a_skew = %f, corr_skew = %f\n",
                   (interval != prev_interval) ? (double)1e6/(interval - prev_interval) : -1,
                   (double)1e-6*interval, (double)1e-6*xskew, (double)1e-6*skew);
            mp_msg(MSGT_TV, MSGL_DBG3, "vcnt = %d, acnt = %d\n", priv->video_cnt, priv->audio_cnt);

            prev_skew = skew;
            prev_interval = interval;

            /* allocate a new buffer, if needed */
            pthread_mutex_lock(&priv->video_buffer_mutex);
            if (priv->video_buffer_size_current < priv->video_buffer_size_max) {
                if (priv->video_cnt == priv->video_buffer_size_current) {
                    unsigned char *newbuf = calloc(priv->bytesperline, priv->height);
                    if (newbuf) {
                        memmove(priv->video_ringbuffer+priv->video_tail+1, priv->video_ringbuffer+priv->video_tail,
                               (priv->video_buffer_size_current-priv->video_tail)*sizeof(unsigned char *));
                        memmove(priv->video_timebuffer+priv->video_tail+1, priv->video_timebuffer+priv->video_tail,
                               (priv->video_buffer_size_current-priv->video_tail)*sizeof(long long));
                        priv->video_ringbuffer[priv->video_tail] = newbuf;
                        if ((priv->video_head >= priv->video_tail) && (priv->video_cnt > 0)) priv->video_head++;
                        priv->video_buffer_size_current++;
                    }
                }
            }
            pthread_mutex_unlock(&priv->video_buffer_mutex);

            if (priv->video_cnt == priv->video_buffer_size_current) {
                if (!priv->immediate_mode) {
                    mp_msg(MSGT_TV, MSGL_ERR, "\nvideo buffer full - dropping frame\n");
                }
            } else {
                if (priv->immediate_mode) {
                    priv->video_timebuffer[priv->video_tail] = interval;
                } else {
                    // compensate for audio skew
                    // negative skew => there are more audio samples, increase interval
                    // positive skew => less samples, shorten the interval
                    priv->video_timebuffer[priv->video_tail] = interval - skew;
                }

                if ( priv->tv_param->mjpeg )
                    copy_frame(priv, priv->video_ringbuffer[priv->video_tail],
                               priv->mmap+(priv->mjpeg_bufsize)*i);
                else
                    copy_frame(priv, priv->video_ringbuffer[priv->video_tail],
                               priv->mmap+priv->mbuf.offsets[frame]);
                priv->video_tail = (priv->video_tail+1)%priv->video_buffer_size_current;
                priv->video_cnt++;
            }

            if ( priv->tv_param->mjpeg )
            {
                num = frame;
                if (ioctl(priv->video_fd, MJPIOC_QBUF_CAPT, &num) < 0)
                {
                    mp_msg(MSGT_TV, MSGL_ERR, "\n  MJP: ioctl MJPIOC_QBUF_CAPT end failed: %s\n",
                           strerror(errno));
                    continue;
                }
            }
            else
            {
                if (ioctl(priv->video_fd, VIDIOCMCAPTURE, &priv->buf[frame]) == -1)
                {
                    mp_msg(MSGT_TV, MSGL_ERR, "\nioctl mcapture failed: %s\n", strerror(errno));
                    continue;
                }
            }
        }
    }
    mp_msg(MSGT_TV, MSGL_INFO, "  MJP: returning! \n");
    return NULL;
}

static double grab_video_frame(priv_t *priv, char *buffer, int len)
{
    double interval;

    if (priv->first) {
        pthread_create(&priv->video_grabber_thread, NULL, video_grabber, priv);
        priv->first = 0;
    }

    while (priv->video_cnt == 0) {
        usleep(10000);
    }

    pthread_mutex_lock(&priv->video_buffer_mutex);
    interval = (double)priv->video_timebuffer[priv->video_head]*1e-6;
    fast_memcpy(buffer, priv->video_ringbuffer[priv->video_head], len);
    priv->video_cnt--;
    priv->video_head = (priv->video_head+1)%priv->video_buffer_size_current;
    pthread_mutex_unlock(&priv->video_buffer_mutex);

    return interval;
}

static int get_video_framesize(priv_t *priv)
{
    return priv->bytesperline * priv->height;
}

static void *audio_grabber(void *data)
{
    priv_t *priv = (priv_t*)data;
    struct timeval tv;
    int i, audio_skew_ptr = 0;
    long long current_time, prev_skew = 0;

    pthread_mutex_lock(&priv->audio_starter);

    audio_in_start_capture(&priv->audio_in);
    for (i = 0; i < priv->aud_skew_cnt; i++)
        priv->audio_skew_buffer[i] = 0;

    for (; !priv->shutdown;)
    {
        if (audio_in_read_chunk(&priv->audio_in, priv->audio_ringbuffer+priv->audio_tail*priv->audio_in.blocksize) < 0)
            continue;

        gettimeofday(&tv, NULL);

        priv->audio_recv_blocks_total++;
        current_time = (long long)1e6*tv.tv_sec + tv.tv_usec - priv->starttime;

        priv->audio_skew_total -= priv->audio_skew_buffer[audio_skew_ptr];
        priv->audio_skew_buffer[audio_skew_ptr] = current_time
            - 1e6*priv->audio_secs_per_block*priv->audio_recv_blocks_total;
        priv->audio_skew_total += priv->audio_skew_buffer[audio_skew_ptr];
        audio_skew_ptr = (audio_skew_ptr+1) % priv->aud_skew_cnt;

        pthread_mutex_lock(&priv->skew_mutex);
        // linear interpolation - here we interpolate current skew value
        // from the moving average, which we expect to be in the middle
        // of the interval
        if (priv->audio_recv_blocks_total > priv->aud_skew_cnt) {
            priv->audio_skew = priv->audio_skew_total/priv->aud_skew_cnt;
            priv->audio_skew += (priv->audio_skew*priv->aud_skew_cnt)/(2*priv->audio_recv_blocks_total-priv->aud_skew_cnt);
        } else {
            // this smoothes the evolution of audio_skew at startup a bit
            priv->audio_skew = ((priv->aud_skew_cnt+priv->audio_recv_blocks_total)*priv->audio_skew_total)/(priv->aud_skew_cnt*priv->audio_recv_blocks_total);
        }
        // current skew factor (assuming linearity)
        // used for further interpolation in video_grabber
        // probably overkill but seems to be necessary for
        // stress testing by dropping half of the audio frames ;)
        // especially when using ALSA with large block sizes
        // where audio_skew remains a long while behind
        if ((priv->audio_skew_measure_time != 0) && (current_time - priv->audio_skew_measure_time != 0)) {
            priv->audio_skew_factor = (double)(priv->audio_skew-prev_skew)/(current_time - priv->audio_skew_measure_time);
        } else {
            priv->audio_skew_factor = 0.0;
        }

        priv->audio_skew_measure_time = current_time;
        prev_skew = priv->audio_skew;
        pthread_mutex_unlock(&priv->skew_mutex);

        if ((priv->audio_tail+1) % priv->audio_buffer_size == priv->audio_head) {
            mp_msg(MSGT_TV, MSGL_ERR, "\ntoo bad - dropping audio frame !\n");
            priv->audio_drop++;
        } else {
            priv->audio_tail = (priv->audio_tail+1) % priv->audio_buffer_size;
            priv->audio_cnt++;
        }
    }
    return NULL;
}

static double grab_audio_frame(priv_t *priv, char *buffer, int len)
{
    mp_dbg(MSGT_TV, MSGL_DBG2, "grab_audio_frame(priv=%p, buffer=%p, len=%d)\n",
        priv, buffer, len);

    if (priv->first) {
        pthread_create(&priv->video_grabber_thread, NULL, video_grabber, priv);
        priv->first = 0;
    }

    // compensate for dropped audio frames
    if (priv->audio_drop && (priv->audio_head == priv->audio_tail)) {
        priv->audio_drop--;
        priv->audio_sent_blocks_total++;
        memset(buffer, 0, len);
        return (double)priv->audio_sent_blocks_total*priv->audio_secs_per_block;
    }

    while (priv->audio_head == priv->audio_tail) {
        usleep(10000);
    }
    fast_memcpy(buffer, priv->audio_ringbuffer+priv->audio_head*priv->audio_in.blocksize, len);
    priv->audio_head = (priv->audio_head+1) % priv->audio_buffer_size;
    priv->audio_cnt--;
    priv->audio_sent_blocks_total++;
    return (double)priv->audio_sent_blocks_total*priv->audio_secs_per_block;
}

static int get_audio_framesize(priv_t *priv)
{
    return priv->audio_in.blocksize;
}
