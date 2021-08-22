/*
Copyright (c) 2021 Sergey "SnakE" Gromov.
Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#include "out_collector.h"

#include "buffer.h"
#include "winerr.h"

struct out_collector
{
    struct buffer oc_buffer;
    HANDLE oc_pipe_read;
    HANDLE oc_pipe_write;
    HANDLE oc_thread;
};

static DWORD WINAPI thread_proc(LPVOID param)
{
    struct out_collector* oc = param;
    char buf[1024];
    char* p = buf;

    for (;;) {
        DWORD read_count;
        BOOL ok = ReadFile(oc->oc_pipe_read, p, buf + sizeof(buf) - p, &read_count, NULL);

        if (!ok) {
            if (GetLastError() != ERROR_BROKEN_PIPE)
                log_win_error("Error calling ReadFile");
            break;
        }

        /* Translate CRLF to LF. Normally this is done by C streams. */
        char* e = p + read_count;
        p = buf;
        for (;;) {
            char* cr = memchr(p, '\r', e - p);

            if (!cr) {
                buf_write(&oc->oc_buffer, p, e - p);
                p = buf;
                break;
            }

            if (cr == e - 1) {
                /* CR at the end of the buffer. Might be followed by a LF in the next chunk. */
                buf[0] = *cr;
                p = buf + 1;
                break;
            }

            buf_write(&oc->oc_buffer, p, cr - p);
            if (cr[1] != '\n')
                buf_putc(&oc->oc_buffer, *cr);
            p = cr + 1;
        }
    }

    return 0;
}

struct out_collector* out_collector_create()
{
    struct out_collector* oc = malloc(sizeof(struct out_collector));
    buf_init(&oc->oc_buffer);
    CreatePipe(&oc->oc_pipe_read, &oc->oc_pipe_write, NULL, 0);
    oc->oc_thread = CreateThread(NULL, 0, thread_proc, oc, 0, NULL);
    return oc;
}

HANDLE out_collector_handle(struct out_collector* oc)
{
    return oc->oc_pipe_write;
}

char* out_collector_finish(struct out_collector* oc)
{
    char* result;

    CloseHandle(oc->oc_pipe_write);

    WaitForSingleObject(oc->oc_thread, INFINITE);

    CloseHandle(oc->oc_thread);
    CloseHandle(oc->oc_pipe_read);

    buf_putc(&oc->oc_buffer, 0);
    result = buf_release(&oc->oc_buffer);

    free(oc);

    return result;
}
