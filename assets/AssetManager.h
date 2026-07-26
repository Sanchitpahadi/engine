#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "Mesh.h"
#include "material.h"
#include "shader.h"
#include "Texture.h"

/* Singleton. Caches meshes and textures by source path so re-loading the
   same file (e.g. on scene load) doesn't re-parse/re-upload it. Also owns
Material instances created for spawned/loaded entities so lifetime isn't
  tied to any one Scene.
*/ 
class AssetManager {
public:
    static AssetManager& Get();

    Mesh*       LoadMesh(const std::string& path);          // cached, loads .obj via ObjLoader
    std::string GetMeshPath(Mesh* mesh) const;              // reverse lookup, used by SceneSerializer

    Texture*    LoadTexture(const std::string& path);       // cached, owns the Texture (and its GL handle)

    Material* CreateMaterial();
    void      DestroyMaterial(Material* mat);

    void    SetDefaultShader(Shader* shader) { defaultShader = shader; }
    Shader* GetDefaultShader() const { return defaultShader; }

    void Clear(); // drops all cached meshes/textures/materials (e.g. on "New Scene")

private:
    AssetManager() = default;

    std::unordered_map<std::string, std::unique_ptr<Mesh>>    meshCache;
    std::unordered_map<Mesh*, std::string>                    meshPathLookup;
    std::unordered_map<std::string, std::unique_ptr<Texture>> textureCache;
    std::vector<std::unique_ptr<Material>>                    materials;
    Shader* defaultShader = nullptr;
};