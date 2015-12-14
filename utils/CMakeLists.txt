cmake_minimum_required(VERSION 2.8)

project(Utils)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -std=c++11")
set(CMAKE_VERBOSE_MAKEFILE ON)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)
add_subdirectory(${PROJECT_SOURCE_DIR}/tests/String)
add_subdirectory(${PROJECT_SOURCE_DIR}/tests/FileSystem)
add_subdirectory(${PROJECT_SOURCE_DIR}/tests/System)


