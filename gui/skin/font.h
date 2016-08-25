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

#ifndef MPLAYER_GUI_FONT_H
#define MPLAYER_GUI_FONT_H

#include "gui/app/app.h"
#include "gui/util/bitmap.h"

#define MAX_FONT_NAME 128

typedef struct {
    int x, y;     // location
    int w, h;     // size
} fntChar;

typedef struct {
    fntChar *Chr;
    int extra_chrs;
    unsigned char *bit8_chr;
    guiImage Bitmap;
    char name[MAX_FONT_NAME];
} bmpFont;

int fntFindID(char *name);
void fntFreeFonts(void);
int fntRead(char *path, char *fname);
guiImage *fntTextRender(guiItem *item, int px, char *txt);
int fntTextWidth(int id, char *str);

#endif /* MPLAYER_GUI_FONT_H */
