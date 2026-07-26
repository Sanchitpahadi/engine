
#pragma once
#include <string>

enum class EntityType
{
    Empty,
    Cube,
    Sphere
};

struct CreateEntityRequest
{
    EntityType type;
    std::string name;
};