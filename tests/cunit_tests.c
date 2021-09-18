/*
Copyright (c) 2021 Sergey "SnakE" Gromov.
Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#include <cunit.h>
#include "../lib/process.h"

extern const char* self_command;

static void test_run_isolated()
{
    int status;
    char* output;

    status = execute_process(&output, self_command, "--host", "aux_tests.test_with_global_state", NULL);
    CUNIT_ASSERT_CONTAINS("global state ok", output);
    CUNIT_ASSERT_EQ_D(0, status);
    free(output);

    status = execute_process(&output, self_command, "--host", "aux_tests.test_with_global_state", NULL);
    CUNIT_ASSERT_CONTAINS("global state ok", output);
    CUNIT_ASSERT_EQ_D(0, status);
    free(output);
}

static void test_fails_for_unknown_test()
{
    char* output;
    int status = execute_process(&output, self_command, "--host", "aux_tests.gibberish", NULL);
    CUNIT_ASSERT(status != 0);
    free(output);
}

static void test_can_run_single_test()
{
    char* output;
    int status = execute_process(&output, self_command, "aux_tests.dummy_test", NULL);
    CUNIT_ASSERT_CONTAINS("aux_tests.dummy_test: success", output);
    CUNIT_ASSERT_EQ_D(0, status);
    free(output);
}

static void test_produce_report_for_single_test()
{
    char* output;
    int status = execute_process(&output, self_command, "aux_tests.dummy_test", NULL);
    CUNIT_ASSERT_CONTAINS("1 ran, 0 failed", output);
    CUNIT_ASSERT(status == 0);
    free(output);
}

static void test_produce_report_for_failing_test()
{
    char* output;
    int status = execute_process(&output, self_command, "aux_tests.failing_test", NULL);
    CUNIT_ASSERT_CONTAINS("aux_tests.failing_test: failure", output);
    CUNIT_ASSERT_CONTAINS("1 ran, 1 failed", output);
    CUNIT_ASSERT(status != 0);
    free(output);
}

static void test_run_multiple_individual_tests()
{
    char* output;
    int status = execute_process(&output, self_command, "aux_tests.dummy_test", "aux_tests.failing_test", NULL);
    CUNIT_ASSERT_CONTAINS("aux_tests.dummy_test: success", output);
    CUNIT_ASSERT_CONTAINS("aux_tests.failing_test: failure", output);
    CUNIT_ASSERT_CONTAINS("2 ran, 1 failed", output);
    CUNIT_ASSERT(status != 0);
    free(output);
}

static void test_report_error_for_unknown_test_in_a_list()
{
    char* output;
    int status = execute_process(&output, self_command, "aux_tests.dummy_test", "aux_tests.gibberish", NULL);
    CUNIT_ASSERT_CONTAINS("aux_tests.dummy_test: success", output);
    CUNIT_ASSERT_CONTAINS("Test not found: 'aux_tests.gibberish'", output);
    CUNIT_ASSERT(!strstr(output, " ran, "));
    CUNIT_ASSERT(status != 0);
    free(output);
}

static void test_run_all_tests_from_named_fixture()
{
    char* output;
    int status = execute_process(&output, self_command, "aux_tests", NULL);
    CUNIT_ASSERT_CONTAINS("aux_tests.test_with_global_state: success", output);
    CUNIT_ASSERT_CONTAINS("aux_tests.dummy_test: success", output);
    CUNIT_ASSERT_CONTAINS("aux_tests.failing_test: failure", output);
    CUNIT_ASSERT_CONTAINS("3 ran, 1 failed", output);
    CUNIT_ASSERT(status != 0);
    free(output);
}

static void test_fails_for_unknown_fixture()
{
    char* output;
    int status = execute_process(&output, self_command, "gibberish", NULL);
    CUNIT_ASSERT_CONTAINS("Fixture not found: 'gibberish'", output);
    CUNIT_ASSERT(status != 0);
    free(output);
}

static void test_mix_tests_and_fixtures()
{
    char* output;
    int status =
        execute_process(
            &output,
            self_command,
            "cunit_tests.test_fails_for_unknown_fixture",
            "aux_tests",
            "cunit_tests.test_can_run_single_test",
            NULL);

    CUNIT_ASSERT_CONTAINS("cunit_tests.test_fails_for_unknown_fixture: success", output);
    CUNIT_ASSERT_CONTAINS("aux_tests.test_with_global_state: success", output);
    CUNIT_ASSERT_CONTAINS("aux_tests.dummy_test: success", output);
    CUNIT_ASSERT_CONTAINS("aux_tests.failing_test: failure", output);
    CUNIT_ASSERT_CONTAINS("cunit_tests.test_can_run_single_test: success", output);
    CUNIT_ASSERT_CONTAINS("5 ran, 1 failed", output);
    CUNIT_ASSERT(status != 0);

    free(output);
}

static void test_setup_runs_before_test_teardown_runs_after()
{
    char* output;
    int status = execute_process(&output, self_command, "aux_tests.dummy_test", NULL);
    CUNIT_ASSERT_CONTAINS("\n1. setup\n2. testcase\n3. teardown\n", output);
    CUNIT_ASSERT_EQ_D(0, status);
    free(output);
}

struct cunit_fixture cunit_tests = {
    NULL,
    NULL,
    {
        CUNIT_TEST(test_run_isolated),
        CUNIT_TEST(test_fails_for_unknown_test),
        CUNIT_TEST(test_can_run_single_test),
        CUNIT_TEST(test_produce_report_for_single_test),
        CUNIT_TEST(test_produce_report_for_failing_test),
        CUNIT_TEST(test_run_multiple_individual_tests),
        CUNIT_TEST(test_report_error_for_unknown_test_in_a_list),
        CUNIT_TEST(test_run_all_tests_from_named_fixture),
        CUNIT_TEST(test_fails_for_unknown_fixture),
        CUNIT_TEST(test_mix_tests_and_fixtures),
        CUNIT_TEST(test_setup_runs_before_test_teardown_runs_after),
        NULL
    }
};
