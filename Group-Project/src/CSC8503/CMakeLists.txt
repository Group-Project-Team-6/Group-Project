cmake_minimum_required(VERSION 3.22.1)
project(Group-Project)

#CSC8503 Component
add_library(CSC8503 STATIC
	GameTechRenderer.cpp
	GameWorld.cpp
	Transform.cpp

	GameTechRenderer.h
	GameWorld.h
	Transform.h
)