cmake_minimum_required(VERSION 3.22.1)
project(Group-Project)

#Executable
add_executable(Game
	GameEntity.h
	DebugMode.h

	main.cpp
	GameEntity.cpp
	DebugMode.cpp
)