include(feature_checks.cmake)

add_library(cunit cunit.c buffer.c debug.c fast_append.c)

if(WIN32)
    target_sources(cunit PRIVATE process_createprocess.c out_collector.c winerr.c)
else()
    target_sources(cunit PRIVATE process_popen.c)
endif()

target_include_directories(cunit PUBLIC ${CMAKE_CURRENT_BINARY_DIR} INTERFACE .)
