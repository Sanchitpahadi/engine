#pragma once
#include "GameState.h"
#include "scene.h"
#include "DefaultManager.h"
#include "transform.h"
class EntityFactory
{
public:
    static Entity Create(Scene* scene, const CreateEntityRequest& req)
    {
        Entity e = scene->CreateEntity(req.name);

        //scene->transforms[e] = Transform();

        switch (req.type)
        {
            case EntityType::Cube:
            {
                auto& mr = scene->meshRenderers[e];
                mr.mesh = DefaultManager::GetCubeMesh();
                mr.material = DefaultManager::GetDefaultMaterial();
                break;
            }

            case EntityType::Sphere:
            {
                auto& mr = scene->meshRenderers[e];
                mr.mesh = DefaultManager::GetSphereMesh();
                mr.material = DefaultManager::GetDefaultMaterial();
                break;
            }

            default:
                break;
        }

        return e;
    }
};