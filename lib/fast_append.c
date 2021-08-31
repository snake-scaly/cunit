/*
Copyright (c) 2021 Sergey "SnakE" Gromov.
Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#include "fast_append.h"

void fast_append(char** dest, size_t* destsz, const char* str)
{
    while (*destsz > 1 && *str) {
        *(*dest)++ = *str++;
        (*destsz)--;
    }
    **dest = 0;
}
