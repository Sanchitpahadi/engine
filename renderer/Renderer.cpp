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
    for (const auto& obj : scene.GetObjects())
    {
        if (!obj.mesh || !obj.material || !obj.material->shader)
            continue;

        obj.material->shader->use();

        obj.material->shader->setMat4("model", obj.transform.GetMatrix());
        obj.material->shader->setMat4("view", camera.GetViewMatrix());
        obj.material->shader->setMat4("projection", camera.GetProjection());

        /*

        shader.setVec3("objectColor", glm::vec3(0.5f, 0.0f, 1.0f));
        shader.setVec3("lightColor", glm::vec3(1.0f));
        shader.setVec3("lightPos", light.transform.position);
        shader.setVec3("viewPos", camera.Position);

        shader.setMat4("view", camera.GetViewMatrix());
        shader.setMat4("projection", camera.GetProjection());
    
        */

        obj.mesh->Bind();
        Draw(*obj.mesh);
    }
}
