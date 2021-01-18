cmake_minimum_required(VERSION 3.16)
project(BubeEngine)

## enet
add_subdirectory(enet)

option(BUILD_CLIENT "Enable building the client" ON)
option(BUILD_SERVER "Enable building the dedicated server" OFF)

## client
if (BUILD_CLIENT)
    include(src/ClientCMakeLists.txt)
endif()

## server
## FIXME FIXME FIXME FIXME When building the server, it gets in the way of the client building by defining STANDALONE
if (BUILD_SERVER)
    include(src/ServerCMakeLists.txt)
endif()