/*
Copyright (c) 2021 Sergey "SnakE" Gromov.
Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#include <cunit.h>

extern struct cunit_fixture aux_tests;
extern struct cunit_fixture cunit_tests;

const char* self_command;

static const struct cunit_named_fixture all_fixtures[] = {
    CUNIT_FIXTURE(aux_tests),
    CUNIT_FIXTURE(cunit_tests),
    NULL
};

int main(int argc, const char* argv[])
{
    self_command = argv[0];
    return cunit_main(argc, argv, all_fixtures);
}
