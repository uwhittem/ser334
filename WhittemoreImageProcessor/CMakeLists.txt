cmake_minimum_required(VERSION 3.10)
project(WhittemoreImageProcessor C)

set(CMAKE_C_STANDARD 11)

add_executable(WhittemoreImageProcessor
        WhittemoreImageProcessor.c
        BMPHandler.c
        Image.c
)

target_include_directories(WhittemoreImageProcessor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
