/*
 * HTTP Cookies
 * Reads Netscape and Mozilla cookies.txt files
 *
 * Copyright (c) 2003 Dave Lambley <mplayer@davel.me.uk>
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
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#ifndef _MSC_VER
#include <dirent.h>
#endif
#include <inttypes.h>
#include <limits.h>

#include "cookies.h"
#include "http.h"
#include "mp_msg.h"

#define MAX_COOKIES 20

char *cookies_file = NULL;

typedef struct cookie_list_type {
    char *name;
    char *value;
    char *domain;
    char *path;

    int secure;

    struct cookie_list_type *next;
} cookie_list_t;

/* Pointer to the linked list of cookies */
static struct cookie_list_type *cookie_list = NULL;


/* Like strdup, but stops at anything <31. */
static char *col_dup(const char *src)
{
    char *dst;
    int length = 0;

    while (src[length] > 31)
	length++;

    dst = malloc(length + 1);
    strncpy(dst, src, length);
    dst[length] = 0;

    return dst;
}

static int right_hand_strcmp(const char *cookie_domain, const char *url_domain)
{
    int c_l;
    int u_l;

    c_l = strlen(cookie_domain);
    u_l = strlen(url_domain);

    if (c_l > u_l)
	return -1;
    return strcmp(cookie_domain, url_domain + u_l - c_l);
}

static int left_hand_strcmp(const char *cookie_path, const char *url_path)
{
    return strncmp(cookie_path, url_path, strlen(cookie_path));
}

/* Finds the start of all the columns */
static int parse_line(char **ptr, char *cols[7])
{
    int col;
    cols[0] = *ptr;

    for (col = 1; col < 7; col++) {
	for (; (**ptr) > 31; (*ptr)++);
	if (**ptr == 0)
	    return 0;
	(*ptr)++;
	if ((*ptr)[-1] != 9)
	    return 0;
	cols[col] = (*ptr);
    }

    return 1;
}

/* Loads a file into RAM */
static char *load_file(const char *filename, off_t * length)
{
    int fd = -1;
    char *buffer = NULL;

    mp_msg(MSGT_NETWORK, MSGL_V, "Loading cookie file: %s\n", filename);

    fd = open(filename, O_RDONLY | O_BINARY);
    if (fd < 0) {
	mp_msg(MSGT_NETWORK, MSGL_V, "Could not open");
	goto err_out;
    }

    *length = lseek(fd, 0, SEEK_END);

    if (*length < 0) {
	mp_msg(MSGT_NETWORK, MSGL_V, "Could not find EOF");
	goto err_out;
    }

    if (*length > SIZE_MAX - 1) {
	mp_msg(MSGT_NETWORK, MSGL_V, "File too big, could not malloc.");
	goto err_out;
    }

    lseek(fd, 0, SEEK_SET);

    if (!(buffer = malloc(*length + 1))) {
	mp_msg(MSGT_NETWORK, MSGL_V, "Could not malloc.");
	goto err_out;
    }

    if (read(fd, buffer, *length) != *length) {
	mp_msg(MSGT_NETWORK, MSGL_V, "Read is behaving funny.");
	goto err_out;
    }
    close(fd);
    buffer[*length] = 0;

    return buffer;

err_out:
    if (fd != -1) close(fd);
    free(buffer);
    return NULL;
}

/* Loads a cookies.txt file into a linked list. */
static struct cookie_list_type *load_cookies_from(const char *filename,
						  struct cookie_list_type
						  *list)
{
    char *ptr, *buffer;
    off_t length;

    buffer =
    ptr = load_file(filename, &length);
    if (!ptr)
	return list;

    while (*ptr) {
	char *cols[7];
	if (parse_line(&ptr, cols)) {
	    struct cookie_list_type *new;
	    new = malloc(sizeof(cookie_list_t));
	    new->name = col_dup(cols[5]);
	    new->value = col_dup(cols[6]);
	    new->path = col_dup(cols[2]);
	    new->domain = col_dup(cols[0]);
	    new->secure = (*(cols[3]) == 't') || (*(cols[3]) == 'T');
	    new->next = list;
	    list = new;
	}
    }
    free(buffer);
    return list;
}

/* Attempt to load cookies.txt from various locations. Returns a pointer to the linked list contain the cookies. */
static struct cookie_list_type *load_cookies(void)
{
    struct cookie_list_type *list = NULL;
#ifndef _MSC_VER
    DIR *dir;
    struct dirent *ent;
    char *buf;

    char *homedir;
#endif

    if (cookies_file)
	return load_cookies_from(cookies_file, list);

#ifdef _MSC_VER
    return list;
#else
    homedir = getenv("HOME");
    if (!homedir)
	return list;


    buf = malloc(strlen(homedir) + sizeof("/.mozilla/default") + 1);
    sprintf(buf, "%s/.mozilla/default", homedir);
    dir = opendir(buf);
    free(buf);

    if (dir) {
	while ((ent = readdir(dir)) != NULL) {
	    if ((ent->d_name)[0] != '.') {
		unsigned len = strlen(homedir) +
		               sizeof("/.mozilla/default/") +
		               strlen(ent->d_name) + sizeof("cookies.txt") + 1;
		buf = malloc(len);
		snprintf(buf, len, "%s/.mozilla/default/%s/cookies.txt",
			 homedir, ent->d_name);
		list = load_cookies_from(buf, list);
		free(buf);
	    }
	}
	closedir(dir);
    }

    buf = malloc(strlen(homedir) + sizeof("/.netscape/cookies.txt") + 1);
    sprintf(buf, "%s/.netscape/cookies.txt", homedir);
    list = load_cookies_from(buf, list);
    free(buf);

    return list;
#endif
}

/* Take an HTTP_header_t, and insert the correct headers. The cookie files are read if necessary. */
void
cookies_set(HTTP_header_t * http_hdr, const char *domain, const char *url)
{
    int found_cookies = 0;
    struct cookie_list_type *cookies[MAX_COOKIES];
    struct cookie_list_type *list, *start;
    int i;
    char *path;
    char *buf;

    path = strchr(url, '/');
    if (!path)
	path = "";

    if (!cookie_list)
	cookie_list = load_cookies();


    list = start = cookie_list;

    /* Find which cookies we want, removing duplicates. Cookies with the longest domain, then longest path take priority */
    while (list) {
	/* Check the cookie domain and path. Also, we never send "secure" cookies. These should only be sent over HTTPS. */
	if ((right_hand_strcmp(list->domain, domain) == 0)
	    && (left_hand_strcmp(list->path, path) == 0) && !list->secure) {
	    int replacing = 0;
	    for (i = 0; i < found_cookies; i++) {
		if (strcmp(list->name, cookies[i]->name) == 0) {
		    replacing = 0;
		    if (strlen(list->domain) <= strlen(cookies[i]->domain)) {
			cookies[i] = list;
		    } else if (strlen(list->path) <= strlen(cookies[i]->path)) {
			cookies[i] = list;
		    }
		}
	    }
	    if (found_cookies > MAX_COOKIES) {
		/* Cookie jar overflow! */
		break;
	    }
	    if (!replacing)
		cookies[found_cookies++] = list;
	}
	list = list->next;
    }


    buf = strdup("Cookie:");

    for (i = 0; i < found_cookies; i++) {
	char *nbuf;

	nbuf = malloc(strlen(buf) + strlen(" ") + strlen(cookies[i]->name) +
		    strlen("=") + strlen(cookies[i]->value) + strlen(";") + 1);
	sprintf(nbuf, "%s %s=%s;", buf, cookies[i]->name,
		 cookies[i]->value);
	free(buf);
	buf = nbuf;
    }

    if (found_cookies)
	http_set_field(http_hdr, buf);
    free(buf);
}
