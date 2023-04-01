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

    int start_width = 256;
    int start_height = 144;
    int current_width = start_width;
    int current_height = start_height;
    int width = 1280;
    int height = 720;
    bool maintain_width = true;

    GLuint pixel_program;
    GLuint scale_program;

    bool scale = true;
    framebuffer fbo;

    std::string shape_name = "vox/monu1.vox";
    VoxelShape shape = VoxelShape(shape_name.c_str());


    struct Camera
    {
        // We will forget about the decimal values until we implement the scale program which will implement decimal camera movement
        double x, y;
    };
    Camera camera{ 77, 47 };
    void processCamera();


};
