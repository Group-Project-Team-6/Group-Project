cmake_minimum_required(VERSION 3.22)
project(Group-Game-Project)

set(-S src -B build)

add_executable(Game
	src/game/main.cpp
)