#include "winerr.h"

#include <stdio.h>
/*
Copyright (c) 2021 Sergey "SnakE" Gromov.
Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#include <windows.h>

void log_win_error(const char* context)
{
    DWORD err;
    char* errmsg;
    DWORD fm_result;

    err = GetLastError();

    fm_result = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        err,
        0,
        (LPSTR)&errmsg,
        0,
        NULL);

    if (fm_result != 0) {
        fprintf(stderr, "%s: (0x%08lX) %s\n", context, err, errmsg);
        LocalFree(errmsg);
    } else {
        DWORD fmterr = GetLastError();
        fprintf(stderr, "%s: 0x%08lX\n", context, err);
        fprintf(stderr, "Failed to get error message: 0x%08lX", fmterr);
    }
}
