#pragma once

#include <glm/glm.hpp>

struct Scene; // forward declaration 

struct PhysicsComponent
{
    glm::vec3 velocity{0.0f};
    bool isGrounded = false;
};

class PhysicsSystem
{
public:
    void Update(Scene& scene, float dt);
};