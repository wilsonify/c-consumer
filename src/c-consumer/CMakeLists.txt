cmake_minimum_required(VERSION 2.8.12)

project(c-consumer)

add_definitions("-std=c++11")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(COMMON_SRCS
    utils.h
    utils.c
    platform_utils.c
    )

add_executable(c-consumer main.c ${COMMON_SRCS})
target_link_libraries(c-consumer ${CONAN_LIBS})
