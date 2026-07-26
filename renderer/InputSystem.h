#pragma once
#include <GLFW/glfw3.h>

class InputSystem
{
public:
    void Update(GLFWwindow* window, float dt)
    {
        // Example: Escape closes app
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }
};