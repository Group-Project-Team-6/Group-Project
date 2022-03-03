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

#GUI Component
#add_library(imgui SHARED IMPORTED
#	src/imgui/imgui_demo.cpp
#	src/imgui/imgui_draw.cpp
#	src/imgui/imgui_impl_glfw.cpp
#	src/imgui/imgui_impl_opengl3.cpp
#	src/imgui/imgui_tables.cpp
#	src/imgui/imgui_widgets.cpp
#	src/imgui/imgui.cpp

#	src/imgui/imconfig.h
#	src/imgui/imgui_impl_glfw.h
#	src/imgui/imgui_impl_opengl3_loader.h
#	src/imgui/imgui_impl_opengl3.h
#	src/imgui/imgui_internal.h
#	src/imgui/imgui.h
#	src/imgui/imstb_rectpack.h
#	src/imgui/imstb_textedit.h
#	src/imgui/imstb_truetype.h
#	src/imgui/glfw/GLFW/glfw3.h
#)
#set_property(TARGET imgui PROPERTY
#	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/src/imgui/glfw/GLFW"
#)