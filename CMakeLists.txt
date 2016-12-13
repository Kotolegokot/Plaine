cmake_minimum_required(VERSION 3.1)
project(Plaine)

set(CMAKE_REQUIRED_FLAGS "-std=c++14")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DDEBUG_OUTPUT=false")
set(CMAKE_CXX_FLAGS_DEBUG "-g" )
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/Modules)
find_package(Irrlicht REQUIRED)
find_package(Bullet REQUIRED)
find_package(Freetype REQUIRED)
find_package(Threads REQUIRED)

file(GLOB_RECURSE CPPs "src/*.cpp")
file(GLOB_RECURSE Hs "include/*.h")
add_executable(${PROJECT_NAME} main.cpp ${CPPs} ${Hs})

include_directories("include/")
include_directories(SYSTEM
	${IRRLICHT_INCLUDE_DIR}
	${BULLET_INCLUDE_DIRS}
	${FREETYPE_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}
	${IRRLICHT_LIBRARY}
	${BULLET_COLLISION_LIBRARY}
	${BULLET_DYNAMICS_LIBRARY}
	${BULLET_MATH_LIBRARY}
	${FREETYPE_LIBRARY}
	Threads::Threads)

if(DEBUG)
	set(CMAKE_BUILD_TYPE "Debug")
endif()
