cmake_minimum_required(VERSION 3.29)
project(Module11 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Module11
    WhittemoreDataLoader.c
    WhittemoreDataLoader.h
    WhittemorePageTable.c
    WhittemorePageTable.h
    Simulator.c)
