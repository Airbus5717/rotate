cmake_minimum_required(VERSION 3.0.0)
project(rotate VERSION 0.1.0)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 11)
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(vr ${SOURCES})

set(BUILD_SHARED_LIBS OFF)

