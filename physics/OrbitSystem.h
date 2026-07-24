#include <iostream>
#include "scene.h"

class OrbitSystem {
public:
    void Update(Scene& scene, float totalTime)
    {
        for (auto& [entity, orbit] : scene.orbits)
        {
            if (!scene.transforms.count(orbit.target)) continue;
            if (!scene.transforms.count(entity)) continue;

            glm::vec3 targetPos = scene.transforms[orbit.target].position;
            float angle = totalTime * orbit.speed + orbit.phase;

            glm::vec3 offset(
                sin(angle) * orbit.radius,
                0.0f,
                cos(angle) * orbit.radius
            );

            scene.transforms[entity].position = targetPos + offset;
        }
    }
};