#include "ResourceManager.h"
#include <glm/glm.hpp>

void ResourceManager::Init()
{
    defaultShader = new Shader();
    defaultShader->Init("shader.vs", "shader.fs"); 

    defaultMaterial = new Material();
    defaultMaterial->shader = defaultShader;
    defaultMaterial->color  = glm::vec3(1.0f, 1.0f, 1.0f);
}

void ResourceManager::Shutdown()
{
    delete defaultMaterial;
    delete defaultShader;
}