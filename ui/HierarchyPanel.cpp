#include "HierarchyPanel.h"

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

    // ── Add entity ────────────────────────────────────────────────────────
    static char newName[64] = "Entity";
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 80.0f);
    ImGui::InputText("##newname", newName, sizeof(newName));
    ImGui::SameLine();
    if (ImGui::Button("+ Add", ImVec2(-1, 0))) {
        Entity e = gs->scene->CreateEntity(newName);
        gs->selectedObjectID = (int)e;
        strncpy(newName, "Entity", sizeof(newName));
    }

    ImGui::Separator();

    // ── Entity list ───────────────────────────────────────────────────────
    if (ImGui::TreeNodeEx("Scene", ImGuiTreeNodeFlags_DefaultOpen)) {
        Entity toDelete = (Entity)-1;

        for (Entity e : gs->scene->entities) {
            const std::string& label = gs->scene->names.count(e)
                ? gs->scene->names[e]
                : "Entity " + std::to_string(e);

            bool selected = (gs->selectedObjectID == (int)e);
            if (ImGui::Selectable(label.c_str(), selected))
                gs->selectedObjectID = (int)e;

            if (ImGui::BeginPopupContextItem()) {
                ImGui::TextDisabled("%s", label.c_str());
                ImGui::Separator();
                if (ImGui::MenuItem("Delete"))
                    toDelete = e;
                ImGui::EndPopup();
            }
        }

        // Defer deletion so we don't invalidate the iterator mid-loop
        if (toDelete != (Entity)-1) {
            if (gs->selectedObjectID == (int)toDelete)
                gs->selectedObjectID = -1;
            gs->scene->DestroyEntity(toDelete);
        }

        ImGui::TreePop();
    }

    ImGui::End();
}
