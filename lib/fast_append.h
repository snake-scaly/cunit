/*
Copyright (c) 2021 Sergey "SnakE" Gromov.
Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#ifndef CUNIT_FAST_APPEND_H
#define CUNIT_FAST_APPEND_H

#include <stddef.h>

/*
Copies str to *dest but never exceeds *destsz. Guarantees null termination.
Modifies *dest and *destsz to point to the terminating null character to facilitate
chain appending.
*/
void fast_append(char** dest, size_t* destsz, const char* str);

#endif // CUNIT_FAST_APPEND_H
