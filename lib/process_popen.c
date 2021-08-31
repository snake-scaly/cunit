/*
Copyright (c) 2021 Sergey "SnakE" Gromov.
Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#include "process.h"

#include "buffer.h"
#include "fast_append.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <wait.h>

static void append_popen_param(char** dest, size_t* destsz, const char* param)
{
    fast_append(dest, destsz, "\"");
    fast_append(dest, destsz, param);
    fast_append(dest, destsz, "\" ");
}

static void build_popen_command(char* buffer, size_t buffer_size, va_list ap)
{
    const char* param;
    while ((param = va_arg(ap, const char*)))
        append_popen_param(&buffer, &buffer_size, param);
    fast_append(&buffer, &buffer_size, "2>&1");
}

static char* read_whole_stream(FILE* file)
{
    struct buffer buf;
    buf_init(&buf);

    for (;;) {
        size_t read_size;
        assert(buf_free_size(&buf) > 0);
        read_size = fread(buf_free_ptr(&buf), 1, buf_free_size(&buf), file);
        if (read_size == 0)
            break;
        buf_advance(&buf, read_size);
    }

    buf_putc(&buf, 0);
    return buf_release(&buf);
}

/* Twice as slow as CreateProcess in Windows. */
int execute_process(char **output, ...)
{
    char command[1024];
    va_list ap;
    FILE* pipe;

    va_start(ap, output);
    build_popen_command(command, sizeof(command), ap);
    va_end(ap);

    pipe = popen(command, "r");
    *output = read_whole_stream(pipe);
    return WEXITSTATUS(pclose(pipe));
}
