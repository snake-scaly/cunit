/*
Copyright (c) 2021 Sergey "SnakE" Gromov.
Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#ifndef CUNIT_H
#define CUNIT_H

#include "cunit_assert.h"

#define CUNIT_TEST(name) { #name, &name }
#define CUNIT_FIXTURE(name) { #name, &name }

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
