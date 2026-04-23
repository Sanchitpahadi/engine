#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform {
public:
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f); // in degrees
    glm::vec3 scale    = glm::vec3(1.0f);

    glm::mat4 GetMatrix() const
    {
        glm::mat4 model = glm::mat4(1.0f);

        // translation
        model = glm::translate(model, position);

        // rotation (XYZ order)
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1,0,0));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0,1,0));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0,0,1));

        // scale
        model = glm::scale(model, scale);

        return model;
    }

    void movement(Window& w, float dt) {
        float speed = 3.0f;
        float velocity = speed * dt;

        if (w.IsKeyPressed(GLFW_KEY_I)) position.z -= speed * dt;
        if (w.IsKeyPressed(GLFW_KEY_K)) position.z += speed * dt;
        if (w.IsKeyPressed(GLFW_KEY_J)) position.x -= speed * dt;
        if (w.IsKeyPressed(GLFW_KEY_L)) position.x += speed * dt;
        if (w.IsKeyPressed(GLFW_KEY_SPACE))  position += glm::vec3(0, 1, 0) * velocity;
        if (w.IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) position -= glm::vec3(0, 1, 0) * velocity;

    }
};