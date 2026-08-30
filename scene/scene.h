#pragma once

#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>
#include "Entity.hpp"
#include "SceneObject.h"
#include "MeshRendererComponent.h"
#include "OrbitComponent.h"
#include "RigidBodyComponent.h"
#include "BoxColliderComponent.h"

class Scene
{
public:
    std::unordered_map<Entity, TransformComponent>     transforms;
    std::unordered_map<Entity, MeshRendererComponent>  meshRenderers;
    std::unordered_map<Entity, RigidBodyComponent>     physics;     
    std::unordered_map<Entity, BoxColliderComponent>   colliders;  
    std::unordered_map<Entity, OrbitComponent>         orbits;
    std::unordered_map<Entity, std::string>            names;

    Entity CreateEntity(const std::string& name = "Entity")
    {
        Entity id = nextEntity++;
        entities.push_back(id);
        names[id]      = name;
        transforms[id] = TransformComponent{};
        return id;
    }

    void DestroyEntity(Entity e)
    {
        entities.erase(std::remove(entities.begin(), entities.end(), e), entities.end());
        names.erase(e);
        transforms.erase(e);
        meshRenderers.erase(e);
        colliders.erase(e);
        orbits.erase(e);
    }

    Entity nextEntity = 0;
    std::vector<Entity> entities;
    Entity ground = 0;
};