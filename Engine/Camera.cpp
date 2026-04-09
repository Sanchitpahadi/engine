#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

float speed = 5.0f;
float sensitivity = 0.1f;

Camera::Camera()
{
    Position = glm::vec3(0.0f, 0.0f, 3.0f);
    Yaw = -90.0f;
    Pitch = 0.0f;
}

glm::mat4 Camera::GetViewMatrix()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    return glm::lookAt(Position , Position +  glm::normalize(front), glm::vec3(0,1,0));
}

void Camera::ProcessKeyboard(GLFWwindow* window, float dt)
{
    float velocity = speed * dt;

    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = 0;
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0,1,0)));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Position += front * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Position -= front * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Position -= right * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Position += right * velocity;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        Position += glm::vec3(0, 1, 0) * velocity;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        Position -= glm::vec3(0, 1, 0) * velocity;
}

void Camera::ProcessMouse(float xoffset, float yoffset)
{
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (Pitch > 89.0f) Pitch = 89.0f;
    if (Pitch < -89.0f) Pitch = -89.0f;
}