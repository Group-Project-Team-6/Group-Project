cmake_minimum_required(VERSION 3.22)
project(Group-Game-Project)

include_directories(
	src
)

if(MSVC)
    include_directories(
	Group-Project/src/Bullet/include
	)
	link_directories(
	${PROJECT_SOURCE_DIR}/Group-Project/src/Bullet/lib
	${PROJECT_SOURCE_DIR}/Group-Project/src/Bullet/debug/lib
	)
endif()

add_executable(Game
	src/game/main.cpp
)	
