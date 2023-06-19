#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "lower/helpful.h"
#include "lower/framebuffer.h"
#include "vox/voxelshape.h"

class Game {
public:
    GLuint screen_vao;

    // Fundamental Functions
    void init();
    void loop();
    void render();

public:
    GLFWwindow* window;

    int start_width = 300;
    int start_height = 200;
    int current_width = start_width;
    int current_height = start_height;
    int width = 1200;
    int height = 800;
    bool maintain_width = true;

    GLuint pixel_program;
    GLuint scale_program;

    bool scale = true;
    framebuffer fbo;

    std::string shape_name = "vox/monu1.vox";
    VoxelShape shape = VoxelShape(shape_name.c_str());
};
