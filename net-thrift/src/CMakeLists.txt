set(OUTPUTNAME thrift_wrapper)
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/lib")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories($ENV{THRIFT_INCLUDE_PATH})
include_directories($ENV{BOOST_INCLUDE_PATH})

aux_source_directory(. DIR_SRCS)

add_library(${OUTPUTNAME} STATIC ${DIR_SRCS})

