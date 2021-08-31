CUnit
=====

A unit-testing framework for C, in C.

This was my exercise in TDD following Kent Beck's xUnit example in Test
Driven Development By Example.

Features
--------

* The framework is a C library. Only a compiler is required.
* A test suite compiles into a self-contained executable.
* Minimal use of macros for test definitions. Macros can be avoided
  completely if desired.
* Each test runs in a separate process. This allows tests to use global
  variables for fixturing. Any crashes or memory leaks are reported as
  failures.

Bugs
----

* Running each test in a separate process is relatively slow in Windows.
  This is less of a problem in Linux.
* Automatic memory leak detection only works in Windows. You'll have to use
  mtrace or valgrind in Linux.

Basic Usage
-----------

```c
#include <cunit.h>

static void test_feature1()
{
    ASSERT_EQ_D(2 + 2, 4);
}

static void test_succeeds()
{
    const char* good_output = "feature test";
    ASSERT_STRSTR(good_output, "ure t");
}

static void test_willfail()
{
    const char* bad_output = "wrong output";
    ASSERT_STRSTR(bad_output, "bad");
}

struct cunit_fixture example_tests = {
    NULL,
    NULL,
    {
        CUNIT_TEST(test_feature1),
        CUNIT_TEST(test_succeeds),
        CUNIT_TEST(test_willfail),
        NULL
    }
};

struct cunit_named_fixture all_fixtures[] = {
    CUNIT_FIXTURE(example_tests),
    NULL
};

int main(int argc, const char** argv)
{
    return cunit_main(argc, argv, all_fixtures);
}
```

Building and running this program will produce the following output:

```
$ ./example
example_tests.test_feature1: success
example_tests.test_succeeds: success
example_tests.test_willfail: failure
ASSERT_STRSTR: `bad` not in `wrong output` at D:\snake\projects\cunit\example.c:17
3 ran, 1 failed
$ ./example --list
example_tests.test_feature1
example_tests.test_succeeds
example_tests.test_willfail
```

SetUp, TearDown, Leak Detection
-------------------------------

```c
#include <cunit.h>

static char* value;

static void setup()
{
    value = strdup("some string");
}

static void teardown()
{
    free(value);
}

static void test_fails_on_memory_leak()
{
    value = NULL;
}

static void test_succeeds()
{
    ASSERT_STRCMP(value, "some string");
}

struct cunit_fixture example_tests = {
    setup,
    teardown,
    {
        CUNIT_TEST(test_fails_on_memory_leak),
        CUNIT_TEST(test_succeeds),
        NULL
    }
};

struct cunit_named_fixture all_fixtures[] = {
    CUNIT_FIXTURE(example_tests),
    NULL
};

int main(int argc, const char** argv)
{
    return cunit_main(argc, argv, all_fixtures);
}
```

Output of a test run. Note that the second test succeeds even though the
first test breaks setup.

```
$ ./example
example_tests.test_fails_on_memory_leak: failure
Detected memory leaks!
Dumping objects ->
{107} normal block at 0x00D071D8, 12 bytes long.
 Data: <some string > 73 6F 6D 65 20 73 74 72 69 6E 67 00
Object dump complete.
example_tests.test_succeeds: success
2 ran, 1 failed
```
