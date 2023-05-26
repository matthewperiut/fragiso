#include "helpful.h"

#include <iostream>
#include <vector>
#include <fstream>

#include <chrono>
#include <iostream>


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

#ifdef DEBUG
    #include <algorithm>
    std::vector<std::string> not_found;
    bool containsString(const std::vector<std::string>& strings, const std::string& searchString)
    {
        return std::find(strings.begin(), strings.end(), searchString) != strings.end();
    }
#endif

void sendUniform4fSafely(GLuint program, std::string name, float r, float g, float b, float a)
{


    glUseProgram(program);
    GLuint location = glGetUniformLocation(program, name.c_str());
    if(location == -1)
    {
#ifdef DEBUG
        if (containsString(not_found, name))
        {
            return;
        }
        else
        {
            not_found.emplace_back(name);
        }
#endif
        std::cout << "Uniform " + name + " not found or not active" << std::endl;
    }
    else
    {
        glUniform4f(location, r, g, b, a);
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cout << "(" + name + ") Error: " << error << std::endl;
        }
    }
}

void sendUniform3fSafely(GLuint program, std::string name, float x, float y, float z)
{
    glUseProgram(program);
    GLuint location = glGetUniformLocation(program, name.c_str());
    if(location == -1)
    {
#ifdef DEBUG
        if (containsString(not_found, name))
        {
            return;
        }
        else
        {
            not_found.emplace_back(name);
        }
#endif
        std::cout << "Uniform " + name + " not found or not active" << std::endl;
    }
    else
    {
        glUniform3f(location, x, y, z);
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cout << "(" + name + ") Error: " << error << std::endl;
        }
    }
}

void sendUniform2fSafely(GLuint program, std::string name, float x, float y)
{
    glUseProgram(program);
    GLuint location = glGetUniformLocation(program, name.c_str());
    if(location == -1)
    {
#ifdef DEBUG
        if (containsString(not_found, name))
        {
            return;
        }
        else
        {
            not_found.emplace_back(name);
        }
#endif
        std::cout << "Uniform " + name + " not found or not active" << std::endl;
    }
    else
    {
        glUniform2f(location, x, y);
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
    {
#ifdef DEBUG
        if (containsString(not_found, name))
        {
            return;
        }
        else
        {
            not_found.emplace_back(name);
        }
#endif
        std::cout << "Uniform " + name + " not found or not active" << std::endl;
    }
    else
    {
        glUniform1f(location, t);
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cout << "(" + name + ") Error: " << error << std::endl;
        }
    }
}