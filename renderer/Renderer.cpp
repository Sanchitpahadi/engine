#include "Renderer.h"
#include<iostream>

Renderer::Renderer()
{

}

//  For 3d we need this 
void Renderer::Init()
{
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

//  just how the background color  color is set ========// ===== its uses is in loop cause backgorund colors is in loop

void Renderer::SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

//  just know it erases previous frame and ready for next one  //
void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

 // view port lol related to sizes 
void Renderer::SetViewport(int width, int height) const
{
    glViewport(0, 0, width, height);
}

// give it vertex array and how many corner it will render square  //
void Renderer::DrawArrays(unsigned int vao, int count) const
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, count);
}

 //  Draws the scene  //

void Renderer::Draw(const Mesh& mesh) const
{
    mesh.Bind();
    glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT, 0);
}


