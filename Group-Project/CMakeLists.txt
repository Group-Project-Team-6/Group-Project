cmake_minimum_required(VERSION 3.22)
project(Group-Game-Project)

include_directories(
	src
)

 include_directories(
	src/Bullet/include/bullet
)
link_directories(
	${PROJECT_SOURCE_DIR}/Group-Project/src/Bullet/lib
	${PROJECT_SOURCE_DIR}/Group-Project/src/Bullet/debug/lib
)

add_executable(Game
	src/game/main.cpp
)	
