#pragma once
#include <Glad/glad.h>

#include "Mesh.h"
#include "SceneObject.h"
#include "Camera.h"


class Renderer
{
public:

    Renderer();
    
    void Init();
    void SetClearColor(float r, float g, float b, float a);
    void Clear() const;

    void SetViewport(int width, int height) const;

    void DrawArrays(unsigned int vao, int count) const;
    void Draw(const Mesh& mesh)const;
    void Draw(const SceneObject& obj, const SceneObject& light, const Camera& camera) const;

};
