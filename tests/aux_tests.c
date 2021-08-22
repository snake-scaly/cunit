/*
Copyright (c) 2021 Sergey "SnakE" Gromov.
Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#include <cunit.h>

#include <stdio.h>

static int global_state;
static int fixture_step;

static void setup()
{
    fixture_step++;
    printf("%d. setup\n", fixture_step);
    fflush(stdout);
}

static void teardown()
{
    fixture_step++;
    printf("%d. teardown\n", fixture_step);
    fflush(stdout);
}

static void test_with_global_state()
{
    global_state++;
    ASSERT_EQ_D(global_state, 1);
    printf("global state ok\n");
}

static void dummy_test()
{
    fixture_step++;
    fprintf(stderr, "%d. testcase\n", fixture_step);
    fflush(stderr); /* avoid reordering stdout and stderr messages */
}

static void failing_test()
{
    ASSERT(0);
}

struct cunit_fixture aux_tests = {
    setup,
    teardown,
    {
        CUNIT_TEST(test_with_global_state),
        CUNIT_TEST(dummy_test),
        CUNIT_TEST(failing_test),
        NULL
    }
};
