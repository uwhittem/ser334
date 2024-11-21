cmake_minimum_required(VERSION 3.29)
project(WhittemoreLoadBalancer C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(WhittemoreLoadBalancer
    InstanceHost-1.h
    LoadBalancer-1.h
    User-1.c
    WhittemoreInstanceHost.c
    WhittemoreLoadBalancer.c)
