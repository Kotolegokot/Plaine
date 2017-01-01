cmake_minimum_required(VERSION 3.1)
project(PlaineServer)

set(CMAKE_REQUIRED_FLAGS "-std=c++14")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/Modules)
find_package(Bullet REQUIRED)
find_package(SFML REQUIRED COMPONENTS network)
find_package(Threads REQUIRED)
file(GLOB_RECURSE CPPs "src/*.cpp")
file(GLOB_RECURSE HPPs "src/*.hpp")
add_executable(${PROJECT_NAME} ${CPPs} ${HPPs})

include_directories("src/")
include_directories(SYSTEM
    ${BULLET_INCLUDE_DIRS}
    ${SFML_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME}
    ${BULLET_COLLISION_LIBRARY}
    ${BULLET_DYNAMICS_LIBRARY}
    ${BULLET_MATH_LIBRARY}
    ${SFML_LIBRARIES}
    Threads::Threads)

if(DEBUG)
    set(CMAKE_BUILD_TYPE "Debug")
endif()
