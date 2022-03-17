cmake_minimum_required(VERSION 3.22.1)
project(Group-Project)

#CSC8503 Component
add_library(CSC8503
	GameLoadingRenderer.cpp
	GameTechRenderer.cpp
	GameWorld.cpp
	Transform.cpp

	GameLoadingRenderer.h
	GameTechRenderer.h
	GameWorld.h
	Transform.h
)