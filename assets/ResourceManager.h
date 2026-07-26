#pragma once
#include "material.h"
#include "shader.h"

class ResourceManager
{
public:
    void Init();
    void Shutdown();

    Material* GetDefaultMaterial() { return defaultMaterial; }
    Shader*   GetDefaultShader()   { return defaultShader; }

private:
    Material* defaultMaterial = nullptr;
    Shader*   defaultShader   = nullptr;
};