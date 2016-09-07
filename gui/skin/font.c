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

/**
 * @file
 * @brief Font file parser and font rendering
 */

#include <glib.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "font.h"
#include "skin.h"
#include "gui/util/mem.h"
#include "gui/util/misc.h"
#include "gui/util/string.h"

#include "mp_msg.h"
#include "libavutil/avstring.h"

#define ASCII_CHRS 128   // number of ASCII characters
#define EXTRA_CHRS 128   // (arbitrary) number of non-ASCII characters
#define UTF8LENGTH 4     // length of an UTF-8 encoding according to RFC 3629

#define MAX_FONTS 25

#define fntAlignLeft   0
#define fntAlignCenter 1
#define fntAlignRight  2

static bmpFont *Fonts[MAX_FONTS];

/**
 * @brief Allocate and initialize memory for a #bmpFont::Chr array.
 *
 * @note If @a Font->Chr is not NULL, memory will be reallocated.
 *
 * @param Font pointer to a #Fonts element
 * @param nmemb number of members the old array should grow by
 *
 * @return pointer to the newly allocated array or NULL (error)
 *
 * @note As a side effect, @a Font->Chr will point to the newly
 *       allocated memory and @a Font->chrs will be increased accordingly
 *       (if and only if memory could be allocated).
 */
static void *fntAllocChr(bmpFont *Font, size_t nmemb)
{
    void *ptr;
    int i;

    ptr = realloc(Font->Chr, (Font->chrs + nmemb) * sizeof(*Font->Chr));

    if (ptr) {
        Font->Chr = ptr;

        for (i = Font->chrs + nmemb - 1; i >= Font->chrs; i--) {
            Font->Chr[i].x = -1;
            Font->Chr[i].y = -1;
            Font->Chr[i].w = -1;
            Font->Chr[i].h = -1;
        }

        Font->chrs += nmemb;
    }

    return ptr;
}

/**
 * @brief Allocate and initialize memory for a #bmpFont::bit8_chr array.
 *
 * @note If @a Font->bit8_chr is not NULL, memory will be reallocated.
 *
 * @param Font pointer to a #Fonts element
 * @param nmemb number of members the old array should grow by
 *
 * @return pointer to the newly allocated array or NULL (error)
 *
 * @note As a side effect, @a Font->bit8_chr will point to the newly
 *       allocated memory and @a Font->extra_chrs will be increased accordingly
 *       (if and only if memory could be allocated).
 */
static void *fntAllocBit8Chr(bmpFont *Font, size_t nmemb)
{
    void *ptr;

    ptr = realloc(Font->bit8_chr, (Font->extra_chrs + nmemb) * UTF8LENGTH * sizeof(*Font->bit8_chr));

    if (ptr) {
        Font->bit8_chr = ptr;

        memset(Font->bit8_chr + Font->extra_chrs * UTF8LENGTH, 0, nmemb * UTF8LENGTH * sizeof(*Font->bit8_chr));

        Font->extra_chrs += nmemb;
    }

    return ptr;
}

/**
 * @brief Add a font to #Fonts.
 *
 * @param name name of the font
 *
 * @return an identification >= 0 (ok), -1 (out of memory) or -2 (#MAX_FONTS exceeded)
 */
static int fntAddNewFont(char *name)
{
    int id;

    for (id = 0; id < MAX_FONTS; id++)
        if (!Fonts[id])
            break;

    if (id == MAX_FONTS)
        return -2;

    Fonts[id] = calloc(1, sizeof(*Fonts[id]));

    if (!Fonts[id])
        return -1;

    av_strlcpy(Fonts[id]->name, name, MAX_FONT_NAME);

    if (!fntAllocChr(Fonts[id], ASCII_CHRS + EXTRA_CHRS)) {
        nfree(Fonts[id]);
        return -1;
    }

    if (!fntAllocBit8Chr(Fonts[id], EXTRA_CHRS)) {
        nfree(Fonts[id]->Chr);
        nfree(Fonts[id]);
        return -1;
    }

    return id;
}

/**
 * @brief Free all memory allocated to a font.
 *
 * @param id font ID
 */
static void fntFreeFont(int id)
{
    free(Fonts[id]->Chr);
    free(Fonts[id]->bit8_chr);
    bpFree(&Fonts[id]->Bitmap);
    nfree(Fonts[id]);
}

/**
 * @brief Free all memory allocated to fonts.
 */
void fntFreeFonts(void)
{
    int i;

    for (i = 0; i < MAX_FONTS; i++)
        if (Fonts[i])
            fntFreeFont(i);
}

/**
 * @brief Read and parse a font file.
 *
 * @param path directory the font file is in
 * @param fname name of the font
 *
 * @return 0 (ok), -1 or -2 (return code of #fntAddNewFont()),
 *                 -3 (file error) or -4 (#skinImageRead() error)
 */
int fntRead(char *path, char *fname)
{
    FILE *file;
    unsigned char buf[512];
    unsigned char item[32];
    unsigned char param[256];
    int id, n, i;

    id = fntAddNewFont(fname);

    if (id < 0)
        return id;

    av_strlcpy(buf, path, sizeof(buf));
    av_strlcat(buf, fname, sizeof(buf));
    av_strlcat(buf, ".fnt", sizeof(buf));
    file = fopen(buf, "rt");

    if (!file) {
        fntFreeFont(id);
        return -3;
    }

    while (fgetstr(buf, sizeof(buf), file)) {
        strswap(buf, '\t', ' ');
        despace(buf);
        decomment(buf);

        if (!*buf)
            continue;

        n = (buf[0] == '"' && buf[1] == '=' ? 1 : 0);
        cutStr(buf, item, '=', n);
        cutStr(buf, param, '=', n + 1);

        if (item[0] == '"') {
            if (!item[1])
                item[0] = '=';
            else if (item[1] == '"')
                item[1] = 0;
            else
                cutStr(item, item, '"', 1);

            if (item[0] & 0x80) {
                if (Fonts[id]->bit8_count % EXTRA_CHRS == 0) {
                    if (!fntAllocChr(Fonts[id], EXTRA_CHRS) ||
                        !fntAllocBit8Chr(Fonts[id], EXTRA_CHRS))
                        continue;
                }

                strncpy(Fonts[id]->bit8_chr + Fonts[id]->bit8_count * UTF8LENGTH, item, UTF8LENGTH);

                i = Fonts[id]->bit8_count++ + ASCII_CHRS;
            } else
                i = item[0];

            cutStr(param, buf, ',', 0);
            Fonts[id]->Chr[i].x = atoi(buf);

            cutStr(param, buf, ',', 1);
            Fonts[id]->Chr[i].y = atoi(buf);

            cutStr(param, buf, ',', 2);
            Fonts[id]->Chr[i].w = atoi(buf);

            cutStr(param, buf, ',', 3);
            Fonts[id]->Chr[i].h = atoi(buf);

            mp_msg(MSGT_GPLAYER, MSGL_DBG2, "[font]  char: '%s' params: %d,%d %dx%d\n", item, Fonts[id]->Chr[i].x, Fonts[id]->Chr[i].y, Fonts[id]->Chr[i].w, Fonts[id]->Chr[i].h);
        } else if (strcmp(item, "image") == 0) {
            av_strlcpy(buf, path, sizeof(buf));
            av_strlcat(buf, param, sizeof(buf));

            mp_msg(MSGT_GPLAYER, MSGL_DBG2, "[font] image file: %s\n", buf);

            if (skinImageRead(buf, &Fonts[id]->Bitmap) != 0) {
                fntFreeFont(id);
                fclose(file);
                return -4;
            }
        }
    }

    fclose(file);

    return 0;
}

/**
 * @brief Find the ID of a font by its name.
 *
 * @param name name of the font
 *
 * @return an identification >= 0 (ok) or -1 (not found)
 */
int fntFindID(char *name)
{
    int i;

    for (i = 0; i < MAX_FONTS; i++)
        if (Fonts[i])
            if (strcmp(name, Fonts[i]->name) == 0)
                return i;

    return -1;
}

/**
 * @brief Get the #bmpFont::Chr index of the character @a str points to.
 *
 *        Move pointer @a str to the character according to @a direction
 *        afterwards.
 *
 * @param id font ID
 * @param str pointer to the string
 * @param uft8 flag indicating whether @a str contains UTF-8 characters
 * @param direction +1 (forward) or -1 (backward)
 *
 * @return index >= 0 (ok) or -1 (not found)
 */
static int fntGetCharIndex(int id, unsigned char **str, gboolean utf8, int direction)
{
    unsigned char *p, uchar[6] = "";   // glib implements 31-bit UTF-8
    int i, c = -1;

    if (**str & 0x80) {
        if (utf8) {
            p    = *str;
            *str = g_utf8_next_char(*str);
            strncpy(uchar, p, *str - p);

            if (direction < 0)
                *str = g_utf8_prev_char(p);
        } else {
            uchar[0] = **str;
            *str    += direction;
        }

        for (i = 0; i < Fonts[id]->bit8_count; i++) {
            if (strncmp(Fonts[id]->bit8_chr + i * UTF8LENGTH, uchar, UTF8LENGTH) == 0)
                return i + ASCII_CHRS;

            if (!utf8 &&
                ((Fonts[id]->bit8_chr + i * UTF8LENGTH)[0] == (*uchar >> 6 | 0xc0) &&
                 (Fonts[id]->bit8_chr + i * UTF8LENGTH)[1] == ((*uchar & 0x3f) | 0x80) &&
                 (Fonts[id]->bit8_chr + i * UTF8LENGTH)[2] == 0))
                c = i + ASCII_CHRS;
        }
    } else {
        c = **str;

        if (utf8 && (direction < 0))
            *str = g_utf8_prev_char(*str);
        else
            *str += direction;
    }

    return c;
}

/**
 * @brief Get the rendering width of a text.
 *
 * @param id font ID
 * @param str string to be examined
 *
 * @return width of the rendered string (in pixels)
 */
int fntTextWidth(int id, char *str)
{
    int size = 0, c;
    gboolean utf8;
    unsigned char *p;

    utf8 = g_utf8_validate(str, -1, NULL);
    p    = str;

    while (*p) {
        c = fntGetCharIndex(id, &p, utf8, 1);

        if (c == -1 || Fonts[id]->Chr[c].w == -1)
            c = ' ';

        if (Fonts[id]->Chr[c].w != -1)
            size += Fonts[id]->Chr[c].w;
    }

    return size;
}

/**
 * @brief Get the rendering height of a text.
 *
 * @param id font ID
 * @param str string to be examined
 *
 * @return height of the rendered string (in pixels)
 */
static int fntTextHeight(int id, char *str)
{
    int max = 0, c, h;
    gboolean utf8;
    unsigned char *p;

    utf8 = g_utf8_validate(str, -1, NULL);
    p    = str;

    while (*p) {
        c = fntGetCharIndex(id, &p, utf8, 1);

        if (c == -1 || Fonts[id]->Chr[c].w == -1)
            c = ' ';

        h = Fonts[id]->Chr[c].h;

        if (h > max)
            max = h;
    }

    return max;
}

/**
 * @brief Render a text on an item.
 *
 * @param item item the text shall be placed on
 * @param px x position for the text in case it is wider than the item width
 * @param txt text to be rendered
 *
 * @return image containing the rendered text
 */
guiImage *fntTextRender(guiItem *item, int px, char *txt)
{
    unsigned char *u;
    unsigned int i;
    int c, dx, tw, th, fbw, iw, id, ofs;
    int x, y, fh, fw, fyc, yc;
    uint32_t *ibuf;
    uint32_t *obuf;
    gboolean utf8;

    id = item->fontid;
    tw = fntTextWidth(id, txt);

    if (!tw)
        return NULL;

    iw  = item->width;
    fbw = Fonts[id]->Bitmap.Width;
    th  = fntTextHeight(id, txt);

    if (item->height != th)
        bpFree(&item->Bitmap);

    if (!item->Bitmap.Image) {
        item->Bitmap.Height    = item->height = th;
        item->Bitmap.Width     = item->width = iw;
        item->Bitmap.ImageSize = item->height * iw * 4;

        if (!item->Bitmap.ImageSize)
            return NULL;

        item->Bitmap.Bpp   = 32;
        item->Bitmap.Image = malloc(item->Bitmap.ImageSize);

        if (!item->Bitmap.Image)
            return NULL;
    }

    obuf = (uint32_t *)item->Bitmap.Image;
    ibuf = (uint32_t *)Fonts[id]->Bitmap.Image;

    for (i = 0; i < item->Bitmap.ImageSize / 4; i++)
        obuf[i] = GUI_TRANSPARENT;

    if (tw <= iw) {
        switch (item->align) {
        default:
        case fntAlignLeft:
            dx = 0;
            break;

        case fntAlignCenter:
            dx = (iw - tw) / 2;
            break;

        case fntAlignRight:
            dx = iw - tw;
            break;
        }
    } else
        dx = px;

    ofs = dx;

    utf8 = g_utf8_validate(txt, -1, NULL);
    u    = txt;

    while (*u) {
        c = fntGetCharIndex(id, &u, utf8, 1);

        if (c != -1)
            fw = Fonts[id]->Chr[c].w;

        if (c == -1 || fw == -1) {
            c  = ' ';
            fw = Fonts[id]->Chr[c].w;
        }

        if (fw == -1)
            continue;

        fh  = Fonts[id]->Chr[c].h;
        fyc = Fonts[id]->Chr[c].y * fbw + Fonts[id]->Chr[c].x;
        yc  = dx;

        if (dx >= 0) {
            for (y = 0; y < fh; y++) {
                for (x = 0; x < fw; x++)
                    if (dx + x >= 0 && dx + x < iw)
                        obuf[yc + x] = ibuf[fyc + x];

                fyc += fbw;
                yc  += iw;
            }
        }

        dx += fw;
    }

    if (ofs > 0 && tw > item->width) {
        dx = ofs;
        u  = txt + strlen(txt);

        while (u > (unsigned char *)txt) {
            c = fntGetCharIndex(id, &u, utf8, -1);

            if (c != -1)
                fw = Fonts[id]->Chr[c].w;

            if (c == -1 || fw == -1) {
                c  = ' ';
                fw = Fonts[id]->Chr[c].w;
            }

            if (fw == -1)
                continue;

            fh  = Fonts[id]->Chr[c].h;
            fyc = Fonts[id]->Chr[c].y * fbw + Fonts[id]->Chr[c].x;

            dx -= fw;
            yc  = dx;

            if (dx >= 0) {
                for (y = 0; y < fh; y++) {
                    for (x = fw - 1; x >= 0; x--)
                        if (dx + x >= 0 && dx + x < iw)
                            obuf[yc + x] = ibuf[fyc + x];

                    fyc += fbw;
                    yc  += iw;
                }
            }
        }
    }

    return &item->Bitmap;
}
