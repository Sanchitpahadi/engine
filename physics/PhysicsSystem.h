#pragma once
#include "CollisionSystem.h"

class Scene;

class PhysicsSystem
{
public:
    void Update(Scene& scene, float dt);
    glm::vec3 gravity = glm::vec3(0.0f, -9.8f, 0.0f);

private:
    CollisionSystem collisionSystem;
};