#pragma once
#include "shader.h"
#include <glm/glm.hpp>

class Material {
public:
    Shader* shader = nullptr;
    glm::vec3 color = glm::vec3(1.0f);
    unsigned int textureID = 1;
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

        /* 
           Without this line the shader's `useTexture` uniform never gets
           updated and stays at GL's default (false), so textures would
           silently never show up. GLSL bool uniforms upload the same way
           as int uniforms (0/1), so setInt works fine here.
        */ 
        shader->setInt("useTexture", useTexture ? 1 : 0);

        if (useTexture) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureID);
            shader->setInt("texture0", 0);
        }
    }
};