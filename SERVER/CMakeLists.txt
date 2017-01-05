cmake_minimum_required(VERSION 3.1)

set(EXECUTABLE_NAME server)

set(CMAKE_REQUIRED_FLAGS "-std=c++14")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/Modules)
find_package(Bullet REQUIRED)
find_package(Threads REQUIRED)
find_package(ASIO REQUIRED)
file(GLOB_RECURSE CPPs "src/*.cpp")
file(GLOB_RECURSE HPPs "src/*.hpp")
add_executable(${EXECUTABLE_NAME} ${CPPs} ${HPPs})

include_directories("src/")
include_directories(SYSTEM
    ${BULLET_INCLUDE_DIRS}
    ${ASIO_INCLUDE_DIR})

target_link_libraries(${EXECUTABLE_NAME}
    ${BULLET_COLLISION_LIBRARY}
    ${BULLET_DYNAMICS_LIBRARY}
    ${BULLET_MATH_LIBRARY}
    Threads::Threads)
