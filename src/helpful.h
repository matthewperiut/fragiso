#pragma once

#include <gl/glew.h>
#include <iostream>
#include "voxelshape.h"

VoxelShape convertMagicaVoxelToVoxelShape(const char* filename);
void GLAPIENTRY openglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);