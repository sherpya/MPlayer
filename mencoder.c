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

#define VCODEC_COPY 0
#define VCODEC_FRAMENO 1
// real codecs:
#define VCODEC_LIBAVCODEC 4
#define VCODEC_VFW 7
#define VCODEC_LIBDV 8
#define VCODEC_XVID 9
#define VCODEC_QTVIDEO 10
#define VCODEC_NUV 11
#define VCODEC_RAW 12
#define VCODEC_X264 13

#define ACODEC_COPY 0
#define ACODEC_PCM 1
#define ACODEC_VBRMP3 2
#define ACODEC_NULL 3
#define ACODEC_LAVC 4
#define ACODEC_TOOLAME 5
#define ACODEC_FAAC 6
#define ACODEC_TWOLAME 7

#ifdef __MINGW32__
#define SIGHUP   1
#define SIGQUIT  3
#define SIGPIPE 13
#endif

#include "config.h"

#include <inttypes.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(__MINGW32__) || defined(__CYGWIN__)
#include <windows.h>
#endif

#include "libavutil/avstring.h"
#include "input/input.h"
#include "libaf/af_format.h"
#include "libao2/audio_out.h"
#include "libavcodec/avcodec.h"
#include "libmpcodecs/ae.h"
#include "libmpcodecs/dec_audio.h"
#include "libmpcodecs/dec_video.h"
#include "libmpcodecs/mp_image.h"
#include "libmpcodecs/vd.h"
#include "libmpcodecs/vf.h"
#include "libmpdemux/aviprint.h"
#include "libmpdemux/demuxer.h"
#include "libmpdemux/mp3_hdr.h"
#include "libmpdemux/ms_hdr.h"
#include "libmpdemux/muxer.h"
#include "libmpdemux/stheader.h"
#include "libvo/fastmemcpy.h"
#include "sub/font_load.h"
#include "sub/sub.h"
#include "libvo/video_out.h"
#include "osdep/timer.h"
#include "stream/stream.h"
#include "stream/stream_bd.h"
#ifdef CONFIG_DVDREAD
#include "stream/stream_dvd.h"
#endif
#include "stream/stream_dvdnav.h"
#include "sub/ass_mp.h"
#include "codec-cfg.h"
#include "edl.h"
#include "help_mp.h"
#include "m_config.h"
#include "m_option.h"
#include "mp_fifo.h"
#include "mp_msg.h"
#include "mpcommon.h"
#include "parser-cfg.h"
#include "parser-mecmd.h"
#include "path.h"
#include "sub/spudec.h"
#include "sub/vobsub.h"
#include "sub/eosd.h"
#include "mencoder.h"


int vo_doublebuffering=0;
int vo_directrendering=0;
int vo_config_count=1;

//--------------------------

// cache2:
int stream_cache_size=-1;
#ifdef CONFIG_STREAM_CACHE
float stream_cache_min_percent=20.0;
float stream_cache_seek_min_percent=50.0;
#endif

int audio_id=-1;
int video_id=-1;
int dvdsub_id=-1;
int vobsub_id=-1;
char* audio_lang=NULL;
char* dvdsub_lang=NULL;

static char** audio_codec_list=NULL;  // override audio codec
static char** video_codec_list=NULL;  // override video codec
static char** audio_fm_list=NULL;     // override audio codec family
static char** video_fm_list=NULL;     // override video codec family

static int out_audio_codec=-1;
static int out_video_codec=-1;

int out_file_format=MUXER_TYPE_AVI;	// default to AVI
int quiet=0;
double video_time_usage=0;
double vout_time_usage=0;
double max_video_time_usage=0;
double max_vout_time_usage=0;
double cur_video_time_usage=0;
double cur_vout_time_usage=0;
int benchmark=0;

// A-V sync:
static float default_max_pts_correction=-1;//0.01f;
static float max_pts_correction=0;//default_max_pts_correction;
static float c_total=0;

static float audio_preload=0.5;
static float audio_delay_fix=0.0;
static float audio_delay=0.0;
static int ignore_start=0;
static int audio_density=2;

double force_fps=0;
static double force_ofps=0; // set to 24 for inverse telecine
static int skip_limit=-1;
float playback_speed=1.0;

static int force_srate=0;
static int audio_output_format=AF_FORMAT_UNKNOWN;

char *vobsub_out=NULL;
unsigned int vobsub_out_index=0;
char *vobsub_out_id=NULL;

char* out_filename=NULL;

char *force_fourcc=NULL;
int force_audiofmttag=-1;

char* passtmpfile="divx2pass.log";

static int play_n_frames=-1;
static int play_n_frames_mf=-1;

// sub:
char *font_name=NULL;
char *sub_font_name=NULL;
float font_factor=0.75;
char **sub_name=NULL;
char **sub_paths = NULL;
float sub_delay=0;
float sub_fps=0;
int   sub_auto = 0;
char *vobsub_name = NULL;
int   subcc_enabled=0;
int   suboverlap_enabled = 1;

int auto_expand=1;
int encode_duplicates=1;

// infos are empty by default
char *info_name=NULL;
char *info_artist=NULL;
char *info_genre=NULL;
char *info_subject=NULL;
char *info_copyright=NULL;
char *info_sourceform=NULL;
char *info_comment=NULL;

// Needed by libmpcodecs vf_vo.c
int config_video_out(const vo_functions_t *vo, uint32_t width, uint32_t height,
                     uint32_t d_width, uint32_t d_height, uint32_t flags,
                     char *title, uint32_t format) {
  return 1;
}
// Needed by libmpdemux.
int mp_input_check_interrupt(int time) {
  usec_sleep(time);
  return 0;
}
// Needed by getch2
void mplayer_put_key(int code)
{
}

char *current_module;

// Needed by mpcommon.c
void set_osd_subtitle(subtitle *subs) {
    vo_sub = subs;
    vo_osd_changed(OSDTYPE_SUBTITLE);
}

static double seek_to_sec;
static off_t seek_to_byte=0;

static m_time_size_t end_at = { .type = END_AT_NONE, .pos = 0 };

static char * frameno_filename=NULL;

typedef struct {
    unsigned char* start;
    int in_size;
    float frame_time;
    int already_read;
    int flush;
} s_frame_data;

static edl_record_ptr edl_records = NULL; ///< EDL entries memory area
static edl_record_ptr next_edl_record = NULL; ///< only for traversing edl_records
static short edl_muted; ///< Stores whether EDL is currently in muted mode.
static short edl_seeking; ///< When non-zero, stream is seekable.
static int edl_seek_type; ///< When non-zero, frames are discarded instead of seeking.

/* This header requires all the global variable declarations. */
#include "cfg-mencoder.h"


/* FIXME */
static void mencoder_exit(int level, const char *how)
{
    if (how)
	mp_msg(MSGT_MENCODER, MSGL_INFO, MSGTR_ExitingHow, how);
    else
	mp_msg(MSGT_MENCODER, MSGL_INFO, MSGTR_Exiting);

    exit(level);
}

static void parse_cfgfiles( m_config_t* conf )
{
  char *conffile;
  if (!disable_system_conf &&
      m_config_parse_config_file(conf, MPLAYER_CONFDIR "/mencoder.conf", 1) < 0)
    mencoder_exit(1,MSGTR_ConfigFileError);

  if (!disable_user_conf) {
    if ((conffile = get_path("mencoder.conf")) == NULL) {
      mp_msg(MSGT_CPLAYER,MSGL_ERR,MSGTR_GetpathProblem);
    } else {
      if (m_config_parse_config_file(conf, conffile, 1) < 0)
        mencoder_exit(1,MSGTR_ConfigFileError);
      free(conffile);
    }
  }
}


//---------------------------------------------------------------------------

static int dec_audio(sh_audio_t *sh_audio,unsigned char* buffer,int total){
    int size=0;
    int at_eof=0;
    while(size<total && !at_eof){
	int res;
	int len=total-size;
		if(len>MAX_OUTBURST) len=MAX_OUTBURST;
		res = mp_decode_audio(sh_audio, len);
		if (res == -2)
		    mp_msg(MSGT_MENCODER, MSGL_FATAL, "Audio format change detected!\n"
		           "MEncoder currently cannot handle format changes - patches welcome!\n");
		if (res < 0)
                    at_eof = 1;
		if(len>sh_audio->a_out_buffer_len) len=sh_audio->a_out_buffer_len;
		fast_memcpy(buffer+size,sh_audio->a_out_buffer,len);
		sh_audio->a_out_buffer_len-=len; size+=len;
		if(sh_audio->a_out_buffer_len>0)
		    memmove(sh_audio->a_out_buffer,&sh_audio->a_out_buffer[len],sh_audio->a_out_buffer_len);
    }
    return size;
}

//---------------------------------------------------------------------------

static volatile int at_eof=0;
static volatile int interrupted=0;

static void exit_sighandler(int x){
    at_eof=3;
    interrupted=2; /* 1 means error */
}

static muxer_t* muxer=NULL;

static void add_subtitles(char *filename, float fps, int silent)
{
    sub_data *subd;
#ifdef CONFIG_ASS
    ASS_Track *asst = 0;
#endif

    if (!filename) return;

    subd = sub_read_file(filename, fps);
#ifdef CONFIG_ASS
    if (ass_enabled)
#ifdef CONFIG_ICONV
        asst = ass_read_file(ass_library, filename, sub_cp);
#else
        asst = ass_read_file(ass_library, filename, 0);
#endif
    if (ass_enabled && subd && !asst)
        asst = ass_read_subdata(ass_library, subd, fps);

    if (!asst && !subd && !silent)
#else
    if (!subd && !silent)
#endif
        mp_msg(MSGT_CPLAYER, MSGL_ERR, MSGTR_CantLoadSub,
		    filename_recode(filename));

#ifdef CONFIG_ASS
    if (!asst && !subd) return;
    ass_track = asst;
#else
    if (!subd) return;
#endif
    mp_msg(MSGT_IDENTIFY, MSGL_INFO, "ID_FILE_SUB_FILENAME=%s\n",
	   filename_recode(filename));
    subdata = subd;
}

/* Provide a timer value suitable for use in A/V sync calculations.
 * mux->timer records the current position of the muxer stream.
 * mux->encoder_delay is how many frames are currently buffered by the
 * encoder. To mencoder core, encoder-buffered frames have been "dealt with"--
 * they'll get to the muxer eventually. So, for the purposes of A/V sync,
 * we need to add the total time length of buffered frames to the muxer stream
 * position. */
static double adjusted_muxer_time(muxer_stream_t *mux)
{
    if (! mux) return MP_NOPTS_VALUE;
    return mux->timer + (double) mux->encoder_delay * mux->h.dwScale/mux->h.dwRate;
}

/* This function returns the absolute time for which MEncoder will switch files
 * or move in the file so audio can be cut correctly. -1 if there is no limit. */
static float stop_time(demuxer_t* demuxer, muxer_stream_t* mux_v)
{
	float timeleft = -1;
	if (play_n_frames >= 0)
		timeleft = adjusted_muxer_time(mux_v) + play_n_frames * (double)(mux_v->h.dwScale) / mux_v->h.dwRate;
	if (end_at.type == END_AT_TIME && (timeleft > end_at.pos || timeleft == -1))
		timeleft = end_at.pos;
	if (next_edl_record && demuxer && demuxer->video) { // everything is OK to be checked
		float tmp = adjusted_muxer_time(mux_v) + next_edl_record->start_sec - demuxer->video->pts;
		if (timeleft == -1 || timeleft > tmp) {
			// There's less time in EDL than what we already know
			if (next_edl_record->action == EDL_SKIP && edl_seeking) {
				timeleft = tmp;
			} else if (next_edl_record->action == EDL_MUTE) {
				//timeleft = next_edl_record->start_sec - demuxer->video->pts;
				// For the moment (and probably forever) EDL mute doesn't work in MEncoder
			}
		}
	}
	return timeleft;
}

/** \brief Seeks audio forward to pts by dumping audio packets
 *  \return The current audio pts. */
static float forward_audio(float pts, demux_stream_t *d_audio, muxer_stream_t* mux_a)
{
    sh_audio_t * sh_audio = d_audio ? d_audio->sh : NULL;
    int samplesize, avg;
    float a_pts = calc_a_pts(sh_audio, d_audio);

    if (!sh_audio) return a_pts;

    if (sh_audio->audio.dwScale) samplesize = sh_audio->audio.dwSampleSize;
    else samplesize = (sh_audio->wf ? sh_audio->wf->nBlockAlign : 1);
    avg = (sh_audio->wf ? sh_audio->wf->nAvgBytesPerSec : sh_audio->i_bps);

    // after a demux_seek, a_pts will be zero until you read some audio.
    // carefully checking if a_pts is truely correct by reading tiniest amount of data possible.
    if (pts > a_pts && a_pts == 0.0 && samplesize) {
        if (demux_read_data(sh_audio->ds,mux_a->buffer,samplesize) <= 0) return a_pts; // EOF
        a_pts = calc_a_pts(sh_audio, d_audio);
    }

    while (pts > a_pts) {
        int len;
        if (samplesize) {
            len = avg * (pts - a_pts > 0.5 ? 0.5 : pts - a_pts);
            len/= samplesize; if(len<1) len=1;
            len*= samplesize;
            len = demux_read_data(sh_audio->ds,mux_a->buffer,len);
        } else {
            unsigned char * crap;
            len = ds_get_packet(sh_audio->ds, &crap);
        }
        if (len <= 0) break; // EOF of audio.
        a_pts = calc_a_pts(sh_audio, d_audio);
    }
    return a_pts;
}

/** \brief Seeks slowly by dumping frames.
 *  \return 1 for success, 2 for EOF. */
static int slowseek(float end_pts, demux_stream_t *d_video,
                    demux_stream_t *d_audio, muxer_stream_t *mux_a,
                    s_frame_data *frame_data, int framecopy, int print_info)
{
    sh_video_t * sh_video = d_video->sh;
    vf_instance_t * vfilter = sh_video ? sh_video->vfilter : NULL;
    int done = 0;

    while (!interrupted) {
        float a_pts = 0.;

        if (!frame_data->already_read) { // when called after fixdelay, a frame is already read
            frame_data->in_size = video_read_frame(sh_video, &frame_data->frame_time, &frame_data->start, force_fps);
            frame_data->flush = frame_data->in_size < 0 && d_video->eof;
            if (frame_data->flush) {
                frame_data->in_size = 0;
                frame_data->start = NULL;
            }
            if(frame_data->in_size<0) return 2;
            sh_video->timer += frame_data->frame_time;
        }
        frame_data->already_read = 0;

        a_pts = forward_audio(sh_video->pts - frame_data->frame_time + audio_delay, d_audio, mux_a);

        if (done) {
            // wait for keyframe in case of -ovc copy
            if (!framecopy || (sh_video->ds->flags & 1)) {
                frame_data->already_read = 1;
                return 1;
            }
        }
        if (sh_video->pts >= end_pts) done = 1;

        if (vfilter) {
            int softskip = (vfilter->control(vfilter, VFCTRL_SKIP_NEXT_FRAME, 0) == CONTROL_TRUE);
            void *decoded_frame = decode_video(sh_video, frame_data->start, frame_data->in_size, !softskip, MP_NOPTS_VALUE, MP_NOPTS_VALUE, NULL);
	    if (decoded_frame)
		filter_video(sh_video, decoded_frame, MP_NOPTS_VALUE, MP_NOPTS_VALUE);
	    else if (frame_data->flush)
		return 2;
        }

        if (print_info) mp_msg(MSGT_MENCODER, MSGL_STATUS,
               MSGTR_EdlSkipStartEndCurrent,
               next_edl_record->start_sec, next_edl_record->stop_sec,
               sh_video->pts, a_pts);
    }
    if (interrupted) return 2;
    return 1;
}

/// Deletes audio or video as told by -delay to sync
static void fixdelay(demux_stream_t *d_video, demux_stream_t *d_audio,
                     muxer_stream_t *mux_a, s_frame_data *frame_data,
                     int framecopy)
{
    // TODO: Find a way to encode silence instead of deleting video
    sh_video_t * sh_video = d_video->sh;
    float a_pts;

    // demux_seek has a weirdness that sh_video->pts is meaningless,
    // until a single frame is read... Same for audio actually too.
    // Reading one frame, and keeping it.
    frame_data->in_size = video_read_frame(sh_video, &frame_data->frame_time, &frame_data->start, force_fps);
    if(frame_data->in_size<0) return;
    sh_video->timer += frame_data->frame_time;
    frame_data->already_read = 1;

    a_pts = forward_audio(sh_video->pts - frame_data->frame_time + audio_delay, d_audio, mux_a);

    if (audio_delay > 0) return;
    else if (sh_video->pts - frame_data->frame_time + audio_delay >= a_pts) return;

    slowseek(a_pts - audio_delay, d_video, d_audio, mux_a, frame_data, framecopy, 0);
}

/** \brief Seeks for EDL
 *  \return 1 for success, 0 for failure, 2 for EOF. */
static int edl_seek(edl_record_ptr next_edl_record, demuxer_t *demuxer,
                    demux_stream_t *d_audio, muxer_stream_t *mux_a,
                    s_frame_data *frame_data, int framecopy)
{
    sh_video_t * sh_video = demuxer->video ? demuxer->video->sh : NULL;

    if (!sh_video) return 0;
    if (sh_video->pts >= next_edl_record->stop_sec) return 1; // nothing to do...

    if (!edl_seek_type) {
        if(demux_seek(demuxer, next_edl_record->stop_sec - sh_video->pts, audio_delay, 0)){
            sh_video->pts = demuxer->video->pts;
            //if (vo_vobsub) vobsub_seek(vo_vobsub,sh_video->pts);
            //if(vo_spudec) spudec_reset(vo_spudec);
            if (audio_delay != 0.0) fixdelay(demuxer->video, d_audio, mux_a, frame_data, framecopy);
            return 1;
        }
        // non-seekable stream.
        return 0;
    }

    // slow seek, read every frame.

    return slowseek(next_edl_record->stop_sec, demuxer->video, d_audio, mux_a, frame_data, framecopy, 1);
}


int main(int argc,char* argv[]){

stream_t* stream=NULL;
stream_t* ostream=NULL;
demuxer_t* demuxer=NULL;
stream_t* stream2=NULL;
demuxer_t* demuxer2=NULL;
demux_stream_t *d_audio=NULL;
demux_stream_t *d_video=NULL;
demux_stream_t *d_dvdsub=NULL;
sh_audio_t *sh_audio=NULL;
sh_video_t *sh_video=NULL;
int file_format=DEMUXER_TYPE_UNKNOWN;
int i=DEMUXER_TYPE_UNKNOWN;
void *vobsub_writer=NULL;
s_frame_data frame_data = { .start = NULL, .in_size = 0, .frame_time = 0., .already_read = 0 };

uint32_t ptimer_start;
uint32_t audiorate=0;
uint32_t videorate=0;
uint32_t audiosamples=1;
uint32_t videosamples=1;
uint32_t skippedframes=0;
uint32_t duplicatedframes=0;
uint32_t badframes=0;

muxer_stream_t* mux_a=NULL;
// For correct deallocation
unsigned char *mux_v_buffer = NULL;
muxer_stream_t* mux_v=NULL;
off_t muxer_f_size=0;

double v_pts_corr=0;
double v_timer_corr=0;
double sub_offset=0;
int did_seek=0;

m_entry_t* filelist = NULL;
char* filename=NULL;

int decoded_frameno=0;
int next_frameno=-1;
int curfile=0;
int new_srate=0;

unsigned int timer_start=0;
ao_data_t ao_data = {0,0,0,0,OUTBURST,-1,0};

audio_encoding_params_t aparams;
audio_encoder_t *aencoder = NULL;

user_correct_pts = 0;

  common_preinit(&argc, &argv);

  // Create the config context and register the options
  mconfig = m_config_new();
  m_config_register_options(mconfig,mencoder_opts);
  m_config_register_options(mconfig, common_opts);

  // Preparse the command line
  m_config_preparse_command_line(mconfig,argc,argv);

  print_version("MEncoder");

 parse_cfgfiles(mconfig);
 filelist = m_config_parse_me_command_line(mconfig, argc, argv);
 if(!filelist) mencoder_exit(1, MSGTR_ErrorParsingCommandLine);
 if (!common_init())
   mencoder_exit(1,NULL);

{
	char *extension;

	if (!out_filename) mencoder_exit(1,MSGTR_MissingOutputFilename);
	extension=strrchr(out_filename,'.');
	if (extension != NULL && strlen(extension) > 3 && strlen(extension) < 6)
	{
		extension++;

		switch (out_file_format)
		{
			case MUXER_TYPE_AVI:
			if (av_strcasecmp(extension,"avi"))
				mp_msg(MSGT_MENCODER, MSGL_WARN, MSGTR_MencoderWrongFormatAVI);
			break;

			case MUXER_TYPE_MPEG:
			if (av_strcasecmp(extension,"mpg") &&
				av_strcasecmp(extension,"mpeg") &&
				av_strcasecmp(extension,"vob"))
				mp_msg(MSGT_MENCODER, MSGL_WARN, MSGTR_MencoderWrongFormatMPG);
			break;
		}
	}
}
 /* Display what configure line was used */
 mp_msg(MSGT_MENCODER, MSGL_V, "Configuration: " CONFIGURATION "\n");


if (frameno_filename) {
  stream2=open_stream(frameno_filename, NULL, NULL);
  if(stream2){
    demuxer2=demux_open(stream2,DEMUXER_TYPE_AVI,-1,-1,-2,NULL);
    if(demuxer2) mp_msg(MSGT_MENCODER, MSGL_INFO, MSGTR_UsingPass3ControlFile, frameno_filename);
    else mp_msg(MSGT_DEMUXER,MSGL_ERR,MSGTR_FormatNotRecognized);
  }
}

  /* HACK, for some weird reason, push() has to be called twice,
     otherwise options are not saved correctly */
  m_config_push(mconfig);
play_next_file:
  m_config_push(mconfig);
  m_entry_set_options(mconfig,&filelist[curfile]);
  filename = filelist[curfile].name;

  if(!filename){
	mp_msg(MSGT_CPLAYER, MSGL_FATAL, MSGTR_MissingFilename);
	mencoder_exit(1,NULL);
  }

  if (vobsub_name)
    vo_vobsub = vobsub_open(vobsub_name, spudec_ifo, 1, &vo_spudec);
#ifdef CONFIG_ASS
  // must be before demuxer open, since the settings are
  // used in generating the ASSTrack
  if (ass_enabled && ass_library)
    ass_mp_reset_config(ass_library);
#endif

  stream=open_stream(filename,0,&file_format);

  if(!stream){
	mp_msg(MSGT_CPLAYER, MSGL_FATAL, MSGTR_CannotOpenFile_Device);
	mencoder_exit(1,NULL);
  }

  mp_msg(MSGT_CPLAYER, MSGL_INFO, MSGTR_OpenedStream, file_format, (int)(stream->start_pos), (int)(stream->end_pos));

if(stream->type==STREAMTYPE_BD){
  if(audio_lang && audio_id==-1) audio_id=bd_aid_from_lang(stream,audio_lang);
  if(dvdsub_lang && dvdsub_id==-1) dvdsub_id=bd_sid_from_lang(stream,dvdsub_lang);
}

#ifdef CONFIG_LIBBLURAY
if(stream->type==STREAMTYPE_BLURAY){
  if(audio_lang && audio_id==-1) audio_id=bluray_id_from_lang(stream,stream_ctrl_audio,audio_lang);
  if(dvdsub_lang && dvdsub_id==-1) dvdsub_id=bluray_id_from_lang(stream,stream_ctrl_sub,dvdsub_lang);
}
#endif

#ifdef CONFIG_DVDREAD
if(stream->type==STREAMTYPE_DVD){
  if(audio_lang && audio_id==-1) audio_id=dvd_aid_from_lang(stream,audio_lang);
  if(dvdsub_lang && dvdsub_id==-1) dvdsub_id=dvd_sid_from_lang(stream,dvdsub_lang);
}
#endif

#ifdef CONFIG_DVDNAV
if(stream->type==STREAMTYPE_DVDNAV){
  if(audio_lang && audio_id==-1) audio_id=mp_dvdnav_aid_from_lang(stream,audio_lang);
  if(dvdsub_lang && dvdsub_id==-1) dvdsub_id=mp_dvdnav_sid_from_lang(stream,dvdsub_lang);
}
#endif

  stream->start_pos+=seek_to_byte;

  if(stream_cache_size>0) stream_enable_cache(stream,stream_cache_size*1024ull,0,0);

  if(demuxer2) audio_id=-2; /* do NOT read audio packets... */

  demuxer=demux_open(stream,file_format,audio_id,video_id,dvdsub_id,filename);
  if(!demuxer){
    mp_msg(MSGT_DEMUXER, MSGL_FATAL, MSGTR_FormatNotRecognized);
    mp_msg(MSGT_DEMUXER, MSGL_FATAL, MSGTR_CannotOpenDemuxer);
	mencoder_exit(1,NULL);
  }

  if (ts_prog) {
    demux_program_t prog = { .progid = ts_prog };
    if (demux_control(demuxer, DEMUXER_CTRL_IDENTIFY_PROGRAM, &prog) != DEMUXER_CTRL_NOTIMPL) {
      audio_id = prog.aid; // switching is handled by select_audio below
      video_id = prog.vid; // switching is handled by select_video below
    }
  }
  select_video(demuxer, video_id);
  select_audio(demuxer, audio_id, audio_lang);

  if (dvdsub_id == -1 && dvdsub_lang)
    dvdsub_id = demuxer_sub_track_by_lang(demuxer, dvdsub_lang);

  if (dvdsub_id == -1)
    dvdsub_id = demuxer_default_sub_track(demuxer);

  for (i = 0; i < MAX_S_STREAMS; i++) {
    sh_sub_t *sh = demuxer->s_streams[i];
    if (sh && sh->sid == dvdsub_id) {
      demuxer->sub->id = i;
      demuxer->sub->sh = sh;
      break;
    }
  }

  if(dvd_chapter>1) {
    float pts;
    if (demuxer_seek_chapter(demuxer, dvd_chapter-1, 1, &pts, NULL, NULL) >= 0 && pts > -1.0)
      seek_to_sec = pts;
  }

d_audio=demuxer2 ? demuxer2->audio : demuxer->audio;
d_video=demuxer->video;
d_dvdsub=demuxer->sub;
sh_audio=d_audio->sh;
sh_video=d_video->sh;

  if(!sh_video)
  {
	mp_msg(MSGT_CPLAYER,MSGL_FATAL,MSGTR_VideoStreamRequired);
	mencoder_exit(1,NULL);
  }

  if(!video_read_properties(sh_video)){
      mp_msg(MSGT_CPLAYER, MSGL_FATAL, MSGTR_CannotReadVideoProperties);
      mencoder_exit(1,NULL);
  }

  mp_msg(MSGT_MENCODER,MSGL_INFO, MSGTR_FilefmtFourccSizeFpsFtime,
   demuxer->file_format,sh_video->format, sh_video->disp_w,sh_video->disp_h,
   sh_video->fps,sh_video->frametime
  );

  if(force_fps){
    sh_video->fps=force_fps;
    sh_video->frametime=1.0f/sh_video->fps;
    mp_msg(MSGT_MENCODER,MSGL_INFO,MSGTR_ForcingInputFPS, sh_video->fps);
  }

  if(sh_audio && out_audio_codec<0){
    if(audio_id==-2)
	mp_msg(MSGT_MENCODER,MSGL_ERR,MSGTR_DemuxerDoesntSupportNosound);
    mp_msg(MSGT_MENCODER,MSGL_FATAL,MSGTR_NoAudioEncoderSelected);
    mencoder_exit(1,NULL);
  }
  if(sh_video && out_video_codec<0){
    mp_msg(MSGT_MENCODER,MSGL_FATAL,MSGTR_NoVideoEncoderSelected);
    mencoder_exit(1,NULL);
  }

if(sh_audio && (out_audio_codec || seek_to_sec || !sh_audio->wf || playback_speed != 1.0)){
  // Go through the codec.conf and find the best codec...
  mp_msg(MSGT_CPLAYER,MSGL_INFO,"==========================================================================\n");
  if(!init_best_audio_codec(sh_audio,audio_codec_list,audio_fm_list)){
    sh_audio=d_audio->sh=NULL; // failed to init :(
  }
  mp_msg(MSGT_CPLAYER,MSGL_INFO,"==========================================================================\n");
}

  if (sh_audio) {
    new_srate = sh_audio->samplerate;
    if (playback_speed != 1.0) {
        new_srate *= playback_speed;
        // limits are taken from libaf/af_resample.c
        if (new_srate < 8000) new_srate = 8000;
        if (new_srate > 192000) new_srate = 192000;
        playback_speed = (float)new_srate / (float)sh_audio->samplerate;
    }
  }

// set up video encoder:

if (!curfile) { // curfile is non zero when a second file is opened
if (vobsub_out) {
    unsigned int palette[16], width, height;
    unsigned char tmp[3] = { 0, 0, 0 };
    if (spudec_ifo && vobsub_parse_ifo(NULL,spudec_ifo, palette, &width, &height, 1, dvdsub_id, tmp) >= 0)
	vobsub_writer = vobsub_out_open(vobsub_out, palette, sh_video->disp_w, sh_video->disp_h,
					vobsub_out_id?vobsub_out_id:(char *)tmp, vobsub_out_index);
#ifdef CONFIG_DVDREAD
    if (vobsub_writer == NULL) {
	char tmp[3];
	if (vobsub_out_id == NULL && stream->type == STREAMTYPE_DVD) {
	    int i;
	    dvd_priv_t *dvd = (dvd_priv_t*)stream->priv;
	    for (i = 0; i < dvd->nr_of_subtitles; ++i)
		if (dvd->subtitles[i].id == dvdsub_id) {
		    tmp[0] = (dvd->subtitles[i].language >> 8) & 0xff;
		    tmp[1] = dvd->subtitles[i].language & 0xff;
		    tmp[2] = 0;
		    vobsub_out_id = tmp;
		    break;
		}
	}
	vobsub_writer=vobsub_out_open(vobsub_out, stream->type==STREAMTYPE_DVD?((dvd_priv_t *)(stream->priv))->cur_pgc->palette:NULL,
				      sh_video->disp_w, sh_video->disp_h, vobsub_out_id, vobsub_out_index);
    }
#endif
}
else if (!vo_spudec) {
init_vo_spudec(stream, sh_video, d_dvdsub ? d_dvdsub->sh : NULL);
}

ostream = open_output_stream(out_filename, 0);
if(!ostream) {
  mp_msg(MSGT_MENCODER, MSGL_FATAL, MSGTR_CannotOpenOutputFile, out_filename);
  mencoder_exit(1,NULL);
}

muxer=muxer_new_muxer(out_file_format,ostream);
if(!muxer) {
  mp_msg(MSGT_MENCODER, MSGL_FATAL, MSGTR_CannotInitializeMuxer);
  mencoder_exit(1,NULL);
}
#if 0
//disabled: it horrybly distorts filtered sound
if(out_file_format == MUXER_TYPE_MPEG) audio_preload = 0;
#endif

muxer->audio_delay_fix = audio_delay_fix;

// ============= VIDEO ===============

mux_v=muxer_new_stream(muxer,MUXER_TYPE_VIDEO);

mux_v->buffer_size=0x800000; // 8MB
mux_v->buffer=mux_v_buffer=malloc(mux_v->buffer_size);

mux_v->source=sh_video;

mux_v->h.dwSampleSize=0; // VBR
#ifdef CONFIG_FFMPEG
{
    double fps = force_ofps?force_ofps:sh_video->fps*playback_speed;
    AVRational q= av_d2q(fps, fps*1001+2);
    mux_v->h.dwScale= q.den;
    mux_v->h.dwRate = q.num;
}
#else
mux_v->h.dwScale=10000;
mux_v->h.dwRate=mux_v->h.dwScale*(force_ofps?force_ofps:sh_video->fps*playback_speed);
#endif

mux_v->codec=out_video_codec;

mux_v->bih=NULL;
}
sh_video->codec=NULL;
sh_video->vfilter=NULL; // fixme!

switch(mux_v->codec){
case VCODEC_COPY:
	if (!curfile) {
		if (sh_video->bih) {
			mux_v->bih=malloc(sh_video->bih->biSize);
			memcpy(mux_v->bih, sh_video->bih, sh_video->bih->biSize);
		}
    else
    {
	mux_v->bih=calloc(1,sizeof(*mux_v->bih));
	mux_v->bih->biSize=sizeof(*mux_v->bih);
	mux_v->bih->biWidth=sh_video->disp_w;
	mux_v->bih->biHeight=sh_video->disp_h;
	mux_v->bih->biCompression=sh_video->format;
	mux_v->bih->biPlanes=1;
	mux_v->bih->biBitCount=24; // FIXME!!!
	mux_v->bih->biSizeImage=mux_v->bih->biWidth*mux_v->bih->biHeight*(mux_v->bih->biBitCount/8);
    }
	}
    mp_msg(MSGT_MENCODER, MSGL_INFO, MSGTR_VCodecFramecopy,
	mux_v->bih->biWidth, mux_v->bih->biHeight,
	mux_v->bih->biBitCount, mux_v->bih->biCompression);

	if (curfile) {
		if (sh_video->bih) {
			if ((mux_v->bih->biSize != sh_video->bih->biSize) ||
			    memcmp(mux_v->bih, sh_video->bih, sh_video->bih->biSize))
			{
				mp_msg(MSGT_MENCODER, MSGL_INFO, MSGTR_VCodecFramecopy,
				       sh_video->bih->biWidth, sh_video->bih->biHeight,
				       sh_video->bih->biBitCount, sh_video->bih->biCompression);
				mp_msg(MSGT_MENCODER,MSGL_FATAL,MSGTR_FrameCopyFileMismatch);
				mencoder_exit(1,NULL);
			}
		}
		else {
			if ((mux_v->bih->biWidth != sh_video->disp_w) ||
			    (mux_v->bih->biHeight != sh_video->disp_h) ||
			    (mux_v->bih->biCompression != sh_video->format)) {
				mp_msg(MSGT_MENCODER, MSGL_INFO, MSGTR_VCodecFramecopy,
				       sh_video->disp_w, sh_video->disp_w, 24, sh_video->format);
				mp_msg(MSGT_MENCODER,MSGL_FATAL,MSGTR_FrameCopyFileMismatch);
				mencoder_exit(1,NULL);
			}
		}
	}
    break;
case VCODEC_FRAMENO:
	if (!curfile) {
    mux_v->bih=calloc(1,sizeof(*mux_v->bih));
    mux_v->bih->biSize=sizeof(*mux_v->bih);
    mux_v->bih->biWidth=sh_video->disp_w;
    mux_v->bih->biHeight=sh_video->disp_h;
    mux_v->bih->biPlanes=1;
    mux_v->bih->biBitCount=24;
    mux_v->bih->biCompression=mmioFOURCC('F','r','N','o');
    mux_v->bih->biSizeImage=mux_v->bih->biWidth*mux_v->bih->biHeight*(mux_v->bih->biBitCount/8);
	}
    break;
default: {
    static vf_instance_t * ve = NULL;
  if (!ve) {
    switch(mux_v->codec){
    case VCODEC_LIBAVCODEC:
        sh_video->vfilter=vf_open_encoder(NULL,"lavc",(char *)mux_v); break;
    case VCODEC_RAW:
        sh_video->vfilter=vf_open_encoder(NULL,"raw",(char *)mux_v); break;
    case VCODEC_VFW:
        sh_video->vfilter=vf_open_encoder(NULL,"vfw",(char *)mux_v); break;
    case VCODEC_LIBDV:
        sh_video->vfilter=vf_open_encoder(NULL,"libdv",(char *)mux_v); break;
    case VCODEC_XVID:
        sh_video->vfilter=vf_open_encoder(NULL,"xvid",(char *)mux_v); break;
    case VCODEC_QTVIDEO:
        sh_video->vfilter=vf_open_encoder(NULL,"qtvideo",(char *)mux_v); break;
    case VCODEC_NUV:
        sh_video->vfilter=vf_open_encoder(NULL,"nuv",(char *)mux_v); break;
    case VCODEC_X264:
        sh_video->vfilter=vf_open_encoder(NULL,"x264",(char *)mux_v); break;
    }
    if(!mux_v->bih || !sh_video->vfilter){
        mp_msg(MSGT_MENCODER,MSGL_FATAL,MSGTR_EncoderOpenFailed);
        mencoder_exit(1,NULL);
    }
    ve = sh_video->vfilter;
  } else sh_video->vfilter = ve;
    // append 'expand' filter, it fixes stride problems and renders osd:
#ifdef CONFIG_ASS
    if (auto_expand && !ass_enabled) { /* we do not want both */
#else
    if (auto_expand) {
#endif
      char* vf_args[] = { "osd", "1", NULL };
      sh_video->vfilter=vf_open_filter(sh_video->vfilter,"expand",vf_args);
    }

#ifdef CONFIG_ASS
  if(ass_enabled) {
    int i;
    int insert = 1;
    if (vf_settings)
      for (i = 0; vf_settings[i].name; ++i)
        if (strcmp(vf_settings[i].name, "ass") == 0) {
          insert = 0;
          break;
        }
    if (insert) {
      char* vf_arg[] = {"auto", "1", NULL};
      vf_instance_t* vf_ass = vf_open_filter(sh_video->vfilter,"ass",vf_arg);
      if (vf_ass)
        sh_video->vfilter=vf_ass;
      else
        mp_msg(MSGT_CPLAYER,MSGL_ERR, "ASS: cannot add video filter\n");
    }

    if (ass_library) {
      for (i = 0; i < demuxer->num_attachments; ++i) {
        demux_attachment_t* att = demuxer->attachments + i;
        if (extract_embedded_fonts &&
            att->name && att->type && att->data && att->data_size &&
            (strcmp(att->type, "application/x-truetype-font") == 0 ||
             strcmp(att->type, "application/x-font") == 0))
          ass_add_font(ass_library, att->name, att->data, att->data_size);
      }
    }
  }
#endif

  sh_video->vfilter=append_filters(sh_video->vfilter);
  eosd_init(sh_video->vfilter);

#ifdef CONFIG_ASS
  if (ass_enabled)
    eosd_ass_init(ass_library);
#endif

// after reading video params we should load subtitles because
// we know fps so now we can adjust subtitles time to ~6 seconds AST
// check .sub
    load_subtitles(filename, sh_video->fps, add_subtitles);

    mp_msg(MSGT_CPLAYER,MSGL_INFO,"==========================================================================\n");
    init_best_video_codec(sh_video,video_codec_list,video_fm_list);
    mp_msg(MSGT_CPLAYER,MSGL_INFO,"==========================================================================\n");
    if(!sh_video->initialized) mencoder_exit(1,NULL);
 }
}

if (!curfile) {
/* force output fourcc to .. */
if ((force_fourcc != NULL) && (strlen(force_fourcc) >= 4))
{
    mux_v->bih->biCompression = mmioFOURCC(force_fourcc[0], force_fourcc[1],
					    force_fourcc[2], force_fourcc[3]);
    mp_msg(MSGT_MENCODER, MSGL_INFO, MSGTR_ForcingOutputFourcc,
	mux_v->bih->biCompression, (char *)&mux_v->bih->biCompression);
}

if (! ignore_start)
    muxer->audio_delay_fix -= sh_video->stream_delay;

//if(demuxer->file_format!=DEMUXER_TYPE_AVI) pts_from_bps=0; // it must be 0 for mpeg/asf!

// ============= AUDIO ===============
if(sh_audio){

if (force_audiofmttag != -1) {
	sh_audio->format = force_audiofmttag;
	if (sh_audio->wf) {
		sh_audio->wf->wFormatTag = sh_audio->format;
	}
	mp_msg(MSGT_MENCODER, MSGL_INFO, MSGTR_ForcingOutputAudiofmtTag,
	       force_audiofmttag);
}

mux_a=muxer_new_stream(muxer,MUXER_TYPE_AUDIO);

mux_a->buffer_size=0x100000; //16384;
mux_a->buffer=malloc(mux_a->buffer_size);
if (!mux_a->buffer)
    mencoder_exit(1,MSGTR_MemAllocFailed);

mux_a->source=sh_audio;

mux_a->codec=out_audio_codec;

ao_data.samplerate = force_srate;
ao_data.channels = 0;
ao_data.format = audio_output_format;
if(!init_audio_filters(sh_audio,
   // input:
   new_srate,
   // output:
   &ao_data.samplerate, &ao_data.channels, &ao_data.format)) {
     mp_msg(MSGT_CPLAYER,MSGL_ERR,MSGTR_AudioFilterChainPreinitError);
     mencoder_exit(1, NULL);
   }

aparams.channels = ao_data.channels;
aparams.sample_rate = ao_data.samplerate;
aparams.sample_format = ao_data.format;
aparams.audio_preload = 1000 * audio_preload;
if(mux_a->codec != ACODEC_COPY) {
    aencoder = new_audio_encoder(mux_a, &aparams);
    if(!aencoder)
        mencoder_exit(1, NULL);
    if(!init_audio_filters(sh_audio,
        new_srate,
        &aparams.sample_rate, &aparams.channels, &aencoder->input_format)) {
      mp_msg(MSGT_CPLAYER,MSGL_FATAL,MSGTR_NoMatchingFilter);
      mencoder_exit(1,NULL);
    }
    ao_data.format = aencoder->input_format;
    ao_data.channels = aparams.channels;
    ao_data.samplerate = aparams.sample_rate;
}
switch(mux_a->codec){
case ACODEC_COPY:
    if (playback_speed != 1.0) mp_msg(MSGT_CPLAYER, MSGL_WARN, MSGTR_NoSpeedWithFrameCopy);
    if (sh_audio->format >= 0x10000) {
	mp_msg(MSGT_MENCODER,MSGL_ERR,MSGTR_CantCopyAudioFormat, sh_audio->format);
	mencoder_exit(1,NULL);
    }
    if (sh_audio->wf){
	mux_a->wf=malloc(sizeof(*mux_a->wf) + sh_audio->wf->cbSize);
	memcpy(mux_a->wf, sh_audio->wf, sizeof(*mux_a->wf) + sh_audio->wf->cbSize);
	if(!sh_audio->i_bps) sh_audio->i_bps=mux_a->wf->nAvgBytesPerSec;
    } else {
	mux_a->wf = malloc(sizeof(*mux_a->wf));
	mux_a->wf->nBlockAlign = 1; //mux_a->h.dwSampleSize;
	mux_a->wf->wFormatTag = sh_audio->format;
	mux_a->wf->nChannels = sh_audio->channels;
	mux_a->wf->nSamplesPerSec = sh_audio->samplerate;
	mux_a->wf->nAvgBytesPerSec=sh_audio->i_bps; //mux_a->h.dwSampleSize*mux_a->wf->nSamplesPerSec;
	mux_a->wf->wBitsPerSample = 16; // FIXME
	mux_a->wf->cbSize=0; // FIXME for l3codeca.acm
    }
    if(sh_audio->audio.dwScale){
	mux_a->h.dwSampleSize=sh_audio->audio.dwSampleSize;
	mux_a->h.dwScale=sh_audio->audio.dwScale;
	mux_a->h.dwRate=sh_audio->audio.dwRate;
    } else {
	mux_a->h.dwSampleSize=mux_a->wf->nBlockAlign;
	mux_a->h.dwScale=mux_a->h.dwSampleSize;
	mux_a->h.dwRate=mux_a->wf->nAvgBytesPerSec;
    }
    mux_a->h.dwRate *= playback_speed;
    mux_a->wf->nSamplesPerSec *= playback_speed;
    mp_msg(MSGT_MENCODER, MSGL_INFO, MSGTR_ACodecFramecopy,
	mux_a->wf->wFormatTag, mux_a->wf->nChannels, mux_a->wf->nSamplesPerSec,
	mux_a->wf->wBitsPerSample, mux_a->wf->nAvgBytesPerSec, mux_a->h.dwSampleSize);
    break;
}

if ( mp_msg_test(MSGT_MENCODER,MSGL_DBG2) ) print_wave_header(mux_a->wf, MSGL_DBG2);

if (! ignore_start)
    muxer->audio_delay_fix += sh_audio->stream_delay;

} // if(sh_audio)

decoded_frameno=0;

signal(SIGINT,exit_sighandler);  // Interrupt from keyboard
signal(SIGQUIT,exit_sighandler); // Quit from keyboard
signal(SIGTERM,exit_sighandler); // kill
signal(SIGHUP,exit_sighandler);  // broken terminal line
signal(SIGPIPE,exit_sighandler); // broken pipe

timer_start=GetTimerMS();
} // if (!curfile) // if this was the first file.
else {
	if (!mux_a != !sh_audio) {
		mp_msg(MSGT_MENCODER,MSGL_FATAL,MSGTR_NoAudioFileMismatch);
		mencoder_exit(1,NULL);
	}
	if (sh_audio && mux_a->codec == ACODEC_COPY) {
		if (playback_speed != 1.0) mp_msg(MSGT_CPLAYER, MSGL_WARN, MSGTR_NoSpeedWithFrameCopy);
		mp_msg(MSGT_MENCODER, MSGL_INFO, MSGTR_ACodecFramecopy,
		       mux_a->wf->wFormatTag, mux_a->wf->nChannels, mux_a->wf->nSamplesPerSec,
		       mux_a->wf->wBitsPerSample, mux_a->wf->nAvgBytesPerSec, mux_a->h.dwSampleSize);
		if (sh_audio->wf) {
			if ((mux_a->wf->wFormatTag != sh_audio->wf->wFormatTag) ||
			    (mux_a->wf->nChannels != sh_audio->wf->nChannels) ||
			    (mux_a->wf->nSamplesPerSec != sh_audio->wf->nSamplesPerSec * playback_speed))
			{
				mp_msg(MSGT_MENCODER, MSGL_INFO, MSGTR_ACodecFramecopy,
				       sh_audio->wf->wFormatTag, sh_audio->wf->nChannels, (int)(sh_audio->wf->nSamplesPerSec * playback_speed),
				       sh_audio->wf->wBitsPerSample, sh_audio->wf->nAvgBytesPerSec, 0);
				mp_msg(MSGT_MENCODER,MSGL_FATAL,MSGTR_AudioCopyFileMismatch);
				mencoder_exit(1,NULL);
			}
		} else {
			if ((mux_a->wf->wFormatTag != sh_audio->format) ||
			    (mux_a->wf->nChannels != sh_audio->channels) ||
			    (mux_a->wf->nSamplesPerSec != sh_audio->samplerate * playback_speed))
			{
				mp_msg(MSGT_MENCODER, MSGL_INFO, MSGTR_ACodecFramecopy,
				       sh_audio->wf->wFormatTag, sh_audio->wf->nChannels, (int)(sh_audio->wf->nSamplesPerSec * playback_speed),
				       sh_audio->wf->wBitsPerSample, sh_audio->wf->nAvgBytesPerSec, 0);
				mp_msg(MSGT_MENCODER,MSGL_FATAL,MSGTR_AudioCopyFileMismatch);
				mencoder_exit(1,NULL);
			}
		}
	} else if (sh_audio) {
		int out_srate = mux_a->wf->nSamplesPerSec;
		int out_channels = mux_a->wf->nChannels;
		int out_format = aencoder->input_format;
		if (!init_audio_filters(sh_audio, new_srate,
					&out_srate, &out_channels,
					&out_format)) {
			mp_msg(MSGT_CPLAYER, MSGL_FATAL, MSGTR_NoMatchingFilter);
			mencoder_exit(1, NULL);
		}
		mux_a->wf->nSamplesPerSec = out_srate;
		mux_a->wf->nChannels = out_channels;
	}
}

if (seek_to_sec) {
    demux_seek(demuxer, seek_to_sec, audio_delay, 1);
//  there is 2 way to handle the -ss option in 3-pass mode:
// > 1. do the first pass for the whole file, and use -ss for 2nd/3rd pases only
// > 2. do all the 3 passes with the same -ss value
//  this line enables behaviour 1. (and kills 2. at the same time):
//    if(demuxer2) demux_seek(demuxer2, d, audio_delay, 1);
}

if (out_file_format == MUXER_TYPE_MPEG)
	{
	if (audio_preload > 0.4) {
	  mp_msg(MSGT_MENCODER, MSGL_WARN, MSGTR_LimitingAudioPreload);
	  audio_preload = 0.4;
	}
	if (audio_density < 4) {
	  mp_msg(MSGT_MENCODER, MSGL_WARN, MSGTR_IncreasingAudioDensity);
	  audio_density = 4;
	}
	}

if(file_format == DEMUXER_TYPE_TV)
	{
	mp_msg(MSGT_MENCODER, MSGL_WARN, MSGTR_ZeroingAudioPreloadAndMaxPtsCorrection);
	audio_preload = 0.0;
	default_max_pts_correction = 0;
	}

play_n_frames=play_n_frames_mf;
if (curfile && end_at.type == END_AT_TIME) end_at.pos += adjusted_muxer_time(mux_v);

if (edl_records) free_edl(edl_records);
next_edl_record = edl_records = NULL;
edl_muted = 0;
edl_seeking = 1;
if (edl_filename) {
    next_edl_record = edl_records = edl_parse_file();
}

if (sh_audio && audio_delay != 0.) fixdelay(d_video, d_audio, mux_a, &frame_data, mux_v->codec==VCODEC_COPY);

// Just assume a seek. Also works if time stamps do not start with 0
did_seek = 1;

while(at_eof != 3){

    int blit_frame=0;
    float a_pts=0;
    float v_pts=0;
    int skip_flag=0; // 1=skip  -1=duplicate
    double a_muxer_time;
    double v_muxer_time;
    int force_audio = at_eof & 1;

    a_muxer_time = adjusted_muxer_time(mux_a);
    v_muxer_time = adjusted_muxer_time(mux_v);
    at_eof = sh_audio ? 0 : 2;

    if((end_at.type == END_AT_SIZE && end_at.pos <= stream_tell(muxer->stream))  ||
       (end_at.type == END_AT_TIME && end_at.pos < v_muxer_time))
        break;

    if(play_n_frames>=0){
      --play_n_frames;
      if(play_n_frames<0) break;
    }
    if(dvd_last_chapter>0) {
      int cur_chapter = demuxer_get_current_chapter(demuxer);
      if(cur_chapter!=-1 && cur_chapter+1>dvd_last_chapter)
        break;
    }

goto_redo_edl:
    if (next_edl_record && sh_video && sh_video->pts >= next_edl_record->start_sec) {
        if (next_edl_record->action == EDL_SKIP && edl_seeking) {
            float last_pos = d_video->pts;
            int result;
            mp_msg(MSGT_CPLAYER, MSGL_DBG4, "EDL_SKIP: start [%f], stop [%f], length [%f]\n",
                   next_edl_record->start_sec, next_edl_record->stop_sec, next_edl_record->length_sec);

            result = edl_seek(next_edl_record, demuxer, d_audio, mux_a, &frame_data, mux_v->codec==VCODEC_COPY);

            if (result == 2) { at_eof=3; break; } // EOF
            else if (result == 0) edl_seeking = 0; // no seeking
            else { // sucess
                did_seek = 1;
                edl_muted = 0;
                if (last_pos >= sh_video->pts) {
                    // backwards seek detected!! Forget about this EDL skip altogether.
                    next_edl_record = next_edl_record->next;
                }
                else for (next_edl_record = edl_records; next_edl_record; next_edl_record = next_edl_record->next) {
                    /* note the use of stop_sec,
                       meaning if by some magical way we landed in the MIDDLE of a censored area,
                       in the next loop it will jump out of it.
                    */
                    if (next_edl_record->stop_sec > sh_video->pts) break; // we got to the right place.
                    if (next_edl_record->action == EDL_MUTE) edl_muted = !edl_muted; // toggle mute each time.
                }

                /* for a pedantic EDL, that doesn't show even a single
                   frame from the "censored" area, uncomment next line. */
                goto goto_redo_edl;
            }
        } else if (next_edl_record->action == EDL_MUTE) {
            edl_muted = !edl_muted;  // This variable does nothing for now.
            mp_msg(MSGT_CPLAYER, MSGL_DBG4, "EDL_MUTE: [%f]\n", next_edl_record->start_sec );
            next_edl_record=next_edl_record->next;
        }
    }


if(sh_audio){
    // get audio:
    while(force_audio || a_muxer_time-audio_preload<v_muxer_time){
        float tottime;
	int bytes_to_mux=0;
	force_audio = 0;

	ptimer_start = GetTimerMS();
	// CBR - copy 0.5 sec of audio
	// or until the end of video:
	tottime = stop_time(demuxer, mux_v);
	if (tottime != -1) {
		tottime -= a_muxer_time;
		if (tottime > 1./audio_density) tottime = 1./audio_density;
	}
	else tottime = 1./audio_density;

	// let's not output more audio than necessary
	if (tottime <= 0) break;

	if(aencoder)
	{
		if(mux_a->h.dwSampleSize) /* CBR */
		{
			int len;
			if(aencoder->set_decoded_len)
			{
				len = mux_a->h.dwSampleSize*(int)(mux_a->h.dwRate*tottime);
				aencoder->set_decoded_len(aencoder, len);
			}
			else
				len = aencoder->decode_buffer_size;

			len = dec_audio(sh_audio, aencoder->decode_buffer, len);
			mux_a->buffer_len += aencoder->encode(aencoder, mux_a->buffer + mux_a->buffer_len,
				len <= 0 && sh_audio->ds->eof ? NULL : aencoder->decode_buffer, len, mux_a->buffer_size-mux_a->buffer_len);
			if(mux_a->buffer_len >= mux_a->wf->nBlockAlign)
				bytes_to_mux = mux_a->wf->nBlockAlign*(mux_a->buffer_len/mux_a->wf->nBlockAlign);
		}
		else	/* VBR */
		{
			int sz = 0;
			while(1)
			{
				int len;
				if(! sz)
					sz = aencoder->get_frame_size(aencoder);
				if(sz > 0 && mux_a->buffer_len >= sz)
				{
					bytes_to_mux = sz;
					break;
				}
				len = dec_audio(sh_audio,aencoder->decode_buffer, aencoder->decode_buffer_size);
				if(len <= 0)
				{
					// try flushing encoder
					if (sh_audio->ds->eof)
						len = aencoder->encode(aencoder, mux_a->buffer + mux_a->buffer_len, NULL, 0, mux_a->buffer_size-mux_a->buffer_len);
					if (len <= 0)
						break;
				} else
					len = aencoder->encode(aencoder, mux_a->buffer + mux_a->buffer_len, aencoder->decode_buffer, len, mux_a->buffer_size-mux_a->buffer_len);
				mux_a->buffer_len += len;
			}
	    }
	    if (v_muxer_time == 0) mux_a->h.dwInitialFrames++;
	}
	else if (mux_a->codec == ACODEC_COPY) {
	if(mux_a->h.dwSampleSize){
	    int len;
		len=mux_a->wf->nAvgBytesPerSec*tottime;
		len/=mux_a->h.dwSampleSize;if(len<1) len=1;
		len*=mux_a->h.dwSampleSize;
		bytes_to_mux=demux_read_data(sh_audio->ds,mux_a->buffer,len);
	} else {
	    // VBR - encode/copy an audio frame
		bytes_to_mux=ds_get_packet(sh_audio->ds,(unsigned char**) &mux_a->buffer);
	    }
	}
	if(bytes_to_mux<=0) {
	    // EOF?
	    if (!sh_audio->a_out_buffer_len && sh_audio->ds->eof) {
		if (mux_a->buffer_len)
	           mp_msg(MSGT_MENCODER, MSGL_WARN, "Audio data left in buffer at end of file. Probably bug in audio encoder, please report.");
	        at_eof |= 2;
	    }
	    break;
	}
	muxer_write_chunk(mux_a,bytes_to_mux,AVIIF_KEYFRAME, MP_NOPTS_VALUE, MP_NOPTS_VALUE);
	a_muxer_time = adjusted_muxer_time(mux_a); // update after muxing
	if(!mux_a->h.dwSampleSize && a_muxer_time>0)
	    mux_a->wf->nAvgBytesPerSec=0.5f+(double)mux_a->size/a_muxer_time; // avg bps (VBR)
	if(mux_a->buffer_len>=bytes_to_mux){
	    mux_a->buffer_len-=bytes_to_mux;
	    memmove(mux_a->buffer,mux_a->buffer+bytes_to_mux,mux_a->buffer_len);
	}


	audiosamples++;
	audiorate+= (GetTimerMS() - ptimer_start);

    }
}

    // get video frame!

    if (!frame_data.already_read) {
        frame_data.in_size=video_read_frame(sh_video,&frame_data.frame_time,&frame_data.start,force_fps);
        frame_data.flush = frame_data.in_size < 0 && d_video->eof && (at_eof & 2) &&
                           mux_v->codec != VCODEC_COPY &&
                           mux_v->codec != VCODEC_FRAMENO;
        if (frame_data.flush) {
            frame_data.in_size = 0;
            frame_data.start = NULL;
        }
        sh_video->timer+=frame_data.frame_time;
    }
    frame_data.frame_time /= playback_speed;
    if(frame_data.in_size<0){ frame_data.already_read = 0; at_eof|=1; continue; }
    ++decoded_frameno;

    v_timer_corr-=frame_data.frame_time-(float)mux_v->h.dwScale/mux_v->h.dwRate;

if(demuxer2){	// 3-pass encoding, read control file (frameno.avi)
    // find our frame:
	while(next_frameno<decoded_frameno){
	    int* start;
	    int len=ds_get_packet(demuxer2->video,(unsigned char**) &start);
	    if(len<0){ at_eof|=1;break;}
	    if(len==0) --skip_flag; else  // duplicate
	    if(len==4) next_frameno=start[0];
	}
    if(at_eof) continue;
	skip_flag=next_frameno-decoded_frameno;
    // find next frame:
	while(next_frameno<=decoded_frameno){
	    int* start;
	    int len=ds_get_packet(demuxer2->video,(unsigned char**) &start);
	    if(len<0){ at_eof|=1;break;}
	    if(len==0) --skip_flag; else  // duplicate
	    if(len==4) next_frameno=start[0];
	}
} else {

// check frame duplicate/drop:

float mux_frametime = (float)mux_v->h.dwScale/mux_v->h.dwRate;

if (v_timer_corr >= mux_frametime && (skip_limit<0 || skip_flag < skip_limit)) {
    v_timer_corr-=mux_frametime;
    ++skip_flag; // skip
}
while (v_timer_corr <= -mux_frametime && (skip_limit<0 || -skip_flag < skip_limit)) {
    v_timer_corr+=mux_frametime;
    --skip_flag; // dup
}

// either v_pts_corr is big, more than 2 times framerate, then we follow its advice,
// or, it cancels out v_timer_corr, in which case be happy and do nothing.

while ((v_pts_corr <= -mux_frametime && skip_flag > 0) || (v_pts_corr <= -2*mux_frametime)) {
    v_pts_corr+=mux_frametime;
    --skip_flag; // dup
}
if ((v_pts_corr >= mux_frametime && skip_flag < 0) || (v_pts_corr >= 2*mux_frametime)) {
  if (skip_flag<=0) { // we can't skip more than 1 frame now
    v_pts_corr-=mux_frametime;
    ++skip_flag; // skip
  }
}

} // demuxer2

ptimer_start = GetTimerMS();

switch(mux_v->codec){
case VCODEC_COPY:
    mux_v->buffer=frame_data.start;
    if(skip_flag<=0) muxer_write_chunk(mux_v,frame_data.in_size,(sh_video->ds->flags&1)?AVIIF_KEYFRAME:0, MP_NOPTS_VALUE, MP_NOPTS_VALUE);
    break;
case VCODEC_FRAMENO:
    mux_v->buffer=(unsigned char *)&decoded_frameno; // tricky
    if(skip_flag<=0) muxer_write_chunk(mux_v,sizeof(int),AVIIF_KEYFRAME, MP_NOPTS_VALUE, MP_NOPTS_VALUE);
    break;
default:
    // decode_video will callback down to ve_*.c encoders, through the video filters
    {
    int drop_frame = skip_flag > 0 &&
                     (!sh_video->vfilter ||
                      ((vf_instance_t *)sh_video->vfilter)->control(sh_video->vfilter, VFCTRL_SKIP_NEXT_FRAME, 0) != CONTROL_TRUE);
    void *decoded_frame = decode_video(sh_video,frame_data.start,frame_data.in_size,
                                       drop_frame, MP_NOPTS_VALUE, MP_NOPTS_VALUE, NULL);
    if (frame_data.flush && !decoded_frame)
        at_eof = 3;
    if (did_seek && sh_video->pts != MP_NOPTS_VALUE) {
        did_seek = 0;
        sub_offset = sh_video->pts;
    }
    // NOTE: this is not really correct, but it allows -ass to work mostly
    // v_muxer_time was tried before, but it is completely off when -ss is used
    // (see bug #1960).
    // sh_video->pts causes flickering with subtitles and complaints from MPEG-4
    // encoder due to not being monotonic.
    // If you change this please note the reason here!
    blit_frame = decoded_frame && filter_video(sh_video, decoded_frame, v_muxer_time + sub_offset, MP_NOPTS_VALUE);}
    v_muxer_time = adjusted_muxer_time(mux_v); // update after muxing

    if (sh_video->vf_initialized < 0) mencoder_exit(1, NULL);

    if(!blit_frame){
      if (play_n_frames >= 0)
        play_n_frames++;
      badframes++;
      if(skip_flag<=0){
	// unwanted skipping of a frame, what to do?
        v_timer_corr-=(float)mux_v->h.dwScale/mux_v->h.dwRate;
#if 0
        // Old code apparently made under the assumption that !blit_frame means
        // decoding failed due to corruption or something.. but duplication and
        // skipping of frames should be entirely disabled when skip_limit==0,
        // and must be in order for many filters to work with -noskip.
        // Eventually this entire block should probably be removed.
	if(skip_limit==0){
	    // skipping not allowed -> write empty frame:
	    if (!encode_duplicates || !sh_video->vfilter || ((vf_instance_t *)sh_video->vfilter)->control(sh_video->vfilter, VFCTRL_DUPLICATE_FRAME, 0) != CONTROL_TRUE)
	      muxer_write_chunk(mux_v,0,0, MP_NOPTS_VALUE, MP_NOPTS_VALUE);
	} else {
	    // skipping allowed -> skip it and distriubute timer error:
	    v_timer_corr-=(float)mux_v->h.dwScale/mux_v->h.dwRate;
	}
#endif
      }
    }
}

videosamples++;
videorate+=(GetTimerMS() - ptimer_start);

if(skip_flag<0){
    // duplicate frame
	if(!quiet) mp_msg(MSGT_MENCODER, MSGL_WARN, MSGTR_DuplicateFrames,-skip_flag);
    while(skip_flag<0){
	duplicatedframes++;
	if (!encode_duplicates || !sh_video->vfilter || ((vf_instance_t *)sh_video->vfilter)->control(sh_video->vfilter, VFCTRL_DUPLICATE_FRAME, 0) != CONTROL_TRUE)
	    muxer_write_chunk(mux_v,0,0, MP_NOPTS_VALUE, MP_NOPTS_VALUE);
	++skip_flag;
    }
} else
if(skip_flag>0){
    // skip frame
	if(!quiet) mp_msg(MSGT_MENCODER, MSGL_WARN, MSGTR_SkipFrame);
	skippedframes++;
    --skip_flag;
}

if(sh_audio && !demuxer2){
    float AV_delay,x;
    // A-V sync!
#if 0
    if(pts_from_bps){
        unsigned int samples=(sh_audio->audio.dwSampleSize)?
          ((ds_tell(d_audio)-sh_audio->a_in_buffer_len)/sh_audio->audio.dwSampleSize) :
          (d_audio->block_no); // <- used for VBR audio
        a_pts=samples*(float)sh_audio->audio.dwScale/(float)sh_audio->audio.dwRate;
    } else
#endif
    {
      // PTS = (last timestamp) + (bytes after last timestamp)/(bytes per sec)
      a_pts=calc_a_pts(sh_audio, d_audio);
    }
    v_pts=sh_video ? sh_video->pts : d_video->pts;
    // av = compensated (with out buffering delay) A-V diff
    AV_delay=(a_pts-v_pts);
    AV_delay-=audio_delay;
    AV_delay /= playback_speed;
    AV_delay-=a_muxer_time-(v_muxer_time-(v_timer_corr+v_pts_corr));
	// compensate input video timer by av:
        x=AV_delay*0.1f;
        if(x<-max_pts_correction) x=-max_pts_correction; else
        if(x> max_pts_correction) x= max_pts_correction;
        if(default_max_pts_correction>=0)
          max_pts_correction=default_max_pts_correction*playback_speed;
        else
          max_pts_correction=sh_video->frametime*0.10 *playback_speed; // +-10% of time
	// sh_video->timer-=x;
	c_total+=x;
	v_pts_corr+=x;
}

    {	float t=(GetTimerMS()-timer_start)*0.001f;
	float len=(demuxer->movi_end-demuxer->movi_start);
	off_t pos = demuxer->filepos >= 0 ? demuxer->filepos : stream_tell(demuxer->stream);
	float p=len>1000 ? (float)(pos-demuxer->movi_start) / len :
                (demuxer_get_percent_pos(demuxer) / 100.0);
#if 0
	if(!len && sh_audio && sh_audio->audio.dwLength>100){
	    p=(sh_audio->audio.dwSampleSize? ds_tell(sh_audio->ds)/sh_audio->audio.dwSampleSize : sh_audio->ds->block_no)
	     / (float)(sh_audio->audio.dwLength);
	}
#endif
      if(!quiet) {
	if( mp_msg_test(MSGT_STATUSLINE,MSGL_V) ) {
		mp_msg(MSGT_STATUSLINE,MSGL_STATUS,"VPos:%6.1fs APos:%6.1fs %6df (%2d%%) %3dfps Trem:%4dmin %3dmb  A-V:%5.3f [%d:%d] A/Vms %d/%d D/B/S %d/%d/%d \r",
	    	v_muxer_time, a_muxer_time, decoded_frameno, (int)(p*100),
	    	(t>1) ? (int)(decoded_frameno/t+0.5) : 0,
	    	(p>0.001) ? (int)((t/p-t)/60) : 0,
	    	(p>0.001) ? (int)(stream_tell(muxer->stream)/p/1024/1024) : 0,
	    	v_pts_corr,
	    	(v_muxer_time>1) ? (int)(mux_v->size/v_muxer_time/125) : 0,
	    	(mux_a && a_muxer_time>1) ? (int)(mux_a->size/a_muxer_time/125) : 0,
			audiorate/audiosamples, videorate/videosamples,
			duplicatedframes, badframes, skippedframes
		);
	} else
	mp_msg(MSGT_STATUSLINE,MSGL_STATUS,"Pos:%6.1fs %6df (%2d%%) %5.2ffps Trem:%4dmin %3dmb  A-V:%5.3f [%d:%d]\r",
	    v_muxer_time, decoded_frameno, (int)(p*100),
	    (t>1) ? (float)(decoded_frameno/t) : 0,
	    (p>0.001) ? (int)((t/p-t)/60) : 0,
	    (p>0.001) ? (int)(stream_tell(muxer->stream)/p/1024/1024) : 0,
	    v_pts_corr,
	    (v_muxer_time>1) ? (int)(mux_v->size/v_muxer_time/125) : 0,
	    (mux_a && a_muxer_time>1) ? (int)(mux_a->size/a_muxer_time/125) : 0
	);
      }
    }
        fflush(stdout);

#ifdef CONFIG_DVDREAD
// DVD sub:
 if(vobsub_writer){
     unsigned char* packet=NULL;
     int len;
     while((len=ds_get_packet_sub(d_dvdsub,&packet, NULL, NULL))>0){
	 mp_msg(MSGT_MENCODER,MSGL_V,"\rDVD sub: len=%d  v_pts=%5.3f  s_pts=%5.3f  \n",len,sh_video->pts,d_dvdsub->pts);
	     vobsub_out_output(vobsub_writer,packet,len,v_muxer_time + d_dvdsub->pts - sh_video->pts);
     }
 }
 else
#endif
    update_subtitles(sh_video, sh_video->pts, d_dvdsub, 0);

 frame_data = (s_frame_data){ .start = NULL, .in_size = 0, .frame_time = 0., .already_read = 0 };

#if 0
 if(ferror(muxer_f)) {
     mp_msg(MSGT_MENCODER,MSGL_FATAL,MSGTR_ErrorWritingFile, out_filename);
     mencoder_exit(1, NULL);
 }
#endif

} // while(!at_eof)

if (!interrupted && filelist[++curfile].name != 0) {
	if (sh_video && sh_video->vfilter) { // Before uniniting sh_video and the filter chain, break apart the VE.
 		vf_instance_t * ve; // this will be the filter right before the ve.
		for (ve = sh_video->vfilter; ve->next && ve->next->next; ve = ve->next);

		if (ve->next) ve->next = NULL; // I'm telling the last filter, before the VE, there is nothing after it
		else sh_video->vfilter = NULL; // There is no chain except the VE.
	}

	if(sh_audio){ uninit_audio(sh_audio);sh_audio=NULL; }
	if(sh_video){ uninit_video(sh_video);sh_video=NULL; }
	if(demuxer) free_demuxer(demuxer);
	if(stream) free_stream(stream); // kill cache thread

	at_eof = 0;

	m_config_pop(mconfig);
	goto play_next_file;
}

/* Emit the remaining frames in the video system */
/*TODO emit frmaes delayed by decoder lag*/
if(sh_video && sh_video->vfilter){
	mp_msg(MSGT_MENCODER, MSGL_INFO, MSGTR_FlushingVideoFrames);
	if (!((vf_instance_t *)sh_video->vfilter)->fmt.have_configured)
		mp_msg(MSGT_MENCODER, MSGL_WARN, MSGTR_FiltersHaveNotBeenConfiguredEmptyFile);
	else
		((vf_instance_t *)sh_video->vfilter)->control(sh_video->vfilter,
    	                                              VFCTRL_FLUSH_FRAMES, 0);
}

if(aencoder)
    if(aencoder->fixup)
        aencoder->fixup(aencoder);

muxer_f_size=muxer_close(muxer);
#if 0
if(ferror(muxer_f) || fclose(muxer_f) != 0) {
    mp_msg(MSGT_MENCODER,MSGL_FATAL,MSGTR_ErrorWritingFile, out_filename);
    mencoder_exit(1, NULL);
}
#endif
if(vobsub_writer)
    vobsub_out_close(vobsub_writer);

if(out_video_codec==VCODEC_FRAMENO && mux_v->timer>100){
    mp_msg(MSGT_MENCODER, MSGL_INFO, MSGTR_RecommendedVideoBitrate,"650MB",(int)((650*1024*1024-muxer_f_size)/mux_v->timer/125));
    mp_msg(MSGT_MENCODER, MSGL_INFO, MSGTR_RecommendedVideoBitrate,"700MB",(int)((700*1024*1024-muxer_f_size)/mux_v->timer/125));
    mp_msg(MSGT_MENCODER, MSGL_INFO, MSGTR_RecommendedVideoBitrate,"800MB",(int)((800*1024*1024-muxer_f_size)/mux_v->timer/125));
    mp_msg(MSGT_MENCODER, MSGL_INFO, MSGTR_RecommendedVideoBitrate,"2 x 650MB",(int)((2*650*1024*1024-muxer_f_size)/mux_v->timer/125));
    mp_msg(MSGT_MENCODER, MSGL_INFO, MSGTR_RecommendedVideoBitrate,"2 x 700MB",(int)((2*700*1024*1024-muxer_f_size)/mux_v->timer/125));
    mp_msg(MSGT_MENCODER, MSGL_INFO, MSGTR_RecommendedVideoBitrate,"2 x 800MB",(int)((2*800*1024*1024-muxer_f_size)/mux_v->timer/125));
}

mp_msg(MSGT_MENCODER, MSGL_INFO, MSGTR_VideoStreamResult,
    (float)(mux_v->size/mux_v->timer*8.0f/1000.0f), (int)(mux_v->size/mux_v->timer), (uint64_t)mux_v->size, (float)mux_v->timer, decoded_frameno);
if(sh_audio)
mp_msg(MSGT_MENCODER, MSGL_INFO, MSGTR_AudioStreamResult,
    (float)(mux_a->size/mux_a->timer*8.0f/1000.0f), (int)(mux_a->size/mux_a->timer), (uint64_t)mux_a->size, (float)mux_a->timer);

muxer_free(muxer);
muxer=NULL;
mux_v=mux_a=NULL;
m_config_free(mconfig);
m_entry_list_free(filelist);
if(sh_audio){ uninit_audio(sh_audio);sh_audio=NULL; }
if(sh_video){ uninit_video(sh_video);sh_video=NULL; }
if(demuxer) free_demuxer(demuxer);
if(stream) free_stream(stream); // kill cache thread
if(ostream) free_stream(ostream);
// Do not free mux_v->buffer, it may have been overwritten
// with something we should not free.
if(mux_v) free(mux_v_buffer);
if(mux_a) free(mux_a->buffer);
common_uninit();
mp_msg_uninit();

return interrupted;
}
