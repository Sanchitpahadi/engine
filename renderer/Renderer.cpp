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

void Renderer::Draw(const SceneObject& obj, const SceneObject& light, const Camera& camera) const
{
    if (!obj.mesh || !obj.material) return;

    obj.material->shader->use();

    obj.material->shader->setMat4("model", obj.transform.GetMatrix());
    obj.material->shader->setMat4("view", camera.GetViewMatrix());
    obj.material->shader->setMat4("projection", camera.GetProjection());

    std::cout<< "is it printing or not??\n"; 
/*
    obj.material->shader->setVec3("objectColor", obj.material->color);

    // 🔥 REAL LIGHT DATA
    obj.material->shader->setVec3("lightPos", light.transform.position);
    obj.material->shader->setVec3("lightColor", light.material->color);

    obj.material->shader->setVec3("viewPos", camera.Position);
*/

    Draw(*obj.mesh);
}