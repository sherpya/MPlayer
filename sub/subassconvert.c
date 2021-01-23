/*
 * Subtitles converter to SSA/ASS in order to allow special formatting
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

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "mp_msg.h"
#include "help_mp.h"
#include "bstr.h"
#include "subassconvert.h"
#include "libavutil/common.h"
#include "libavutil/avstring.h"

struct line {
    char *buf;
    size_t bufsize;
    size_t len;
};

#ifdef __GNUC__
static void append_text(struct line *dst, char *fmt, ...) __attribute__ ((format(printf, 2, 3)));
#endif

static void append_text(struct line *dst, char *fmt, ...)
{
    va_list va;
    int ret;

    va_start(va, fmt);
    ret = vsnprintf(dst->buf + dst->len, dst->bufsize - dst->len, fmt, va);
    if (ret >= 0) {
        dst->len += ret;
        if (dst->len > dst->bufsize)
            dst->len = dst->bufsize;
    }
    va_end(va);
}

static int indexof(const char *s, int c)
{
    char *f = strchr(s, c);
    return f ? (f - s) : -1;
}



/*
 *      SubRip
 *
 *      Support basic tags (italic, bold, underline, strike-through)
 *      and font tag with size, color and face attributes.
 *
 */

struct font_tag {
    struct bstr face;
    int size;
    uint32_t color;
};

static const struct tag_conv {
    const char *from;
    const char *to;
} subrip_basic_tags[] = {
    {"<i>", "{\\i1}"}, {"</i>", "{\\i0}"},
    {"<b>", "{\\b1}"}, {"</b>", "{\\b0}"},
    {"<u>", "{\\u1}"}, {"</u>", "{\\u0}"},
    {"<s>", "{\\s1}"}, {"</s>", "{\\s0}"},
    {"{", "\\{"}, {"}", "\\}"},
    {"\r\n", "\\N"}, {"\n", "\\N"}, {"\r", "\\N"},
};

static const struct {
    const char *s;
    uint32_t v;
} subrip_web_colors[] = {
    /* Named CSS3 colors in BGR format; a subset of those
       at http://www.w3.org/TR/css3-color/#svg-color */
    {"aqua",    0xffff00},
    {"black",   0x000000},
    {"blue",    0xff0000},
    {"cyan",    0xffff00},
    {"fuchsia", 0xff00ff},
    {"gray",    0x808080},
    {"green",   0x008000},
    {"grey",    0x808080},
    {"lime",    0x00ff00},
    {"magenta", 0xff00ff},
    {"maroon",  0x000080},
    {"navy",    0x800000},
    {"olive",   0x008080},
    {"orange",  0x00a5ff},
    {"pink",    0xcbc0ff},
    {"purple",  0x800080},
    {"red",     0x0000ff},
    {"silver",  0xc0c0c0},
    {"teal",    0x808000},
    {"white",   0xffffff},
    {"yellow",  0x00ffff},
};

#define SUBRIP_MAX_STACKED_FONT_TAGS    16
#define SUBRIP_FLAG_COLOR               0x01000000

/**
 * \brief Convert SubRip lines into ASS markup
 * \param orig original SubRip lines. The content will remain untouched.
 * \param dest ASS markup destination buffer.
 * \param dest_buffer_size maximum size for the destination buffer.
 */
void subassconvert_subrip(const char *orig, char *dest, size_t dest_buffer_size)
{
    /* line is not const to avoid warnings with strtol, etc.
     * orig content won't be changed */
    char *line = (char *)orig;
    struct line new_line = {
        .buf     = dest,
        .bufsize = dest_buffer_size,
    };
    struct font_tag font_stack[SUBRIP_MAX_STACKED_FONT_TAGS];
    int sp = 0;

    font_stack[0] = (struct font_tag){}; // type with all defaults
    while (*line && new_line.len < new_line.bufsize - 1) {
        char *orig_line = line;
        int i;

        for (i = 0; i < FF_ARRAY_ELEMS(subrip_basic_tags); i++) {
            const struct tag_conv *tag = &subrip_basic_tags[i];
            int from_len = strlen(tag->from);
            if (strncmp(line, tag->from, from_len) == 0) {
                append_text(&new_line, "%s", tag->to);
                line += from_len;
            }
        }

        if (strncmp(line, "</font>", 7) == 0) {
            /* Closing font tag */
            line += 7;

            if (sp > 0) {
                struct font_tag *tag      = &font_stack[sp];
                struct font_tag *last_tag = &tag[-1];
                sp--;

                if (tag->size) {
                    if (!last_tag->size)
                        append_text(&new_line, "{\\fs}");
                    else if (last_tag->size != tag->size)
                        append_text(&new_line, "{\\fs%d}", last_tag->size);
                }

                if (tag->color & SUBRIP_FLAG_COLOR) {
                    if (!(last_tag->color & SUBRIP_FLAG_COLOR))
                        append_text(&new_line, "{\\c}");
                    else if (last_tag->color != tag->color)
                        append_text(&new_line, "{\\c&H%06X&}",
                                    last_tag->color & 0xffffff);
                }

                if (tag->face.len) {
                    if (!last_tag->face.len)
                        append_text(&new_line, "{\\fn}");
                    else if (bstrcmp(last_tag->face, tag->face) != 0)
                        append_text(&new_line, "{\\fn%.*s}",
                                    BSTR_P(last_tag->face));
                }
            }
        } else if (strncmp(line, "<font ", 6) == 0
                   && sp + 1 < FF_ARRAY_ELEMS(font_stack)) {
            /* Opening font tag */
            char *potential_font_tag_start = line;
            int len_backup = new_line.len;
            struct font_tag *tag = &font_stack[sp + 1];
            int has_valid_attr = 0;

            *tag = tag[-1]; // keep values from previous tag
            line += 5;      // don't skip space!

            while (*line && *line != '>') {
                if (strncmp(line, " size=", 6) == 0) {
                    line += 6;
                    if (*line == '"') line++;
                    tag->size = strtol(line, &line, 10);
                    if (!tag->size)
                        break;
                    append_text(&new_line, "{\\fs%d}", tag->size);
                    has_valid_attr = 1;
                } else if (strncmp(line, " color=", 7) == 0) {
                    line += 7;
                    if (*line == '"') line++;
                    if (*line == '#') {
                        // #RRGGBB format
                        line++;
                        tag->color = strtol(line, &line, 16) & 0x00ffffff;
                        tag->color = ((tag->color & 0xff) << 16) |
                                      (tag->color & 0xff00) |
                                     ((tag->color & 0xff0000) >> 16) |
                                     SUBRIP_FLAG_COLOR;
                    } else {
                        // Standard web colors
                        int i;
                        for (i = 0; i < FF_ARRAY_ELEMS(subrip_web_colors); i++) {
                            const char *color = subrip_web_colors[i].s;
                            const int len = strlen(color);
                            if (av_strncasecmp(line, color, len) == 0) {
                                tag->color = SUBRIP_FLAG_COLOR | subrip_web_colors[i].v;
                                line += len;
                                break;
                            }
                        }

                        if (i == FF_ARRAY_ELEMS(subrip_web_colors)) {
                            /* We didn't find any matching color */
                            line += strcspn(line, "\" >");
                            mp_msg(MSGT_SUBREADER, MSGL_WARN,
                                   MSGTR_SUBTITLES_SubRip_UnknownFontColor, orig);
                            append_text(&new_line, "{\\c}");
                            continue;
                        }
                    }
                    append_text(&new_line, "{\\c&H%06X&}", tag->color & 0xffffff);
                    has_valid_attr = 1;
                } else if (strncmp(line, " face=\"", 7) == 0) {
                    /* Font face attribute */
                    int len;
                    line += 7;
                    len = indexof(line, '"');
                    if (len <= 0)
                        break;
                    tag->face.start = line;
                    tag->face.len   = len;
                    line += len;
                    append_text(&new_line, "{\\fn%.*s}", BSTR_P(tag->face));
                    has_valid_attr = 1;
                }
                if (*line != '>')
                    line++;
            }

            if (!has_valid_attr || *line != '>') { /* Not valid font tag */
                line = potential_font_tag_start;
                new_line.len = len_backup;
            } else {
                sp++;
                line++;
            }
        }

        /* Tag conversion code didn't match */
        if (line == orig_line)
            new_line.buf[new_line.len++] = *line++;
    }
    new_line.buf[new_line.len] = 0;
}


/*
 *      MicroDVD
 *
 *      Based on the specifications found here:
 *      https://trac.videolan.org/vlc/ticket/1825#comment:6
 */

struct microdvd_tag {
    char key;
    int persistent;
    uint32_t data1;
    uint32_t data2;
    struct bstr data_string;
};

#define MICRODVD_PERSISTENT_OFF     0
#define MICRODVD_PERSISTENT_ON      1
#define MICRODVD_PERSISTENT_OPENED  2

// Color, Font, Size, cHarset, stYle, Position, cOordinate
#define MICRODVD_TAGS               "cfshyYpo"

static void microdvd_set_tag(struct microdvd_tag *tags, struct microdvd_tag tag)
{
    int tag_index = indexof(MICRODVD_TAGS, tag.key);

    if (tag_index < 0)
        return;
    memcpy(&tags[tag_index], &tag, sizeof(tag));
}

// italic, bold, underline, strike-through
#define MICRODVD_STYLES             "ibus"

static char *microdvd_load_tags(struct microdvd_tag *tags, char *s)
{
    while (*s == '{') {
        char *start = s;
        char tag_char = *(s + 1);
        struct microdvd_tag tag = {0};

        if (!tag_char || *(s + 2) != ':')
            break;
        s += 3;

        switch (tag_char) {

        /* Style */
        case 'Y':
            tag.persistent = MICRODVD_PERSISTENT_ON;
        case 'y':
            while (*s && *s != '}') {
                int style_index = indexof(MICRODVD_STYLES, *s);

                if (style_index >= 0)
                    tag.data1 |= (1 << style_index);
                s++;
            }
            if (*s != '}')
                break;
            /* We must distinguish persistent and non-persistent styles
             * to handle this kind of style tags: {y:ib}{Y:us} */
            tag.key = tag_char;
            break;

        /* Color: {c:$bbggrr} and {c:$#bbggrr}*/
        case 'C':
            tag.persistent = MICRODVD_PERSISTENT_ON;
        case 'c':
            while(*s == '$' || *s == '#')
                ++s;
            tag.data1 = strtol(s, &s, 16) & 0x00ffffff;
            if (*s != '}')
                break;
            tag.key = 'c';
            break;

        /* Font name */
        case 'F':
            tag.persistent = MICRODVD_PERSISTENT_ON;
        case 'f':
        {
            int len = indexof(s, '}');
            if (len < 0)
                break;
            tag.data_string.start = s;
            tag.data_string.len   = len;
            s += len;
            tag.key = 'f';
            break;
        }

        /* Font size */
        case 'S':
            tag.persistent = MICRODVD_PERSISTENT_ON;
        case 's':
            tag.data1 = strtol(s, &s, 10);
            if (*s != '}')
                break;
            tag.key = 's';
            break;

        /* Charset */
        case 'H':
        {
            //TODO: not yet handled, just parsed.
            int len = indexof(s, '}');
            if (len < 0)
                break;
            tag.data_string.start = s;
            tag.data_string.len   = len;
            s += len;
            tag.key = 'h';
            break;
        }

        /* Position */
        case 'P':
            tag.persistent = MICRODVD_PERSISTENT_ON;
            tag.data1 = (*s++ == '1');
            if (*s != '}')
                break;
            tag.key = 'p';
            break;

        /* Coordinates */
        case 'o':
            tag.persistent = MICRODVD_PERSISTENT_ON;
            tag.data1 = strtol(s, &s, 10);
            if (*s != ',')
                break;
            s++;
            tag.data2 = strtol(s, &s, 10);
            if (*s != '}')
                break;
            tag.key = 'o';
            break;

        default:    /* Unknown tag, we consider it's text */
            break;
        }

        if (tag.key == 0)
            return start;

        microdvd_set_tag(tags, tag);
        s++;
    }
    return s;
}

static void microdvd_open_tags(struct line *new_line, struct microdvd_tag *tags)
{
    int i, sidx;
    for (i = 0; i < sizeof(MICRODVD_TAGS) - 1; i++) {
        if (tags[i].persistent == MICRODVD_PERSISTENT_OPENED)
            continue;
        switch (tags[i].key) {
        case 'Y':
        case 'y':
            for (sidx = 0; sidx < sizeof(MICRODVD_STYLES) - 1; sidx++)
                if (tags[i].data1 & (1 << sidx))
                    append_text(new_line, "{\\%c1}", MICRODVD_STYLES[sidx]);
            break;

        case 'c':
            append_text(new_line, "{\\c&H%06X&}", tags[i].data1);
            break;

        case 'f':
            append_text(new_line, "{\\fn%.*s}", BSTR_P(tags[i].data_string));
            break;

        case 's':
            append_text(new_line, "{\\fs%d}", tags[i].data1);
            break;

        case 'p':
            if (tags[i].data1 == 0)
                append_text(new_line, "{\\an8}");
            break;

        case 'o':
            append_text(new_line, "{\\pos(%d,%d)}",
                        tags[i].data1, tags[i].data2);
            break;
        }
        if (tags[i].persistent == MICRODVD_PERSISTENT_ON)
            tags[i].persistent = MICRODVD_PERSISTENT_OPENED;
    }
}

static void microdvd_close_no_persistent_tags(struct line *new_line,
                                              struct microdvd_tag *tags)
{
    int i, sidx;

    for (i = sizeof(MICRODVD_TAGS) - 2; i>=0; i--) {
        if (tags[i].persistent != MICRODVD_PERSISTENT_OFF)
            continue;
        switch (tags[i].key) {

        case 'y':
            for (sidx = sizeof(MICRODVD_STYLES) - 2; sidx >= 0; sidx--)
                if (tags[i].data1 & (1 << sidx))
                    append_text(new_line, "{\\%c0}", MICRODVD_STYLES[sidx]);
            break;

        case 'c':
            append_text(new_line, "{\\c}");
            break;

        case 'f':
            append_text(new_line, "{\\fn}");
            break;

        case 's':
            append_text(new_line, "{\\fs}");
            break;
        }
        tags[i].key = 0;
    }
}

/**
 * \brief Convert MicroDVD lines into ASS markup
 * \param orig original MicroDVD line. The content will remain untouched.
 * \param dest ASS markup destination buffer.
 * \param dest_buffer_size maximum size for the destination buffer.
 */
void subassconvert_microdvd(const char *orig, char *dest, size_t dest_buffer_size)
{
    /* line is not const to avoid warnings with strtol, etc.
     * orig content won't be changed */
    char *line = (char *)orig;
    struct line new_line = {
        .buf     = dest,
        .bufsize = dest_buffer_size,
    };
    struct microdvd_tag tags[sizeof(MICRODVD_TAGS) - 1] = {{0}};

    /* '/' at beginning of line is often used to indicate italic text */
    if(*line == '/') {
        struct microdvd_tag tag = {0};
        tag.key = 'y';
        tag.data1 = 1;
        microdvd_set_tag(tags, tag);
        ++line;
    }

    while (*line) {
        line = microdvd_load_tags(tags, line);
        microdvd_open_tags(&new_line, tags);

        while (*line && *line != '|')
            new_line.buf[new_line.len++] = *line++;

        if (*line == '|') {
            microdvd_close_no_persistent_tags(&new_line, tags);
            append_text(&new_line, "\\N");
            line++;

            if(*line == '/') {
                struct microdvd_tag tag = {0};
                tag.key = 'y';
                tag.data1 = 1;
                microdvd_set_tag(tags, tag);
                ++line;
            }
        }
    }
    new_line.buf[new_line.len] = 0;
}
