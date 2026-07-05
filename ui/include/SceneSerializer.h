#pragma once

#include <string>
#include "scene.h" 

// Full scene save/load: entities, names, transforms, mesh renderer (mesh path
// + color), colliders, rigid bodies, orbits. Meshes are re-resolved through
// AssetManager on load (cached by path, so repeats are cheap).
class SceneSerializer {
public:
    static bool Save(Scene& scene, const std::string& path);
    static bool Load(Scene& scene, const std::string& path);
};
