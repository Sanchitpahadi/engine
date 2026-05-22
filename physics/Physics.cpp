#include "Physics.h"
#include "scene.h"  
#include <glm/glm.hpp>

struct AABB
{
    glm::vec3 min;
    glm::vec3 max;
};

AABB GetAABB(const TransformComponent& t, glm::vec3 size)
{
    glm::vec3 half = (size * t.scale) * 0.5f;

    return {
        t.position - half,
        t.position + half
    };
}

bool CheckAABB(const AABB& a, const AABB& b)
{
    return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
           (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
           (a.min.z <= b.max.z && a.max.z >= b.min.z);
}

void PhysicsSystem::Update(Scene& scene, float dt)
{
    for (auto& [entity, phys] : scene.physics)
    {
        auto it = scene.transforms.find(entity);
        if (it == scene.transforms.end()) continue;

        auto& trans = it->second;

        // Gravity
        phys.velocity.y += -9.8f * dt;

        // Move
        trans.position += phys.velocity * dt;

        AABB playerAABB = GetAABB(trans, glm::vec3(1.0f));

        auto groundIt = scene.transforms.find(scene.ground);
        if (groundIt == scene.transforms.end()) continue;

        AABB groundAABB = GetAABB(groundIt->second, glm::vec3(1.0f));

        if (CheckAABB(playerAABB, groundAABB) && phys.velocity.y <= 0.0f)
        {
            float penetration = groundAABB.max.y - playerAABB.min.y;

            trans.position.y += penetration;
            phys.velocity.y = 0.0f;
            phys.isGrounded = true;
        }
        else
        {
            phys.isGrounded = false;
        }
    }
}