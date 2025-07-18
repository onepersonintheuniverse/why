cmake_minimum_required(VERSION 3.10)

project(whylang C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if (BIG_ENDIAN)
    add_compile_definitions(BIG_ENDIAN)
endif()

set(CMAKE_C_FLAGS "-Wall")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_C_FLAGS_MINISIZEREL "-Os -DNDEBUG")
set(CMAKE_C_FLAGS_DEBUG "-O0")

include_directories(include)
add_executable(why src/main.c)
add_executable(betest src/betest.c)
