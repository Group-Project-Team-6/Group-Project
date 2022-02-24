cmake_minimum_required(VERSION 3.0)
project(Group-Game-Project)
set(CMAKE_PREFIX_PATH ${CMAKE_HOME_DIRECTORY}/src)
#set(ENV{VULKAN_SDK} "C:/VulkanSDK/1.3.204.0")

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

	src/Physics/PhysicsTestScene.h
	src/Physics/btNCLmotionState.h
)

#Graphic Component
add_library(OpenGLRendering
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

add_library(VulkanRendering
    src/VulkanRendering/VulkanTexture.cpp
    src/VulkanRendering/VulkanShaderBuilder.cpp
    src/VulkanRendering/VulkanShader.cpp
    src/VulkanRendering/VulkanRenderPassBuilder.cpp
    src/VulkanRendering/VulkanRenderer.cpp
    src/VulkanRendering/VulkanPipelineBuilder.cpp
    src/VulkanRendering/VulkanMesh.cpp
    src/VulkanRendering/VulkanDescriptorSetLayoutBuilder.cpp
    src/VulkanRendering/VulkanDebugger.cpp
    
    src/VulkanRendering/VulkanUtility.h
    src/VulkanRendering/VulkanTexture.h
    src/VulkanRendering/VulkanShaderBuilder.h
    src/VulkanRendering/VulkanShader.h
    src/VulkanRendering/VulkanRenderPassBuilder.h
    src/VulkanRendering/VulkanRenderer.h
    src/VulkanRendering/VulkanPipelineBuilder.h
    src/VulkanRendering/VulkanMesh.h
    src/VulkanRendering/VulkanDescriptorSetLayoutBuilder.h
    src/VulkanRendering/VulkanDebugger.h
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

#Executable
add_executable(Game
	src/game/GameEntity.h

	src/game/main.cpp
	src/game/GameEntity.cpp
)

#Targeting Directories & Linking Libraries
find_package(Bullet CONFIG)
if(Bullet_FOUND)
	target_include_directories(Physics PRIVATE ${BULLET_INCLUDE_DIRS})
else()
	message(FATAL_ERROR "Could not find Bullet Physics Engine!")
endif()

if (DEFINED VULKAN_SDK_PATH)
  set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include") # 1.1 Make sure this include path is correct
  set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib") # 1.2 Make sure lib path is correct
  set(Vulkan_FOUND "True")
else()
  find_package(Vulkan REQUIRED) # throws error if could not find Vulkan
  message(STATUS "Found Vulkan: $ENV{VULKAN_SDK}")
endif()
if (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Could not find Vulkan library!")
else()
    message(STATUS "Using vulkan lib at: ${Vulkan_LIBRARIES}")
endif()

target_link_libraries(Game Physics OpenGLRendering CSC8503 Common)
target_link_libraries(Physics ${BULLET_LIBRARIES} Common OpenGLRendering CSC8503)
target_link_libraries(CSC8503 Common OpenGLRendering)
target_link_libraries(OpenGLRendering Common)
target_link_libraries(VulkanRendering Vulkan::Headers Common)
target_link_libraries(Common ${BULLET_LIBRARIES})
