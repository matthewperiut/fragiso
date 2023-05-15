#pragma once

#include <GL/glew.h>
#include <iostream>

void GLAPIENTRY openglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
void sendUniform3fSafely(GLuint program, std::string name, float x, float y, float z);
void sendUniform1fSafely(GLuint program, std::string name, float t);