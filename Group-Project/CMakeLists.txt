cmake_minimum_required(VERSION 3.22)
project(Group-Game-Project)

if (MSVC)
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_C_COMPILER   i586-mingw32msvc-gcc)
set(CMAKE_CXX_COMPILER i586-mingw32msvc-g++)
endif()

set(CMAKE_FIND_ROOT_PATH  .../Group-Project)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(-S src -B build)

add_executable(Game
	src/game/main.cpp
)	