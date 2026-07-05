#include "BodiesPanel.h"
#include "imgui.h"
#include "GameState.h"
#include "scene.h"
#include <glm/glm.hpp>
#include <string>

void BodiesPanel::Show(GameState* gs)
{
    ImGui::Begin("Rigid Bodies", &show, ImGuiWindowFlags_NoCollapse);

    if (!gs->scene) {
        ImGui::TextDisabled("No scene loaded");
        ImGui::End();
        return;
    }

    Scene* scene = gs->scene;
    ImGui::Text("Bodies: %d", (int)scene->physics.size());
    ImGui::Separator();

    if (ImGui::BeginTable("bodies_table", 7,
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
    {
        ImGui::TableSetupColumn("Entity");
        ImGui::TableSetupColumn("Static");
        ImGui::TableSetupColumn("Mass");
        ImGui::TableSetupColumn("Velocity");
        ImGui::TableSetupColumn("Speed");
        ImGui::TableSetupColumn("Grounded");
        ImGui::TableSetupColumn("Position");
        ImGui::TableHeadersRow();

        for (auto& [entity, rb] : scene->physics)
        {
            ImGui::TableNextRow();
            std::string name = scene->names.count(entity)
                ? scene->names[entity] : ("Entity " + std::to_string(entity));

            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", name.c_str());
            ImGui::TableSetColumnIndex(1); ImGui::Text("%s", rb.isStatic ? "yes" : "no");

            ImGui::TableSetColumnIndex(2);
            if (rb.isStatic) ImGui::TextDisabled("inf");
            else             ImGui::Text("%.2f", rb.mass);

            ImGui::TableSetColumnIndex(3);
            ImGui::Text("(%.2f, %.2f, %.2f)", rb.velocity.x, rb.velocity.y, rb.velocity.z);

            ImGui::TableSetColumnIndex(4); ImGui::Text("%.2f", glm::length(rb.velocity));
            ImGui::TableSetColumnIndex(5); ImGui::Text("%s", rb.isGrounded ? "yes" : "no");

            ImGui::TableSetColumnIndex(6);
            if (scene->transforms.count(entity)) {
                glm::vec3 p = scene->transforms[entity].position;
                ImGui::Text("(%.1f, %.1f, %.1f)", p.x, p.y, p.z);
            } else {
                ImGui::TextDisabled("--");
            }
        }
        ImGui::EndTable();
    }
    ImGui::End();
}