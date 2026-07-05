#include "StatsPanel.h"
#include "scene.h"
#include <unordered_set>

void StatsPanel::Show(GameState* gs)
{
    ImGui::SetNextWindowDockID(ImGui::GetID("MyDockSpace"), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 150), ImGuiCond_FirstUseEver);
    ImGui::Begin("Stats", &show, ImGuiWindowFlags_NoCollapse);

    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("FPS: %.1f", io.Framerate);
    ImGui::SameLine(150);
    ImGui::Text("Frame Time: %.3f ms", 1000.0f / io.Framerate);
    ImGui::Separator();

    int drawCalls = 0;
    std::unordered_set<Mesh*> uniqueMeshes;

    if (gs->scene) {
        for (auto& [entity, mr] : gs->scene->meshRenderers) {
            if (!mr.mesh) continue;
            drawCalls++;
            uniqueMeshes.insert(mr.mesh);
        }
    }

    int totalVerts = 0, totalTris = 0;
    for (Mesh* m : uniqueMeshes) {
        totalVerts += m->GetIndexCount();
        totalTris  += m->GetIndexCount() / 3;
    }

    ImGui::Text("Vertices: %d", totalVerts);  ImGui::SameLine(150); ImGui::Text("Triangles: %d", totalTris);
    ImGui::Text("Draw Calls: %d", drawCalls); ImGui::SameLine(150); ImGui::Text("Unique Meshes: %d", (int)uniqueMeshes.size());
    ImGui::Separator();

    if (gs->scene) {
        ImGui::Text("Entities: %d", (int)gs->scene->entities.size());
        ImGui::SameLine(150);
        ImGui::Text("Rigid Bodies: %d", (int)gs->scene->physics.size());
        ImGui::Text("Colliders: %d", (int)gs->scene->colliders.size());
        ImGui::SameLine(150);
        ImGui::Text("Orbits: %d", (int)gs->scene->orbits.size());
        ImGui::Separator();
    }

    if      (gs->isPlaying && !gs->isPaused)
        ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "PLAYING");
    else if (gs->isPlaying &&  gs->isPaused)
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "PAUSED");
    else
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "STOPPED");

    ImGui::End();
}