#include "OrbitsPanel.h"
#include "imgui.h"
#include "GameState.h"   
#include "scene.h"
#include <glm/glm.hpp>
#include <string>

void OrbitsPanel::Show(GameState* gs)
{
    ImGui::Begin("Orbits", &show, ImGuiWindowFlags_NoCollapse);

    if (!gs->scene) {
        ImGui::TextDisabled("No scene loaded");
        ImGui::End();
        return;
    }

    Scene* scene = gs->scene;
    ImGui::Text("Orbiting bodies: %d", (int)scene->orbits.size());
    ImGui::Separator();

    if (scene->orbits.empty()) {
        ImGui::TextDisabled("Nothing is orbiting anything.");
        ImGui::End();
        return;
    }

    if (ImGui::BeginTable("orbits_table", 6,
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
    {
        ImGui::TableSetupColumn("Entity");
        ImGui::TableSetupColumn("Target");
        ImGui::TableSetupColumn("Radius");
        ImGui::TableSetupColumn("Speed");
        ImGui::TableSetupColumn("Live Distance");
        ImGui::TableSetupColumn("Position");
        ImGui::TableHeadersRow();

        for (auto& [entity, orbit] : scene->orbits)
        {
            ImGui::TableNextRow();

            std::string name = scene->names.count(entity)
                ? scene->names[entity] : ("Entity " + std::to_string(entity));
            std::string targetName = scene->names.count(orbit.target)
                ? scene->names[orbit.target] : ("Entity " + std::to_string(orbit.target));

            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", name.c_str());
            ImGui::TableSetColumnIndex(1); ImGui::Text("%s", targetName.c_str());
            ImGui::TableSetColumnIndex(2); ImGui::Text("%.2f", orbit.radius);
            ImGui::TableSetColumnIndex(3); ImGui::Text("%.2f", orbit.speed);

            bool haveBoth = scene->transforms.count(entity) && scene->transforms.count(orbit.target);
            if (haveBoth) {
                glm::vec3 p = scene->transforms[entity].position;
                glm::vec3 t = scene->transforms[orbit.target].position;
                ImGui::TableSetColumnIndex(4); ImGui::Text("%.2f", glm::length(p - t));
                ImGui::TableSetColumnIndex(5); ImGui::Text("(%.1f, %.1f, %.1f)", p.x, p.y, p.z);
            } else {
                ImGui::TableSetColumnIndex(4); ImGui::TextDisabled("--");
                ImGui::TableSetColumnIndex(5); ImGui::TextDisabled("missing transform");
            }
        }
        ImGui::EndTable();
    }
    ImGui::End();
}