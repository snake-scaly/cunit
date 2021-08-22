/*
Copyright (c) 2021 Sergey "SnakE" Gromov.
Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#ifndef CUNIT_BUFFER_H
#define CUNIT_BUFFER_H

#include <stddef.h>

struct buffer
{
    char* buf;
    size_t buf_size;
    size_t buf_pos;
};

void buf_init(struct buffer* buf);
char* buf_free_ptr(const struct buffer* buf);
size_t buf_free_size(const struct buffer* buf);
void buf_advance(struct buffer* buf, size_t read_size);
void buf_write(struct buffer* buf, char* p, size_t size);
void buf_putc(struct buffer* buf, int c);
char* buf_release(struct buffer* buf);

#endif /* CUNIT_BUFFER_H */
