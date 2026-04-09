#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

class OBJLoader
{
public:
    static bool Load(const std::string& path,
                     std::vector<float>& vertices,
                     std::vector<unsigned int>& indices);
};