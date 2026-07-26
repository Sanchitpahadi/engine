#include "HierarchyPanel.h"
#include <vector>

void HierarchyPanel::Show(GameState* gs)
{
    ImGui::Begin("Hierarchy", &show, ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Scene: %s", gs->currentScenePath.c_str());
    ImGui::Separator();

    if (!gs->scene) {
        ImGui::TextDisabled("No scene loaded");
        ImGui::End();
        return;
    }

    ImGui::Text("Entities: %d", (int)gs->scene->entities.size());

    // ── Add entity ─────────────────────────────
    static char newName[64] = "Entity";

    ImGui::InputText("Name", newName, sizeof(newName));

    if (ImGui::Button("+ Add Empty"))
    {
        gs->createRequests.push_back({ EntityType::Empty, newName });
        strcpy(newName, "Entity");
    }

    if (ImGui::Button("Add Cube"))
    {
        gs->createRequests.push_back({ EntityType::Cube, "Cube" });
    }

    ImGui::SameLine();

    if (ImGui::Button("Add Sphere"))
    {
        gs->createRequests.push_back({ EntityType::Sphere, "Sphere" });
    }

    ImGui::Separator();

    // ── Entity list ────────────────────────────
    if (ImGui::TreeNodeEx("Scene", ImGuiTreeNodeFlags_DefaultOpen)) {

        Entity toDelete = (Entity)-1;

        for (Entity e : gs->scene->entities)
        {
            const std::string& label = gs->scene->names.count(e)
                ? gs->scene->names[e]
                : "Entity " + std::to_string(e);

            bool selected = (gs->selectedObjectID == (int)e);

            if (ImGui::Selectable(label.c_str(), selected))
                gs->selectedObjectID = (int)e;

            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::MenuItem("Delete"))
                    toDelete = e;

                ImGui::EndPopup();
            }
        }

        if (toDelete != (Entity)-1)
        {
            if (gs->selectedObjectID == (int)toDelete)
                gs->selectedObjectID = -1;

            gs->scene->DestroyEntity(toDelete);
        }

        ImGui::TreePop();
    }

    ImGui::End();
}