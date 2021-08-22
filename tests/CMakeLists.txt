add_executable(tests main.c aux_tests.c cunit_tests.c)
target_link_libraries(tests cunit)
add_test(NAME tests COMMAND tests cunit_tests)
