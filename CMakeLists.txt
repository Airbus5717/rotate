cmake_minimum_required(VERSION 3.0.0)
project(rotate VERSION 0.1.0)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS " -Wall -Wextra -O2 -Wcast-align=strict -Wcast-function-type -Wwrite-strings -Wno-builtin-macro-redefined -Winline -Wpedantic -pedantic-errors -DDEBUG_MODE")
#-nodefaultlibs -lc
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(rotate ${SOURCES})
