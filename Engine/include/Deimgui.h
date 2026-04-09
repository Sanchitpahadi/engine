#pragma once

#include <string>
#include <functional>
#include <vector>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

// ----------------------------------------------------------------
//  A panel you register once and it draws itself every frame
// ----------------------------------------------------------------
struct UIPanel {
    std::string              name;
    bool                     open = true;
    std::function<void()>    draw;   // lambda with all the widgets
};

// ----------------------------------------------------------------
//  DebugUI  –  owns the entire ImGui lifecycle
// ----------------------------------------------------------------
class DebugUI {
public:

    // ---------- lifecycle ----------
    void Init(GLFWwindow* window, const char* glslVersion = "#version 330");
    void Shutdown();

    // Call at the TOP of your game loop (before any ImGui:: calls)
    void BeginFrame();

    // Call at the BOTTOM of your game loop (after all ImGui:: calls)
    void EndFrame();

    // ---------- panels ----------
    // Register a named panel with a lambda that contains your widgets
    void RegisterPanel(const std::string& name, std::function<void()> drawFn);

    // Render all registered panels
    void RenderPanels();

    // ---------- built-in helpers you can call inside a lambda ----------

    // Colour picker that writes directly into a glm::vec3  (rgb 0-1)
    static void ColourPicker3(const char* label, glm::vec3& colour);

    // Colour picker that writes directly into a glm::vec4  (rgba 0-1)
    static void ColourPicker4(const char* label, glm::vec4& colour);

    // Show a loaded OpenGL texture as a preview image
    // texID  = the GLuint returned by your Texture class
    // size   = display size in pixels
    static void TexturePreview(const char* label, unsigned int texID,
                               float width = 128.f, float height = 128.f);

    // Drag-able XYZ transform controls (position / rotation / scale)
    static void TransformWidget(const char*  label,
                                glm::vec3&   position
                              //  glm::vec3&   rotation,
                               // float&       scale
                               );

    // Simple on-screen log – call Log() from anywhere, shows in panel
    void Log(const std::string& msg);
    void RenderLog(const char* panelName = "Log");

    // FPS counter label
    static void FPSOverlay();

    // Intro-screen helper: returns true while the intro is showing
    bool IntroScreen(const char* message = "PRESS SPACE TO START");
    bool IsIntroShowing() const { return m_showIntro; }
    void DismissIntro()         { m_showIntro = false; }

private:
    std::vector<UIPanel>       m_panels;
    std::vector<std::string>   m_log;
    bool                       m_showIntro = true;
};