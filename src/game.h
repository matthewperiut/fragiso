#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "helpful.h"

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

public:
    GLFWwindow* window;
    int start_width = 256;
    int start_height = 144;
    int current_width = start_width;
    int current_height = start_height;
    int width = 1280;
    int height = 720;
    GLuint pixel_program;
    GLuint scale_program;

    bool scale = true;
    GLuint fbo_handle;
    GLuint fbo_texture;

    VoxelShape shape = convertMagicaVoxelToVoxelShape("vox/monu1.vox");

    void createFBO();

    void fps();

    struct Camera
    {
        // We will forget about the decimal values until we implement the scale program which will implement decimal camera movement
        double x, y;
    };
    Camera camera{ 77, 47 };
    void sendCamera();

    bool maintain_width = true;

    void regenFBO();
};
