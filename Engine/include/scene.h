#pragma once

#include <vector>
#include "SceneObject.h"
#include <unordered_map>
#include"MeshRendererComponent.h"



using Entity = uint32_t;


class Scene
{
public:
    Entity CreateEntity()
    {
        Entity id = nextEntity++;
        entities.push_back(id);
        return id;
    }

    // COMPONENT STORAGE
    std::unordered_map<Entity, TransformComponent> transforms;
    std::unordered_map<Entity, MeshRendererComponent> meshRenderers;

private:
    Entity nextEntity = 0;
    std::vector<Entity> entities;
};