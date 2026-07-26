#pragma once
#include "Mesh.h"
#include "material.h"

class DefaultManager
{
public:
    static void Init(Mesh* cube, Mesh* sphere, Material* mat)
    {
        cubeMesh = cube;
        sphereMesh = sphere;
        defaultMaterial = mat;
    }

    static Mesh* GetCubeMesh() { return cubeMesh; }
    static Mesh* GetSphereMesh() { return sphereMesh; }
    static Material* GetDefaultMaterial() { return defaultMaterial; }

private:
    static inline Mesh* cubeMesh = nullptr;
    static inline Mesh* sphereMesh = nullptr;
    static inline Material* defaultMaterial = nullptr;
};