/*
 * MPEG-PES audio output driver
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <errno.h>

#include "config.h"

#include "audio_out.h"
#include "audio_out_internal.h"

#include "libaf/af_format.h"
#include "libmpdemux/mpeg_packetizer.h"
#include "libvo/video_out.h" /* only for vo_pts */
#include "subopt-helper.h"

#include "mp_msg.h"
#include "help_mp.h"

#ifdef CONFIG_DVB
#include <poll.h>
#include <sys/ioctl.h>
#include <linux/dvb/audio.h>
audio_mixer_t dvb_mixer={255,255};
#endif

#define true 1
#define false 0

extern int vo_mpegpes_fd;
int ao_mpegpes_fd = -1;

#include <errno.h>

static const ao_info_t info =
{
#ifdef CONFIG_DVB
	"DVB audio output",
#else
	"MPEG-PES audio output",
#endif
	"mpegpes",
	"A'rpi",
	""
};

LIBAO_EXTERN(mpegpes)


// to set/get/query special features/parameters
static int control(int cmd,void *arg){
#ifdef CONFIG_DVB
    switch(cmd){
	case AOCONTROL_GET_VOLUME:
	  if(ao_mpegpes_fd >= 0){
	    ((ao_control_vol_t*)(arg))->left=dvb_mixer.volume_left/2.56;
	    ((ao_control_vol_t*)(arg))->right=dvb_mixer.volume_right/2.56;
	    return CONTROL_OK;
	  }
	  return CONTROL_ERROR;
	case AOCONTROL_SET_VOLUME:
	  if(ao_mpegpes_fd >= 0){
	    dvb_mixer.volume_left=((ao_control_vol_t*)(arg))->left*2.56;
	    dvb_mixer.volume_right=((ao_control_vol_t*)(arg))->right*2.56;
	    if(dvb_mixer.volume_left>255) dvb_mixer.volume_left=255;
	    if(dvb_mixer.volume_right>255) dvb_mixer.volume_right=255;
	    //	 printf("Setting DVB volume: %d ; %d  \n",dvb_mixer.volume_left,dvb_mixer.volume_right);
	    if ( (ioctl(ao_mpegpes_fd, AUDIO_SET_MIXER, &dvb_mixer) < 0)){
		mp_msg(MSGT_AO, MSGL_ERR, MSGTR_AO_MPEGPES_CantSetMixer,
		    strerror(errno));
	      return CONTROL_ERROR;
	    }
	    return CONTROL_OK;
	  }
	  return CONTROL_ERROR;
    }
#endif
    return CONTROL_UNKNOWN;
}

static int freq=0;
static int freq_id=0;

#ifdef CONFIG_DVB
static int init_device(int card)
{
	char ao_file[30];
	sprintf(ao_file, "/dev/dvb/adapter%d/audio0", card);
	mp_msg(MSGT_VO,MSGL_INFO, "Opening %s\n", ao_file);
	if((ao_mpegpes_fd = open(ao_file,O_RDWR|O_NONBLOCK)) < 0)
	{
        	mp_msg(MSGT_VO, MSGL_ERR, "DVB AUDIO DEVICE: %s\n", strerror(errno));
        	return -1;
	}
	if( (ioctl(ao_mpegpes_fd, AUDIO_SELECT_SOURCE, AUDIO_SOURCE_MEMORY) < 0))
	{
		mp_msg(MSGT_VO, MSGL_ERR, "DVB AUDIO SELECT SOURCE: %s\n", strerror(errno));
		goto fail;
	}
	if((ioctl(ao_mpegpes_fd, AUDIO_PLAY) < 0))
	{
		mp_msg(MSGT_VO, MSGL_ERR, "DVB AUDIO PLAY: %s\n", strerror(errno));
		goto fail;
	}
	if((ioctl(ao_mpegpes_fd, AUDIO_SET_AV_SYNC, true) < 0))
	{
		mp_msg(MSGT_VO, MSGL_ERR, "DVB AUDIO SET AV SYNC: %s\n", strerror(errno));
		goto fail;
	}
	//FIXME: in vo_mpegpes audio was initialized as MUTEd
	if((ioctl(ao_mpegpes_fd, AUDIO_SET_MUTE, false) < 0))
	{
		mp_msg(MSGT_VO, MSGL_ERR, "DVB AUDIO SET MUTE: %s\n", strerror(errno));
		goto fail;
	}
	return ao_mpegpes_fd;
fail:
	close(ao_mpegpes_fd);
	ao_mpegpes_fd = -1;
	return -1;
}
#endif

static int preinit(const char *arg)
{
	int card = -1;
	char *ao_file = NULL;

	const opt_t subopts[] = {
		{"card", OPT_ARG_INT, &card, NULL},
		{"file", OPT_ARG_MSTRZ, &ao_file, NULL},
		{NULL}
	};

	if(subopt_parse(ao_subdevice, subopts) != 0)
	{
		mp_msg(MSGT_VO, MSGL_ERR, "AO_MPEGPES, Unrecognized options\n");
		return -1;
	}
#ifdef CONFIG_DVB
	if(card==-1)
	{
		//search the first usable card
		int n;
		char file[30];
		for(n=0; n<4; n++)
		{
			sprintf(file, "/dev/dvb/adapter%d/audio0", n);
			if(access(file, F_OK | W_OK)==0)
			{
				card = n+1;
				break;
			}
        	}
	}
	if((card < 1) || (card > 4))
	{
		mp_msg(MSGT_VO, MSGL_ERR, "DVB card number must be between 1 and 4\n");
		return -1;
	}
	card--;

	if(!ao_file)
		return init_device(card);
#else
	if(!ao_file)
		return vo_mpegpes_fd;	//video fd
#endif

	ao_mpegpes_fd = open(ao_file, O_WRONLY | O_CREAT, 0666);
	if(ao_mpegpes_fd < 0)
	{
		mp_msg(MSGT_VO, MSGL_ERR, "ao_mpegpes: %s\n", strerror(errno));
		return -1;
	}
	return ao_mpegpes_fd;
}

static int my_ao_write(const unsigned char* data,int len){
    int orig_len = len;
#ifdef CONFIG_DVB
#define NFD   1
    struct pollfd pfd[NFD];

    pfd[0].fd     = ao_mpegpes_fd;
    pfd[0].events = POLLOUT;

    while(len>0){
        if(poll(pfd,NFD,1)){
            if(pfd[0].revents & POLLOUT){
                int ret = write(ao_mpegpes_fd, data, len);
                if(ret<=0){
                    mp_msg(MSGT_VO, MSGL_ERR, "ao_mpegpes write: %s\n", strerror(errno));
                    usleep(0);
                } else {
                    len-=ret;
                    data+=ret;
                }
            } else usleep(1000);
        }
    }

#else
    if(ao_mpegpes_fd < 0) return 0; // no file
    write(ao_mpegpes_fd, data, len); // write to file
#endif
    return orig_len;
}


// open & setup audio device
// return: 1=success 0=fail
static int init(int rate,int channels,int format,int flags){
    if(preinit(NULL)<0) return 0;

    ao_data.channels=2;
    ao_data.outburst=2000;
    switch(format){
	case AF_FORMAT_S16_BE:
	case AF_FORMAT_MPEG2:
	case AF_FORMAT_AC3_BE:
	    ao_data.format=format;
	    break;
	case AF_FORMAT_AC3_LE:
	    ao_data.format=AF_FORMAT_AC3_BE;
	    break;
	default:
	    ao_data.format=AF_FORMAT_S16_BE;
    }

    switch(rate){
	case 48000:	freq_id=0;break;
	case 96000:	freq_id=1;break;
	case 44100:	freq_id=2;break;
	case 32000:	freq_id=3;break;
	default:
	    mp_msg(MSGT_AO, MSGL_ERR, MSGTR_AO_MPEGPES_UnsupSamplerate, rate);
#if 0
	    if(rate>48000) rate=96000; else
	    if(rate>44100) rate=48000; else
	    if(rate>32000) rate=44100; else
	    rate=32000;
	    goto retry;
#else
	    rate=48000; freq_id=0;
#endif
    }

    ao_data.bps=rate*2*2;
    freq=ao_data.samplerate=rate;

    return 1;
}

// close audio device
static void uninit(int immed){
	if (ao_mpegpes_fd >= 0)
		close(ao_mpegpes_fd);
	ao_mpegpes_fd = -1;
}

// stop playing and empty buffers (for seeking/pause)
static void reset(void){

}

// stop playing, keep buffers (for pause)
static void audio_pause(void)
{
    // for now, just call reset();
    reset();
}

// resume playing, after audio_pause()
static void audio_resume(void)
{
}

// return: how many bytes can be played without blocking
static int get_space(void){
    float x=(float)(vo_pts-ao_data.pts)/90000.0;
    int y;
    //FIXME: is it correct?
    if(vo_mpegpes_fd < 0) return 32000;	//not using -vo mpegpes
//    printf("vo_pts: %5.3f  ao_pts: %5.3f\n",vo_pts/90000.0,ao_data.pts/90000.0);
    if(x<=0) return 0;
    y=freq*4*x;y/=ao_data.outburst;y*=ao_data.outburst;
    if(y>32000) y=32000;
//    printf("diff: %5.3f -> %d  \n",x,y);
    return y;
}

// plays 'len' bytes of 'data'
// it should round it down to outburst*n
// return: number of bytes played
static int play(void* data,int len,int flags){
//    printf("\nao_mpegpes: play(%d) freq=%d\n",len,freq_id);
    if(ao_data.format==AF_FORMAT_MPEG2)
	send_mpeg_pes_packet (data, len, 0x1C0, ao_data.pts, 1, my_ao_write);
    else {
//	if(len>2000) len=2000;
//	printf("ao_mpegpes: len=%d  \n",len);
	send_mpeg_lpcm_packet(data, len, 0xA0, ao_data.pts, freq_id, my_ao_write);
    }
    return len;
}

// return: delay in seconds between first and last sample in buffer
static float get_delay(void){

    return 0.0;
}
