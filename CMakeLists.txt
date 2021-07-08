cmake_minimum_required(VERSION 3.19)
project(boids)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Simulation.cpp Simulation.h Boid.cpp Boid.h Vector2d.cpp Vector2d.h)
add_executable(boids ${SOURCE_FILES})
include_directories(/usr/local/include)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(boids sfml-system sfml-window sfml-graphics sfml-audio sfml-network)