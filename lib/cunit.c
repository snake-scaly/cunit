/*
Copyright (c) 2021 Sergey "SnakE" Gromov.
Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#include "cunit.h"

#include "debug.h"
#include "process.h"

#define UNAVAILABLE 69

struct test_report
{
    int ran;
    int failed;
};

static const struct cunit_named_test* get_test_by_name(const struct cunit_named_test tests[], const char* name)
{
    const struct cunit_named_test* test;

    for (test = tests; test->name; test++) {
        if (strcmp(test->name, name) == 0)
            return test;
    }

    return NULL;
}

static const struct cunit_fixture* get_fixture_by_name(const struct cunit_named_fixture fixtures[], const char* name)
{
    const struct cunit_named_fixture* ref;

    for (ref = fixtures; ref->name; ref++) {
        if (strcmp(ref->name, name) == 0)
            return ref->fixture;
    }

    return NULL;
}

static int host_test_by_name(
    const struct cunit_named_fixture fixtures[],
    const char* fixture_name,
    const char* test_name)
{
    const struct cunit_fixture* fixture;
    const struct cunit_named_test* test;

    fixture = get_fixture_by_name(fixtures, fixture_name);
    if (!fixture)
        return 0;

    test = get_test_by_name(fixture->tests, test_name);
    if (!test)
        return 0;

    if (fixture->setup)
        fixture->setup();
    test->func();
    if (fixture->teardown)
        fixture->teardown();

    return 1;
}

static int host_test_by_compound_name(const struct cunit_named_fixture fixtures[], const char* name)
{
    char* name_dup;
    char* separator;
    int found = 0;

    name_dup = strdup(name);
    separator = strchr(name_dup, '.');
    if (!separator)
        goto out;
    *separator = 0;

    found = host_test_by_name(fixtures, name_dup, separator + 1);

out:
    free(name_dup);
    return found;
}

static int run_test_in_subprocess(const char* test_name, const char* self, struct test_report* report)
{
    char* output;
    int status = execute_process(&output, self, "--host", test_name, NULL);

    if (status == UNAVAILABLE) {
        printf("Test not found: '%s'\n", test_name);
    } else {
        printf("%s: %s\n", test_name, status == 0 ? "success" : "failure");
        printf("%s", output);
    }

    free(output);

    report->ran++;
    if (status != 0)
        report->failed++;

    return status != UNAVAILABLE;
}

static int run_fixture(
    const char* fixture_name,
    const struct cunit_named_fixture fixtures[],
    const char* self,
    struct test_report* report)
{
    const struct cunit_fixture* fixture = get_fixture_by_name(fixtures, fixture_name);
    const struct cunit_named_test* test;
    char test_name[256];

    if (!fixture) {
        printf("Fixture not found: '%s'\n", fixture_name);
        return 0;
    }

    for (test = fixture->tests; test->name; test++) {
        sprintf_s(test_name, sizeof(test_name), "%s.%s", fixture_name, test->name);
        run_test_in_subprocess(test_name, self, report);
    }

    return 1;
}

static int run_command_line_item(
    const char* name,
    const struct cunit_named_fixture fixtures[],
    const char* self,
    struct test_report* report)
{
    if (strchr(name, '.')) {
        return run_test_in_subprocess(name, self, report);
    } else {
        return run_fixture(name, fixtures, self, report);
    }
}

static void list_tests(const struct cunit_named_fixture* fixtures)
{
    const struct cunit_named_fixture* nf;
    for (nf = fixtures; nf->name; nf++) {
        const struct cunit_named_test* nt;
        for (nt = nf->fixture->tests; nt->name; nt++) {
            printf("%s.%s\n", nf->name, nt->name);
        }
    }
}

static const char* filename(const char* path)
{
    const char* fwd = strrchr(path, '/');
    const char* bak = strrchr(path, '\\');
    const char* sep = max(fwd, bak);
    return sep ? sep + 1 : path;
}

static void print_help(const char* self)
{
    printf(
        "Usage: %s [OPTION]... [FIXTURE]... [TEST]...\n"
        "A suite of unit tests.\n"
        "\n"
        "  -l, --list   list all tests in the suite\n"
        "  --host TEST  run a single TEST in-process\n"
        "  -h, --help   print this help message\n"
        "\n"
        "Executing without options runs all tests.\n"
        "\n"
        "The --host option is used internally to run each test in a separate process.\n"
        "It can also be used to debug an individual test.\n",
        filename(self));
}

int cunit_main(int argc, const char* argv[], const struct cunit_named_fixture fixtures[])
{
    struct test_report report = {0};

    if (argc == 2 && (strcmp(argv[1], "--list") == 0 || strcmp(argv[1], "-l") == 0)) {
        list_tests(fixtures);
        return 0;
    } else if (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)) {
        print_help(argv[0]);
        return 0;
    } else if (argc == 3 && strcmp(argv[1], "--host") == 0) {
        if (!host_test_by_compound_name(fixtures, argv[2])) {
            printf("Test not found: '%s'\n", argv[2]);
            return UNAVAILABLE;
        }
    } else if (argc == 1) {
        const struct cunit_named_fixture* fixture;
        for (fixture = fixtures; fixture->name; fixture++)
            run_fixture(fixture->name, fixtures, argv[0], &report);
        printf("%d ran, %d failed\n", report.ran, report.failed);
    } else {
        int i;
        for (i = 1; i < argc; i++)
            if (!run_command_line_item(argv[i], fixtures, argv[0], &report))
                return UNAVAILABLE;
        printf("%d ran, %d failed\n", report.ran, report.failed);
    }

    return check_memory_leaks() || report.failed ? 1 : 0;
}
