macro(CREATE_LIBRARY)

    set(options "")
    set(oneValueArgs TARGET_NAME TARGET_TYPE)
    set(multiValueArgs SRC_DIR INC_DIR LIBRARY_CONTENTS)
    cmake_parse_arguments(CREATE_LIBRARY "${options}" "${oneValueArgs}" 
	                  "${multiValueArgs}" "${ARGN}")

    set(TARGET_NAME ${CREATE_LIBRARY_TARGET_NAME})
    set(TARGET_TYPE ${CREATE_LIBRARY_TARGET_TYPE})
    set(INC_DIR ${CREATE_LIBRARY_INC_DIR})
    set(SRC_DIR ${CREATE_LIBRARY_SRC_DIR})

    file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/colorspaceconversion/*.cpp")
    file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/entropyencoding/*.cpp")

    message("SOURCE_FILES=${SOURCE_FILES}")

    add_library(${TARGET_NAME} ${TARGET_TYPE})
    target_include_directories(${TARGET_NAME} PUBLIC ${INC_DIR}) 

endmacro()
