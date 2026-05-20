#pragma once
#include "shader.h"
#include <glm/glm.hpp>

class Material {
public:
    Shader* shader = nullptr;

    glm::vec3 color = glm::vec3(1.0f);

    unsigned int textureID = 0;
    bool useTexture = false;

    void Bind(const glm::mat4& model,
              const glm::mat4& view,
              const glm::mat4& projection)
            {
                    if (!shader) return;

                shader->use();

                shader->setMat4("model", model);
                shader->setMat4("view", view);
                shader->setMat4("projection", projection);

                shader->setVec3("objectColor", color);

                if (useTexture) {
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, textureID);
                    shader->setInt("texture0", 0);
                }
            }

};