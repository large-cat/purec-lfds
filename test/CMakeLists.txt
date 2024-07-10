cmake_minimum_required(VERSION 3.10)
project(lfds-test)

# 设置C++标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/libshared/inc)
include_directories(${PROJECT_SOURCE_DIR}/test/inc)

file(GLOB FILES libshared_src libshared/src/*/*.c)

add_executable(test_misc
    ${libshared_src}
    test/src/libtest_misc/libtest_misc_pal_helpers.c
    test/src/libtest_misc/libtest_misc_determine_erg.c
    test/src/libtest_misc/libtest_misc_globals.c
    test/src/libtest_misc/libtest_misc_query.c
    test/src/libtest_misc/libtest_misc_memory_helpers.c
)

target_link_libraries(test_misc purec-lfds)