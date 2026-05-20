#include "Renderer.h"
#include<iostream>

Renderer::Renderer()
{

}

void Renderer::Init()
{
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void Renderer::SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::SetViewport(int width, int height) const
{
    glViewport(0, 0, width, height);
}

void Renderer::DrawArrays(unsigned int vao, int count) const
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, count);
}

void Renderer::Draw(const Mesh& mesh) const
{
    mesh.Bind();
    glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT, 0);
}


void Renderer::Draw(const Scene& scene, const Camera& camera) const
{
    for (const auto& [entity, renderer] : scene.meshRenderers)
    {

        auto it = scene.transforms.find(entity);
        if (it == scene.transforms.end())
            continue;

        const TransformComponent& transform = it->second;

        if (!renderer.mesh || !renderer.material || !renderer.material->shader)
            continue;
        renderer.material->Bind(transform.GetMatrix(),camera.GetViewMatrix(),camera.GetProjection());
        Draw(*renderer.mesh);
    }
}

