cmake_minimum_required(VERSION 2.8)
project(server)

set(OUTPUTNAME server)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-deprecated -Wno-comment -Wno-unused-local-typedefs -Wno-maybe-uninitialized -Wno-unused-variable -g -O2 -std=c++14")

#add_definitions(-DENABLE_BOOST_SERIALIZATION)
#add_definitions(-DENABLE_MSGPACK)
add_definitions(-DENABLE_JSON)

aux_source_directory(. DIR_SRCS)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/../..)
include_directories(${PROJECT_SOURCE_DIR}/../user_define_classes)
include_directories(${PROJECT_SOURCE_DIR}/../../easyrpc)
include_directories(${PROJECT_SOURCE_DIR}/../../easypack)
include_directories(${PROJECT_SOURCE_DIR}/../../easypack/msgpack)
include_directories(${PROJECT_SOURCE_DIR}/../../easypack/kapok)
include_directories(${PROJECT_SOURCE_DIR}/../../spdlog/include)
include_directories(${PROJECT_SOURCE_DIR}/../googletest/googletest/include)
include_directories($ENV{BOOST_INCLUDE_PATH})

link_directories($ENV{BOOST_LIB_PATH})
link_directories(${PROJECT_SOURCE_DIR}/../googletest/googlemork/gtest)

add_executable(${OUTPUTNAME} ${DIR_SRCS})

target_link_libraries(${OUTPUTNAME} boost_serialization)
target_link_libraries(${OUTPUTNAME} boost_system)
target_link_libraries(${OUTPUTNAME} gtest)
target_link_libraries(${OUTPUTNAME} gtest_main)
target_link_libraries(${OUTPUTNAME} pthread)
