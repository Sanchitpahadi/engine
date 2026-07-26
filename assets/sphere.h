// sphere.h

#pragma once
#include <vector>
#include <cmath>
#include <cstdint>

// Returns interleaved floats in layout: px, py, pz, u, v, nx, ny, nz
inline std::vector<float> GenerateSphereVertices(int sectors = 36, int stacks = 18, float radius = 1.0f)
{
    const float PI = 3.14159265358979323846f;
    std::vector<float> verts;
    verts.reserve((sectors + 1) * (stacks + 1) * 8);

    float sectorStep = 2.0f * PI / sectors;
    float stackStep  = PI / stacks;

    for (int i = 0; i <= stacks; ++i)
    {
        float stackAngle = PI / 2.0f - i * stackStep;  // pi/2 -> -pi/2
        float xy = radius * cosf(stackAngle);
        float z  = radius * sinf(stackAngle);

        for (int j = 0; j <= sectors; ++j)
        {
            float sectorAngle = j * sectorStep;

            float px = xy * cosf(sectorAngle);
            float py = xy * sinf(sectorAngle);
            float pz = z;

            float nx = px / radius;
            float ny = py / radius;
            float nz = pz / radius;

            float u = (float)j / sectors;
            float v = (float)i / stacks;

            // Layout: pos | uv | normal  (matches your cube vertex format)
            verts.push_back(px);
            verts.push_back(py);
            verts.push_back(pz);
            verts.push_back(u);
            verts.push_back(v);
            verts.push_back(nx);
            verts.push_back(ny);
            verts.push_back(nz);
        }
    }
    return verts;
}

inline std::vector<unsigned int> GenerateSphereIndices(int sectors = 36, int stacks = 18)
{
    std::vector<unsigned int> indices;
    indices.reserve(sectors * stacks * 6);

    for (int i = 0; i < stacks; ++i)
    {
        unsigned int k1 = i * (sectors + 1);
        unsigned int k2 = k1 + sectors + 1;

        for (int j = 0; j < sectors; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != stacks - 1)
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
    return indices;
}