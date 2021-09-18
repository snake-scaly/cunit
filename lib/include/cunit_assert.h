/*
Copyright (c) 2021 Sergey "SnakE" Gromov.
Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#ifndef CUNIT_ASSERT_H
#define CUNIT_ASSERT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CUNIT_ASSERT(condition) \
do {                            \
    if (!(condition)) {         \
        printf("Assert: %s at %s:%d\n", #condition, __FILE__, __LINE__); \
        exit(1);                \
    }                           \
} while (0)

#define CUNIT_ASSERT_EQ_D(expected, actual) \
do {                                        \
    if ((actual) != (expected)) {           \
        printf("Assert: expected %d, actual %d at %s:%d\n", expected, actual, __FILE__, __LINE__); \
        exit(1);                            \
    }                                       \
} while (0)

#define CUNIT_ASSERT_EQ_STR(expected, actual) \
do {                                          \
    if (strcmp(actual, expected) != 0) {      \
        printf("Assert: expected `%s`, actual `%s` at %s:%d\n", expected, actual, __FILE__, __LINE__); \
        exit(1);                              \
    }                                         \
} while (0)

#define CUNIT_ASSERT_CONTAINS(expected, actual) \
do {                                            \
    if (!strstr(actual, expected)) {            \
        printf("Assert: `%s` not in `%s` at %s:%d\n", expected, actual, __FILE__, __LINE__); \
        exit(1);                                \
    }                                           \
} while (0)

#endif // CUNIT_ASSERT_H
