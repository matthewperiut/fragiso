#pragma once

#include <GLFW/glfw3.h>

void UpdateRayDirection();
void ProcessInput(GLFWwindow* window);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void SendFPSCamera(unsigned int program);
void SendOrthoCamera(unsigned int program);
void Send2DCamera(unsigned int program);