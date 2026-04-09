#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
    glm::vec3 Position;
    float Yaw;
    float Pitch;

    Camera();

    glm::mat4 GetViewMatrix();

    void ProcessKeyboard(GLFWwindow* window, float deltaTime);
    void ProcessMouse(float xoffset, float yoffset);
};