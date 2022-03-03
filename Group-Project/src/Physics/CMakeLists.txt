cmake_minimum_required(VERSION 3.22.1)
project(Group-Project)

#Physics Component
add_library(Physics
	PhysicsTestScene.cpp
	PhysicsObject.cpp

	PhysicsTestScene.h
	PhysicsObject.h
	btNCLmotionState.h
)