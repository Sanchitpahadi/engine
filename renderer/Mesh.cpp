#include "Mesh.h"

Mesh::Mesh()
{

}


Mesh::~Mesh()
{
    glDeleteBuffers(1, &ebo);
}

// ============= it just initilized cubes vertices and indices ===============//
void Mesh::Initc(const std::vector<float>& vertices,
                 const std::vector<unsigned int>& indices)
{
    indexCount = indices.size();

    vao.Bind();

    vbo.Init(vertices.size() * sizeof(float), vertices.data());

    // i just didnt make a endexBuffer a diffrent file with its code ===== ///
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int),
                 indices.data(),
                 GL_STATIC_DRAW);

    
    layout.push<float>(3);
    layout.push<float>(2);
    layout.push<float>(3);


    vao.AddBuffer(vbo, layout);

    std::cout << "IndexCount: " << indexCount << std::endl;


    Unbind();
}

 // =========== Object created in its vertices is handeled by objloader ========///
void Mesh::Inito(const std::vector<float>& vertices,
           const std::vector<unsigned int>& indices)
{
    indexCount = indices.size();

    vao.Bind();

    vbo.Init(vertices.size() * sizeof(float), vertices.data());

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),indices.data(), GL_STATIC_DRAW);

  
    
    layout.push<float>(3);
    
    vao.AddBuffer(vbo, layout);

    Unbind();    
}


// ============== BINDS THE MESH ============= //
void Mesh::Bind() const
{
    vao.Bind();
}

// ============= UNbind MEsh ================//
void Mesh::Unbind() const
{
    vao.Unbind();
}


