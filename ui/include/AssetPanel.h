#pragma once
#include <string>
#include "FileBrowser.h"
#include "SceneObject.h"
#include "shader.h"
#include "scene.h"

/*
    ImGui panel: browse asset folder, spawn .obj files as entities on screen,
    and save/load the full scene (JSON) via SceneSerializer. Follows the same
    pattern as your other panels (HierarchyPanel, PropertiesPanel, etc).
*/
 class AssetPanel {
public:
    void Init(const std::string& assetRoot, Shader* defaultShader);
    void Render(Scene& scene, Entity selectedEntity);

    bool open = true;

private:
    FileBrowser browser;
    std::string pendingPick; // set via browser's onSelect callback when user clicks Apply
    char rootPathBuf[256] = "Resources";

    void SpawnMeshEntity(Scene& scene, const std::string& objPath);
    void ApplyTextureToSelected(Scene& scene, Entity selected, const std::string& texPath);
};