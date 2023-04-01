//
// Created by Matthew Periut on 3/31/23.
//

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

GLFWwindow* initWindow(std::string title, int width, int height, bool vsync);
GLuint compileShader(const char* shaderSource, GLenum shaderType);
GLuint createShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
GLuint generateQuadVAO();