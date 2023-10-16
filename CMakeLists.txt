cmake_minimum_required(VERSION 3.26)
project(pdc1 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(pdc1
        main.c
        strategies.c
        strategies.h
        analytics.c
        analytics.h)

set(CMAKE_C_COMPILER mpicc)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
