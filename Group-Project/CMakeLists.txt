cmake_minimum_required(VERSION 3.0)
project(Group-Game-Project)

find_package(Bullet CONFIG REQUIRED)
if(Bullet_FOUND)
	include_directories(${BULLET_INCLUDE_DIRS})
endif()

add_executable(Game
	src/game/main.cpp
)

target_link_libraries(Game ${BULLET_LIBRARIES})