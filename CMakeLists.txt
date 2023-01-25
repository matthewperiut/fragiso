cmake_minimum_required(VERSION 3.23)
project(fragiso)

set(CMAKE_CXX_STANDARD 14)

add_executable(fragiso src/main.cpp src/game.cpp src/game.h src/voxelshape.h)

find_package(GLEW REQUIRED)
target_link_libraries(fragiso PRIVATE GLEW::GLEW)

# this is heuristically generated, and may not be correct
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(fragiso PRIVATE glfw)

file(COPY "shaders" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})