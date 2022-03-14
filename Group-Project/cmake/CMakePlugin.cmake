cmake_minimum_required(VERSION 3.22.1)

include(CMakeSettings)

set(CMAKE_C_FLAGS)
set(CMAKE_CXX_FLAGS)
set(CMAKE_C_FLAGS_DEBUG "-D_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG")
set(CMAKE_C_FLAGS_RELEASE "-DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG")

# Setup platforms
if( PLATFORM_WINDOWS )
	add_compile_options(-DWINDOWSPC)
elseif( PLATFORM_PS4 )
	if( NOT (EXISTS "${PROJECT_SOURCE_DIR}/platforms/ps4" AND IS_DIRECTORY "${PROJECT_SOURCE_DIR}/platforms/ps4") )
		message(FATAL_ERROR "Could not find PlayStation4 engine source directory, cannot continue.")
	endif()
	add_definitions(-DPS4)	# Patch for PS4: Scaleform use a namespace named 'PS4', and it conflicts with the define 'PS4'. Using add_definitions allow us to remove it later.
	add_compile_options(-D__ORBIS__ -DX64 -D__SSE3__)
endif()

# Define standard configurations
if( CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_CONFIGURATION_TYPES MATCHES "Debug;Dev;Release" )
	list(APPEND Configs Debug Dev Release)
	set(CMAKE_CONFIGURATION_TYPES ${Configs} CACHE STRING "List of supported configurations." FORCE)
	set(CMAKE_INSTALL_PREFIX $ENV{SR_BIN_DIR} CACHE STRING "Default installation directory." FORCE)
	message(FATAL_ERROR "Default configuration was reset, please re-run CMake.")
endif()

# Initialize the development configuration using release configuration
set(CMAKE_C_FLAGS_DEV "${CMAKE_C_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_DEV "${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_STATIC_LINKER_FLAGS_DEV "${CMAKE_STATIC_LINKER_FLAGS_RELEASE}")
set(CMAKE_SHARED_LINKER_FLAGS_DEV "${CMAKE_SHARED_LINKER_FLAGS_RELEASE}")
set(CMAKE_MODULE_LINKER_FLAGS_DEV "${CMAKE_MODULE_LINKER_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS_DEV "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")

# Set global defines
add_compile_options(-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE)
add_compile_options($<$<CONFIG:DEBUG>:-D_SECURE_SCL_THROWS=0> $<$<CONFIG:DEBUG>:-D_SILENCE_DEPRECATION_OF_SECURE_SCL_THROWS>)
add_compile_options(-D_HAS_ITERATOR_DEBUGGING=$<CONFIG:DEBUG> -D_SECURE_SCL=$<CONFIG:DEBUG>)
if( NOT PLATFORM_XBOXONE )
	add_compile_options(-D_HAS_EXCEPTIONS=0)
endif()

add_compile_options($<$<NOT:$<CONFIG:RELEASE>>:-DDEVELOPMENT>)
add_compile_options($<$<NOT:$<CONFIG:RELEASE>>:-DUNIT_TESTS>)
add_compile_options($<$<NOT:$<CONFIG:RELEASE>>:-DUSE_CALLSTACK>)

add_compile_options($<$<NOT:$<CONFIG:RELEASE>>:-DHAS_PROFILER>)
add_compile_options($<$<NOT:$<CONFIG:RELEASE>>:-DHAS_LOADING_PROFILER>)
if( NOT PLATFORM_WEB)
	add_compile_options($<$<NOT:$<CONFIG:RELEASE>>:-DHAS_CONSOLE_SERVER>)
endif()

# Define platform architecture
if( PLATFORM_WINDOWS OR PLATFORM_OSX OR PLATFORM_LINUX OR PLATFORM_XBOXONE OR PLATFORM_PS4 OR PLATFORM_WEB OR PLATFORM_UWP )
	add_compile_options(-DPLATFORM_ARCH_X86)
elseif( PLATFORM_IOS OR PLATFORM_ANDROID )
	add_compile_options(-DPLATFORM_ARCH_ARM)
else()
	message(FATAL_ERROR "Unknown platform architecture!")
endif()

# Define 32 versus 64 bit architecture
if( PLATFORM_64BIT )
	add_compile_options(-DPLATFORM_64BIT)
else()
	add_compile_options(-DPLATFORM_32BIT)
endif()

if(NOT BUILD_SHARED_LIBS)
	add_compile_options(-DSTATIC_PLUGIN_LINKING)
endif()

# Define if platform can compile game data
if( ENGINE_CAN_COMPILE )
	add_compile_options($<$<NOT:$<CONFIG:RELEASE>>:-DCAN_COMPILE>)
	add_compile_options($<$<NOT:$<CONFIG:RELEASE>>:-DMOJOSHADER_NO_VERSION_INCLUDE>)
endif()

# Setup global per-platform compiler/linker options
if( PLATFORM_WINDOWS OR PLATFORM_XBOXONE OR PLATFORM_UWP )
	# Debug information
	if( ENGINE_USE_DEBUG_INFO )
		add_compile_options(/Zi)
	else()
		add_compile_options($<$<CONFIG:DEBUG>:/Zi>)
	endif()

	# Enable full optimization in dev/release
	add_compile_options($<$<CONFIG:DEBUG>:/Od> $<$<NOT:$<CONFIG:DEBUG>>:/Ox>)

	# Inline function expansion
	add_compile_options(/Ob2)

	# Enable intrinsic functions in dev/release
	add_compile_options($<$<NOT:$<CONFIG:DEBUG>>:/Oi>)

	# Favor fast code
	add_compile_options(/Ot)

	# Enable fiber-safe optimizations in dev/release
	add_compile_options($<$<NOT:$<CONFIG:DEBUG>>:/GT>)

	# Enable string pooling
	add_compile_options(/GF)

	# Disable C++ exceptions
	# PLUGIN CUSTOM CHANGES
	if (NOT EDITOR_PLUGIN)
		replace_compile_flags("/EHsc" "")
	endif()

	# Select static/dynamic runtime library
	if( PLATFORM_WINDOWS )
		add_compile_options($<$<CONFIG:DEBUG>:/MTd> $<$<NOT:$<CONFIG:DEBUG>>:/MT>)
	elseif( PLATFORM_XBOXONE OR PLATFORM_UWP )
		add_compile_options($<$<CONFIG:DEBUG>:/MDd> $<$<NOT:$<CONFIG:DEBUG>>:/MD>)
	endif()

	# Use security checks only in debug
	if( PLATFORM_UWP )
		add_compile_options($<$<CONFIG:DEBUG>:/sdl> $<$<NOT:$<CONFIG:DEBUG>>:/sdl->)
	else()
		add_compile_options($<$<CONFIG:DEBUG>:/GS> $<$<NOT:$<CONFIG:DEBUG>>:/GS->)
	endif()

	# Enable function-level linking
	add_compile_options(/Gy)

	# Enable SIMD
	if( PLATFORM_WINDOWS )
		if( PLATFORM_64BIT )
			if( ENGINE_USE_AVX )
				add_compile_options(/arch:AVX -DAVX)
			endif()
		else()
			add_compile_options(/arch:SSE2)
		endif()
	endif()

	# Use fast floating point model
	add_compile_options(/fp:fast)

	# Disable run-time type information (RTTI)
	replace_compile_flags("/GR" "/GR-")

	# Set warning level 3
	add_compile_options(/W3)

	# Disable specific warnings
	add_compile_options(/wd4351 /wd4005)

	# Disable specific warnings for MSVC14 and above
	if( (PLATFORM_WINDOWS OR PLATFORM_UWP) AND (NOT MSVC_VERSION LESS 1900) )
		add_compile_options(/wd4838 /wd4312 /wd4477 /wd4244 /wd4091 /wd4311 /wd4302 /wd4476 /wd4474)
		add_compile_options(/wd4309)	# truncation of constant value
	endif()

	if( PLATFORM_XBOXONE AND (NOT MSVC_VERSION LESS 1900) )
		add_compile_options(/wd4577 /wd4838 /wd4312)
	endif()

	# Force specific warnings as errors
	add_compile_options(/we4101)

	# Treat all other warnings as errors
	add_compile_options(/WX)

	# Disable specific link libraries
	if( PLATFORM_WINDOWS )
		add_linker_flags(/NODEFAULTLIB:"MSVCRT.lib")
	endif()

	# Disable specific linker warnings

	# This object file does not define any previously undefined public symbols,
	# so it will not be used by any link operation that consumes this library
	add_linker_flags("/ignore:4221")

	# Missing PDB files. We get this warning for freetype and zlib used by scaleform.
	#
	# Unfortunately, this linker warning cannot be ignored even though it is mostly
	# harmless and will occur frequently for third-party libraries. See
	#
	#     http://www.geoffchappell.com/studies/msvc/link/link/options/ignore.htm
	#
	# To be able to ignore this warning, you need to patch link.exe. We have a tool
	# for that:
	#
	#     tools/visual_studio_plugins/patch_linker.rb
	#
	add_linker_flags("/ignore:4099")

	# In debug builds we get this warning because we link with some non-debug libraries
	# (for performance reasons). It is harmless, so we ignore it.
	add_linker_flags("/ignore:4098")

	if( PLATFORM_UWP )
		add_linker_flags(/IGNORE:4264)
	endif()

	# Debug information linker flags
	if( ENGINE_USE_DEBUG_INFO )
		add_exe_linker_flags(/DEBUG)
		add_exe_linker_flags(/MAP)
	else()
		add_exe_linker_flags(/MAP debug)
	endif()

	# Disable incremental linking
	replace_linker_flags("/INCREMENTAL" "/INCREMENTAL:NO" debug)
	if( PLATFORM_UWP )
		add_exe_linker_flags(/INCREMENTAL:NO)
	endif()

	# Enable multi-processor compilation for Visual Studio 2012 and above
	if( MSVC_VERSION GREATER 1600 OR PLATFORM_XBOXONE )
		add_compile_options(/MP)
	endif()

	# Platform specific options
	if( PLATFORM_XBOXONE )
		add_compile_options(/ZW)			# Consume Windows Runtime
		add_compile_options(/wd4530)		# C++ exception handler used, but unwind semantics are not enabled
		add_linker_flags("/ignore:4264")	# archiving object file compiled with /ZW into a static library
	elseif( PLATFORM_UWP )
		add_compile_options(/ZW)			# Consume Windows Runtime
		add_compile_options(/EHsc)			# C++ exceptions
	endif()

	# Clean-up linker flags case since VS IDE doesn't recognize them properly
	replace_linker_flags("/debug" "/DEBUG" debug)
	replace_linker_flags("/machine:x64" "/MACHINE:X64")

elseif( PLATFORM_PS4 )
	# Debug information
	if( ENGINE_USE_DEBUG_INFO )
		add_compile_options(-g)
		add_exe_linker_flags("-Wl,-Map=$(OutDir)$(TargetName).pdb")
	else()
		add_compile_options($<$<CONFIG:DEBUG>:-g>)
		add_exe_linker_flags("-Wl,-Map=$(OutDir)$(TargetName).pdb" debug)
	endif()

	# Enable full optimization in dev/release
	add_compile_options($<$<CONFIG:DEBUG>:-O0> $<$<NOT:$<CONFIG:DEBUG>>:-O3>)

	# Use fast floating point model
	add_compile_options(-ffast-math)

	# Disable specific warnings
	add_compile_options(-Wno-reorder -Wno-overloaded-virtual -Wno-missing-braces -Wno-logical-op-parentheses -Wno-unknown-pragmas)

	# Treat all other warnings as errors
	add_compile_options(-Werror -Wno-deprecated-declarations)
else()
	message(FATAL_ERROR "Unknown platform!")
endif()
