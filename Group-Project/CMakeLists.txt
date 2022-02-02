cmake_minimum_required(VERSION 3.0)
project(Group-Game-Project)

set(CMAKE_PREFIX_PATH ${CMAKE_HOME_DIRECTORY}/src)

find_package(Bullet CONFIG REQUIRED)
if(Bullet_FOUND)
	include_directories(${BULLET_INCLUDE_DIRS})
endif()

add_library(Physics
	src/Physics/PhysicsTestScene.cpp
)

add_library(Common
	src/common/Assets.cpp
	src/common/Assets.h
	src/Common/Camera.cpp
	src/common/Camera.h
	src/common/GameTimer.cpp
	src/common/GameTimer.h
	src/common/Keyboard.cpp
	src/common/Keyboard.h
	src/common/Maths.cpp
	src/common/Maths.h
	src/common/Matrix2.cpp
	src/common/Matrix2.h
	src/common/Matrix3.cpp
	src/common/Matrix4.h
	src/common/MeshAnimation.cpp
	src/common/MeshAnimation.h
	src/common/MeshGeometry.cpp
	src/common/MeshGeometry.h
	src/common/MeshMaterial.cpp
	src/common/MeshMaterial.h
	src/common/Mouse.cpp
	src/common/Mouse.h
	src/common/Plane.cpp
	src/common/Plane.h
	src/common/Quaternion.cpp
	src/common/Quaternion.h
	src/common/RendererBase.cpp
	src/common/RendererBase.h
	src/common/ShaderBase.cpp
	src/common/ShaderBase.h
	src/common/SimpleFont.cpp
	src/common/SimpleFont.h
	src/common/TextureBase.cpp
	src/common/TextureBase.h
	src/common/TextureLoader.cpp
	src/common/TextureLoader.h
	src/common/TextureWriter.cpp
	src/common/TextureWriter.h
	src/common/Vector2.cpp
	src/common/Vector2.h
	src/common/Vector3.cpp
	src/common/Vector3.h
	src/common/Vector4.cpp
	src/common/Vector4.h
	src/common/Win32Keyboard.cpp
	src/common/Win32Keyboard.h
	src/common/Win32Mouse.cpp
	src/common/Win32Mouse.h
	src/common/Win32Window.cpp
	src/common/Win32Window.h
	src/common/Window.cpp
	src/common/Window.h
)

add_executable(Game
	src/game/main.cpp
)

target_link_libraries(Physics ${BULLET_LIBRARIES} Common)
target_link_libraries(Game ${BULLET_LIBRARIES} Physics)