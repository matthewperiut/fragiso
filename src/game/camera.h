#pragma once

#include <GLFW/glfw3.h>

void UpdateRayDirection();
void ProcessInput(GLFWwindow* window);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void SendCamera(unsigned int program);