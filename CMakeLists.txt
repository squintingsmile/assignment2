cmake_minimum_required(VERSION 3.12.0)
project(assignment2 C)

set(CMAKE_C_STANDARD 99)

add_executable(assignment2 instructions.c parser.h parser.c main.c instructions.h)