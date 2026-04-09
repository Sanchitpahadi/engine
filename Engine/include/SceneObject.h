#include"Window.hpp"
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 

#include "Mesh.h"
class SceneObject {
public:
    glm::vec3 position = glm::vec3(0.f);
    glm::vec3 rotation = glm::vec3(0.f);
    float     scale    = 1.f;
    bool      autoRotate = false;
    Mesh*  mesh;

    void Update(float deltaTime) {
        if (autoRotate)
            rotation.y = glfwGetTime() * 50.f;
    }

    void Render(Shader& shader,glm::mat4& view, glm::mat4& projection) {

        shader.use();
        glm::mat4 m = glm::mat4(1.f);
            // Apply transforms in order: Scale → Rotate → Translate
        m = glm::translate(m, position);
        m = glm::rotate(m, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        m = glm::rotate(m, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        m = glm::rotate(m, glm::radians(rotation.z), glm::vec3(0, 0, 1));
        m = glm::scale(m, glm::vec3(scale));
        
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"),       1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"),      1, GL_FALSE, glm::value_ptr(m)); // ✅ proper mat4
        glUniform3fv(glGetUniformLocation(shader.ID, "objectColor"),       1, glm::value_ptr(glm::vec3(0.2f, 0.f, 0.5f)));


        mesh->Draw();
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