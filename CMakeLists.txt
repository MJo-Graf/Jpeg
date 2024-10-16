cmake_minimum_required(VERSION 3.28.3)
project(Jpeg VERSION 1.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

string(TOLOWER ${PROJECT_NAME} LIBRARYNAME)
string(TOLOWER ${PROJECT_NAME}_cli EXECUTABLE_NAME)

add_library(${LIBRARYNAME} SHARED)
target_sources(${LIBRARYNAME} PRIVATE ${SRC_DIR}/colorspaceconversion/colorspaceconversion.cpp)
target_include_directories(${LIBRARYNAME} PUBLIC ${INC_DIR})

add_executable(${EXECUTABLE_NAME})
target_sources(${EXECUTABLE_NAME} PRIVATE ${SRC_DIR}/clitool/jpeg.cpp)
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${INC_DIR})
