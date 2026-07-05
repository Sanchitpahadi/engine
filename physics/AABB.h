#pragma once
#include <glm/glm.hpp>
#include <algorithm>

struct AABB { glm::vec3 min; glm::vec3 max; };

inline AABB ComputeAABB(const glm::vec3& position, const glm::vec3& scale,
                         const glm::vec3& size, const glm::vec3& offset = glm::vec3(0.0f))
{
    glm::vec3 half = (size * scale) * 0.5f;
    glm::vec3 center = position + offset;
    return { center - half, center + half };
}

// normal points from a -> b, overlap on smallest axis 
inline bool IntersectAABB(const AABB& a, const AABB& b, glm::vec3& outNormal, float& outPenetration)
{
    float ox = std::min(a.max.x, b.max.x) - std::max(a.min.x, b.min.x);
    if (ox <= 0.0f) return false;
    float oy = std::min(a.max.y, b.max.y) - std::max(a.min.y, b.min.y);
    if (oy <= 0.0f) return false;
    float oz = std::min(a.max.z, b.max.z) - std::max(a.min.z, b.min.z);
    if (oz <= 0.0f) return false;

    glm::vec3 dir = (b.min + b.max) * 0.5f - (a.min + a.max) * 0.5f;

    if (ox < oy && ox < oz) { outPenetration = ox; outNormal = { dir.x < 0 ? -1.f : 1.f, 0, 0 }; }
    else if (oy < oz)       { outPenetration = oy; outNormal = { 0, dir.y < 0 ? -1.f : 1.f, 0 }; }
    else                    { outPenetration = oz; outNormal = { 0, 0, dir.z < 0 ? -1.f : 1.f }; }
    return true;
}