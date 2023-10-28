#include "game.h"
#include "lower/boilerplate.h"
#include <chrono>
#include <iostream>
#include "game/camera.h"
#include "lower/helpful.h"


VoxelShape calculateNormals(VoxelShape& vox);

void Game::init()
{
    window = initWindow("isometric", width, height, true, true);

    // Create the vertex buffer object and vertex array object for the rectangle
    screen_vao = generateQuadVAO();

    // Read and build the shader pixel_program
    pixel_program = createShaderProgram("shaders/vertex.vert", "shaders/fragment.frag");

    shape.send(pixel_program, "voxelShape");

    fbo.create(width, height);

    if(glewIsSupported("GL_ARB_debug_output"))
    {
        glDebugMessageCallback(openglCallbackFunction, nullptr);
        glEnable(GL_DEBUG_OUTPUT);
    }

    sendUniform2fSafely(pixel_program, "resolution", float(current_width), float(current_height));
}

void Game::loop()
{
    while (!glfwWindowShouldClose(window))
    {
        render();

        ProcessInput(window);
        Send2DCamera(pixel_program);

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

    updateTimeUniform(pixel_program);
}