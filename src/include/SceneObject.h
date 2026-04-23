#pragma once

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 

#include "Mesh.h"
#include "material.h"

#include "transform.h"

class SceneObject {
public:
    glm::vec3 position = glm::vec3(0.f);

    Transform transform;

    Mesh* mesh = nullptr;
    Material* material = nullptr;
    

};

