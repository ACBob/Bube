cmake_minimum_required(VERSION 3.16)
project(BubeEngine)

# TODO: What's the disbenefit of compiling without -fno-exceptions?
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fomit-frame-pointer -ffast-math -Wall -fsigned-char -fno-rtti")

## enet
add_subdirectory(src/thirdparty/enet)
## freetype
add_subdirectory(src/thirdparty/FreeType)
## angelscript
add_subdirectory(src/thirdparty/angelscript)

option(BUBE_BUILD_CLIENT "Enable building the client" ON)
option(BUBE_BUILD_SERVER "Enable building the dedicated server" ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/game)

## client
if (BUBE_BUILD_CLIENT)
    include(src/ClientCMakeLists.txt)
endif()

## server
if (BUBE_BUILD_SERVER)
    include(src/ServerCMakeLists.txt)
endif()