#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "scene.h"
#include "Camera.h"

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction; // normalised
};

// Unproject a mouse click into a world-space ray
inline Ray ScreenToRay(double mouseX, double mouseY,
                       int screenW, int screenH,
                       const glm::mat4& view,
                       const glm::mat4& proj)
{
    // 1. Normalised device coords  [-1, 1]
    float ndcX =  (2.0f * (float)mouseX / screenW) - 1.0f;
    float ndcY = -((2.0f * (float)mouseY / screenH) - 1.0f); // flip Y

    // 2. Clip space → eye space
    glm::vec4 rayClip  = glm::vec4(ndcX, ndcY, -1.0f, 1.0f);
    glm::vec4 rayEye   = glm::inverse(proj) * rayClip;
    rayEye             = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

    // 3. Eye space → world space
    glm::vec3 rayWorld = glm::normalize(
        glm::vec3(glm::inverse(view) * rayEye));

    // Camera position is the translation column of the inverse view
    glm::vec3 origin = glm::vec3(glm::inverse(view)[3]);

    return { origin, rayWorld };
}

// Slab-method AABB intersection. Returns t (distance) or -1 on miss.
inline float RayAABB(const Ray& ray,
                     glm::vec3 boxMin, glm::vec3 boxMax)
{
    glm::vec3 invDir = 1.0f / ray.direction;
    glm::vec3 t0 = (boxMin - ray.origin) * invDir;
    glm::vec3 t1 = (boxMax - ray.origin) * invDir;
    glm::vec3 tMin = glm::min(t0, t1);
    glm::vec3 tMax = glm::max(t0, t1);
    float tEnter = glm::max(glm::max(tMin.x, tMin.y), tMin.z);
    float tExit  = glm::min(glm::min(tMax.x, tMax.y), tMax.z);
    if (tExit < 0.0f || tEnter > tExit) return -1.0f;
    return tEnter;
}

// Test every entity and return the closest one hit, or -1 (no entity)
inline int PickEntity(const Ray& ray, Scene& scene)
{
    int   closest = -1;
    float bestT   = 1e30f;

    for (Entity e : scene.entities)
    {
        if (!scene.transforms.count(e)) continue;

        auto& tr = scene.transforms[e];

        // Use scale as half-extents (your cube mesh is -0.5..0.5)
        glm::vec3 half  = tr.scale * 0.5f;
        glm::vec3 boxMin = tr.position - half;
        glm::vec3 boxMax = tr.position + half;

        float t = RayAABB(ray, boxMin, boxMax);
        if (t > 0.0f && t < bestT)
        {
            bestT   = t;
            closest = (int)e;
        }
    }
    return closest;
}