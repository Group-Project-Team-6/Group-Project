add_subdirectory(Audio)
add_subdirectory(Bullet)
add_subdirectory(common)
add_subdirectory(CSC8503)
add_subdirectory(FMOD)
add_subdirectory(OpenGLRendering)
add_subdirectory(Physics)
add_subdirectory(game)

#Targeting Directories & Linking Libraries
find_package(Bullet CONFIG REQUIRED)
if(Bullet_FOUND)
	target_include_directories(Physics PRIVATE ${BULLET_INCLUDE_DIRS})
else()
	message(WARNING "Bullet Physics Engine Cannot Be Found, Build Might Not Work Properly")
endif()

add_library(FMOD SHARED IMPORTED)
set_property(TARGET FMOD PROPERTY
	IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/FMOD/api/core/lib/x64/fmod.dll"
)
set_property(TARGET FMOD PROPERTY
	IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/FMOD/api/core/lib/x64/fmod_vc.lib"
)
set_property(TARGET FMOD PROPERTY
	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/FMOD/api/core/inc"
)

target_link_libraries(Game Physics OpenGLRendering CSC8503 Common Audio)
target_link_libraries(Physics ${BULLET_LIBRARIES} Common OpenGLRendering CSC8503)
target_link_libraries(CSC8503 Common OpenGLRendering ${BULLET_LIBRARIES})
target_link_libraries(OpenGLRendering Common)
target_link_libraries(Common ${BULLET_LIBRARIES})
target_link_libraries(Audio FMOD)

#Copy fmod.dll to Output Directory
get_property(FMOD_DLL TARGET FMOD PROPERTY IMPORTED_LOCATION)
file(COPY ${FMOD_DLL} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
file(COPY ${FMOD_DLL} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)