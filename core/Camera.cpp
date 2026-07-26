#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
Camera::Camera()
{
    Yaw = -90.0f;
    Pitch = 0.0f;
    UpdateVectors();
}

void Camera::UpdateVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    Front = glm::normalize(front);
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(Position, Position + Front, glm::vec3(0,1,0));
}

void Camera::SetProjection(float fov, float aspect, float near, float far)
{
    projection = glm::perspective(glm::radians(fov), aspect, near, far);
}

glm::mat4 Camera::GetProjection() const
{
    return projection;
}

void Camera::ProcessKeyboard(GLFWwindow* window, float dt)
{
    float velocity = speed * dt;
    glm::vec3 right = glm::normalize(glm::cross(Front, glm::vec3(0,1,0)));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Position += Front * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Position -= Front * velocity;
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

    UpdateVectors();
}