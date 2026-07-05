#include "PhysicsSystem.h"
#include "scene.h"

void PhysicsSystem::Update(Scene& scene, float dt)
{
    for (auto& [entity, rb] : scene.physics) {
        if (rb.isStatic || !scene.transforms.count(entity)) continue;

        rb.isGrounded = false; // recomputed by Resolve this frame

        if (rb.useGravity) rb.velocity += gravity * dt;
        rb.velocity += rb.force * rb.invMass * dt;
        rb.force = glm::vec3(0.0f);

        if (rb.drag > 0.0f) rb.velocity *= (1.0f - rb.drag * dt);

        scene.transforms[entity].position += rb.velocity * dt;
    }

    auto contacts = collisionSystem.Detect(scene);
    collisionSystem.Resolve(scene, contacts);
}