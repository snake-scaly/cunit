/*
Copyright (c) 2021 Sergey "SnakE" Gromov.
Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#ifndef CUNIT_H
#define CUNIT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CUNIT_TEST(name) { #name, &name }
#define CUNIT_FIXTURE(name) { #name, &name }

#define ASSERT(condition) \
do {                      \
    if (!(condition)) {   \
        printf("ASSERT: %s at %s:%d\n", #condition, __FILE__, __LINE__); \
        exit(1);          \
    }                     \
} while (0)

#define ASSERT_EQ_D(actual, expected) \
do {                                  \
    if ((actual) != (expected)) {     \
        printf("ASSERT_EQ_D: expected %d, actual %d at %s:%d\n", expected, actual, __FILE__, __LINE__); \
        exit(1);                      \
    }                                 \
} while (0)

#define ASSERT_STRCMP(actual, expected) \
do {                                    \
    if (strcmp(actual, expected) != 0) { \
        printf("ASSERT_STRCMP: expected `%s`, actual `%s` at %s:%d\n", expected, actual, __FILE__, __LINE__); \
        exit(1);                        \
    }                                   \
} while (0)

#define ASSERT_STRSTR(actual, expected) \
do {                                    \
    if (!strstr(actual, expected)) {    \
        printf("ASSERT_STRSTR: `%s` not in `%s` at %s:%d\n", expected, actual, __FILE__, __LINE__); \
        exit(1);                        \
    }                                   \
} while (0)

struct cunit_named_test
{
    const char* name;
    void (*func)();
};

struct cunit_fixture
{
    void (*setup)();
    void (*teardown)();
    struct cunit_named_test tests[];
};

struct cunit_named_fixture
{
    const char* name;
    const struct cunit_fixture* fixture;
};

int cunit_main(int argc, const char* argv[], const struct cunit_named_fixture fixtures[]);

#endif /* CUNIT_H */
