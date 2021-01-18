cmake_minimum_required(VERSION 3.16)
project(BubeEngine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fomit-frame-pointer -ffast-math -Wall -fsigned-char -fno-exceptions -fno-rtti")

## enet
add_subdirectory(src/enet)

option(BUILD_CLIENT "Enable building the client" ON)
option(BUILD_SERVER "Enable building the dedicated server" ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/game)

## client
if (BUILD_CLIENT)
    include(src/ClientCMakeLists.txt)
endif()

## server
if (BUILD_SERVER)
    include(src/ServerCMakeLists.txt)
endif()