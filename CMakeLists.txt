cmake_minimum_required(VERSION 3.12)
project(high_low C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic -Wall -Werror -Wstrict-prototypes -ansi -fno-math-errno")

add_executable(high_low main.c)