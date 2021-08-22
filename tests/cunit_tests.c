/*
Copyright (c) 2021 Sergey "SnakE" Gromov.
Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#include <cunit.h>
#include <process.h>

extern const char* self_command;

static void test_run_isolated()
{
    int status;
    char* output;

    status = execute_process(&output, self_command, "--host", "aux_tests.test_with_global_state", NULL);
    ASSERT_STRSTR(output, "global state ok");
    ASSERT_EQ_D(status, 0);
    free(output);

    status = execute_process(&output, self_command, "--host", "aux_tests.test_with_global_state", NULL);
    ASSERT_STRSTR(output, "global state ok");
    ASSERT_EQ_D(status, 0);
    free(output);
}

static void test_fails_for_unknown_test()
{
    char* output;
    int status = execute_process(&output, self_command, "--host", "aux_tests.gibberish", NULL);
    ASSERT(status != 0);
    free(output);
}

static void test_can_run_single_test()
{
    char* output;
    int status = execute_process(&output, self_command, "aux_tests.dummy_test", NULL);
    ASSERT_STRSTR(output, "aux_tests.dummy_test: success");
    ASSERT_EQ_D(status, 0);
    free(output);
}

static void test_produce_report_for_single_test()
{
    char* output;
    int status = execute_process(&output, self_command, "aux_tests.dummy_test", NULL);
    ASSERT_STRSTR(output, "1 ran, 0 failed");
    ASSERT(status == 0);
    free(output);
}

static void test_produce_report_for_failing_test()
{
    char* output;
    int status = execute_process(&output, self_command, "aux_tests.failing_test", NULL);
    ASSERT_STRSTR(output, "aux_tests.failing_test: failure");
    ASSERT_STRSTR(output, "1 ran, 1 failed");
    ASSERT(status != 0);
    free(output);
}

static void test_run_multiple_individual_tests()
{
    char* output;
    int status = execute_process(&output, self_command, "aux_tests.dummy_test", "aux_tests.failing_test", NULL);
    ASSERT_STRSTR(output, "aux_tests.dummy_test: success");
    ASSERT_STRSTR(output, "aux_tests.failing_test: failure");
    ASSERT_STRSTR(output, "2 ran, 1 failed");
    ASSERT(status != 0);
    free(output);
}

static void test_report_error_for_unknown_test_in_a_list()
{
    char* output;
    int status = execute_process(&output, self_command, "aux_tests.dummy_test", "aux_tests.gibberish", NULL);
    ASSERT_STRSTR(output, "aux_tests.dummy_test: success");
    ASSERT_STRSTR(output, "Test not found: 'aux_tests.gibberish'");
    ASSERT(!strstr(output, " ran, "));
    ASSERT(status != 0);
    free(output);
}

static void test_run_all_tests_from_named_fixture()
{
    char* output;
    int status = execute_process(&output, self_command, "aux_tests", NULL);
    ASSERT_STRSTR(output, "aux_tests.test_with_global_state: success");
    ASSERT_STRSTR(output, "aux_tests.dummy_test: success");
    ASSERT_STRSTR(output, "aux_tests.failing_test: failure");
    ASSERT_STRSTR(output, "3 ran, 1 failed");
    ASSERT(status != 0);
    free(output);
}

static void test_fails_for_unknown_fixture()
{
    char* output;
    int status = execute_process(&output, self_command, "gibberish", NULL);
    ASSERT_STRSTR(output, "Fixture not found: 'gibberish'");
    ASSERT(status != 0);
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

    ASSERT_STRSTR(output, "cunit_tests.test_fails_for_unknown_fixture: success");
    ASSERT_STRSTR(output, "aux_tests.test_with_global_state: success");
    ASSERT_STRSTR(output, "aux_tests.dummy_test: success");
    ASSERT_STRSTR(output, "aux_tests.failing_test: failure");
    ASSERT_STRSTR(output, "cunit_tests.test_can_run_single_test: success");
    ASSERT_STRSTR(output, "5 ran, 1 failed");
    ASSERT(status != 0);

    free(output);
}

static void test_setup_runs_before_test_teardown_runs_after()
{
    char* output;
    int status = execute_process(&output, self_command, "aux_tests.dummy_test", NULL);
    ASSERT_STRSTR(output, "\n1. setup\n2. testcase\n3. teardown\n");
    ASSERT_EQ_D(status, 0);
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
