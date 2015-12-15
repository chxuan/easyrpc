set(STATICLIBNAME thrift_wrapper)
set(SHAREDLIBNAME thrift_wrapper_so)
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/lib")

aux_source_directory(. DIR_SRCS)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories($ENV{THRIFT_INCLUDE_PATH})
include_directories($ENV{BOOST_INCLUDE_PATH})

link_directories($ENV{THRIFT_LIB_PATH})
link_directories($ENV{BOOST_LIB_PATH})

add_library(${STATICLIBNAME} STATIC ${DIR_SRCS})
add_library(${SHAREDLIBNAME} SHARED ${DIR_SRCS})

set_target_properties(${SHAREDLIBNAME} PROPERTIES OUTPUT_NAME ${STATICLIBNAME})
set_target_properties(${SHAREDLIBNAME} PROPERTIES VERSION 1.1.0 SOVERSION 1)

target_link_libraries(${SHAREDLIBNAME} -Wl,--whole-archive -lthrift -lboost_serialization -Wl,--no-whole-archive)

