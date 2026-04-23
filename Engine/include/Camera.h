#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
    glm::vec3 Position = glm::vec3(0.0f, 0.0f, 3.0f);


    Camera();

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjection() const;

    void SetProjection(float fov, float aspect, float near, float far);

    void ProcessKeyboard(GLFWwindow* window, float dt);
    void ProcessMouse(float xoffset, float yoffset);

private:
    void UpdateVectors();

    glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up    = glm::vec3(0.0f, 1.0f,  0.0f);
    float Yaw;
    float Pitch;

    float speed = 5.0f;
    float sensitivity = 0.1f;

    glm::mat4 projection;

};