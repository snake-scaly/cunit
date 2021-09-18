cmake_minimum_required(VERSION 3.13)
project(cunit LANGUAGES C VERSION 1.1)
enable_testing()

add_subdirectory(lib)
add_subdirectory(tests)
