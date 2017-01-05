cmake_minimum_required(VERSION 3.1)
project (Plaine)

option(CLIENT "Compile the client" ON)
option(SERVER "Compile the server" ON)
option(DEBUG  "Set CMAKE_BUILD_TYPE to Debug" OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

if (DEBUG)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

if(CLIENT)
    add_subdirectory(CLIENT)
endif()

if(SERVER)
    add_subdirectory(SERVER)
endif()
