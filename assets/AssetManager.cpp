#include "AssetManager.h"
#include "ObjLoader.h"
#include <algorithm>
#include <iostream>

AssetManager& AssetManager::Get() {
    static AssetManager inst;
    return inst;
}

Mesh* AssetManager::LoadMesh(const std::string& path) {
    auto it = meshCache.find(path);
    if (it != meshCache.end()) return it->second.get();

    std::vector<float> verts;
    std::vector<unsigned int> indices;
    if (!LoadOBJ(path, verts, indices)) {
        std::cerr << "[AssetManager] Failed to load mesh: " << path << "\n";
        return nullptr;
    }

    auto mesh = std::make_unique<Mesh>();
    mesh->Initc(verts, indices);

    Mesh* raw = mesh.get();
    meshPathLookup[raw] = path;
    meshCache[path] = std::move(mesh);
    return raw;
}

std::string AssetManager::GetMeshPath(Mesh* mesh) const {
    auto it = meshPathLookup.find(mesh);
    return it != meshPathLookup.end() ? it->second : std::string();
}

Texture* AssetManager::LoadTexture(const std::string& path) {
    auto it = textureCache.find(path);
    if (it != textureCache.end()) return it->second.get();

    auto tex = std::make_unique<Texture>(path.c_str());
    Texture* raw = tex.get();
    textureCache[path] = std::move(tex);
    return raw;
}

Material* AssetManager::CreateMaterial() {
    auto mat = std::make_unique<Material>();
    mat->shader = defaultShader;
    Material* raw = mat.get();
    materials.push_back(std::move(mat));
    return raw;
}

void AssetManager::DestroyMaterial(Material* mat) {
    materials.erase(std::remove_if(materials.begin(), materials.end(),
        [mat](const std::unique_ptr<Material>& m) { return m.get() == mat; }),
        materials.end());
}

void AssetManager::Clear() {
    meshCache.clear();
    meshPathLookup.clear();
    textureCache.clear(); // Texture destructors run here, calling glDeleteTextures
    materials.clear();
}