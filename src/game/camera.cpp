#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

glm::vec3 rayDirection{1,1,1};

glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
float cameraPitch = 0.0f;
float cameraYaw = 90.0f;

bool mouse = false;

void UpdateRayDirection()
{
    rayDirection.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    rayDirection.y = sin(glm::radians(cameraPitch));
    rayDirection.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    rayDirection = glm::normalize(rayDirection);
    std::cout << rayDirection.x << " " << rayDirection.y << " " << rayDirection.z << " " << std::endl;
}

void ProcessInput(GLFWwindow* window)
{
    const float cameraSpeed = 0.05f;
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPosition += cameraSpeed * rayDirection;
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPosition -= cameraSpeed * rayDirection;
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPosition -= glm::normalize(glm::cross(rayDirection, glm::vec3(0.0f, 1.0f, 0.0f))) * cameraSpeed;
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPosition += glm::normalize(glm::cross(rayDirection, glm::vec3(0.0f, 1.0f, 0.0f))) * cameraSpeed;

    static bool canmouse = true;
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && canmouse)
    {
        mouse = !mouse;
        if (mouse)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        canmouse = false;
    }
    else if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
    {
        canmouse = true;
    }
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (!mouse)
        return;

    static float lastX = 400, lastY = 300;
    static bool firstMouse = true;

    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    cameraYaw += xoffset;
    cameraPitch += yoffset;

    if(cameraPitch > 89.0f)
        cameraPitch = 89.0f;
    if(cameraPitch < -89.0f)
        cameraPitch = -89.0f;

    UpdateRayDirection();
}

void SendCamera(unsigned int shaderProgram)
{
    GLint cameraPositionLocation = glGetUniformLocation(shaderProgram, "cameraPosition");
    GLint rayDirectionLocation = glGetUniformLocation(shaderProgram, "cameraDirection");

    //std::cout << cameraPosition.x << " " << cameraPosition.y << " " << cameraPosition.z << " " << std::endl;
    //std::cout << rayDirection.x << " " << rayDirection.y << " " << rayDirection.z << " " << std::endl;

    glUniform3fv(cameraPositionLocation, 1, glm::value_ptr(cameraPosition));
    glUniform3fv(rayDirectionLocation, 1, glm::value_ptr(rayDirection));
}
