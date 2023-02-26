#include "helpful.h"

#define OGT_VOX_IMPLEMENTATION
#include "ogt_vox.h"

#include <iostream>
#include <vector>
#include <fstream>

#include <chrono>
#include <iostream>

VoxelShape convertMagicaVoxelToVoxelShape(const char* filename)
{
    auto start = std::chrono::high_resolution_clock::now(); // Get the current time

    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size))
    {
        /* worked! */
        const ogt_vox_scene* scene = ogt_vox_read_scene(reinterpret_cast<const uint8_t *>(buffer.data()), buffer.size());
        const ogt_vox_model* model = scene->models[0];

        VoxelShape voxelShape = VoxelShape(model->size_y,model->size_z,model->size_x);

        for (int x = 0; x < model->size_x; x++)
        {
            for (int y = 0; y < model->size_y; y++)
            {
                for (int z = 0; z < model->size_z; z++)
                {
                    int voxel_index = x + (y * model->size_x) + (z * model->size_x * model->size_y);
                    uint8_t color_index = model->voxel_data[voxel_index];
                    ogt_vox_rgba color = scene->palette.color[color_index];
                    voxelShape.setPixel(y,z,x, RGBA(color.r, color.g, color.b, color.a));
                }
            }
        }

        auto end = std::chrono::high_resolution_clock::now(); // Get the current time again

        // Calculate the time difference
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "\"" << filename << "\" loaded in " << duration.count() << " milliseconds" << std::endl;

        return voxelShape;
    }
    else
    {
        std::cout << "Failed to load vox" << std::endl;
        exit(-1);
    }
}

void GLAPIENTRY openglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    std::cout << "---------------------opengl-callback-start------------" << std::endl;
    std::cout << "message: "<< message << std::endl;
    std::cout << "type: ";
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            std::cout << "ERROR";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            std::cout << "DEPRECATED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            std::cout << "UNDEFINED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            std::cout << "PERFORMANCE";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            std::cout << "PORTABILITY";
            break;
        case GL_DEBUG_TYPE_MARKER:
            std::cout << "MARKER";
            break;
        case GL_DEBUG_TYPE_OTHER:
            std::cout << "OTHER";
            break;
    }
    std::cout << std::endl;

    std::cout << "id: " << id << std::endl;
    std::cout << "severity: ";
    switch (severity) {
        case GL_DEBUG_SEVERITY_LOW:
            std::cout << "LOW";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            std::cout << "MEDIUM";
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            std::cout << "HIGH";
            break;
    }
    std::cout << std::endl;
    std::cout << "---------------------opengl-callback-end--------------" << std::endl;
}

void sendUniform3fSafely(GLuint program, std::string name, float x, float y, float z)
{
    glUseProgram(program);
    GLuint location = glGetUniformLocation(program, name.c_str());
    if(location == -1)
        std::cout << "Uniform " + name + " not found or not active" << std::endl;
    else
    {
        glUniform3f(location, x, y, z);
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cout << "(" + name + ") Error: " << error << std::endl;
        }
    }
}

void sendUniform1fSafely(GLuint program, std::string name, float t)
{
    glUseProgram(program);
    GLuint location = glGetUniformLocation(program, name.c_str());
    if(location == -1)
        std::cout << "Uniform " + name + " not found or not active" << std::endl;
    else
    {
        glUniform1f(location, t);
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cout << "(" + name + ") Error: " << error << std::endl;
        }
    }
}