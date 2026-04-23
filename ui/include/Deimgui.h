#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <functional>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

// ==================== GAME STATE ====================
struct GameState {
    bool isPlaying = false;
    bool isPaused = false;
    int selectedObjectID = -1;
    std::string currentScenePath = "Untitled";
    
    // Callbacks for external systems
    std::function<void()> onSave = nullptr;
    std::function<void()> onLoad = nullptr;
    std::function<void()> onNew = nullptr;
    
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

// ==================== MAIN DEIMGUI CLASS ====================
class Deimgui
{
private:
    GameState gameState;
    
    // Window visibility toggles
    bool showHierarchy = true;
    bool showInspector = true;
    bool showStats = true;
    bool showToolbar = true;
    
public:
    Deimgui() {}
    
    ~Deimgui() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    // ==================== INITIALIZATION ====================
    void init(GLFWwindow* win) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        
        ImGuiIO& io = ImGui::GetIO();
        
        // Enable docking
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        // Optional: Enable multi-viewport for floating windows
        // 
        
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
        // Setup docking area (invisible container)
        ShowDockspace();
        
        // Render UI panels
        if (showToolbar) ShowToolbar();
        if (showHierarchy) ShowHierarchy();
        if (showInspector) ShowInspector();
        if (showStats) ShowStats();
    }

    void rendering() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    // ==================== PUBLIC ACCESSORS ====================
    GameState& GetGameState() { return gameState; }
    bool IsPlaying() const { return gameState.isPlaying; }
    bool IsPaused() const { return gameState.isPaused; }
    int GetSelectedObjectID() const { return gameState.selectedObjectID; }

    // ==================== PRIVATE UI PANELS ====================

private:

    // DOCKSPACE - Invisible container for all panels
    void ShowDockspace() {
        static bool dockspaceOpen = true;
        
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        
        // Hide all window decorations
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        
        ImGui::Begin("DockSpace Master", &dockspaceOpen, window_flags);
        
        // Create docking area with PassthruCentralNode
        // This allows mouse input to pass through to the viewport
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
        
        ImGui::End();
    }

    // TOOLBAR - Top button bar with Save/Load/Play controls
    void ShowToolbar() {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x, viewport->WorkPos.y));
        ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, 50));
        
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking |
                                 ImGuiWindowFlags_NoTitleBar;
        
        ImGui::SetNextWindowBgAlpha(0.95f);
        ImGui::Begin("##Toolbar", nullptr, flags);
        ImGui::SetCursorPosY(7);
        
        // FILE OPERATIONS
        if (ImGui::Button("Save", ImVec2(70, 35))) {
            gameState.SaveScene();
        }
        ImGui::SameLine(0, 10);
        
        if (ImGui::Button("Load", ImVec2(70, 35))) {
            gameState.LoadScene();
        }
        ImGui::SameLine(0, 10);
        
        if (ImGui::Button("New", ImVec2(70, 35))) {
            gameState.NewScene();
        }
        
        // Separator - Fixed: use Separator() instead of SeparatorEx()
        ImGui::SameLine(0, 20);
        ImGui::Separator();
        ImGui::SameLine(0, 20);
        
        // PLAYBACK CONTROLS
        const char* playLabel = gameState.isPlaying ? "Playing" : "Play";
        if (ImGui::Button(playLabel, ImVec2(70, 35))) {
            gameState.isPlaying = !gameState.isPlaying;
            if (gameState.isPlaying) gameState.isPaused = false;
        }
        ImGui::SameLine(0, 10);
        
        const char* pauseLabel = gameState.isPaused ? "Resume" : "Pause";
        if (ImGui::Button(pauseLabel, ImVec2(70, 35))) {
            gameState.isPaused = !gameState.isPaused;
        }
        ImGui::SameLine(0, 10);
        
        if (ImGui::Button("Stop", ImVec2(70, 35))) {
            gameState.isPlaying = false;
            gameState.isPaused = false;
        }
        
        ImGui::End();
    }

    // HIERARCHY - Left panel showing scene objects
    void ShowHierarchy() {
        // Fixed: use SetNextWindowDockID() instead of SetNextWindowDocking()
        ImGui::SetNextWindowDockID(ImGui::GetID("MyDockSpace"), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(250, 400), ImGuiCond_FirstUseEver);
        
        ImGui::Begin("Hierarchy", &showHierarchy, ImGuiWindowFlags_NoCollapse);
        
        ImGui::Text("Scene Objects");
        ImGui::Separator();
        
        // Tree structure - Fixed: use TreeNodeEx() instead of TreeNode() with flags
        if (ImGui::TreeNodeEx("Scene", ImGuiTreeNodeFlags_DefaultOpen)) {
            
            // Cube object
            bool cubSelected = (gameState.selectedObjectID == 0);
            if (ImGui::Selectable("Cube", &cubSelected)) {
                gameState.selectedObjectID = 0;
            }
            
            // Light object
            bool lightSelected = (gameState.selectedObjectID == 1);
            if (ImGui::Selectable("Light", &lightSelected)) {
                gameState.selectedObjectID = 1;
            }
            
            // Camera object
            bool camSelected = (gameState.selectedObjectID == 2);
            if (ImGui::Selectable("Camera", &camSelected)) {
                gameState.selectedObjectID = 2;
            }
            
            ImGui::TreePop();
        }
        
        ImGui::End();
    }

    // INSPECTOR - Right panel showing selected object properties
    void ShowInspector() {
        // Fixed: use SetNextWindowDockID() instead of SetNextWindowDocking()
        ImGui::SetNextWindowDockID(ImGui::GetID("MyDockSpace"), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_FirstUseEver);
        
        ImGui::Begin("Inspector", &showInspector, ImGuiWindowFlags_NoCollapse);
        
        ImGui::Text("Current Scene: %s", gameState.currentScenePath.c_str());
        ImGui::Separator();
        
        if (gameState.selectedObjectID == -1) {
            ImGui::TextDisabled("Select an object to view properties");
        }
        else {
            ImGui::Text("Selected: Object #%d", gameState.selectedObjectID);
            ImGui::Separator();
            
            switch (gameState.selectedObjectID) {
                case 0: ShowCubeProperties(); break;
                case 1: ShowLightProperties(); break;
                case 2: ShowCameraProperties(); break;
            }
        }
        
        ImGui::End();
    }

    // STATS - Bottom panel showing performance metrics
    void ShowStats() {
        // Fixed: use SetNextWindowDockID() instead of SetNextWindowDocking()
        ImGui::SetNextWindowDockID(ImGui::GetID("MyDockSpace"), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(400, 150), ImGuiCond_FirstUseEver);
        
        ImGui::Begin("Stats", &showStats, ImGuiWindowFlags_NoCollapse);
        
        ImGuiIO& io = ImGui::GetIO();
        
        // Performance metrics
        ImGui::Text("FPS: %.1f", io.Framerate);
        ImGui::SameLine(150);
        ImGui::Text("Frame Time: %.3f ms", 1000.0f / io.Framerate);
        
        ImGui::Separator();
        
        // Rendering stats
        ImGui::Text("Vertices: 24");
        ImGui::SameLine(150);
        ImGui::Text("Triangles: 12");
        
        ImGui::Text("Draw Calls: 2");
        ImGui::SameLine(150);
        ImGui::Text("Batches: 1");
        
        ImGui::End();
    }

    // ==================== PROPERTY PANELS ====================

    void ShowCubeProperties() {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            static float pos[3] = {0.0f, 0.0f, 0.0f};
            static float rot[3] = {0.0f, 0.0f, 0.0f};
            static float scale[3] = {1.0f, 1.0f, 1.0f};
            
            ImGui::DragFloat3("Position", pos, 0.01f);
            ImGui::DragFloat3("Rotation", rot, 1.0f);
            ImGui::DragFloat3("Scale", scale, 0.01f);
        }
        
        if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("Vertices: 24");
            ImGui::Text("Triangles: 12");
            ImGui::Text("Material: Default");
        }
    }

    void ShowLightProperties() {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            static float pos[3] = {2.0f, 2.0f, 2.0f};
            ImGui::DragFloat3("Position##light", pos, 0.01f);
        }
        
        if (ImGui::CollapsingHeader("Light Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
            static float intensity = 1.0f;
            static float color[3] = {1.0f, 1.0f, 1.0f};
            static float range = 10.0f;
            
            ImGui::SliderFloat("Intensity", &intensity, 0.0f, 2.0f);
            ImGui::ColorEdit3("Color##light", color);
            ImGui::SliderFloat("Range", &range, 0.1f, 100.0f);
        }
    }

    void ShowCameraProperties() {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            static float pos[3] = {0.0f, 2.0f, 5.0f};
            ImGui::DragFloat3("Position##camera", pos, 0.01f);
        }
        
        if (ImGui::CollapsingHeader("Camera Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
            static float fov = 60.0f;
            static float near = 0.1f;
            static float far = 1000.0f;
            
            ImGui::SliderFloat("FOV", &fov, 30.0f, 120.0f);
            ImGui::DragFloat("Near Plane", &near, 0.01f, 0.001f, 10.0f);
            ImGui::DragFloat("Far Plane", &far, 1.0f, 10.0f, 10000.0f);
        }
    }
};