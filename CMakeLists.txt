cmake_minimum_required(VERSION 3.21)
project(sandbox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

add_executable(sandbox main.cpp main.h)
