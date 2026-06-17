#include "StatsPanel.h"

void StatsPanel::Show(GameState* gs)
{
    ImGui::SetNextWindowDockID(ImGui::GetID("MyDockSpace"), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 150), ImGuiCond_FirstUseEver);
    ImGui::Begin("Stats", &show, ImGuiWindowFlags_NoCollapse);

    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("FPS: %.1f",        io.Framerate);
    ImGui::SameLine(150);
    ImGui::Text("Frame Time: %.3f ms", 1000.0f / io.Framerate);
    ImGui::Separator();

    // TODO: hook these up to real render stats
    ImGui::Text("Vertices: 24");   ImGui::SameLine(150); ImGui::Text("Triangles: 12");
    ImGui::Text("Draw Calls: 2");  ImGui::SameLine(150); ImGui::Text("Batches: 1");

    ImGui::Separator();

    if      (gs->isPlaying && !gs->isPaused)
        ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "PLAYING");
    else if (gs->isPlaying &&  gs->isPaused)
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "PAUSED");
    else
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "STOPPED");

    ImGui::End();
}
