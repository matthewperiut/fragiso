#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "voxelshape.h"

class Game {
public:
    GLuint VBO, VAO, EBO;

    // Fundamental Functions
    void init();
    void loop();
    void render();
    void readShader(GLuint& program, const char* vertexPath, const char* fragmentPath);
    void validateProgram(GLuint programToValidate);

    // Game Functionality
    void sendVoxelShapeToFragmentShader(VoxelShape& shape) const;

private:
    GLFWwindow* window;
    GLuint pixelProgram;
    GLuint scaleProgram;

    GLuint fbo_handle;
    GLuint fbo_texture;

    VoxelShape shape = VoxelShape(11,11,11);

    void createFBO();
};
