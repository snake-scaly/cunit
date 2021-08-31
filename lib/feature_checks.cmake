include(CheckIncludeFile)

check_include_file(crtdbg.h HAVE_CRTDBG)

configure_file(config.h.in config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
