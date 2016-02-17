#set(STATICLIBNAME tcpserver_wrapper)
#set(SHAREDLIBNAME tcpserver_wrapper_so)
#set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/lib")

set(OUTPUTNAME TcpServerTest)
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")

aux_source_directory(. DIR_SRCS)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories($ENV{BOOST_INCLUDE_PATH})

link_directories($ENV{BOOST_LIB_PATH})

#add_library(${STATICLIBNAME} STATIC ${DIR_SRCS})
#add_library(${SHAREDLIBNAME} SHARED ${DIR_SRCS})

#set_target_properties(${SHAREDLIBNAME} PROPERTIES OUTPUT_NAME ${STATICLIBNAME})
#set_target_properties(${SHAREDLIBNAME} PROPERTIES VERSION 1.1.0 SOVERSION 1)

add_executable(${OUTPUTNAME} ${DIR_SRCS})

target_link_libraries(${OUTPUTNAME} boost_serialization)
target_link_libraries(${OUTPUTNAME} boost_thread)
target_link_libraries(${OUTPUTNAME} boost_system)
