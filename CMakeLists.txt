cmake_minimum_required(VERSION 3.0.0)
project(rotate VERSION 0.1.0)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -march=native -mtune=native -g -Wconversion -O2")

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(vr ${SOURCES})

