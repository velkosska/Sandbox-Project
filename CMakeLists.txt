cmake_minimum_required(VERSION 3.10)
project(sandbox C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

include_directories(include)

add_executable(sandbox
    src/main.c
    src/sandbox.c
    src/isolation.c
    src/limits.c
    src/utils.c
)
add_executable(test_utils tests/test_utils.c src/utils.c)
add_executable(test_limits tests/test_limits.c src/limits.c src/utils.c)
