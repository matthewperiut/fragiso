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
    int width = 1280;
    int height = 720;
    GLuint pixelProgram;
    GLuint scaleProgram;

    bool scale = true;
    GLuint fbo_handle;
    GLuint fbo_texture;

    VoxelShape shape = VoxelShape(10,10,10);

    void createFBO();

    void fps();

    struct Camera
    {
        // We will forget about the decimal values until we implement the scale program which will implement decimal camera movement
        double x, y;
    };
    Camera camera{ 77, 47 };
    void sendCamera();
};
