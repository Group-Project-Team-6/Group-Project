cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_GENERATOR_PLATFORM x64)
project(Group-Project)
set(CMAKE_PREFIX_PATH ${CMAKE_HOME_DIRECTORY}/src)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Lib)
set(CMAKE_CONFIGURATION_TYPES Debug;Release)

add_subdirectory(src)

function(print)
	foreach(var ${ARGN})
		message("${var} = ${${var}}")
	endforeach()
endfunction()

print(PROJECT_NAME CMAKE_SOURCE_DIR PROJECT_BINARY_DIR)