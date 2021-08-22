/*
Copyright (c) 2021 Sergey "SnakE" Gromov.
Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#include "process.h"

#include "buffer.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static void append_popen_param(char* buffer, size_t buffer_size, const char* param)
{
    strcat_s(buffer, buffer_size, "\"");
    strcat_s(buffer, buffer_size, param);
    strcat_s(buffer, buffer_size, "\" ");
}

static void build_popen_command(char* buffer, size_t buffer_size, va_list ap)
{
    /* In Windows, if popen finds that the command starts and ends with quotes
       it strips them unconditionally. This breaks quotation if there are multiple
       arguments with the first and last arguments quoted. Adding an extra pair of
       quotes around the whole command works around this issue. */

    const char* param;

    assert(buffer_size > 2);
    buffer[0] = '"';
    buffer[1] = 0;

    while ((param = va_arg(ap, const char*)))
        append_popen_param(buffer, buffer_size, param);

    strcat_s(buffer, buffer_size, "2>&1\"");
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
int execute_process_popen(char **output, ...)
{
    char command[1024];
    va_list ap;
    FILE* pipe;

    va_start(ap, output);
    build_popen_command(command, sizeof(command), ap);
    va_end(ap);

    pipe = _popen(command, "rt");
    *output = read_whole_stream(pipe);
    return _pclose(pipe);
}
