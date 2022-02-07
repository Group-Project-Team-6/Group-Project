cmake_minimum_required(VERSION 3.0)
project(Group-Game-Project)

set(CMAKE_PREFIX_PATH ${CMAKE_HOME_DIRECTORY}/src)

find_package(Bullet CONFIG REQUIRED)
if(Bullet_FOUND)
	include_directories(${BULLET_INCLUDE_DIRS})
endif()

add_library(CSC8503
	src/CSC8503/GameTechRenderer.cpp
	src/CSC8503/GameTechRenderer.h
	src/CSC8503/GameWorld.cpp
	src/CSC8503/GameWorld.h
	src/CSC8503/Transform.cpp
	src/CSC8503/Transform.h
)

add_library(Physics
	src/Physics/PhysicsTestScene.cpp
	src/Physics/PhysicsTestScene.h
	src/Physics/btNCLmotionState.h
)

add_library(OpenGLRendering
	src/OpenGLRendering/OGLComputeShader.cpp
	src/OpenGLRendering/OGLComputeShader.h
	src/OpenGLRendering/OGLMesh.cpp
	src/OpenGLRendering/OGLMesh.h
	src/OpenGLRendering/OGLRenderer.cpp
	src/OpenGLRendering/OGLRenderer.h
	src/OpenGLRendering/OGLShader.cpp
	src/OpenGLRendering/OGLShader.h
	src/OpenGLRendering/OGLTexture.cpp
	src/OpenGLRendering/OGLTexture.h
	src/OpenGLRendering/glad.c
)

add_library(imgui SHARED IMPORTED
	src/imgui/imgui_demo.cpp
	src/imgui/imgui_draw.cpp
	src/imgui/imgui_impl_glfw.cpp
	src/imgui/imgui_impl_opengl3.cpp
	src/imgui/imgui_tables.cpp
	src/imgui/imgui_widgets.cpp
	src/imgui/imgui.cpp

	src/imgui/imconfig.h
	src/imgui/imgui_impl_glfw.h
	src/imgui/imgui_impl_opengl3_loader.h
	src/imgui/imgui_impl_opengl3.h
	src/imgui/imgui_internal.h
	src/imgui/imgui.h
	src/imgui/imstb_rectpack.h
	src/imgui/imstb_textedit.h
	src/imgui/imstb_truetype.h
	src/imgui/glfw/GLFW/glfw3.h
)

set_property(TARGET imgui PROPERTY
	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/src/imgui/glfw/GLFW"
)

add_library(Common
	src/Common/Assets.cpp
	src/Common/Assets.h
	src/Common/Camera.cpp
	src/Common/Camera.h
	src/Common/GameTimer.cpp
	src/Common/GameTimer.h
	src/Common/Keyboard.cpp
	src/Common/Keyboard.h
	src/Common/Maths.cpp
	src/Common/Maths.h
	src/Common/Matrix2.cpp
	src/Common/Matrix2.h
	src/Common/Matrix3.cpp
	src/Common/Matrix3.h
	src/Common/Matrix4.cpp
	src/Common/Matrix4.h
	src/Common/MeshAnimation.cpp
	src/Common/MeshAnimation.h
	src/Common/MeshGeometry.cpp
	src/Common/MeshGeometry.h
	src/Common/MeshMaterial.cpp
	src/Common/MeshMaterial.h
	src/Common/Mouse.cpp
	src/Common/Mouse.h
	src/Common/Plane.cpp
	src/Common/Plane.h
	src/Common/Quaternion.cpp
	src/Common/Quaternion.h
	src/Common/RenderObject.cpp
	src/Common/RenderObject.h
	src/Common/RendererBase.cpp
	src/Common/RendererBase.h
	src/Common/ShaderBase.cpp
	src/Common/ShaderBase.h
	src/Common/SimpleFont.cpp
	src/Common/SimpleFont.h
	src/Common/TextureBase.cpp
	src/Common/TextureBase.h
	src/Common/TextureLoader.cpp
	src/Common/TextureLoader.h
	src/Common/TextureWriter.cpp
	src/Common/TextureWriter.h
	src/Common/Vector2.cpp
	src/Common/Vector2.h
	src/Common/Vector3.cpp
	src/Common/Vector3.h
	src/Common/Vector4.cpp
	src/Common/Vector4.h
	src/Common/Win32Keyboard.cpp
	src/Common/Win32Keyboard.h
	src/Common/Win32Mouse.cpp
	src/Common/Win32Mouse.h
	src/Common/Win32Window.cpp
	src/Common/Win32Window.h
	src/Common/Window.cpp
	src/Common/Window.h
)

add_executable(Game
	src/game/main.cpp
	src/game/GameEntity.cpp
	src/game/GameEntity.h
)

target_link_libraries(Game Physics OpenGLRendering CSC8503 Common)
target_link_libraries(Physics ${BULLET_LIBRARIES} Common OpenGLRendering CSC8503)
target_link_libraries(CSC8503 Common OpenGLRendering)
target_link_libraries(OpenGLRendering Common)
target_link_libraries(Common ${BULLET_LIBRARIES})
