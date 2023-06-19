#include "game.h"
#include "lower/boilerplate.h"
#include <chrono>
#include <iostream>
#include "game/camera.h"
#include "lower/helpful.h"

extern Game game;

VoxelShape calculateNormals(VoxelShape& vox);

void windowSizeCallback(GLFWwindow* window, int width, int height)
{
    game.width = width;
    game.height = height;

    float aspect_ratio = float(width)/float(height);
    float scale = 1;

    if (game.maintain_width)
    {
        scale = width / game.current_width;
        game.current_height = height/scale;

        game.width = scale * game.current_width;
        game.height = scale * game.current_height;
    }
    else
    {

    }

    game.fbo.regen();
}

void Game::init()
{
    window = initWindow("isometric", width, height, true);

    // Set the clear color to fill the screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Create the vertex buffer object and vertex array object for the rectangle
    screen_vao = generateQuadVAO();

    // Read and build the shader pixel_program
    pixel_program = createShaderProgram("shaders/vertex.vert", "shaders/fragment.frag");

    shape.send(pixel_program, "voxelShape");

    fbo.create(width, height);
    //calculateNormals(shape);

    if(glewIsSupported("GL_ARB_debug_output"))
    {
        glDebugMessageCallback(openglCallbackFunction, nullptr);
        glEnable(GL_DEBUG_OUTPUT);
    }

    glfwSetWindowSizeCallback(window, windowSizeCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    sendUniform2fSafely(pixel_program, "resolution", float(current_width), float(current_height));
}

void Game::loop()
{
    auto start = std::chrono::high_resolution_clock::now();
    auto last = std::chrono::high_resolution_clock::now();
    while (!glfwWindowShouldClose(window))
    {
        auto now = std::chrono::high_resolution_clock::now();

        render();

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        sendUniform1fSafely(pixel_program, "time", ((float)duration.count()/1000.f));
        ProcessInput(window);
        SendOrthoCamera(pixel_program);
        //SendFPSCamera(pixel_program);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Game::render() {
    // Apply the shader pixel_program and render a rectangle
    glUseProgram(pixel_program);

    glBindVertexArray(screen_vao);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, scale*fbo.handle);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    if (scale)
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo.handle);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, current_width, current_height,
                          0, 0, width, height,
                          GL_COLOR_BUFFER_BIT,
                          GL_NEAREST);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    }

    glBindVertexArray(0);
}