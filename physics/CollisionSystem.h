#pragma once
#include <vector>
#include "Entity.hpp"
#include "AABB.h"

class Scene;

struct Contact { Entity a; Entity b; glm::vec3 normal; float penetration; };

class CollisionSystem
{
public:
    std::vector<Contact> Detect(Scene& scene);
    void Resolve(Scene& scene, const std::vector<Contact>& contacts);
};