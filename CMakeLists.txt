cmake_minimum_required(VERSION 3.23)
project(fragiso)

set(CMAKE_CXX_STANDARD 14)

add_executable(fragiso src/main.cpp src/game.cpp src/game.h src/voxelshape.h src/helpful.cpp src/helpful.h)

find_package(GLEW REQUIRED)
target_link_libraries(fragiso PRIVATE GLEW::GLEW)

# this is heuristically generated, and may not be correct
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(fragiso PRIVATE glfw)

add_custom_command(
        TARGET fragiso
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E
        create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:fragiso>/shaders)