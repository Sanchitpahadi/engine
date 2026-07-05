#include "CollisionSystem.h"
#include "scene.h"
#include <unordered_map>
#include <algorithm>

std::vector<Contact> CollisionSystem::Detect(Scene& scene)
{
    std::vector<Contact> contacts;
    std::unordered_map<Entity, AABB> aabbs;

    for (auto& [entity, col] : scene.colliders) {
        if (!scene.transforms.count(entity)) continue;
        auto& t = scene.transforms[entity];
        aabbs[entity] = ComputeAABB(t.position, t.scale, col.size, col.offset);
    }

    for (auto itA = aabbs.begin(); itA != aabbs.end(); ++itA) {
        auto itB = itA; ++itB;
        for (; itB != aabbs.end(); ++itB) {
            Entity a = itA->first, b = itB->first;
            if (!scene.physics.count(a) && !scene.physics.count(b)) continue;

            bool aStatic = scene.physics.count(a) ? scene.physics[a].isStatic : true;
            bool bStatic = scene.physics.count(b) ? scene.physics[b].isStatic : true;
            if (aStatic && bStatic) continue;

            glm::vec3 normal; float pen;
            if (IntersectAABB(itA->second, itB->second, normal, pen))
                contacts.push_back({ a, b, normal, pen });
        }
    }
    return contacts;
}

void CollisionSystem::Resolve(Scene& scene, const std::vector<Contact>& contacts)
{
    for (const auto& c : contacts) {
        if (!scene.transforms.count(c.a) || !scene.transforms.count(c.b)) continue;

        RigidBodyComponent dummy; dummy.invMass = 0.0f;
        RigidBodyComponent& rbA = scene.physics.count(c.a) ? scene.physics[c.a] : dummy;
        RigidBodyComponent& rbB = scene.physics.count(c.b) ? scene.physics[c.b] : dummy;

        float totalInvMass = rbA.invMass + rbB.invMass;
        if (totalInvMass <= 0.0f) continue;

        auto& tA = scene.transforms[c.a];
        auto& tB = scene.transforms[c.b];

        // positional correction
        glm::vec3 correction = c.normal * c.penetration;
        tA.position -= correction * (rbA.invMass / totalInvMass);
        tB.position += correction * (rbB.invMass / totalInvMass);

        // impulse along normal
        glm::vec3 relVel = rbB.velocity - rbA.velocity;
        float velAlongNormal = glm::dot(relVel, c.normal);
        if (velAlongNormal > 0.0f) continue;

        float restitution = std::min(rbA.restitution, rbB.restitution);
        float impulseMag = -(1.0f + restitution) * velAlongNormal / totalInvMass;
        glm::vec3 impulse = impulseMag * c.normal;

        rbA.velocity -= impulse * rbA.invMass;
        rbB.velocity += impulse * rbB.invMass;

        if (c.normal.y < -0.5f) rbB.isGrounded = true;
        if (c.normal.y >  0.5f) rbA.isGrounded = true;
    }
}