cmake_minimum_required(VERSION 3.22.1)
project(Group-Project)

#Common Component
add_library(Networking STATIC
	GameClient.cpp
	GameServer.cpp
	NetworkBase.cpp
	NetworkObject.cpp
	NetworkState.cpp
	
	GameClient.h
	GameServer.h
	NetworkBase.h
	NetworkObject.h
	NetworkState.h
)