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
 * @brief List management
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "mem.h"
#include "string.h"
#include "gui/app/gui.h"

#include "mp_msg.h"
#include "path.h"

static gainItem *gainList;
static unsigned int gainCount;

static plItem *plList;
static plItem *plCurrent;

static urlItem *urlList;

/**
 * @brief Manage playlists and URL lists.
 *
 * @param cmd task to be performed
 * @param data list item (or string in case of GAINLIST_ITEM_FIND) for the task
 *
 * @return pointer to top of list (GET command),
 *         pointer to current list item (ITEM command),
 *         pointer to copied item (PLITEM_COPY) or
 *         NULL (PLAYLIST_DELETE, PLITEM_FREE or unknown command)
 *
 * @note PLAYLIST_ITEM_GET_POS returns the position number as pointer value
 *       (if @a data is NULL the last position number, i.e. number of items),
 *       and position 0 means "not found"
 */
void *listMgr(int cmd, void *data)
{
    uintptr_t pos;
    gainItem *gdat = (gainItem *)data;
    char *cdat     = (char *)data;
    plItem *pdat   = (plItem *)data;
    urlItem *udat  = (urlItem *)data;
    plItem *item;

    switch (cmd) {
    /* ReplayGain list (sorted) */

    case GAINLIST_ITEM_INSERT:

        if (!gainList || (strcmp(gainList->filename, gdat->filename) >= 0)) {
            gdat->next = gainList;
            gainList   = gdat;
        } else {
            gainItem *item = gainList;

            while (item->next && (strcmp(item->next->filename, gdat->filename) < 0))
                item = item->next;

            gdat->next = item->next;
            item->next = gdat;
        }

        gainCount++;
        return gdat;

    case GAINLIST_ITEM_FIND:

        if (!gainList || (strcmp(gainList->filename, cdat) == 0))
            return gainList;
        else {
            gainItem *left = gainList, *right = NULL;
            unsigned int count = gainCount;

            while (count > 1) {
                int cmp;
                unsigned int i, newcount = count / 2;

                right = left;

                for (i = 0; i < newcount && right->next != NULL; i++)
                    right = right->next;

                cmp = strcmp(right->filename, cdat);

                if (cmp == 0)
                    return right;
                else if (cmp < 0)
                    left = right;

                count -= newcount;
            }

            if (right && (strcmp(right->filename, cdat) == 0))
                return right;
            else
                return NULL;
        }

    /* playlist */

    case PLAYLIST_GET:

        return plList;

    case PLAYLIST_ITEM_APPEND:

        if (plList) {
            item = plList;

            while (item->next)
                item = item->next;

            item->next = pdat;
            pdat->prev = item;
            pdat->next = NULL;
        } else {
            pdat->next = pdat->prev = NULL;
            plCurrent  = plList = pdat;
        }

        return plCurrent;

    case PLAYLIST_ITEM_INSERT:

        if (plCurrent) {
            pdat->next = plCurrent->next;

            if (pdat->next)
                pdat->next->prev = pdat;

            pdat->prev      = plCurrent;
            plCurrent->next = pdat;

            plCurrent = pdat;

            return plCurrent;
        } else
            return listMgr(PLAYLIST_ITEM_APPEND, pdat);

    case PLAYLIST_ITEM_FIND:

        if (plList) {
            item = plList;

            do {
                if (gstrcmp(item->path, pdat->path) == 0 &&
                    gstrcmp(item->name, pdat->name) == 0 &&
                    gstrcmp(item->title, pdat->title) == 0 &&
                    item->start == pdat->start && item->stop == pdat->stop)
                    return item;

                item = item->next;
            } while (item);
        }

        return NULL;

    case PLAYLIST_ITEM_SET_CURR:

        plCurrent = pdat;
        return plCurrent;

    case PLAYLIST_ITEM_GET_CURR:

        return plCurrent;

    case PLAYLIST_ITEM_GET_POS:

        pos = 0;

        if (plList) {
            uintptr_t i = 0;

            item = plList;

            do {
                i++;

                if (item == pdat) {
                    pos = i;
                    break;
                }

                item = item->next;
            } while (item);

            if (!pdat)
                pos = i;
        }

        return (void *)pos;

    case PLAYLIST_ITEM_GET_PREV:

        if (plCurrent && plCurrent->prev) {
            plCurrent = plCurrent->prev;
            return plCurrent;
        }

        return NULL;

    case PLAYLIST_ITEM_GET_NEXT:

        if (plCurrent && plCurrent->next) {
            plCurrent = plCurrent->next;
            return plCurrent;
        }

        return NULL;

    case PLAYLIST_ITEM_GET_LAST:

        if (plList) {
            item = plList;

            while (item->next)
                item = item->next;

            return item;
        }

        return NULL;

    case PLAYLIST_ITEM_DEL_CURR:

        if (plCurrent) {
            plItem *curr = plCurrent;

            if (curr->prev)
                curr->prev->next = curr->next;
            if (curr->next)
                curr->next->prev = curr->prev;

            plCurrent = curr->next;

            if (curr == plList)
                plList = plCurrent;

            free(curr->path);
            free(curr->name);
            free(curr->title);
            free(curr);
        }

        return plCurrent;

    case PLAYLIST_DELETE:

        while (plList) {
            item = plList->next;

            listMgr(PLITEM_FREE, plList);

            plList = item;
        }

        plCurrent = NULL;
        return NULL;

    case PLITEM_COPY:

        item = calloc(1, sizeof(*item));

        if (item) {
            item->path  = gstrdup(pdat->path);
            item->name  = gstrdup(pdat->name);
            item->title = gstrdup(pdat->title);
            item->start = pdat->start;
            item->stop  = pdat->stop;
        }

        return item;

    case PLITEM_FREE:

        if (pdat) {
            free(pdat->path);
            free(pdat->name);
            free(pdat->title);
            free(pdat);
        }

        return NULL;

    /* URL list */

    case URLLIST_GET:

        return urlList;

    case URLLIST_ITEM_ADD:

        if (urlList) {
            urlItem *item = urlList;

            while (item) {
                if (strcmp(udat->url, item->url) == 0) {
                    free(udat->url);
                    free(udat);
                    return NULL;
                }

                if (item->next)
                    item = item->next;
                else {
                    item->next = udat;
                    udat->next = NULL;
                    break;
                }
            }
        } else {
            udat->next = NULL;
            urlList    = udat;
        }

        return udat;

    case URLLIST_DELETE:

        while (urlList) {
            urlItem *item = urlList->next;

            free(urlList->url);
            free(urlList);

            urlList = item;
        }

        return NULL;

    default:

        return NULL;
    }
}

/**
 * @brief Free a string list.
 *
 * @param list pointer to the string list
 */
void listFree(char ***list)
{
    if (*list) {
        char **l = *list;

        while (*l) {
            free(*l);
            l++;
        }

        nfree(*list);
    }
}

/**
 * @brief Set string list to @a entry.
 *
 * @param list pointer to the string list
 * @param entry new (and only) element of the list
 *
 * @note Actually, a new list will be created and the old list will be freed.
 */
void listSet(char ***list, const char *entry)
{
    listFree(list);

    *list = malloc(2 * sizeof(char *));

    if (*list) {
        (*list)[0] = gstrdup(entry);
        (*list)[1] = NULL;
    }
}

/**
 * @brief Replace the first element in a string list that starts with @a search.
 *
 * @note If no such element is found, @a replace will be appended.
 *
 * @param list pointer to the string list
 * @param search element to search
 * @param replace replacement element
 */
void listRepl(char ***list, const char *search, const char *replace)
{
    int i      = 0;
    char **org = *list;

    if (!replace)
        return;

    if (*list) {
        size_t len = (search ? strlen(search) : 0);

        for (i = 0; (*list)[i]; i++) {
            if (gstrncmp((*list)[i], search, len) == 0) {
                free((*list)[i]);
                (*list)[i] = strdup(replace);
                return;
            }
        }

        *list = realloc(*list, (i + 2) * sizeof(char *));
    } else
        *list = malloc(2 * sizeof(char *));

    if (!*list) {
        *list = org;
        return;
    }

    (*list)[i]     = strdup(replace);
    (*list)[i + 1] = NULL;
}

/**
 * @brief Duplicate a string list (by allocating new memory).
 *
 * @note The list must be NULL-terminated.
 *
 * @param list string list to be duplicated
 *
 * @return duplicated list
 */
char **listDup(const char * const *list)
{
    char **dup = NULL;

    if (list) {
        int i = 0;

        while (list[i])
            i++;

        dup = calloc(i + 1, sizeof(char *));

        if (dup) {
            while (--i >= 0)
                dup[i] = strdup(list[i]);
        }
    }

    return dup;
}

/**
 * @brief Append or insert a file to the playlist.
 *
 * @param what file to be added
 * @param how command (#PLAYLIST_ITEM_APPEND or #PLAYLIST_ITEM_INSERT) to be performed
 *
 * @return #True (ok) or #False (error)
 */
int add_to_gui_playlist(const char *what, int how)
{
    const char *file;
    char *path;
    plItem *item;

    if (!what || !*what || (how != PLAYLIST_ITEM_APPEND && how != PLAYLIST_ITEM_INSERT))
        return False;

    file = mp_basename(what);
    path = strdup(what);

    if (!path)
        return False;

    if (file > what)
        path[file - what - 1] = 0;
    else
        strcpy(path, ".");

    item = calloc(1, sizeof(*item));

    if (!item) {
        free(path);
        return False;
    }

    mp_msg(MSGT_GPLAYER, MSGL_DBG2, "[list] adding %s/%s\n", path, file);

    item->name = strdup(file);
    item->path = path;

    listMgr(how, item);

    return True;
}
