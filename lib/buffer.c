/*
Copyright (c) 2021 Sergey "SnakE" Gromov.
Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#include "buffer.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

void buf_init(struct buffer* buf)
{
    buf->buf_size = 32;
    buf->buf_pos = 0;
    buf->buf = malloc(buf->buf_size);
}

char* buf_free_ptr(const struct buffer* buf)
{
    return buf->buf + buf->buf_pos;
}

size_t buf_free_size(const struct buffer* buf)
{
    return buf->buf_size - buf->buf_pos;
}

void buf_advance(struct buffer* buf, size_t read_size)
{
    buf->buf_pos += read_size;
    assert(buf->buf_pos <= buf->buf_size);

    if (buf->buf_pos == buf->buf_size) {
        buf->buf_size *= 2;
        buf->buf = realloc(buf->buf, buf->buf_size);
    }
}

void buf_write(struct buffer* buf, char* p, size_t size)
{
    while (size) {
        size_t cp = buf_free_size(buf);
        if (cp > size)
            cp = size;
        memcpy(buf_free_ptr(buf), p, cp);
        buf_advance(buf, cp);
        p += cp;
        size -= cp;
    }
}

void buf_putc(struct buffer* buf, int c)
{
    assert(buf->buf_pos < buf->buf_size);
    buf->buf[buf->buf_pos] = (char)c;
    buf_advance(buf, 1);
}

char* buf_release(struct buffer* buf)
{
    char* result = realloc(buf->buf, buf->buf_pos);
    buf->buf = NULL;
    return result;
}
