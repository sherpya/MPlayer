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

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

#include <windows.h>
#include "glob.h"

union FIND_DATA {
    WIN32_FIND_DATAA ansi;
    WIN32_FIND_DATAW wide;
};

static int is_wide = 1;

#ifdef __GNUC__
__attribute__((constructor)) static void detectOS(void) {
    OSVERSIONINFOA osv;
    memset(&osv, 0, sizeof(osv));
    osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

    if ((GetVersionEx((LPOSVERSIONINFOA) &osv)) && (osv.dwPlatformId != VER_PLATFORM_WIN32_NT))
        is_wide = 0;
}
#endif

static HANDLE FindFirstFileCompat(const char *pattern, union FIND_DATA *lpFindFileData) {
    wchar_t pattern_w[MAX_PATH];
    HANDLE handle;

    if (!is_wide)
        return FindFirstFileA(pattern, &lpFindFileData->ansi);

    if (!MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, pattern, -1, pattern_w, sizeof(pattern_w) / sizeof(*pattern_w)))
        return INVALID_HANDLE_VALUE;

    handle = FindFirstFileW(pattern_w, &lpFindFileData->wide);
    if (handle != INVALID_HANDLE_VALUE) {
        char filename[MAX_PATH];
        if (WideCharToMultiByte(CP_UTF8, 0, lpFindFileData->wide.cFileName, -1, filename, MAX_PATH, NULL, NULL))
            strncpy(lpFindFileData->ansi.cFileName, filename, MAX_PATH);
        else {
            FindClose(handle);
            handle = INVALID_HANDLE_VALUE;
        }
    }

    return handle;
}

static BOOL FindNextFileCompat(HANDLE hFindHandle, union FIND_DATA *lpFindFileData) {
    if (!is_wide)
        return FindNextFileA(hFindHandle, &lpFindFileData->ansi);

    if (!FindNextFileW(hFindHandle, &lpFindFileData->wide))
        return FALSE;

    char filename[MAX_PATH];
    if (!WideCharToMultiByte(CP_UTF8, 0, lpFindFileData->wide.cFileName, -1, filename, MAX_PATH, NULL, NULL))
        return FALSE;

    strncpy(lpFindFileData->ansi.cFileName, filename, MAX_PATH);

    return TRUE;
}


int glob(const char *pattern, int flags,
         int (*errfunc)(const char *epath, int eerrno), glob_t *pglob)
{
    HANDLE searchhndl;
    union FIND_DATA found_file;
    if (errfunc)
        printf("glob():ERROR:Sorry errfunc not supported by this implementation\n");
    if (flags)
        printf("glob():ERROR:Sorry no flags supported by this globimplementation\n");
    //printf("PATTERN \"%s\"\n",pattern);
    pglob->gl_pathc = 0;
    searchhndl      = FindFirstFileCompat(pattern, &found_file);
    if (searchhndl == INVALID_HANDLE_VALUE) {
        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            pglob->gl_pathc = 0;
            //printf("could not find a file matching your search criteria\n");
            return 1;
        } else {
            //printf("glob():ERROR:FindFirstFile: %i\n",GetLastError());
            return 1;
        }
    }
    pglob->gl_pathv    = malloc(sizeof(char *));
    pglob->gl_pathv[0] = strdup(found_file.ansi.cFileName);
    pglob->gl_pathc++;
    while (1) {
        if (!FindNextFileCompat(searchhndl, &found_file)) {
            if (GetLastError() == ERROR_NO_MORE_FILES) {
                //printf("glob(): no more files found\n");
                break;
            } else {
                //printf("glob():ERROR:FindNextFile:%i\n",GetLastError());
                return 1;
            }
        } else {
            //printf("glob: found file %s\n",found_file.cFileName);
            pglob->gl_pathc++;
            pglob->gl_pathv = realloc(pglob->gl_pathv, pglob->gl_pathc * sizeof(char *));
            pglob->gl_pathv[pglob->gl_pathc - 1] = strdup(found_file.ansi.cFileName);
        }
    }
    FindClose(searchhndl);
    return 0;
}

void globfree(glob_t *pglob)
{
    int i;
    for (i = 0; i < pglob->gl_pathc; i++)
        free(pglob->gl_pathv[i]);
    free(pglob->gl_pathv);
}

#if 0
int main(void)
{
    glob_t gg;
    printf("globtest\n");
    glob("*.jpeg", 0, NULL, &gg);
    {
        int i;
        for (i = 0; i < gg.gl_pathc; i++)
            printf("GLOBED:%i %s\n", i, gg.gl_pathv[i]);
    }
    globfree(&gg);

    return 0;
}

#endif
