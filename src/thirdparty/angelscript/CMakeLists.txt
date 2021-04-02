cmake_minimum_required(VERSION 3.5)

project(angelscript)

option(BUILD_SHARED_LIBS "Build shared library" OFF)
option(AS_NO_EXCEPTIONS "Disable exception handling in script context" OFF)

if(APPLE)
    option(BUILD_FRAMEWORK "Build Framework bundle for OSX" OFF)
endif()

file(READ ../../include/angelscript.h ANGELSCRIPT_H)
string(REGEX MATCH "#define ANGELSCRIPT_VERSION_STRING \"([0-9]*).([0-9]*).([0-9]*)" ANGELSCRIPT_VERSION_REGEX ${ANGELSCRIPT_H})
set(ANGELSCRIPT_VERSION_MAJOR ${CMAKE_MATCH_1})
set(ANGELSCRIPT_VERSION_MINOR ${CMAKE_MATCH_2})
set(ANGELSCRIPT_VERSION_PATCH ${CMAKE_MATCH_3})
set(PROJECT_VERSION ${ANGELSCRIPT_VERSION_MAJOR}.${ANGELSCRIPT_VERSION_MINOR}.${ANGELSCRIPT_VERSION_PATCH})

message(STATUS "Configuring angelscript ${PROJECT_VERSION}")

find_package(Threads)

set(ANGELSCRIPT_HEADERS
    ../../include/angelscript.h
    as_array.h
    as_builder.h
    as_bytecode.h
    as_callfunc.h
    as_compiler.h
    as_config.h
    as_configgroup.h
    as_context.h
    as_criticalsection.h
    as_datatype.h
    as_debug.h
    as_generic.h
    as_map.h
    as_memory.h
    as_module.h
    as_objecttype.h
    as_outputbuffer.h
    as_parser.h
    as_property.h
    as_restore.h
    as_scriptcode.h
    as_scriptengine.h
    as_scriptfunction.h
    as_scriptnode.h
    as_scriptobject.h
    as_string.h
    as_string_util.h
    as_texts.h
    as_thread.h
    as_tokendef.h
    as_tokenizer.h
    as_typeinfo.h
    as_variablescope.h
)

set(ANGELSCRIPT_SOURCE
    as_atomic.cpp
    as_builder.cpp
    as_bytecode.cpp
    as_callfunc.cpp
    as_callfunc_mips.cpp
    as_callfunc_x86.cpp
    as_callfunc_x64_gcc.cpp
    as_callfunc_x64_msvc.cpp
    as_callfunc_x64_mingw.cpp
    as_compiler.cpp
    as_configgroup.cpp
    as_context.cpp
    as_datatype.cpp
    as_gc.cpp
    as_generic.cpp
    as_globalproperty.cpp
    as_memory.cpp
    as_module.cpp
    as_objecttype.cpp
    as_outputbuffer.cpp
    as_parser.cpp
    as_restore.cpp
    as_scriptcode.cpp
    as_scriptengine.cpp
    as_scriptfunction.cpp
    as_scriptnode.cpp
    as_scriptobject.cpp
    as_string.cpp
    as_string_util.cpp
    as_thread.cpp
    as_tokenizer.cpp
    as_typeinfo.cpp
    as_variablescope.cpp
)

if(MSVC AND CMAKE_CL_64)
    enable_language(ASM_MASM)
    if(CMAKE_ASM_MASM_COMPILER_WORKS)
        set(ANGELSCRIPT_SOURCE ${ANGELSCRIPT_SOURCE} as_callfunc_x64_msvc_asm.asm)
    else()
        message(FATAL ERROR "MSVC x86_64 target requires a working assembler")
    endif()
endif()

if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "^arm")
    enable_language(ASM)
    if(CMAKE_ASM_COMPILER_WORKS)
        set(ANGELSCRIPT_SOURCE ${ANGELSCRIPT_SOURCE} as_callfunc_arm.cpp as_callfunc_arm_gcc.S)
        set_property(SOURCE as_callfunc_arm_gcc.S APPEND PROPERTY COMPILE_FLAGS " -Wa,-mimplicit-it=always")
    else()
        message(FATAL ERROR "ARM target requires a working assembler")
    endif()
endif()

if(MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")  
endif()

if(NOT BUILD_FRAMEWORK)
    set(ANGELSCRIPT_LIBRARY_NAME angelscript)
else()
    set(ANGELSCRIPT_LIBRARY_NAME Angelscript) # OS X frameworks should have capitalized name
    set(BUILD_SHARED_LIBS TRUE)
endif()
set(ANGELSCRIPT_LIBRARY_NAME ${ANGELSCRIPT_LIBRARY_NAME} CACHE STRING "" FORCE)

add_library(${ANGELSCRIPT_LIBRARY_NAME} ${ANGELSCRIPT_SOURCE} ${ANGELSCRIPT_HEADERS})

target_include_directories(${ANGELSCRIPT_LIBRARY_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../include)

if(MSVC)
    target_compile_definitions(${ANGELSCRIPT_LIBRARY_NAME} PRIVATE -D_CRT_SECURE_NO_WARNINGS)    
endif()

target_compile_definitions(${ANGELSCRIPT_LIBRARY_NAME} PRIVATE -DANGELSCRIPT_EXPORT -D_LIB)

if(AS_NO_EXCEPTIONS)
	target_compile_definitions(${ANGELSCRIPT_LIBRARY_NAME} PRIVATE AS_NO_EXCEPTIONS)
endif()

# Fix x64 issues on Linux
if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64" AND UNIX AND NOT APPLE)
    target_compile_options(${ANGELSCRIPT_LIBRARY_NAME} PRIVATE -fPIC)
endif()

# Don't override the default library output path to avoid conflicts when building for multiple target platforms
#set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../lib)
target_link_libraries(${ANGELSCRIPT_LIBRARY_NAME} Threads::Threads)

set_target_properties(${ANGELSCRIPT_LIBRARY_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

if(BUILD_FRAMEWORK)
    set_target_properties(${ANGELSCRIPT_LIBRARY_NAME} PROPERTIES
        FRAMEWORK TRUE
        FRAMEWORK_VERSION ${PROJECT_VERSION}
        MACOSX_FRAMEWORK_IDENTIFIER com.angelcode.Angelscript
        MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${PROJECT_VERSION}
        MACOSX_FRAMEWORK_BUNDLE_VERSION ${PROJECT_VERSION}
        XCODE_ATTRIBUTE_INSTALL_PATH "@rpath"
        PUBLIC_HEADER ../../include/angelscript.h
    )
endif()

if(MSVC)
    set_target_properties(${ANGELSCRIPT_LIBRARY_NAME} PROPERTIES COMPILE_FLAGS "/MP")
endif()

# Don't override the default runtime output path to avoid conflicts when building for multiple target platforms
#set(RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../bin)

#See https://cmake.org/cmake/help/latest/manual/cmake-packages.7.html#creating-packages for a detailed explanation about this part
install(TARGETS ${ANGELSCRIPT_LIBRARY_NAME} EXPORT AngelscriptTargets
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	INCLUDES DESTINATION include
)

install(FILES
	${CMAKE_CURRENT_SOURCE_DIR}/../../include/angelscript.h
	DESTINATION include
	COMPONENT Devel
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/Angelscript/AngelscriptConfigVersion.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

export(EXPORT AngelscriptTargets
	FILE "${CMAKE_CURRENT_BINARY_DIR}/Angelscript/AngelscriptTargets.cmake"
	NAMESPACE Angelscript::
)
configure_file(cmake/AngelscriptConfig.cmake
	"${CMAKE_CURRENT_BINARY_DIR}/Angelscript/AngelscriptConfig.cmake"
	COPYONLY
)

set(ConfigPackageLocation lib/cmake/Angelscript)
install(EXPORT AngelscriptTargets
	FILE AngelscriptTargets.cmake
	NAMESPACE Angelscript::
	DESTINATION ${ConfigPackageLocation}
)
install(
	FILES
		cmake/AngelscriptConfig.cmake
		"${CMAKE_CURRENT_BINARY_DIR}/Angelscript/AngelscriptConfigVersion.cmake"
	DESTINATION ${ConfigPackageLocation}
	COMPONENT Devel
)

