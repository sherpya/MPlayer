/*
 * AVOption parsing helper
 * Copyright (C) 2008 Michael Niedermayer
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

#include <stdlib.h>
#include <string.h>

#include "av_opts.h"
#include "libavutil/opt.h"

int parse_avopts(void *v, char *str){
    char *start;

    if (!v)
        return -1;
    if (!str)
        return 0;
    if (strcmp(str, "help") == 0) {
        av_opt_show2(v, NULL, -1, 0);
        return -1;
    }

    start= str= strdup(str);

    while(str && *str){
        char *next_opt, *arg;

        next_opt= strchr(str, ',');
        if(next_opt) *next_opt++= 0;

        arg     = strchr(str, '=');
        if(arg)      *arg++= 0;

        if (av_opt_set(v, str, arg, 0) < 0) {
            free(start);
            return -1;
        }
        str= next_opt;
    }

    free(start);
    return 0;
}
