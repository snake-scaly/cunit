/*
Copyright (c) 2021 Sergey "SnakE" Gromov.
Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#ifndef CUNIT_OUT_COLLECTOR_H
#define CUNIT_OUT_COLLECTOR_H

#include <windows.h>

struct out_collector;

struct out_collector* out_collector_create();
HANDLE out_collector_handle(struct out_collector* oc);
char* out_collector_finish(struct out_collector* oc);

#endif /* CUNIT_OUT_COLLECTOR_H */
