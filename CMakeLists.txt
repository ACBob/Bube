cmake_minimum_required(VERSION 3.16)
project(BubeEngine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fomit-frame-pointer -ffast-math -Wall -fsigned-char -fno-exceptions -fno-rtti")

## enet
add_subdirectory(src/thirdparty/enet)
## freetype
add_subdirectory(src/thirdparty/FreeType)

option(BUBE_BUILD_CLIENT "Enable building the client" ON)
option(BUBE_BUILD_SERVER "Enable building the dedicated server" ON)
option(BUBE_COMPILE_LUA "Enable compilation of the Lua Api. (Enables/Disables Lua)" ON)
option(BUBE_NEW_MAP "Use the new Bube map format. Expect bugs!" ON)

if(BUBE_NEW_MAP)
    add_definitions(-DBUBE_NEW_MAP)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/game)

## client
if (BUBE_BUILD_CLIENT)
    include(src/ClientCMakeLists.txt)
endif()

## server
if (BUBE_BUILD_SERVER)
    include(src/ServerCMakeLists.txt)
endif()