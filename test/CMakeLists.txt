cmake_minimum_required(VERSION 3.10)
project(lfds-test)

# 设置C++标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/libshared/inc)
include_directories(${PROJECT_SOURCE_DIR}/libtest/inc)

file(GLOB libshared_src libshared/src/*/*.c)
file(GLOB libtest_src libtest/src/*/*.c)

add_library(test_pub SHARED
    ${libshared_src}
    ${libtest_src}
)

target_link_libraries(test_pub purec-lfds)