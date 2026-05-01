#pragma once

#include <vector>
#include "SceneObject.h"

class Scene
{
private:
    std::vector<SceneObject> objects;

public:
    SceneObject& CreateObject()
    {
        objects.emplace_back();
        return objects.back();
    }

    // ✅ non-const version (modifiable)
    std::vector<SceneObject>& GetObjects()
    {
        return objects;
    }

    // ✅ const version (read-only)
    const std::vector<SceneObject>& GetObjects() const
    {
        return objects;
    }
};