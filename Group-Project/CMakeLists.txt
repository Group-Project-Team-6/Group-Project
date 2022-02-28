cmake_minimum_required(VERSION 3.22)
project(Group-Game-Project)
set(CMAKE_PREFIX_PATH ${CMAKE_HOME_DIRECTORY}/src)
add_subdirectory(src/OpenGLRendering/KHR)
add_compile_definitions(GLEW_STATIC)

include(GNUInstallDirs)

function(print)
	foreach(var ${ARGN})
		message("${var} = ${${var}}")
	endforeach()
endfunction()

print(PROJECT_NAME CMAKE_SOURCE_DIR)

#CSC8503 Component
add_library(CSC8503
	src/CSC8503/GameTechRenderer.cpp
	src/CSC8503/GameWorld.cpp
	src/CSC8503/Transform.cpp

	src/CSC8503/GameTechRenderer.h
	src/CSC8503/GameWorld.h
	src/CSC8503/Transform.h
)

#Physics Component
add_library(Physics
	src/Physics/PhysicsTestScene.cpp
	src/Physics/PhysicsObject.cpp

	src/Physics/PhysicsTestScene.h
	src/Physics/PhysicsObject.h
	src/Physics/btNCLmotionState.h
)

#Graphic Component
add_library(OpenGLRendering STATIC
	src/OpenGLRendering/OGLComputeShader.cpp
	src/OpenGLRendering/OGLMesh.cpp
	src/OpenGLRendering/OGLRenderer.cpp	
	src/OpenGLRendering/OGLShader.cpp
	src/OpenGLRendering/OGLTexture.cpp
	src/OpenGLRendering/glad.c

	src/OpenGLRendering/OGLComputeShader.h
	src/OpenGLRendering/OGLMesh.h
	src/OpenGLRendering/OGLRenderer.h
	src/OpenGLRendering/OGLShader.h
	src/OpenGLRendering/OGLTexture.h
)

#GUI Component
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

#Common Component
add_library(Common
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

	src/Common/Assets.h
	src/Common/Camera.h
	src/Common/GameTimer.h
	src/Common/Keyboard.h
	src/Common/Maths.h
	src/Common/Matrix2.h
	src/Common/Matrix3.cpp
	src/Common/Matrix3.h
	src/Common/Matrix4.cpp
	src/Common/Matrix4.h
	src/Common/MeshAnimation.h
	src/Common/MeshGeometry.h
	src/Common/MeshMaterial.h
	src/Common/Mouse.h
	src/Common/Plane.h
	src/Common/Quaternion.h
	src/Common/RenderObject.cpp
	src/Common/RenderObject.h
	src/Common/RendererBase.cpp
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

add_library(Audio
	src/Audio/common.h
	src/Audio/common_platform.h
	src/Audio/AudioManager.h

	src/Audio/common.cpp
	src/Audio/common_platform.cpp
	src/Audio/AudioManager.cpp
)

#Executable
add_executable(Game
	src/game/GameEntity.h
	src/game/Game.h
	src/game/TransformConverter.h
	src/game/PlayerInput.h

	src/game/main.cpp
	src/game/GameEntity.cpp
	src/game/Game.cpp
	src/game/TransformConverter.cpp
	src/game/PlayerInput.cpp
)

#Targeting Directories & Linking Libraries
find_package(OpenGL REQUIRED)
if(!OpenGL_FOUND)
	message(WARNING "OpenGL Not Found")
endif()

find_package(Bullet CONFIG REQUIRED)
if(Bullet_FOUND)
	target_include_directories(Physics PRIVATE ${BULLET_INCLUDE_DIRS})
else()
	message(WARNING "Bullet Physics Engine Cannot Be Found, Build Might Not Work Properly")
endif()

target_include_directories(Game
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/..>
		$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE
		${CMAKE_CURRENT_LIST_DIR}
)

install(TARGETS Game
	EXPORT Game_Export
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		NAMELINK_SKIP
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(EXPORT Game_Export
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/game-${PROJECT_VERSION}
)

target_link_libraries(Game Physics OpenGLRendering CSC8503 Common Audio)
target_link_libraries(Physics ${BULLET_LIBRARIES} Common OpenGLRendering CSC8503)
target_link_libraries(CSC8503 Common OpenGLRendering ${BULLET_LIBRARIES})
target_link_libraries(OpenGLRendering Common KHR_LIB ${OPENGL_gl_LIBRARY})
target_link_libraries(Common ${BULLET_LIBRARIES})
target_link_libraries(Audio FMOD)

