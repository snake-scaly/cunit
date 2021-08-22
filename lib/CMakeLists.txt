include(feature_checks.cmake)
add_library(cunit cunit.c buffer.c debug.c process_popen.c process_createprocess.c out_collector.c winerr.c)
target_include_directories(cunit PUBLIC ${CMAKE_CURRENT_BINARY_DIR} INTERFACE .)