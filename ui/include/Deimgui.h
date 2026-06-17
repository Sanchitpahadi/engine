#pragma once
#include <Glad/glad.h>
#include <GLFW/glfw3.h>


#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "GameState.h"
#include "DockspacePanel.h"
#include "ToolbarPanel.h"
#include "HierarchyPanel.h"
#include "InspectorPanel.h"
#include "StatsPanel.h"

// DEIMGUI
// Thin orchestrator. Owns all panels and the shared GameState.
// No UI logic lives here — each panel handles itself.
class Deimgui
{
public:
    Deimgui() = default;

    ~Deimgui() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    //Lifecycle 
    void init(GLFWwindow* win) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(win, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    void newFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    // Tick all panels
    void basic() {
        dockspace.Show();
        toolbar  .Show(&gameState);
        if (hierarchy.show) hierarchy.Show(&gameState);
        if (inspector.show) inspector.Show(&gameState);
        if (stats    .show) stats    .Show(&gameState);
    }

    void rendering() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup);
        }
    }

    // ── Accessors ─────────────────────────────────────────────────────────
    GameState& GetGameState()        { return gameState; }
    bool IsPlaying()           const { return gameState.isPlaying; }
    bool IsPaused()            const { return gameState.isPaused; }
    int  GetSelectedObjectID() const { return gameState.selectedObjectID; }

    void SetSelectedEntity(Entity e) { gameState.selectedObjectID = (int)e; }

private:
    GameState      gameState;

    DockspacePanel dockspace;
    ToolbarPanel   toolbar;
    HierarchyPanel hierarchy;
    InspectorPanel inspector;
    StatsPanel     stats;
};
