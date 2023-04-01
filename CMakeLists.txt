cmake_minimum_required(VERSION 3.23)
project(fragiso)

set(CMAKE_CXX_STANDARD 14)

file (GLOB MY_SRC "src/*" "src/lower/*" "src/vox/*")

add_executable(fragiso ${MY_SRC})

find_package(GLEW REQUIRED)
target_link_libraries(fragiso PRIVATE GLEW::GLEW)

# this is heuristically generated, and may not be correct
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(fragiso PRIVATE glfw)

add_custom_command(
        TARGET fragiso
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E
        create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:fragiso>/shaders)

add_custom_command(
        TARGET fragiso
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E
        create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/vox $<TARGET_FILE_DIR:fragiso>/vox)