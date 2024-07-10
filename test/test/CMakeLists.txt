cmake_minimum_required(VERSION 3.10)
project(lfds-test-main)

# 设置C++标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(lfds-test 
    src/misc.c
    src/main.c
    src/util_cmdline.c
    src/callbacks.c
)

target_link_libraries(lfds-test test_pub purec-lfds)