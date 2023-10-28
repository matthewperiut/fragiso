//
// Created by Matthew Periut on 3/31/23.
//

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

GLFWwindow* initWindow(std::string title, int width, int height, bool vsync, bool resize);
GLuint compileShader(const char* shaderSource, GLenum shaderType);
GLuint createShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
GLuint generateQuadVAO();
void updateTimeUniform(GLuint program);