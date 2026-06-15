#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <algorithm>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "scene.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

// ─────────────────────────────────────────────────────────────────────────────
// GAME STATE
// ─────────────────────────────────────────────────────────────────────────────
struct GameState {
    Scene* scene = nullptr;

    bool isPlaying        = false;
    bool isPaused         = false;
    int  selectedObjectID = -1;
    std::string currentScenePath = "Untitled";

    std::function<void()> onSave = nullptr;
    std::function<void()> onLoad = nullptr;
    std::function<void()> onNew  = nullptr;

    void SaveScene() {
        std::cout << "[SAVED] Scene: " << currentScenePath << std::endl;
        if (onSave) onSave();
    }
    void LoadScene() {
        std::cout << "[LOADED] Scene from disk" << std::endl;
        if (onLoad) onLoad();
    }
    void NewScene() {
        selectedObjectID = -1;
        currentScenePath = "Untitled";
        if (onNew) onNew();
    }
};

// ─────────────────────────────────────────────────────────────────────────────
// DEIMGUI
// ─────────────────────────────────────────────────────────────────────────────
class Deimgui
{
private:
    GameState gameState;

    bool showHierarchy = true;
    bool showInspector = true;
    bool showStats     = true;
    bool showToolbar   = true;

public:
    Deimgui() {}

    ~Deimgui() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    // ── INIT ─────────────────────────────────────────────────────────────────
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

    void basic() {
        ShowDockspace();
        if (showToolbar)   ShowToolbar();
        if (showHierarchy) ShowHierarchy();
        if (showInspector) ShowInspector();
        if (showStats)     ShowStats();
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

    // ── PUBLIC ACCESSORS ─────────────────────────────────────────────────────
    GameState& GetGameState()        { return gameState; }
    bool IsPlaying()           const { return gameState.isPlaying; }
    bool IsPaused()            const { return gameState.isPaused; }
    int  GetSelectedObjectID() const { return gameState.selectedObjectID; }
    
    void SetSelectedEntity(Entity selectedEntity)
    {
        gameState.selectedObjectID = selectedEntity;
    }
// ─────────────────────────────────────────────────────────────────────────────
private:
// ─────────────────────────────────────────────────────────────────────────────
    void ShowDockspace();
    void ShowToolbar();
    void ShowHierarchy();
    void ShowInspector(); 
    void ShowStats();
    void ShowProperties(Entity e);
};