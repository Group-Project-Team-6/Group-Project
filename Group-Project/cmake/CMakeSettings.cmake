cmake_minimum_required(VERSION 3.22.1)

# This module is shared; use include blocker.
if( _SETTINGS_ )
	return()
endif()
set(_SETTINGS_ 1)

include(CMakePlatforms)
include(CMakeMacros)

# Define product build options
#set(PRODUCT_NAME "Stingray")
#set(PRODUCT_COMPANY "Autodesk")
#set(PRODUCT_COPYRIGHT "Copyright (C) 2016 ${PRODUCT_COMPANY}, Inc. All rights reserved.")
set(PRODUCT_VERSION_MAJOR "1")
set(PRODUCT_VERSION_MINOR "9")
set(PRODUCT_VERSION_REVISION "0")

# Bump this revision if projects require any migration.
# Reset to 0 if you increase MAJOR or MINOR versions.
set(PRODUCT_DEV_VERSION_REVISION "0")
string(TIMESTAMP PRODUCT_DEFAULT_BUILD_TIMESTAMP "%Y-%m-%dT%H:%M:%SZ" UTC)

# Build options controlled by TeamCity - do not change here!
set_default(PRODUCT_VERSION_LABEL "$ENV{SR_PRODUCT_VERSION_LABEL}" "Developer Build")
set_default(PRODUCT_VERSION_TCID "$ENV{SR_PRODUCT_VERSION_TCID}" "0")
set_default(PRODUCT_BUILD_TIMESTAMP "$ENV{SR_PRODUCT_BUILD_TIMESTAMP}" "${PRODUCT_DEFAULT_BUILD_TIMESTAMP}")
set_default(PRODUCT_LICENSING_KEY "$ENV{SR_PRODUCT_LICENSING_KEY}" "A72J1")
set_default(PRODUCT_LICENSING_VERSION "$ENV{SR_PRODUCT_LICENSING_VERSION}" "2018.0.0.F")
set_default(PRODUCT_EDITOR_STEAM_APPID "$ENV{SR_PRODUCT_EDITOR_STEAM_APPID}" "0")

# Allow environment variables to override some build options
set(ENGINE_BUILD_IDENTIFIER $ENV{SR_BUILD_IDENTIFIER})

# Check if required variables are properly defined
if( NOT REPOSITORY_DIR OR REPOSITORY_DIR STREQUAL "" )
	message(FATAL_ERROR "REPOSITORY_DIR not set! Please set REPOSITORY_DIR to appropriate location when running this CMake script.")
endif()
string(REGEX REPLACE "\\\\" "/" REPOSITORY_DIR ${REPOSITORY_DIR})

if( NOT CMAKE_INSTALL_PREFIX OR CMAKE_INSTALL_PREFIX STREQUAL "" )
	message(FATAL_ERROR "CMAKE_INSTALL_PREFIX not set! Please set CMAKE_INSTALL_PREFIX to appropriate location when running this CMake script.")
endif()
string(REGEX REPLACE "\\\\" "/" BINARIES_DIR ${CMAKE_INSTALL_PREFIX})

# PLUGIN REPO CUSTOM CHANGE
#if( NOT ENGINE_LIB_DIR )
#	set(ENGINE_LIB_DIR $ENV{SR_LIB_DIR})
#	if( NOT ENGINE_LIB_DIR OR ENGINE_LIB_DIR STREQUAL "" )
#		message(FATAL_ERROR "Environment variable SR_LIB_DIR not set! Please set environment variable SR_LIB_DIR to appropriate location before running this CMake script.")
#	endif()
#endif()
#string(REGEX REPLACE "\\\\" "/" ENGINE_LIB_DIR ${ENGINE_LIB_DIR})

# Generate package location cache
set(PACKAGE_CACHE_FILE "${CMAKE_BINARY_DIR}/cmake_package_cache.cmake")
#generate_package_location_cache()

# Set install directories
if( PLATFORM_WINDOWS )
	set(ENGINE_INSTALL_DIR "${BINARIES_DIR}/engine/win${ARCH_BITS}/$<LOWER_CASE:$<CONFIG>>")
	# PLUGIN REPO CUSTOM CHANGE
	set(EDITOR_INSTALL_DIR "${BINARIES_DIR}/editor/win${ARCH_BITS}/$<LOWER_CASE:$<CONFIG>>")
elseif( PLATFORM_UWP )
	set(ENGINE_INSTALL_DIR "${BINARIES_DIR}/engine/uwp${ARCH_BITS}/$<LOWER_CASE:$<CONFIG>>")
	# PLUGIN REPO CUSTOM CHANGE
	set(EDITOR_INSTALL_DIR "${BINARIES_DIR}/editor/uwp${ARCH_BITS}/$<LOWER_CASE:$<CONFIG>>")
else()
	set(ENGINE_INSTALL_DIR "${BINARIES_DIR}/engine/${PLATFORM_NAME}/$<LOWER_CASE:$<CONFIG>>")
	# PLUGIN REPO CUSTOM CHANGE
	set(EDITOR_INSTALL_DIR "${BINARIES_DIR}/editor/${PLATFORM_NAME}/$<LOWER_CASE:$<CONFIG>>")
endif()
# PLUGIN REPO CUSTOM CHANGE
set(ENGINE_PLUGINS_INSTALL_DIR "${ENGINE_INSTALL_DIR}")
set(EDITOR_PLUGINS_INSTALL_DIR "${EDITOR_INSTALL_DIR}")
set(TOOLS_INSTALL_DIR "${BINARIES_DIR}/tools")
set(PLUGIN_SDK_INSTALL_DIR "${BINARIES_DIR}/plugin_sdk")
set(TOOLS_EXTERNAL_INSTALL_DIR "${BINARIES_DIR}/tools_external")

if( PLATFORM_WINDOWS )
	set(ENGINE_PLUGIN_SUFFIX "w${ARCH_BITS}")
else()
	set(ENGINE_PLUGIN_SUFFIX "")
endif()

# Global options
if( PLATFORM_WINDOWS OR PLATFORM_ANDROID OR PLATFORM_IOS)
	set(BUILD_SHARED_LIBS ON CACHE BOOL "Build plug-ins as shared libraries.")
else()
	set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build plug-ins as static libraries.")
endif()

if( PLATFORM_IOS )
	set_default(ENGINE_IOS_CODE_SIGN_IDENTITY "$ENV{SR_IOS_CODE_SIGN_IDENTITY}" "iPhone Developer")
	set_default(ENGINE_IOS_DEVELOPMENT_TEAM "$ENV{SR_IOS_DEVELOPMENT_TEAM}" "")
endif()

set(BUILD_RUN_TEST OFF CACHE BOOL "Enable special build for run test mode.")

# Engine options
set(ENGINE_USE_DEBUG_INFO ON CACHE BOOL "Enable debug information in all configurations.")
set(ENGINE_USE_SOUND ON CACHE BOOL "Enable Sound (Timpani) support.")
set(ENGINE_USE_APEX OFF CACHE BOOL "Enable APEX clothing support.")
set(ENGINE_USE_AVX OFF CACHE BOOL "Enable AVX instructions set support.")
set(ENGINE_USE_UNWRAPLIB ON CACHE BOOL "Enable UV-unwrapping.")
set(ENGINE_USE_STEAM OFF CACHE BOOL "Enable Steamworks support.")
set(ENGINE_USE_STEAM_DEDICATED_SERVER OFF CACHE BOOL "Enable Steamworks dedicated server support.")
set(ENGINE_USE_EXPERIMENTAL_CRITICALSECTIONS OFF CACHE BOOL "Enable experimental spinlocks on Android.")
set(ENGINE_USE_WEB_WASM ON CACHE BOOL "Enable WebAssembly support for Web platform.")
set(ENGINE_USE_WEB_THREADS OFF CACHE BOOL "Enable threading support for Web platform.")
set(ENGINE_USE_WEB_SIMD OFF CACHE BOOL "Enable SIMD support for Web platform.")
set(ENGINE_USE_IVF ON CACHE BOOL "Enable IVF video support.")

set(ENGINE_USE_GWNAV_PLUGIN ON CACHE BOOL "Enable Gameware Navigation support.")
set(ENGINE_USE_CAPTURE_EXR_PLUGIN ON CACHE BOOL "Enable EXR support.")
set(ENGINE_USE_CAPTURE_FRAME_PLUGIN ON CACHE BOOL "Enable Capture Frames support.")
set(ENGINE_USE_HUMANIK_PLUGIN ON CACHE BOOL "Enable HumanIK support.")
set(ENGINE_USE_SCALEFORMSTUDIO_PLUGIN ON CACHE BOOL "Enable ScaleformStudio support.")
set(ENGINE_USE_WWISE_PLUGIN ON CACHE BOOL "Enable WWise sound support.")
set(ENGINE_USE_OCULUS_PLUGIN ON CACHE BOOL "Enable Oculus support.")
set(ENGINE_USE_STEAMVR_PLUGIN ON CACHE BOOL "Enable SteamVR support.")
set(ENGINE_USE_GEARVR_PLUGIN ON CACHE BOOL "Enable GearVR support.")
set(ENGINE_USE_GOOGLEVR_PLUGIN ON CACHE BOOL "Enable GoogleVR support.")
set(ENGINE_USE_TEXTUREREADER_PLUGIN ON CACHE BOOL "Enable TextureReader support.")
set(ENGINE_USE_ALEMBIC_PLUGIN ON CACHE BOOL "Enable Alembic support.")
set(ENGINE_USE_HOLOLENS_PLUGIN ON CACHE BOOL "Enable Hololens support.")
set(ENGINE_USE_FBXSDK_PLUGIN ON CACHE BOOL "Enable FBX support.")
set(ENGINE_USE_PROTEIN_PLUGIN OFF CACHE BOOL "Enable Protein FBX material support.")
set(ENGINE_USE_XB1_DATA_COMPILER_PLUGIN ON CACHE BOOL "Enable XB1 data compiler support.")
set(ENGINE_USE_PS4_DATA_COMPILER_PLUGIN ON CACHE BOOL "Enable PS4 data compiler support.")
set(ENGINE_USE_D3D12 OFF CACHE BOOL "Enable D3D12 support.")

set(ENGINE_USE_SIMPLE_PLUGIN OFF CACHE BOOL "Enable simple plug-in sample.")
set(ENGINE_USE_BIGGER_PLUGIN OFF CACHE BOOL "Enable bigger plug-in sample.")
set(ENGINE_USE_RENDER_PLUGIN OFF CACHE BOOL "Enable render plug-in sample.")

# Set folder names inside solution files
set(ENGINE_FOLDER_NAME "runtime")
set(ENGINE_PLUGINS_FOLDER_NAME "plugins")
set(ENGINE_USE_SOLUTION_FOLDERS ON)

# Define if platform can compile game data
if( PLATFORM_64BIT AND (PLATFORM_WINDOWS OR PLATFORM_OSX) )
	set(ENGINE_CAN_COMPILE 1)
endif()

# Verify configuration validity
if( NOT ENGINE_CAN_COMPILE )
	set(ENGINE_USE_CAPTURE_EXR_PLUGIN OFF)
	set(ENGINE_USE_CAPTURE_FRAME_PLUGIN OFF)
	set(ENGINE_USE_UNWRAPLIB OFF)
	set(ENGINE_USE_TEXTUREREADER_PLUGIN OFF)
	set(ENGINE_USE_FBXSDK_PLUGIN OFF)
	set(ENGINE_USE_ALEMBIC_PLUGIN OFF)
	set(ENGINE_USE_PROTEIN_PLUGIN OFF)
	set(ENGINE_USE_XB1_DATA_COMPILER_PLUGIN OFF)
	set(ENGINE_USE_PS4_DATA_COMPILER_PLUGIN OFF)
endif()

if( ENGINE_USE_ALEMBIC_PLUGIN AND MSVC_VERSION LESS 1900 )
	message(STATUS "Alembic requires msvc14 or above.")
	set(ENGINE_USE_ALEMBIC_PLUGIN OFF)
endif()

if( ENGINE_USE_PROTEIN_PLUGIN AND NOT ENGINE_USE_FBXSDK_PLUGIN )
	set(ENGINE_USE_PROTEIN_PLUGIN OFF)
endif()

if( ENGINE_USE_STEAM AND NOT (PLATFORM_WINDOWS OR PLATFORM_OSX) )
	message(STATUS "Steam support disabled, unsupported by platform ${PLATFORM_NAME}.")
	set(ENGINE_USE_STEAM OFF)
endif()

if( ENGINE_USE_STEAM_DEDICATED_SERVER AND NOT ENGINE_USE_STEAM )
	message(STATUS "Steam dedicated server support disabled, Steam support isn't enabled.")
	set(ENGINE_USE_STEAM_DEDICATED_SERVER OFF)
endif()

if( ENGINE_USE_PS4_DATA_COMPILER_PLUGIN )
	if( NOT PLATFORM_WINDOWS )
		message(STATUS "PS4 data compiler support disabled, unsupported by platform ${PLATFORM_NAME}.")
		set(ENGINE_USE_PS4_DATA_COMPILER_PLUGIN OFF)
	elseif( NOT (EXISTS "${REPOSITORY_DIR}/runtime/platforms/ps4" AND IS_DIRECTORY "${REPOSITORY_DIR}/runtime/platforms/ps4") )
		message(STATUS "PS4 data compiler support disabled, source not found.")
		set(ENGINE_USE_PS4_DATA_COMPILER_PLUGIN OFF)
	elseif( NOT DEFINED PS4_ROOT OR NOT DEFINED PS4_SDK )
		message(STATUS "PS4 data compiler support disabled, PS4 SDK not found or wrong version.")
		set(ENGINE_USE_PS4_DATA_COMPILER_PLUGIN OFF)
	endif()
	if( EXISTS "${REPOSITORY_DIR}/runtime/platforms/ps4" AND IS_DIRECTORY "${REPOSITORY_DIR}/runtime/platforms/ps4" )
		file(GLOB_RECURSE RESULT "${REPOSITORY_DIR}/runtime/platforms/ps4/*.*")
		list(LENGTH RESULT RES_LEN)
		if( RES_LEN EQUAL 0 ) # DIR is empty
			message(STATUS "PS4 data compiler support disabled, source not found.")
			set(ENGINE_USE_PS4_DATA_COMPILER_PLUGIN OFF)
		endif()
	endif()
endif()