#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "../lower/helpful.h"

glm::vec3 cameraDirection = glm::vec3(-1.0f, 0.0f, 0.0f);
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);

float cameraPitch = 0.0f;
float cameraYaw = 90.0f;

bool mouse = false;

void UpdateRayDirection()
{
    cameraDirection.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraDirection.y = sin(glm::radians(cameraPitch));
    cameraDirection.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraDirection = glm::normalize(cameraDirection);
}

static double lastX = 400.0f, lastY = 300.0f;
void ProcessInput(GLFWwindow* window)
{
    const float cameraSpeed = 0.05f;

    glm::vec3 cameraDirectionXZ = cameraDirection;
    cameraDirectionXZ.y = 0.0f;  // Remove the Y component of the camera direction
    cameraDirectionXZ = glm::normalize(cameraDirectionXZ);  // Normalize the vector after modification

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPosition += cameraSpeed * cameraDirectionXZ;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPosition -= cameraSpeed * cameraDirectionXZ;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPosition -= glm::normalize(glm::cross(cameraDirectionXZ, glm::vec3(0.0f, 1.0f, 0.0f))) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPosition += glm::normalize(glm::cross(cameraDirectionXZ, glm::vec3(0.0f, 1.0f, 0.0f))) * cameraSpeed;

// Add vertical movement for Q and E keys
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cameraPosition += glm::vec3(0.0f, cameraSpeed, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cameraPosition -= glm::vec3(0.0f, cameraSpeed, 0.0f);

    static bool canmouse = true;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && canmouse)
    {
        mouse = !mouse;
        if (mouse)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        canmouse = false;
    }
    else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
    {
        canmouse = true;
        glfwGetCursorPos(window, &lastX, &lastY);
    }
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (!mouse)
        return;

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    cameraYaw += xoffset;
    cameraPitch += yoffset;

    if (cameraPitch > 89.0f)
        cameraPitch = 89.0f;
    if (cameraPitch < -89.0f)
        cameraPitch = -89.0f;

    UpdateRayDirection();
}

void SendFPSCamera(unsigned int shaderProgram)
{
    // Get the location of the viewMatrix uniform in the shader program
    GLint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");

    // Set the viewMatrix uniform using glUniformMatrix4fv
    glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, glm::vec3(0.0f, 1.0f, 0.0f));

    // Activate the shader program before setting the uniform
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    sendUniform1fSafely(shaderProgram, "zoom", 1);
}

glm::vec3 centerPoint = glm::vec3(0,0,0);
float cameraDistance = 3;
void SendOrthoCamera(unsigned int shaderProgram)
{
    // Get the location of the viewMatrix uniform in the shader program
    GLint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");

    // Convert degrees to radians
    float cameraPitchRadians = glm::radians(cameraPitch);
    float cameraYawRadians = glm::radians(cameraYaw);

    // Update camera position based on the distance and angles
    cameraPosition.x = centerPoint.x + cameraDistance * cos(cameraPitchRadians) * sin(cameraYawRadians);
    cameraPosition.y = centerPoint.y + cameraDistance * sin(cameraPitchRadians);
    cameraPosition.z = centerPoint.z + cameraDistance * cos(cameraPitchRadians) * cos(cameraYawRadians);

    // Set the viewMatrix uniform using glUniformMatrix4fv
    glm::mat4 viewMatrix = glm::lookAt(cameraPosition, centerPoint, glm::vec3(0.0f, 1.0f, 0.0f));

    // Activate the shader program before setting the uniform
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    sendUniform1fSafely(shaderProgram, "zoom", 0.25);
}

void Send2DCamera(unsigned int shaderProgram)
{
    GLint cameraPositionLocation = glGetUniformLocation(shaderProgram, "cameraPosition");
    glUniform2f(cameraPositionLocation, cameraPosition.z*50, cameraPosition.x*50);
}
