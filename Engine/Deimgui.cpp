#include "Deimgui.h"
#include <glm/gtc/type_ptr.hpp>
#include <chrono>

void DebugUI::Init(GLFWwindow* window, const char* glslVersion)
{
    // Create ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);
}

void DebugUI::Shutdown()
{
    // Shutdown in reverse order of initialization
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void DebugUI::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void DebugUI::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// ---------- panels ----------
void DebugUI::RegisterPanel(const std::string& name, std::function<void()> drawFn)
{
    m_panels.push_back({name, true, drawFn});
}

void DebugUI::RenderPanels()
{
    for (auto& panel : m_panels) {
        if (ImGui::Begin(panel.name.c_str(), &panel.open)) {
            panel.draw();
        }
        ImGui::End();
    }
}

// ---------- helpers ----------
void DebugUI::ColourPicker3(const char* label, glm::vec3& colour)
{
    ImGui::ColorEdit3(label, glm::value_ptr(colour));
}

void DebugUI::ColourPicker4(const char* label, glm::vec4& colour)
{
    ImGui::ColorEdit4(label, glm::value_ptr(colour));
}

void DebugUI::TexturePreview(const char* label, unsigned int texID,
                              float width, float height)
{
    ImGui::Text("%s", label);
    ImGui::Image((void*)(intptr_t)texID, ImVec2(width, height), 
                 ImVec2(0, 1), ImVec2(1, 0));
}

void DebugUI::TransformWidget(const char* label, glm::vec3& position)
{
    ImGui::DragFloat3(label, glm::value_ptr(position), 0.1f);
}

// ---------- logging ----------
void DebugUI::Log(const std::string& msg)
{
    m_log.push_back(msg);
    if (m_log.size() > 100) {  // Keep only last 100 messages
        m_log.erase(m_log.begin());
    }
}

void DebugUI::RenderLog(const char* panelName)
{
    if (ImGui::Begin(panelName)) {
        ImGui::BeginChild("LogContent", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
        for (const auto& msg : m_log) {
            ImGui::Text("%s", msg.c_str());
        }
        ImGui::EndChild();
    }
    ImGui::End();
}

// ---------- FPS counter ----------
void DebugUI::FPSOverlay()
{
    static float fps = 0.0f;
    static auto lastTime = std::chrono::high_resolution_clock::now();
    
    auto now = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float>(now - lastTime).count();
    lastTime = now;
    
    fps = 1.0f / deltaTime;
    
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowBgAlpha(0.3f);
    if (ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar)) {
        ImGui::Text("FPS: %.1f", fps);
    }
    ImGui::End();
}

// ---------- intro screen ----------
bool DebugUI::IntroScreen(const char* message)
{
    if (!m_showIntro)
        return false;
    
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::SetNextWindowBgAlpha(0.8f);
    
    if (ImGui::Begin("Intro", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar)) {
        ImVec2 center = ImGui::GetWindowSize();
        ImGui::SetCursorPosX(center.x / 2.0f - 100);
        ImGui::SetCursorPosY(center.y / 2.0f - 30);
        ImGui::Text("%s", message);
        
        ImGui::SetCursorPosX(center.x / 2.0f - 100);
        ImGui::SetCursorPosY(center.y / 2.0f);
        ImGui::Text("(Press SPACE to continue)");
        
        if (ImGui::IsKeyPressed(ImGuiKey_Space)) {
            m_showIntro = false;
        }
    }
    ImGui::End();
    
    return true;
}