cmake_minimum_required(VERSION 3.1)
project(PlaineClient)

set(EXECUTABLE_NAME client)

set(CMAKE_REQUIRED_FLAGS "-std=c++14")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DDEBUG_OUTPUT=false")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

if(NOT COMMON_BUILD)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/../cmake/Modules)
endif()

find_package(Irrlicht REQUIRED)
find_package(Bullet REQUIRED)
find_package(Freetype REQUIRED)
find_package(SFML REQUIRED COMPONENTS audio)
find_package(Threads REQUIRED) 
file(GLOB_RECURSE CPPs "src/*.cpp")
file(GLOB_RECURSE Hs   "src/*.h")
add_executable(${EXECUTABLE_NAME} ${CPPs} ${Hs})

include_directories("src/")
include_directories(SYSTEM
	${IRRLICHT_INCLUDE_DIR}
	${BULLET_INCLUDE_DIRS}
	${FREETYPE_INCLUDE_DIRS}
    ${SFML_INCLUDE_DIR})

target_link_libraries(${EXECUTABLE_NAME}
	${IRRLICHT_LIBRARY}
	${BULLET_COLLISION_LIBRARY}
	${BULLET_DYNAMICS_LIBRARY}
	${BULLET_MATH_LIBRARY}
	${FREETYPE_LIBRARY}
    ${SFML_LIBRARIES}
	Threads::Threads)
