set(OUTPUTNAME log_wrapper)
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/lib")

aux_source_directory(. DIR_SRCS)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../utils/include)
include_directories($ENV{LOG4CPP_INCLUDE_PATH})

add_library(${OUTPUTNAME} STATIC ${DIR_SRCS})

