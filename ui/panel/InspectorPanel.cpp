#include "InspectorPanel.h"

void InspectorPanel::Show(GameState* gs) {
    ImGui::Begin("Inspector", &show, ImGuiWindowFlags_NoCollapse);

    if (!gs->scene || gs->selectedObjectID == -1) {
        ImGui::TextDisabled("Select an entity to view properties");
        ImGui::End();
        return;
    }

    Entity e = (Entity)gs->selectedObjectID;

    if (gs->scene->names.count(e)) {
        char buf[64];
        strncpy(buf, gs->scene->names[e].c_str(), sizeof(buf));
        buf[sizeof(buf)-1] = '\0';
        if (ImGui::InputText("##ename", buf, sizeof(buf)))
            gs->scene->names[e] = buf;
    }
    ImGui::SameLine();
    ImGui::TextDisabled("(#%u)", e);
    ImGui::Separator();

    ShowProperties(e, gs);           // ← calls the private method below

    ImGui::End();
}

void InspectorPanel::ShowProperties(Entity e, GameState* gs) {
    Scene* scene = gs->scene;

    if (scene->transforms.count(e)) {
        auto& t = scene->transforms[e];
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::DragFloat3("Position", &t.position.x, 0.01f);
            ImGui::DragFloat3("Rotation", &t.rotation.x, 1.0f);
            ImGui::DragFloat3("Scale",    &t.scale.x,    0.01f);
            ImGui::Spacing();
            if (ImGui::SmallButton("Reset##transform")) {
                t.position = glm::vec3(0.0f);
                t.rotation = glm::vec3(0.0f);
                t.scale    = glm::vec3(1.0f);
            }
        }
    }

    if (scene->meshRenderers.count(e)) {
        auto& mr = scene->meshRenderers[e];
        if (ImGui::CollapsingHeader("Mesh Renderer", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("Mesh:     %s", mr.mesh     ? "Assigned" : "None");
            ImGui::Text("Material: %s", mr.material ? "Assigned" : "None");
            if (mr.material)
                ImGui::ColorEdit3("Color", &mr.material->color.x);
        }
    }

    if (scene->physics.count(e)) {
        auto& ph = scene->physics[e];
        if (ImGui::CollapsingHeader("Physics", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::DragFloat3("Velocity", &ph.velocity.x, 0.01f);
        }
    }
}