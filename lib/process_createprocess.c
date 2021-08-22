/*
Copyright (c) 2021 Sergey "SnakE" Gromov.
Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#include "process.h"

#include "out_collector.h"
#include "winerr.h"

#include <assert.h>
#include <stdarg.h>

#include <windows.h>

static void append_command_line_param(char* buffer, size_t buffer_size, const char* param)
{
    strcat_s(buffer, buffer_size, "\"");
    strcat_s(buffer, buffer_size, param);
    strcat_s(buffer, buffer_size, "\" ");
}

static void build_command_line(char* buffer, size_t buffer_size, va_list ap)
{
    const char* param;
    assert(buffer_size > 0);
    buffer[0] = 0;
    while ((param = va_arg(ap, const char*)))
        append_command_line_param(buffer, buffer_size, param);
}

int execute_process_createprocess(char **output, ...)
{
    char command_line[1024];
    va_list ap;
    STARTUPINFOA startupinfo = {sizeof(startupinfo)};
    PROCESS_INFORMATION procinfo;
    struct out_collector* oc;
    BOOL create_process_result;
    DWORD status;

    *output = NULL;

    va_start(ap, output);
    build_command_line(command_line, sizeof(command_line), ap);
    va_end(ap);

    oc = out_collector_create();
    SetHandleInformation(out_collector_handle(oc), HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);

    startupinfo.dwFlags |= STARTF_USESTDHANDLES;
    startupinfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    startupinfo.hStdOutput = out_collector_handle(oc);
    startupinfo.hStdError = out_collector_handle(oc);

    create_process_result = CreateProcessA(
        NULL,
        command_line,
        NULL,
        NULL,
        TRUE,
        STARTF_USESTDHANDLES,
        NULL,
        NULL,
        &startupinfo,
        &procinfo);

    if (create_process_result) {
        WaitForSingleObject(procinfo.hProcess, INFINITE);
        GetExitCodeProcess(procinfo.hProcess, &status);
        CloseHandle(procinfo.hProcess);
        CloseHandle(procinfo.hThread);
    } else {
        log_win_error("CreateProcess");
        status = 69;
    }

    *output = out_collector_finish(oc);
    return (int)status;
}
