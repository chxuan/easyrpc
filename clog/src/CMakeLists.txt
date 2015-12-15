set(STATICLIBNAME log_wrapper)
set(SHAREDLIBNAME log_wrapper_so)
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/lib")

aux_source_directory(. DIR_SRCS)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../utils/include)
include_directories($ENV{LOG4CPP_INCLUDE_PATH})

link_directories($ENV{LOG4CPP_LIB_PATH})

add_library(${STATICLIBNAME} STATIC ${DIR_SRCS})
add_library(${SHAREDLIBNAME} SHARED ${DIR_SRCS})

set_target_properties(${SHAREDLIBNAME} PROPERTIES OUTPUT_NAME ${STATICLIBNAME})
set_target_properties(${SHAREDLIBNAME} PROPERTIES VERSION 1.1.0 SOVERSION 1)

target_link_libraries(${SHAREDLIBNAME} -Wl,--whole-archive -llog4cpp -Wl,--no-whole-archive)
