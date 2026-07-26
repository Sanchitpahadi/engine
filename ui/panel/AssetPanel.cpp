#include "AssetPanel.h"
#include "AssetManager.h"
#include "SceneSerializer.h"
#include "material.h"
#include "Texture.h"
#include "scene.h"
#include <imgui.h>
#include <algorithm>
#include <cstring>

namespace {
bool HasExt(const std::string& path, const std::string& ext) {
    if (path.size() < ext.size()) return false;
    std::string tail = path.substr(path.size() - ext.size());
    std::transform(tail.begin(), tail.end(), tail.begin(), ::tolower);
    return tail == ext;
}
}

void AssetPanel::Init(const std::string& assetRoot, Shader* defaultShader) {
    browser.Init(assetRoot);
    AssetManager::Get().SetDefaultShader(defaultShader);

    strncpy(rootPathBuf, assetRoot.c_str(), sizeof(rootPathBuf) - 1);
    rootPathBuf[sizeof(rootPathBuf) - 1] = '\0';

    /*
     FileBrowser keeps `selected`/`open` private, so this callback is the
     only way out: it fires with the file path when the user clicks Apply
     inside the browser window. We just stash it and act on it in Render().
    */
    browser.SetOnSelect([this](const std::string& path) {
        pendingPick = path;
    });
}

void AssetPanel::SpawnMeshEntity(Scene& scene, const std::string& objPath) {
    Mesh* mesh = AssetManager::Get().LoadMesh(objPath);
    if (!mesh) return;

    Entity e = scene.CreateEntity("Asset");

    Material* mat = AssetManager::Get().CreateMaterial();
    mat->color = glm::vec3(0.8f, 0.8f, 0.8f);

    MeshRendererComponent mr;
    mr.mesh     = mesh;
    mr.material = mat;
    scene.meshRenderers[e] = mr;
    scene.colliders[e]     = BoxColliderComponent{ glm::vec3(1.0f) };
}

void AssetPanel::ApplyTextureToSelected(Scene& scene, Entity selected, const std::string& texPath) {
    if (!scene.meshRenderers.count(selected)) return;
    auto& mr = scene.meshRenderers[selected];
    if (!mr.material) return;

    Texture* tex = AssetManager::Get().LoadTexture(texPath);
    if (!tex) return;

    mr.material->textureID  = tex->GetID();
    mr.material->useTexture = true;
}

void AssetPanel::Render(Scene& scene, Entity selectedEntity) {
    browser.Render();

    ImGui::Begin("Asset Manager", &open);

    ImGui::InputText("Root Folder", rootPathBuf, sizeof(rootPathBuf));
    ImGui::SameLine();
    if (ImGui::Button("Go"))
        browser.Init(rootPathBuf); // re-scans from the new root

    ImGui::Separator();

    if (!pendingPick.empty()) {
        ImGui::Text("Picked: %s", pendingPick.c_str());

        if (HasExt(pendingPick, ".obj")) {
            if (ImGui::Button("Spawn as Entity")) {
                SpawnMeshEntity(scene, pendingPick);
                pendingPick.clear();
            }
        } else if (HasExt(pendingPick, ".png") || HasExt(pendingPick, ".jpg") || HasExt(pendingPick, ".jpeg")) {
            ImGui::BeginDisabled(selectedEntity == (Entity)-1);
            if (ImGui::Button("Apply Texture to Selected")) {
                ApplyTextureToSelected(scene, selectedEntity, pendingPick);
                pendingPick.clear();
            }
            ImGui::EndDisabled();
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear Pick"))
            pendingPick.clear();
    } else {
        ImGui::TextDisabled("Pick a file in the browser above, then click its Apply button");
    }

    ImGui::Separator();

    // ── Material inspector for the selected entity ──────────────────────
    if (selectedEntity != (Entity)-1 && scene.meshRenderers.count(selectedEntity)) {
        Material* mat = scene.meshRenderers[selectedEntity].material;
        if (mat) {
            ImGui::Text("Material");
            ImGui::ColorEdit3("Color", &mat->color.x);
            ImGui::Checkbox("Use Texture", &mat->useTexture); // flip on/off without reloading
            if (mat->useTexture)
                ImGui::TextDisabled("Texture ID: %u", mat->textureID);
        }
    } else {
        ImGui::TextDisabled("Select an entity to edit its material");
    }

    ImGui::Separator();
    static char scenePathBuf[256] = "Scenes/scene.json";
    ImGui::InputText("Scene Path", scenePathBuf, sizeof(scenePathBuf));

    if (ImGui::Button("Save Scene"))
        SceneSerializer::Save(scene, scenePathBuf);
    ImGui::SameLine();
    if (ImGui::Button("Load Scene"))
        SceneSerializer::Load(scene, scenePathBuf);

    ImGui::End();
}