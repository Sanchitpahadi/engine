#pragma once
#include <glm/glm.hpp>

struct BoxColliderComponent
{
    glm::vec3 size   = glm::vec3(1.0f); // local size before scale
    glm::vec3 offset = glm::vec3(0.0f);
};