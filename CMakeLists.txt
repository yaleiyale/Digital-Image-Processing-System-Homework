cmake_minimum_required(VERSION 3.17)
project(dip)

set(CMAKE_CXX_STANDARD 20)

add_executable(dip main.cpp handle.h imgstruct.h openimg.h writeimg.h)