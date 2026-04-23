#pragma once
#include"Window.hpp"

#include"shader.h"
#include"Camera.h"

class Material {
public:
    Shader* shader = nullptr;

    glm::vec3 color = glm::vec3(1.0f);

    // optional for later
    unsigned int textureID = 0;
    bool useTexture = false;

  void Bind(const Camera& camera, const glm::mat4& model)
{
    shader->use();

    shader->setMat4("model", model);
    shader->setMat4("view", camera.GetViewMatrix());
    shader->setMat4("projection", camera.GetProjection());

    shader->setVec3("objectColor", color);

    // 🔥 REQUIRED FOR LIGHTING
    shader->setVec3("viewPos", camera.Position);


    shader->setVec3("lightPos", glm::vec3(2.0f, 2.0f, 2.0f));
    shader->setVec3("lightColor", glm::vec3(1.0f));
    shader->setVec3("viewPos", camera.Position);

    if (useTexture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        shader->setInt("texture0", 0);
    }
}
};