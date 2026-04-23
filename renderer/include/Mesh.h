#pragma once

#include <vector>
#include <Glad/glad.h>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class Mesh
{
private:

    unsigned int ebo;
    VertexArray vao;
    VertexBuffer vbo;
    VertexBufferLayout layout;

    unsigned int indexCount;
public:
     Mesh();
    ~Mesh();
    void Initc(const std::vector<float>& vertices,
         const std::vector<unsigned int>& indices);
    
    void Inito(const std::vector<float>& vertices,
         const std::vector<unsigned int>& indices);
    void Bind() const;
    void Unbind() const;
     unsigned int GetIndexCount() const { return indexCount; }

void Draw() const
{
    std::cout << "glDrawElements called with: " << GetIndexCount() << std::endl;
    glDrawElements(GL_TRIANGLES, GetIndexCount(), GL_UNSIGNED_INT, 0);
}
};
