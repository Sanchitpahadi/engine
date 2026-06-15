#pragma once
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>

#include "SceneObject.h"
#include "MeshRendererComponent.h"
#include "Physics.h"


/// ======= implimenting the ECS System its on structure somewhere but its good kind of ====///
using Entity = uint32_t;

class Scene
{
public:


    std::unordered_map<Entity, TransformComponent>    transforms;
    std::unordered_map<Entity, MeshRendererComponent> meshRenderers;
    std::unordered_map<Entity, PhysicsComponent>      physics;
    std::unordered_map<Entity, std::string>           names;


    // Create entity — optionally give it a name
    Entity CreateEntity(const std::string& name = "Entity")
    {
        Entity id = nextEntity++;
        entities.push_back(id);
        names[id]      = name;
        transforms[id] = TransformComponent{};
        physics[id] = PhysicsComponent{};
        return id;
    }


    // Destroy entity and all its components
    void DestroyEntity(Entity e)
    {
        entities.erase(std::remove(entities.begin(), entities.end(), e), entities.end());
        names.erase(e);
        transforms.erase(e);
        meshRenderers.erase(e);
        physics.erase(e);
    }


    Entity nextEntity = 0;
    std::vector<Entity> entities;
    Entity ground = 0;

};