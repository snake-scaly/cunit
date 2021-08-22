/*
Copyright (c) 2021 Sergey "SnakE" Gromov.
Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#ifndef CUNIT_PROCESS_H
#define CUNIT_PROCESS_H

int execute_process_createprocess(char **output, ...);
int execute_process_popen(char **output, ...);

#define execute_process execute_process_createprocess

#endif /* CUNIT_PROCESS_H */
