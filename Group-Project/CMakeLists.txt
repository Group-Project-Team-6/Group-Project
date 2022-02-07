cmake_minimum_required(VERSION 3.0)
project(Group-Game-Project)
set(CMAKE_PREFIX_PATH ${CMAKE_HOME_DIRECTORY}/src)

#Common Component
add_library(Common
	src/Common/Assets.h
	src/Common/Camera.h
	src/Common/GameTimer.h
	src/Common/Keyboard.h
	src/Common/Maths.h
	src/Common/Matrix2.h
	src/Common/Matrix4.h
	src/Common/MeshAnimation.h
	src/Common/MeshGeometry.h
	src/Common/MeshMaterial.h
	src/Common/Mouse.h
	src/Common/Plane.h
	src/Common/Quaternion.h
	src/Common/RendererBase.h
	src/Common/ShaderBase.h
	src/Common/SimpleFont.h
	src/Common/TextureBase.h
	src/Common/TextureLoader.h
	src/Common/TextureWriter.h
	src/Common/Vector2.h
	src/Common/Vector3.h
	src/Common/Vector4.h
	src/Common/Win32Keyboard.h
	src/Common/Win32Mouse.h
	src/Common/Win32Window.h
	src/Common/Window.h

	src/Common/Assets.cpp
	src/Common/Camera.cpp
	src/Common/GameTimer.cpp
	src/Common/Keyboard.cpp
	src/Common/Maths.cpp
	src/Common/Matrix2.cpp
	src/Common/Matrix3.cpp
	src/Common/MeshAnimation.cpp
	src/Common/MeshGeometry.cpp
	src/Common/MeshMaterial.cpp
	src/Common/Mouse.cpp
	src/Common/Plane.cpp
	src/Common/Quaternion.cpp
	src/Common/RendererBase.cpp
	src/Common/ShaderBase.cpp
	src/Common/SimpleFont.cpp
	src/Common/TextureBase.cpp
	src/Common/TextureLoader.cpp
	src/Common/TextureWriter.cpp
	src/Common/Vector2.cpp
	src/Common/Vector3.cpp
	src/Common/Vector4.cpp
	src/Common/Win32Keyboard.cpp
	src/Common/Win32Mouse.cpp
	src/Common/Win32Window.cpp
	src/Common/Window.cpp
)

#Physics Component
add_library(Physics
	src/Physics/PhysicsTestScene.h

	src/Physics/PhysicsTestScene.cpp
)

#Graphic Component
add_library(OpenGLRendering
	src/OpenGLRendering/OGLComputeShader.h
	src/OpenGLRendering/OGLMesh.h
	src/OpenGLRendering/OGLRenderer.h
	src/OpenGLRendering/OGLShader.h
	src/OpenGLRendering/OGLTexture.h

	src/OpenGLRendering/OGLComputeShader.cpp
	src/OpenGLRendering/OGLMesh.cpp
	src/OpenGLRendering/OGLRenderer.cpp
	src/OpenGLRendering/OGLShader.cpp
	src/OpenGLRendering/OGLTexture.cpp	
)

#Audio Component
add_library(FMOD SHARED IMPORTED)
set_property(TARGET FMOD PROPERTY
	IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/src/FMOD/api/core/lib/x64/fmod.dll"
)
set_property(TARGET FMOD PROPERTY
	IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/src/FMOD/api/core/lib/x64/fmod_vc.lib"
)
set_property(TARGET FMOD PROPERTY
	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/src/FMOD/api/core/inc"
)

#Executable
add_executable(Game
	src/game/GameEntity.h

	src/game/main.cpp
	src/game/GameEntity.cpp
)

#Targeting Directories & Linking Libraries
find_package(Bullet CONFIG REQUIRED)
if(Bullet_FOUND)
	target_include_directories(Physics PRIVATE ${BULLET_INCLUDE_DIRS})
else()
	message(WARNING "Bullet Physics Engine Cannot Be Found, Build Might Not Work Properly")
endif()

target_link_libraries(Physics ${BULLET_LIBRARIES} Common)
target_link_libraries(Game Physics OpenGLRendering)