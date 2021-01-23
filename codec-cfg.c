/*
 * codec.conf parser
 *
 * to compile test application:
 *  cc -I. -DTESTING -o codec-cfg-test codec-cfg.c mp_msg.o osdep/getch2.o -ltermcap
 * to compile CODECS2HTML:
 *   gcc -DCODECS2HTML -o codecs2html codec-cfg.c
 *
 * TODO: implement informat in CODECS2HTML too
 *
 * Copyright (C) 2001 Szabolcs Berecz <szabi@inf.elte.hu>
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

#define DEBUG

//disable asserts
#define NDEBUG

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>

#include "config.h"
#include "mp_msg.h"
#if defined(CODECS2HTML) || defined(TESTING)
#ifdef __GNUC__
#define mp_msg(t, l, m, args...) fprintf(stderr, m, ##args)
#else
#define mp_msg(t, l, ...) fprintf(stderr, __VA_ARGS__)
#endif
#include <strings.h>
#define av_strcasecmp(a, b) strcasecmp(a, b)
#else
#include <libavutil/avstring.h>
#endif

#include "help_mp.h"

#include "libavutil/avutil.h"
#include "libavutil/common.h"
#include "libmpcodecs/img_format.h"
#include "codec-cfg.h"

#ifdef CODECS2HTML
#define CODEC_CFG_MIN 20100000
#else
#include "codecs.conf.h"
#endif

#define PRINT_LINENUM mp_msg(MSGT_CODECCFG,MSGL_ERR," at line %d\n", line_num)

#define MAX_NR_TOKEN    16

#define MAX_LINE_LEN    1000

#define RET_EOF         -1
#define RET_EOL         -2

#define TYPE_VIDEO      0
#define TYPE_AUDIO      1

static int codecs_conf_release;
char * codecs_file = NULL;

static int add_to_fourcc(char *s, char *alias, unsigned int *fourcc,
                         unsigned int *map)
{
    int i, j, freeslots;
    unsigned int tmp;

    /* find first unused slot */
    for (i = 0; i < CODECS_MAX_FOURCC && fourcc[i] != 0xffffffff; i++)
        /* NOTHING */;
    freeslots = CODECS_MAX_FOURCC - i;
    if (!freeslots)
        goto err_out_too_many;

    do {
        tmp = MKTAG(s[0], s[1], s[2], s[3]);
        for (j = 0; j < i; j++)
            if (tmp == fourcc[j])
                goto err_out_duplicated;
        fourcc[i] = tmp;
        map[i] = alias ? MKTAG(alias[0], alias[1], alias[2], alias[3]) : tmp;
        s += 4;
        i++;
    } while ((*(s++) == ',') && --freeslots);

    if (!freeslots)
        goto err_out_too_many;
    if (*(--s) != '\0')
        goto err_out_parse_error;
    return 1;
err_out_duplicated:
    mp_msg(MSGT_CODECCFG,MSGL_ERR,MSGTR_DuplicateFourcc);
    return 0;
err_out_too_many:
    mp_msg(MSGT_CODECCFG,MSGL_ERR,MSGTR_TooManyFourccs);
    return 0;
err_out_parse_error:
    mp_msg(MSGT_CODECCFG,MSGL_ERR,MSGTR_ParseError);
    return 0;
}

static int add_to_format(char *s, char *alias,unsigned int *fourcc, unsigned int *fourccmap)
{
    int i, j;
    char *endptr;

    /* find first unused slot */
    for (i = 0; i < CODECS_MAX_FOURCC && fourcc[i] != 0xffffffff; i++)
        /* NOTHING */;
    if (i == CODECS_MAX_FOURCC) {
        mp_msg(MSGT_CODECCFG,MSGL_ERR,MSGTR_TooManyFourccs);
        return 0;
    }

    fourcc[i]=strtoul(s,&endptr,0);
    if (*endptr != '\0') {
        mp_msg(MSGT_CODECCFG,MSGL_ERR,MSGTR_ParseErrorFIDNotNumber);
        return 0;
    }

    if(alias){
        fourccmap[i]=strtoul(alias,&endptr,0);
        if (*endptr != '\0') {
            mp_msg(MSGT_CODECCFG,MSGL_ERR,MSGTR_ParseErrorFIDAliasNotNumber);
            return 0;
        }
    } else
        fourccmap[i]=fourcc[i];

    for (j = 0; j < i; j++)
        if (fourcc[j] == fourcc[i]) {
            mp_msg(MSGT_CODECCFG,MSGL_ERR,MSGTR_DuplicateFID);
            return 0;
        }

    return 1;
}

static const struct {
    const char *name;
    const unsigned int num;
} fmt_table[] = {
    // note: due to parser deficiencies/simplicity, if one format
    // name matches the beginning of another, the longer one _must_
    // come first in this list.
    {"YV12",        IMGFMT_YV12},
    {"I420",        IMGFMT_I420},
    {"IYUV",        IMGFMT_IYUV},
    {"NV12",        IMGFMT_NV12},
    {"NV21",        IMGFMT_NV21},
    {"YVU9",        IMGFMT_YVU9},
    {"IF09",        IMGFMT_IF09},
    {"444P16LE",    IMGFMT_444P16_LE},
    {"444P16BE",    IMGFMT_444P16_BE},
    {"444P10LE",    IMGFMT_444P10_LE},
    {"444P10BE",    IMGFMT_444P10_BE},
    {"422P16LE",    IMGFMT_422P16_LE},
    {"422P16BE",    IMGFMT_422P16_BE},
    {"420P16LE",    IMGFMT_420P16_LE},
    {"420P16BE",    IMGFMT_420P16_BE},
    {"444P16",      IMGFMT_444P16},
    {"444P14",      IMGFMT_444P14},
    {"444P12",      IMGFMT_444P12},
    {"444P10",      IMGFMT_444P10},
    {"444P9",       IMGFMT_444P9},
    {"422P16",      IMGFMT_422P16},
    {"422P14",      IMGFMT_422P14},
    {"422P12",      IMGFMT_422P12},
    {"422P10",      IMGFMT_422P10},
    {"422P9",       IMGFMT_422P9},
    {"440P12",      IMGFMT_440P12},
    {"440P10",      IMGFMT_440P10},
    {"420P16",      IMGFMT_420P16},
    {"420P14",      IMGFMT_420P14},
    {"420P12",      IMGFMT_420P12},
    {"420P10",      IMGFMT_420P10},
    {"420P9",       IMGFMT_420P9},
    {"420A",        IMGFMT_420A},
    {"444P",        IMGFMT_444P},
    {"444A",        IMGFMT_444A},
    {"422P",        IMGFMT_422P},
    {"422A",        IMGFMT_422A},
    {"411P",        IMGFMT_411P},
    {"440P",        IMGFMT_440P},
    {"Y800",        IMGFMT_Y800},
    {"Y8A",         IMGFMT_Y8A},
    {"Y8",          IMGFMT_Y8},
    {"Y16LE",       IMGFMT_Y16_LE},
    {"Y16BE",       IMGFMT_Y16_BE},
    {"Y16",         IMGFMT_Y16},

    {"YUY2",        IMGFMT_YUY2},
    {"UYVY",        IMGFMT_UYVY},
    {"YVYU",        IMGFMT_YVYU},

    {"RGB64LE",     IMGFMT_RGB64LE},
    {"RGB64BE",     IMGFMT_RGB64BE},
    {"RGB48LE",     IMGFMT_RGB48LE},
    {"RGB48BE",     IMGFMT_RGB48BE},
    {"BGR48LE",     IMGFMT_BGR48LE},
    {"BGR48BE",     IMGFMT_BGR48BE},
    {"RGB4",        IMGFMT_RGB4},
    {"RGB8",        IMGFMT_RGB8},
    {"RGB15",       IMGFMT_RGB15},
    {"RGB16",       IMGFMT_RGB16},
    {"RGB24",       IMGFMT_RGB24},
    {"RGB32",       IMGFMT_RGB32},
    {"RGBA",        IMGFMT_RGBA},
    {"ARGB",        IMGFMT_ARGB},
    {"BGR4",        IMGFMT_BGR4},
    {"BGR8",        IMGFMT_BGR8},
    {"BGR15LE",     IMGFMT_BGR15LE},
    {"BGR12",       IMGFMT_BGR12},
    {"BGR15",       IMGFMT_BGR15},
    {"BGR16",       IMGFMT_BGR16},
    {"BGR24",       IMGFMT_BGR24},
    {"BGR32",       IMGFMT_BGR32},
    {"BGRA",        IMGFMT_BGRA},
    {"ABGR",        IMGFMT_ABGR},
    {"RGB1",        IMGFMT_RGB1},
    {"BGR1",        IMGFMT_BGR1},
    {"XYZ12",       IMGFMT_XYZ12},
    {"GBR24P",      IMGFMT_GBR24P},
    {"GBR12P",      IMGFMT_GBR12P},
    {"GBR14P",      IMGFMT_GBR14P},
    {"GBR10P",      IMGFMT_GBR10P},

    {"MPES",        IMGFMT_MPEGPES},
    {"ZRMJPEGNI",   IMGFMT_ZRMJPEGNI},
    {"ZRMJPEGIT",   IMGFMT_ZRMJPEGIT},
    {"ZRMJPEGIB",   IMGFMT_ZRMJPEGIB},

    {"IDCT_MPEG2",  IMGFMT_XVMC_IDCT_MPEG2},
    {"MOCO_MPEG2",  IMGFMT_XVMC_MOCO_MPEG2},

    {"VDPAU_MPEG1", IMGFMT_VDPAU_MPEG1},
    {"VDPAU_MPEG2", IMGFMT_VDPAU_MPEG2},
    {"VDPAU_H264",  IMGFMT_VDPAU_H264},
    {"VDPAU_WMV3",  IMGFMT_VDPAU_WMV3},
    {"VDPAU_VC1",   IMGFMT_VDPAU_VC1},
    {"VDPAU_MPEG4", IMGFMT_VDPAU_MPEG4},
    {"VDPAU_HEVC",  IMGFMT_VDPAU_HEVC},

    {NULL,    0}
};


static int add_to_inout(char *sfmt, char *sflags, unsigned int *outfmt,
                        unsigned char *outflags)
{

    static const char * const flagstr[] = {
        "flip",
        "noflip",
        "yuvhack",
        "query",
        "static",
        NULL
    };

    int i, j, freeslots;
    unsigned char flags;

    for (i = 0; i < CODECS_MAX_OUTFMT && outfmt[i] != 0xffffffff; i++)
        /* NOTHING */;
    freeslots = CODECS_MAX_OUTFMT - i;
    if (!freeslots)
        goto err_out_too_many;

    flags = 0;
    if(sflags) {
        do {
            for (j = 0; flagstr[j] != NULL; j++)
                if (!strncmp(sflags, flagstr[j],
                             strlen(flagstr[j])))
                    break;
            if (flagstr[j] == NULL)
                goto err_out_parse_error;
            flags|=(1<<j);
            sflags+=strlen(flagstr[j]);
        } while (*(sflags++) == ',');

        if (*(--sflags) != '\0')
            goto err_out_parse_error;
    }

    do {
        for (j = 0; fmt_table[j].name != NULL; j++)
            if (!strncmp(sfmt, fmt_table[j].name, strlen(fmt_table[j].name)))
                break;
        if (fmt_table[j].name == NULL)
            goto err_out_parse_error;
        outfmt[i] = fmt_table[j].num;
        outflags[i] = flags;
        ++i;
        sfmt+=strlen(fmt_table[j].name);
    } while ((*(sfmt++) == ',') && --freeslots);

    if (!freeslots)
        goto err_out_too_many;

    if (*(--sfmt) != '\0')
        goto err_out_parse_error;

    return 1;
err_out_too_many:
    mp_msg(MSGT_CODECCFG,MSGL_ERR,MSGTR_TooManyOut);
    return 0;
err_out_parse_error:
    mp_msg(MSGT_CODECCFG,MSGL_ERR,MSGTR_ParseError);
    return 0;
}

#if 0
static short get_driver(char *s,int audioflag)
{
    static char *audiodrv[] = {
        "null",
        "mp3lib",
        "pcm",
        "libac3",
        "acm",
        "alaw",
        "msgsm",
        "dshow",
        "dvdpcm",
        "hwac3",
        "libvorbis",
        "ffmpeg",
        "libmad",
        "msadpcm",
        "liba52",
        "g72x",
        "imaadpcm",
        "dk4adpcm",
        "dk3adpcm",
        "roqaudio",
        "faad",
        "realaud",
        "libdv",
        NULL
    };
    static char *videodrv[] = {
        "null",
        "libmpeg2",
        "vfw",
        "dshow",
        "ffmpeg",
        "vfwex",
        "raw",
        "msrle",
        "xanim",
        "msvidc",
        "fli",
        "cinepak",
        "qtrle",
        "nuv",
        "cyuv",
        "qtsmc",
        "ducktm1",
        "roqvideo",
        "qtrpza",
        "mpng",
        "ijpg",
        "zlib",
        "mpegpes",
        "zrmjpeg",
        "realvid",
        "xvid",
        "libdv",
        NULL
    };
    char **drv=audioflag?audiodrv:videodrv;
    int i;

    for(i=0;drv[i];i++) if(!strcmp(s,drv[i])) return i;

    return -1;
}
#endif

static int validate_codec(codecs_t *c, int type)
{
    unsigned int i;
    const char *name = codec_idx2str(c->name_idx);
    const char *tmp_name = name;

    for (i = 0; i < strlen(tmp_name) && isalnum(tmp_name[i]); i++)
        /* NOTHING */;

    if (i < strlen(tmp_name)) {
        mp_msg(MSGT_CODECCFG,MSGL_ERR,MSGTR_InvalidCodecName, name);
        return 0;
    }

    if (!c->info_idx)
        c->info_idx = c->name_idx;

#if 0
    if (c->fourcc[0] == 0xffffffff) {
        mp_msg(MSGT_CODECCFG,MSGL_ERR,MSGTR_CodecLacksFourcc, name);
        return 0;
    }
#endif

    if (!c->drv_idx) {
        mp_msg(MSGT_CODECCFG,MSGL_ERR,MSGTR_CodecLacksDriver, name);
        return 0;
    }

#if 0
//FIXME: codec->driver == 4;... <- this should not be put in here...
//FIXME: Where are they defined ????????????
    if (!c->dll && (c->driver == 4 ||
                (c->driver == 2 && type == TYPE_VIDEO))) {
        mp_msg(MSGT_CODECCFG,MSGL_ERR,MSGTR_CodecNeedsDLL, name);
        return 0;
    }
// FIXME: Can guid.f1 be 0? How does one know that it was not given?
//      if (!(codec->flags & CODECS_FLAG_AUDIO) && codec->driver == 4)

    if (type == TYPE_VIDEO)
        if (c->outfmt[0] == 0xffffffff) {
            mp_msg(MSGT_CODECCFG,MSGL_ERR,MSGTR_CodecNeedsOutfmt, name);
            return 0;
        }
#endif
    return 1;
}

static int add_comment(char *s, char **d)
{
    int pos;

    if (!*d)
        pos = 0;
    else {
        pos = strlen(*d);
        (*d)[pos++] = '\n';
    }
    if (!(*d = realloc(*d, pos + strlen(s) + 1))) {
        mp_msg(MSGT_CODECCFG,MSGL_FATAL,MSGTR_CantAllocateComment);
        return 0;
    }
    strcpy(*d + pos, s);
    return 1;
}

static short get_cpuflags(char *s)
{
    static const char * const flagstr[] = {
        "mmx",
        "sse",
        "3dnow",
        NULL
    };
    int i;
    short flags = 0;

    do {
        for (i = 0; flagstr[i]; i++)
            if (!strncmp(s, flagstr[i], strlen(flagstr[i])))
                break;
        if (!flagstr[i])
            goto err_out_parse_error;
        flags |= 1<<i;
        s += strlen(flagstr[i]);
    } while (*(s++) == ',');

    if (*(--s) != '\0')
        goto err_out_parse_error;

    return flags;
err_out_parse_error:
    return 0;
}

static FILE *fp;
static int line_num = 0;
static char *line;
static char *token[MAX_NR_TOKEN];
static int read_nextline = 1;

static int get_token(int min, int max)
{
    static int line_pos;
    int i;
    char c;

    if (max >= MAX_NR_TOKEN) {
        mp_msg(MSGT_CODECCFG,MSGL_ERR,MSGTR_GetTokenMaxNotLessThanMAX_NR_TOKEN);
        goto out_eof;
    }

    memset(token, 0x00, sizeof(*token) * max);

    if (read_nextline) {
        if (!fgets(line, MAX_LINE_LEN, fp))
            goto out_eof;
        line_pos = 0;
        ++line_num;
        read_nextline = 0;
    }
    for (i = 0; i < max; i++) {
        while (isspace(line[line_pos]))
            ++line_pos;
        if (line[line_pos] == '\0' || line[line_pos] == '#' ||
            line[line_pos] == ';') {
            read_nextline = 1;
            if (i >= min)
                goto out_ok;
            goto out_eol;
        }
        token[i] = line + line_pos;
        c = line[line_pos];
        if (c == '"' || c == '\'') {
            token[i]++;
            while (line[++line_pos] != c && line[line_pos])
                /* NOTHING */;
        } else {
            for (/* NOTHING */; !isspace(line[line_pos]) &&
                                  line[line_pos]; line_pos++)
                /* NOTHING */;
        }
        if (!line[line_pos]) {
            read_nextline = 1;
            if (i >= min - 1)
                goto out_ok;
            goto out_eol;
        }
        line[line_pos] = '\0';
        line_pos++;
    }
out_ok:
    return i;
out_eof:
    read_nextline = 1;
    return RET_EOF;
out_eol:
    return RET_EOL;
}

static codecs_t *video_codecs=NULL;
static codecs_t *audio_codecs=NULL;
static char *codec_strs = NULL;
static unsigned codec_strs_len = 0;
static int nr_vcodecs = 0;
static int nr_acodecs = 0;

const char *codec_idx2str(unsigned idx)
{
    if (idx >= codec_strs_len) return NULL;
    if (idx > 0 && codec_strs[idx - 1]) return NULL;
    return codec_strs + idx;
}

static unsigned codec_addstr(const char *s)
{
#ifdef CODECS2HTML
    int i;
#endif
    int len;
    char *newstr;
    if (!s || !s[0]) return 0;
    len = strlen(s) + 1;
#ifdef CODECS2HTML
    // try to de-duplicate
    for (i = 1; i < codec_strs_len; ) {
        int curlen = strlen(codec_strs + i) + 1;
        if (len == curlen && !strcmp(s, codec_strs + i))
            return i;
        i += curlen;
    }
#endif
    if (codec_strs_len) {
        newstr = realloc(codec_strs, codec_strs_len + len);
    } else {
        codec_strs_len = 1;
        newstr = calloc(1, 1 + len);
    }
    if (!newstr) return 0;
    codec_strs = newstr;
    memcpy(codec_strs + codec_strs_len, s, len);
    codec_strs_len += len;
    return codec_strs_len - len;
}

int parse_codec_cfg(const char *cfgfile)
{
    codecs_t *codec = NULL; // current codec
    codecs_t **codecsp = NULL;// points to audio_codecs or to video_codecs
    char *endptr;   // strtoul()...
    char *comment = NULL;
    int *nr_codecsp;
    int codec_type;     /* TYPE_VIDEO/TYPE_AUDIO */
    int tmp, i;

    // in case we call it a second time
    codecs_uninit_free();

    nr_vcodecs = 0;
    nr_acodecs = 0;

    if(cfgfile==NULL) {
#ifdef CODECS2HTML
        return 0;
#else
        video_codecs = builtin_video_codecs;
        audio_codecs = builtin_audio_codecs;
        nr_vcodecs = sizeof(builtin_video_codecs)/sizeof(codecs_t);
        nr_acodecs = sizeof(builtin_audio_codecs)/sizeof(codecs_t);
        codec_strs = builtin_codec_strs;
        codec_strs_len = sizeof(builtin_codec_strs);
        return 1;
#endif
    }

    mp_msg(MSGT_CODECCFG, MSGL_V, "Reading optional codecs config file %s: ", cfgfile);

    if ((fp = fopen(cfgfile, "r")) == NULL) {
        mp_msg(MSGT_CODECCFG, MSGL_V, "%s\n", strerror(errno));
        return 0;
    }

    if ((line = malloc(MAX_LINE_LEN + 1)) == NULL) {
        mp_msg(MSGT_CODECCFG,MSGL_FATAL,MSGTR_CantGetMemoryForLine, strerror(errno));
        return 0;
    }
    read_nextline = 1;

    /*
     * this only catches release lines at the start of
     * codecs.conf, before audiocodecs and videocodecs.
     */
    while ((tmp = get_token(1, 1)) == RET_EOL)
        /* NOTHING */;
    if (tmp == RET_EOF)
        goto out;
    if (!strcmp(token[0], "release")) {
        if (get_token(1, 2) < 0)
            goto err_out_parse_error;
        tmp = atoi(token[0]);
        if (tmp < CODEC_CFG_MIN)
            goto err_out_release_num;
        codecs_conf_release = tmp;
        while ((tmp = get_token(1, 1)) == RET_EOL)
            /* NOTHING */;
        if (tmp == RET_EOF)
            goto out;
    } else
        goto err_out_release_num;

    /*
     * check if the next block starts with 'audiocodec' or
     * with 'videocodec'
     */
    if (!strcmp(token[0], "audiocodec") || !strcmp(token[0], "videocodec"))
        goto loop_enter;
    goto err_out_parse_error;

    while ((tmp = get_token(1, 1)) != RET_EOF) {
        if (tmp == RET_EOL)
            continue;
        if (!strcmp(token[0], "audiocodec") ||
            !strcmp(token[0], "videocodec")) {
            codec->comment_idx = codec_addstr(comment);
            free(comment);
            comment = NULL;
            if (!validate_codec(codec, codec_type))
                goto err_out_not_valid;
        loop_enter:
            if (*token[0] == 'v') {
                codec_type = TYPE_VIDEO;
                nr_codecsp = &nr_vcodecs;
                codecsp = &video_codecs;
            } else if (*token[0] == 'a') {
                codec_type = TYPE_AUDIO;
                nr_codecsp = &nr_acodecs;
                codecsp = &audio_codecs;
#ifdef DEBUG
            } else {
                mp_msg(MSGT_CODECCFG,MSGL_ERR,"picsba\n");
                goto err_out;
#endif
            }
            if (!(*codecsp = realloc(*codecsp,
                                     sizeof(codecs_t) * (*nr_codecsp + 2)))) {
                mp_msg(MSGT_CODECCFG,MSGL_FATAL,MSGTR_CantReallocCodecsp, strerror(errno));
                goto err_out;
            }
            codec=*codecsp + *nr_codecsp;
            ++*nr_codecsp;
            memset(codec,0,sizeof(codecs_t));
            memset(codec->fourcc, 0xff, sizeof(codec->fourcc));
            memset(codec->outfmt, 0xff, sizeof(codec->outfmt));
            memset(codec->infmt, 0xff, sizeof(codec->infmt));

            if (get_token(1, 1) < 0)
                goto err_out_parse_error;
            for (i = 0; i < *nr_codecsp - 1; i++) {
                if(( (*codecsp)[i].name_idx) &&
                   (!strcmp(token[0], codec_idx2str((*codecsp)[i].name_idx))) ) {
                    mp_msg(MSGT_CODECCFG,MSGL_ERR,MSGTR_CodecNameNotUnique, token[0]);
                    goto err_out_print_linenum;
                }
            }
            if (!(codec->name_idx = codec_addstr(token[0]))) {
                mp_msg(MSGT_CODECCFG,MSGL_ERR,MSGTR_CantStrdupName, strerror(errno));
                goto err_out;
            }
        } else if (!strcmp(token[0], "info")) {
            if (codec->info_idx || get_token(1, 1) < 0)
                goto err_out_parse_error;
            if (!(codec->info_idx = codec_addstr(token[0]))) {
                mp_msg(MSGT_CODECCFG,MSGL_ERR,MSGTR_CantStrdupInfo, strerror(errno));
                goto err_out;
            }
        } else if (!strcmp(token[0], "comment")) {
            if (get_token(1, 1) < 0)
                goto err_out_parse_error;
            add_comment(token[0], &comment);
        } else if (!strcmp(token[0], "fourcc")) {
            if (get_token(1, 2) < 0)
                goto err_out_parse_error;
            if (!add_to_fourcc(token[0], token[1],
                               codec->fourcc,
                               codec->fourccmap))
                goto err_out_print_linenum;
        } else if (!strcmp(token[0], "format")) {
            if (get_token(1, 2) < 0)
                goto err_out_parse_error;
            if (!add_to_format(token[0], token[1],
                               codec->fourcc,codec->fourccmap))
                goto err_out_print_linenum;
        } else if (!strcmp(token[0], "driver")) {
            if (get_token(1, 1) < 0)
                goto err_out_parse_error;
            if (!(codec->drv_idx = codec_addstr(token[0]))) {
                mp_msg(MSGT_CODECCFG,MSGL_ERR,MSGTR_CantStrdupDriver, strerror(errno));
                goto err_out;
            }
        } else if (!strcmp(token[0], "dll")) {
            if (get_token(1, 1) < 0)
                goto err_out_parse_error;
            if (!(codec->dll_idx = codec_addstr(token[0]))) {
                mp_msg(MSGT_CODECCFG,MSGL_ERR,MSGTR_CantStrdupDLL, strerror(errno));
                goto err_out;
            }
        } else if (!strcmp(token[0], "guid")) {
            if (get_token(11, 11) < 0)
                goto err_out_parse_error;
            codec->guid.f1=strtoul(token[0],&endptr,0);
            if ((*endptr != ',' || *(endptr + 1) != '\0') &&
                *endptr != '\0')
                goto err_out_parse_error;
            codec->guid.f2=strtoul(token[1],&endptr,0);
            if ((*endptr != ',' || *(endptr + 1) != '\0') &&
                *endptr != '\0')
                goto err_out_parse_error;
            codec->guid.f3=strtoul(token[2],&endptr,0);
            if ((*endptr != ',' || *(endptr + 1) != '\0') &&
                *endptr != '\0')
                goto err_out_parse_error;
            for (i = 0; i < 8; i++) {
                codec->guid.f4[i]=strtoul(token[i + 3],&endptr,0);
                if ((*endptr != ',' || *(endptr + 1) != '\0') &&
                    *endptr != '\0')
                    goto err_out_parse_error;
            }
        } else if (!strcmp(token[0], "out")) {
            if (get_token(1, 2) < 0)
                goto err_out_parse_error;
            if (!add_to_inout(token[0], token[1], codec->outfmt,
                              codec->outflags))
                goto err_out_print_linenum;
        } else if (!strcmp(token[0], "in")) {
            if (get_token(1, 2) < 0)
                goto err_out_parse_error;
            if (!add_to_inout(token[0], token[1], codec->infmt,
                              codec->inflags))
                goto err_out_print_linenum;
        } else if (!strcmp(token[0], "flags")) {
            if (get_token(1, 1) < 0)
                goto err_out_parse_error;
            if (!strcmp(token[0], "seekable"))
                codec->flags |= CODECS_FLAG_SEEKABLE;
            else
            if (!strcmp(token[0], "align16"))
                codec->flags |= CODECS_FLAG_ALIGN16;
            else
            if (!strcmp(token[0], "dummy"))
                codec->flags |= CODECS_FLAG_DUMMY;
            else
                goto err_out_parse_error;
        } else if (!strcmp(token[0], "status")) {
            if (get_token(1, 1) < 0)
                goto err_out_parse_error;
            if (!av_strcasecmp(token[0], "working"))
                codec->status = CODECS_STATUS_WORKING;
            else if (!av_strcasecmp(token[0], "crashing"))
                codec->status = CODECS_STATUS_NOT_WORKING;
            else if (!av_strcasecmp(token[0], "untested"))
                codec->status = CODECS_STATUS_UNTESTED;
            else if (!av_strcasecmp(token[0], "buggy"))
                codec->status = CODECS_STATUS_PROBLEMS;
            else
                goto err_out_parse_error;
        } else if (!strcmp(token[0], "cpuflags")) {
            if (get_token(1, 1) < 0)
                goto err_out_parse_error;
            if (!(codec->cpuflags = get_cpuflags(token[0])))
                goto err_out_parse_error;
        } else
            goto err_out_parse_error;
    }
    if (!validate_codec(codec, codec_type))
        goto err_out_not_valid;
    mp_msg(MSGT_CODECCFG,MSGL_INFO,MSGTR_AudioVideoCodecTotals, nr_acodecs, nr_vcodecs);
    if(video_codecs) video_codecs[nr_vcodecs].name_idx = 0;
    if(audio_codecs) audio_codecs[nr_acodecs].name_idx = 0;
out:
    free(line);
    line=NULL;
    fclose(fp);
    return 1;

err_out_parse_error:
    mp_msg(MSGT_CODECCFG,MSGL_ERR,MSGTR_ParseError);
err_out_print_linenum:
    PRINT_LINENUM;
err_out:
    codecs_uninit_free();

    free(line);
    line=NULL;
    line_num = 0;
    fclose(fp);
    return 0;
err_out_not_valid:
    mp_msg(MSGT_CODECCFG,MSGL_ERR,MSGTR_CodecDefinitionIncorrect);
    goto err_out_print_linenum;
err_out_release_num:
    mp_msg(MSGT_CODECCFG,MSGL_ERR,MSGTR_OutdatedCodecsConf);
    goto err_out_print_linenum;
}

void codecs_uninit_free(void) {
    free(video_codecs);
    video_codecs=NULL;
    free(audio_codecs);
    audio_codecs=NULL;
    free(codec_strs);
    codec_strs=NULL;
    codec_strs_len = 0;
}

codecs_t *find_audio_codec(unsigned int fourcc, unsigned int *fourccmap,
                           codecs_t *start, int force)
{
    return find_codec(fourcc, fourccmap, start, 1, force);
}

codecs_t *find_video_codec(unsigned int fourcc, unsigned int *fourccmap,
                           codecs_t *start, int force)
{
    return find_codec(fourcc, fourccmap, start, 0, force);
}

codecs_t* find_codec(unsigned int fourcc,unsigned int *fourccmap,
                     codecs_t *start, int audioflag, int force)
{
    int i, j;
    codecs_t *c;

#if 0
    if (start) {
        for (/* NOTHING */; start->name; start++) {
            for (j = 0; j < CODECS_MAX_FOURCC; j++) {
                if (start->fourcc[j] == fourcc) {
                    if (fourccmap)
                        *fourccmap = start->fourccmap[j];
                    return start;
                }
            }
        }
    } else
#endif
    {
        if (audioflag) {
            i = nr_acodecs;
            c = audio_codecs;
        } else {
            i = nr_vcodecs;
            c = video_codecs;
        }
        if(!i) return NULL;
        for (/* NOTHING */; i--; c++) {
            if(start && c<=start) continue;
            for (j = 0; j < CODECS_MAX_FOURCC; j++) {
                if (c->fourcc[j]==fourcc || c->flags & CODECS_FLAG_DUMMY) {
                    if (fourccmap)
                        *fourccmap = c->fourccmap[j];
                    return c;
                }
            }
            if (force) return c;
        }
    }
    return NULL;
}

void stringset_init(stringset_t *set) {
    *set = calloc(1, sizeof(char *));
}

void stringset_free(stringset_t *set) {
    int count = 0;
    while ((*set)[count]) free((*set)[count++]);
    free(*set);
    *set = NULL;
}

void stringset_add(stringset_t *set, const char *str) {
    int count = 0;
    while ((*set)[count]) count++;
    count++;
    *set = realloc(*set, sizeof(char *) * (count + 1));
    (*set)[count - 1] = strdup(str);
    (*set)[count] = NULL;
}

int stringset_test(stringset_t *set, const char *str) {
    stringset_t s;
    for (s = *set; *s; s++)
        if (strcmp(*s, str) == 0)
            return 1;
    return 0;
}

void list_codecs(int audioflag){
    int i;
    codecs_t *c;

    if (audioflag) {
        i = nr_acodecs;
        c = audio_codecs;
        mp_msg(MSGT_CODECCFG,MSGL_INFO,"ac:     afm:      status:   info:  [lib/dll]\n");
    } else {
        i = nr_vcodecs;
        c = video_codecs;
        mp_msg(MSGT_CODECCFG,MSGL_INFO,"vc:     vfm:      status:   info:  [lib/dll]\n");
    }
    if(!i) return;
    for (/* NOTHING */; i--; c++) {
        char* s="unknown ";
        switch(c->status){
        case CODECS_STATUS_WORKING:     s="working ";break;
        case CODECS_STATUS_PROBLEMS:    s="problems";break;
        case CODECS_STATUS_NOT_WORKING: s="crashing";break;
        case CODECS_STATUS_UNTESTED:    s="untested";break;
        }
        if(c->dll_idx)
            mp_msg(MSGT_CODECCFG,MSGL_INFO,"%-11s %-9s %s  %s  [%s]\n",
                   codec_idx2str(c->name_idx),
                   codec_idx2str(c->drv_idx),s,codec_idx2str(c->info_idx),
                   codec_idx2str(c->dll_idx));
        else
            mp_msg(MSGT_CODECCFG,MSGL_INFO,"%-11s %-9s %s  %s\n",
                   codec_idx2str(c->name_idx),
                   codec_idx2str(c->drv_idx),s,codec_idx2str(c->info_idx));
        }
}


#ifdef CODECS2HTML
static void wrapline(FILE *f2,const char *s){
    int c;
    if(!s){
        fprintf(f2,"-");
        return;
    }
    while((c=*s++)){
        if(c==',') fprintf(f2,"<br>"); else fputc(c,f2);
    }
}

static void parsehtml(FILE *f1,FILE *f2,codecs_t *codec){
    int c,d;
    while((c=fgetc(f1))>=0){
        if(c!='%'){
            fputc(c,f2);
            continue;
        }
        d=fgetc(f1);

        switch(d){
        case '.':
        return; // end of section
        case 'n':
            wrapline(f2,codec_idx2str(codec->name_idx)); break;
        case 'i':
            wrapline(f2,codec_idx2str(codec->info_idx)); break;
        case 'c':
            wrapline(f2,codec_idx2str(codec->comment_idx)); break;
        case 'd':
            wrapline(f2,codec_idx2str(codec->dll_idx)); break;
        case 'D':
            fprintf(f2,"%c",!strcmp(codec_idx2str(codec->drv_idx),"dshow")?'+':'-'); break;
        case 'F':
            for(d=0;d<CODECS_MAX_FOURCC;d++)
                if(!d || codec->fourcc[d]!=0xFFFFFFFF)
                    fprintf(f2,"%s%.4s",d?"<br>":"",(codec->fourcc[d]==0xFFFFFFFF || codec->fourcc[d]<0x20202020)?!d?"-":"":(char*) &codec->fourcc[d]);
            break;
        case 'f':
            for(d=0;d<CODECS_MAX_FOURCC;d++)
                if(codec->fourcc[d]!=0xFFFFFFFF)
                    fprintf(f2,"%s0x%X",d?"<br>":"",codec->fourcc[d]);
            break;
        case 'Y':
            for(d=0;d<CODECS_MAX_OUTFMT;d++)
                if(codec->outfmt[d]!=0xFFFFFFFF){
                    for (c=0; fmt_table[c].name; c++)
                        if(fmt_table[c].num==codec->outfmt[d]) break;
                    if(fmt_table[c].name)
                        fprintf(f2,"%s%s",d?"<br>":"",fmt_table[c].name);
                }
            break;
        default:
            fputc(c,f2);
            fputc(d,f2);
        }
    }
}

void skiphtml(FILE *f1){
    int c,d;
    while((c=fgetc(f1))>=0){
        if(c!='%'){
            continue;
        }
        d=fgetc(f1);
        if(d=='.') return; // end of section
    }
}

static void print_int_array(const unsigned int* a, int size)
{
    printf("{ ");
    while (size--)
        if(abs(*a)<256)
            printf("%d%s", *a++, size?", ":"");
        else
            printf("0x%X%s", *a++, size?", ":"");
    printf(" }");
}

static void print_char_array(const unsigned char* a, int size)
{
    printf("{ ");
    while (size--)
        if((*a)<10)
            printf("%d%s", *a++, size?", ":"");
        else
            printf("0x%02x%s", *a++, size?", ":"");
    printf(" }");
}

int main(int argc, char* argv[])
{
    codecs_t *cl;
    FILE *f1;
    FILE *f2;
    int c,d,i;
    int pos;
    int section=-1;
    int nr_codecs;
    int win32=-1;
    int dshow=-1;
    int win32ex=-1;

    /*
     * Take path to codecs.conf from command line, or fall back on
     * etc/codecs.conf
     */
    if (!(nr_codecs = parse_codec_cfg((argc>1)?argv[1]:"etc/codecs.conf")))
        exit(1);
    if (codecs_conf_release < CODEC_CFG_MIN)
        exit(1);

    if (argc > 1) {
        int i, j;
        const char* nm[2];
        codecs_t* cod[2];
        int nr[2];

        nm[0] = "builtin_video_codecs";
        cod[0] = video_codecs;
        nr[0] = nr_vcodecs;

        nm[1] = "builtin_audio_codecs";
        cod[1] = audio_codecs;
        nr[1] = nr_acodecs;

        printf("/* GENERATED FROM %s, DO NOT EDIT! */\n\n",argv[1]);
        printf("#include <stddef.h>\n");
        printf("#include \"codec-cfg.h\"\n\n");
        printf("#define CODEC_CFG_MIN %i\n\n", codecs_conf_release);

        for (i=0; i<2; i++) {
            printf("const codecs_t %s[] = {\n", nm[i]);
            for (j = 0; j < nr[i]; j++) {
                printf("{");

                print_int_array(cod[i][j].fourcc, CODECS_MAX_FOURCC);
                printf(", /* fourcc */\n");

                print_int_array(cod[i][j].fourccmap, CODECS_MAX_FOURCC);
                printf(", /* fourccmap */\n");

                print_int_array(cod[i][j].outfmt, CODECS_MAX_OUTFMT);
                printf(", /* outfmt */\n");

                print_char_array(cod[i][j].outflags, CODECS_MAX_OUTFMT);
                printf(", /* outflags */\n");

                print_int_array(cod[i][j].infmt, CODECS_MAX_INFMT);
                printf(", /* infmt */\n");

                print_char_array(cod[i][j].inflags, CODECS_MAX_INFMT);
                printf(", /* inflags */\n");

                printf("%i, /* name */\n%i, /* info */\n"
                       "%i, /* comment */\n%i, /* dll */\n%i, /* drv */\n",
                       cod[i][j].name_idx, cod[i][j].info_idx,
                       cod[i][j].comment_idx, cod[i][j].dll_idx, cod[i][j].drv_idx);

                printf("{ 0x%08lx, %hu, %hu,",
                       cod[i][j].guid.f1,
                       cod[i][j].guid.f2,
                       cod[i][j].guid.f3);
                print_char_array(cod[i][j].guid.f4, sizeof(cod[i][j].guid.f4));
                printf(" }, /* GUID */\n");
                printf("%hd /* flags */, %hd /* status */, %hd /* cpuflags */ }\n",
                       cod[i][j].flags,
                       cod[i][j].status,
                       cod[i][j].cpuflags);
                if (j < nr[i]) printf(",\n");
            }
            printf("};\n\n");
        }
        printf("const char builtin_codec_strs[] = ");
        print_char_array(codec_strs, codec_strs_len);
        printf(";\n");
        exit(0);
    }

    f1=fopen("DOCS/tech/codecs-in.html","rb"); if(!f1) exit(1);
    f2=fopen("DOCS/codecs-status.html","wb"); if(!f2) exit(1);

    while((c=fgetc(f1))>=0){
        if(c!='%'){
            fputc(c,f2);
            continue;
        }
        d=fgetc(f1);
        if(d>='0' && d<='9'){
            // begin section
            section=d-'0';
            //printf("BEGIN %d\n",section);
            if(section>=5){
                // audio
                cl = audio_codecs;
                nr_codecs = nr_acodecs;
                dshow=7;win32=4;
            } else {
                // video
                cl = video_codecs;
                nr_codecs = nr_vcodecs;
                dshow=4;win32=2;win32ex=6;
            }
            pos=ftell(f1);
            for(i=0;i<nr_codecs;i++){
                fseek(f1,pos,SEEK_SET);
                switch(section){
                case 0:
                case 5:
                    if(cl[i].status==CODECS_STATUS_WORKING)
//                if(!(!strcmp(cl[i].drv,"vfw") || !strcmp(cl[i].drv,"dshow") || !strcmp(cl[i].drv,"vfwex") || !strcmp(cl[i].drv,"acm")))
                        parsehtml(f1,f2,&cl[i]);
                    break;
#if 0
                case 1:
                case 6:
                    if(cl[i].status==CODECS_STATUS_WORKING)
                        if((!strcmp(cl[i].drv,"vfw") || !strcmp(cl[i].drv,"dshow") || !strcmp(cl[i].drv,"vfwex") || !strcmp(cl[i].drv,"acm")))
                            parsehtml(f1,f2,&cl[i]);
                    break;
#endif
                case 2:
                case 7:
                    if(cl[i].status==CODECS_STATUS_PROBLEMS)
                        parsehtml(f1,f2,&cl[i]);
                    break;
                case 3:
                case 8:
                    if(cl[i].status==CODECS_STATUS_NOT_WORKING)
                        parsehtml(f1,f2,&cl[i]);
                    break;
                case 4:
                case 9:
                    if(cl[i].status==CODECS_STATUS_UNTESTED)
                        parsehtml(f1,f2,&cl[i]);
                    break;
                default:
                    printf("Warning! unimplemented section: %d\n",section);
                }
            }
            fseek(f1,pos,SEEK_SET);
            skiphtml(f1);

            continue;
        }
        fputc(c,f2);
        fputc(d,f2);
    }

    fclose(f2);
    fclose(f1);
    return 0;
}

#endif

#ifdef TESTING
int main(void)
{
    codecs_t *c;
    int i,j, nr_codecs, state;

    if (!(parse_codec_cfg("etc/codecs.conf")))
        return 0;
    if (!video_codecs)
        printf("no videoconfig.\n");
    if (!audio_codecs)
        printf("no audioconfig.\n");

    printf("videocodecs:\n");
    c = video_codecs;
    nr_codecs = nr_vcodecs;
    state = 0;
next:
    if (c) {
        printf("number of %scodecs: %d\n", state==0?"video":"audio",
               nr_codecs);
        for(i=0;i<nr_codecs;i++, c++){
            printf("\n============== %scodec %02d ===============\n",
                   state==0?"video":"audio",i);
            printf("name='%s'\n",codec_idx2str(c->name_idx));
            printf("info='%s'\n",codec_idx2str(c->info_idx));
            printf("comment='%s'\n",codec_idx2str(c->comment_idx));
            printf("dll='%s'\n",codec_idx2str(c->dll_idx));
            /* printf("flags=%X  driver=%d status=%d cpuflags=%d\n",
                      c->flags, c->driver, c->status, c->cpuflags); */
            printf("flags=%X status=%d cpuflags=%d\n",
                   c->flags, c->status, c->cpuflags);

            for(j=0;j<CODECS_MAX_FOURCC;j++){
                if(c->fourcc[j]!=0xFFFFFFFF){
                    printf("fourcc %02d:  %08X (%.4s) ===> %08X (%.4s)\n",j,c->fourcc[j],(char *) &c->fourcc[j],c->fourccmap[j],(char *) &c->fourccmap[j]);
                }
            }

            for(j=0;j<CODECS_MAX_OUTFMT;j++){
                if(c->outfmt[j]!=0xFFFFFFFF){
                    printf("outfmt %02d:  %08X (%.4s)  flags: %d\n",j,c->outfmt[j],(char *) &c->outfmt[j],c->outflags[j]);
                }
            }

            for(j=0;j<CODECS_MAX_INFMT;j++){
                if(c->infmt[j]!=0xFFFFFFFF){
                    printf("infmt %02d:  %08X (%.4s)  flags: %d\n",j,c->infmt[j],(char *) &c->infmt[j],c->inflags[j]);
                }
            }

            printf("GUID: %08lX %04X %04X",c->guid.f1,c->guid.f2,c->guid.f3);
            for(j=0;j<8;j++) printf(" %02X",c->guid.f4[j]);
            printf("\n");


        }
    }
    if (!state) {
        printf("audiocodecs:\n");
        c = audio_codecs;
        nr_codecs = nr_acodecs;
        state = 1;
        goto next;
    }
    return 0;
}

#endif
