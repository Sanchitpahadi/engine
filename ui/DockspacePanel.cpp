#include "DockspacePanel.h"

void DockspacePanel::Show()
{
    static bool open = true;

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_MenuBar              |
        ImGuiWindowFlags_NoDocking            |
        ImGuiWindowFlags_NoTitleBar           |
        ImGuiWindowFlags_NoCollapse           |
        ImGuiWindowFlags_NoResize             |
        ImGuiWindowFlags_NoMove               |
        ImGuiWindowFlags_NoBringToFrontOnFocus|
        ImGuiWindowFlags_NoNavFocus;

    const ImGuiViewport* vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(vp->WorkPos);
    ImGui::SetNextWindowSize(vp->WorkSize);
    ImGui::SetNextWindowViewport(vp->ID);

    ImGui::Begin("DockSpace Master", &open, flags);
    ImGui::DockSpace(ImGui::GetID("MyDockSpace"), ImVec2(0, 0),
                     ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::End();
}
