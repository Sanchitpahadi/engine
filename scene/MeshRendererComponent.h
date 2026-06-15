
#include"material.h"
#include "Mesh.h"

class Material;
class Mesh;

struct MeshRendererComponent
{

    Mesh* mesh = nullptr;
    
    Material* material = nullptr;
};