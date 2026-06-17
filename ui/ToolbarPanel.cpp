#include "ToolbarPanel.h"

void ToolbarPanel::Show(GameState* gs)
{
    ImGuiViewport* vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(vp->WorkPos.x, vp->WorkPos.y));
    ImGui::SetNextWindowSize(ImVec2(vp->WorkSize.x, 50));
    ImGui::SetNextWindowBgAlpha(0.95f);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoMove     |
        ImGuiWindowFlags_NoResize   |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoDocking  |
        ImGuiWindowFlags_NoTitleBar;

    ImGui::Begin("##Toolbar", nullptr, flags);
    ImGui::SetCursorPosY(7);

    //  Scene controls 
    if (ImGui::Button("Save", ImVec2(70, 35))) gs->SaveScene();
    ImGui::SameLine(0, 10);
    if (ImGui::Button("Load", ImVec2(70, 35))) gs->LoadScene();
    ImGui::SameLine(0, 10);
    if (ImGui::Button("New",  ImVec2(70, 35))) gs->NewScene();

    ImGui::SameLine(0, 20);
    ImGui::Separator();
    ImGui::SameLine(0, 20);

    //  Play controls 
    if (ImGui::Button(gs->isPlaying ? "Playing" : "Play", ImVec2(70, 35))) {
        gs->isPlaying = !gs->isPlaying;
        if (gs->isPlaying) gs->isPaused = false;
    }
    ImGui::SameLine(0, 10);
    if (ImGui::Button(gs->isPaused ? "Resume" : "Pause", ImVec2(70, 35)))
        gs->isPaused = !gs->isPaused;
    ImGui::SameLine(0, 10);
    if (ImGui::Button("Stop", ImVec2(70, 35))) {
        gs->isPlaying = false;
        gs->isPaused  = false;
    }

    ImGui::End();
}
