cmake_minimum_required(VERSION 3.14)

project(fragiso)

# Pre-compile instructions for macOS
# 1. Install Xcode Command Line Tools: xcode-select --install
# 2. Install Homebrew: /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
# 3. Install CMake and Ninja: brew install cmake ninja
# 4. Install GLFW3, GLEW, and GLM: brew install glfw3 glew glm

# Pre-compile instructions for Windows
# 1. Install Visual Studio: https://visualstudio.microsoft.com/downloads/
# 2. Download and install CMake: https://cmake.org/download/
# 3. Download and install Ninja: https://github.com/ninja-build/ninja/releases
# 4. Download and install GLFW3: https://www.glfw.org/download.html
# 5. Download and install GLEW: http://glew.sourceforge.net/
# 6. Download GLM: https://github.com/g-truc/glm/releases
# 7. Add GLFW3, GLEW, and GLM include and library directories to your system's PATH environment variable

# Pre-compile instructions for Fedora
# 1. Install CMake, Ninja, GCC, and G++: sudo dnf install cmake ninja-build gcc-c++ -y
# 2. Install GLFW3, GLEW, and GLM: sudo dnf install glfw-devel glew-devel glm-devel -y

# Common compile & run instructions
# 1. mkdir build && cd build
# 2. cmake -G Ninja ..
# 3. ninja

file(GLOB MY_SRC "src/*" "src/lower/*" "src/vox/*" "src/png/*" "src/game/*")
add_executable(fragiso ${MY_SRC})

find_package(OpenGL REQUIRED)
target_link_libraries(fragiso ${OPENGL_LIBRARIES})

find_package(glfw3 REQUIRED)
target_link_libraries(fragiso glfw)

find_package(GLEW REQUIRED)
target_link_libraries(fragiso GLEW::GLEW)

find_package(glm REQUIRED)
#target_include_directories(fragiso PRIVATE GLEW::GLEW /usr/include)

target_compile_definitions(fragiso PRIVATE FPNG_NO_SSE=1)

# Use C++17 standard
set_target_properties(fragiso PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

# MacOS-specific configuration
if(APPLE)
    target_compile_options(fragiso PUBLIC -stdlib=libc++)
    target_link_libraries(fragiso "-framework OpenGL")
    set_target_properties(fragiso PROPERTIES LINK_FLAGS "-stdlib=libc++")
endif()

add_custom_command(
        TARGET fragiso
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E
        create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:fragiso>/shaders)

add_custom_command(
        TARGET fragiso
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E
        create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/vox $<TARGET_FILE_DIR:fragiso>/vox)