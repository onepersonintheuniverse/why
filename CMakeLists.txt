set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

cmake_minimum_required(VERSION 3.10)
project(whylang)
include_directories(include)
add_executable(why src/why.c)