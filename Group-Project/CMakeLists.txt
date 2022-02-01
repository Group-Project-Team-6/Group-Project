cmake_minimum_required(VERSION 3.0)
project(Group-Game-Project)

find_package(Bullet CONFIG REQUIRED)

include_directories(${BULLET_INCLUDE_DIR})
link_directories(${BULLET_LIBRARY_DIRS})

add_executable(Game
	src/game/main.cpp
)