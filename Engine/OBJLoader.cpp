#include "OBJLoader.h"
#include <fstream>
#include <sstream>

bool OBJLoader::Load(const std::string& path,
                     std::vector<float>& vertices,
                     std::vector<unsigned int>& indices)
{
    std::ifstream file(path);
    if (!file.is_open()) return false;

    std::vector<glm::vec3> tempVerts;
    std::string line;

    while (std::getline(file, line))
    {
        std::istringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v")
        {
            glm::vec3 v;
            ss >> v.x >> v.y >> v.z;
            tempVerts.push_back(v);
        }
        else if (type == "f")
        {
            unsigned int a,b,c,d;
            if (!(ss >> a >> b >> c >> d)) continue; // skip malformed faces
        
            indices.push_back(a-1);
            indices.push_back(b-1);
            indices.push_back(c-1);
        
            indices.push_back(c-1);
            indices.push_back(d-1);
            indices.push_back(a-1);
        }
    }

    for (auto& v : tempVerts)
    {
        vertices.push_back(v.x);
        vertices.push_back(v.y);
        vertices.push_back(v.z);
    }

    return true;
}