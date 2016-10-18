cmake_minimum_required(VERSION 2.6)
project(PlaneRunner)

set(CMAKE_REQUIRED_FLAGS "-std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DDEBUG_OUTPUT=false")
set(CMAKE_CXX_FLAGS_DEBUG "-g" )
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")
find_package(Irrlicht)
find_package(Bullet)
find_package(Freetype)

file(GLOB CPPs src/*.cpp src/obstacles/*.cpp)
file(GLOB Hs include/*.h)
add_executable(${PROJECT_NAME} main.cpp ${CPPs} ${Hs})

include_directories("include/"
	${IRRLICHT_INCLUDE_DIR}
	${BULLET_INCLUDE_DIRS}
	${FREETYPE_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}
	${IRRLICHT_LIBRARY}
	${BULLET_COLLISION_LIBRARY}
	${BULLET_DYNAMICS_LIBRARY}
	${BULLET_MATH_LIBRARY}
	${FREETYPE_LIBRARY})


