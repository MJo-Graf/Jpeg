cmake_minimum_required(VERSION 3.22.1)
project(Jpeg VERSION 1.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# ADD LIBRARY
string(TOLOWER ${PROJECT_NAME} LIBRARY_NAME)
add_library(${LIBRARY_NAME} SHARED)
target_include_directories(${LIBRARY_NAME} PUBLIC ${INC_DIR})
target_sources(${LIBRARY_NAME} PRIVATE
	${SRC_DIR}/colorspaceconversion/colorspace_conversion.cpp
	${SRC_DIR}/entropyencoding/huffman.cpp
	${SRC_DIR}/payloadextraction/payload_extraction.cpp)

# ADD EXECUTABLE(S)
add_executable(huffman_cli)
target_include_directories(huffman_cli PRIVATE ${INC_DIR})
target_sources(huffman_cli PRIVATE
	      ${SRC_DIR}/clitools/huffman_cli.cpp)
      target_link_libraries(huffman_cli PRIVATE ${LIBRARY_NAME})
