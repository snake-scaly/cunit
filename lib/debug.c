/*
Copyright (c) 2021 Sergey "SnakE" Gromov.
Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#include "debug.h"

#include "config.h"

#ifdef HAVE_CRTDBG
#include <crtdbg.h>
#endif

int check_memory_leaks() {
#ifdef HAVE_CRTDBG
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    return _CrtDumpMemoryLeaks();
#else
    return 0;
#endif
}
