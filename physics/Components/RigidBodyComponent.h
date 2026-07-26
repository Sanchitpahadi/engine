#pragma once
#include <glm/glm.hpp>

struct RigidBodyComponent
{
    float mass    = 1.0f;
    float invMass = 1.0f;

    glm::vec3 velocity = glm::vec3(0.0f);
    glm::vec3 force    = glm::vec3(0.0f);

    float restitution = 0.2f; // 0 = no bounce, 1 = full bounce
    float drag        = 0.0f; // linear damping 0..1

    bool isStatic   = false;
    bool useGravity = true;
    bool isGrounded = false;

    void SetMass(float m) { mass = m; invMass = (isStatic || m <= 0.0f) ? 0.0f : 1.0f / m; }
    void MakeStatic() { isStatic = true; invMass = 0.0f; useGravity = false; }

    void ApplyForce(const glm::vec3& f)    { force += f; }
    void ApplyImpulse(const glm::vec3& imp){ velocity += imp * invMass; }
};