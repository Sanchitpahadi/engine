#pragma once


#include <string>
#include <functional>
#include <vector>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Deimgui
{
    private:

    GLFWwindow* window;
    
    public:

    Deimgui()
    {

    }

    ~Deimgui()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void init(GLFWwindow* win)
    {
        window = win;
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO(); 
        (void)io;

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    void newFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void basic()
    {

        ImGui::Begin("Control Panel");

        ImGui::Text("Hello from ImGui!");

        static float value = 0.0f;
        ImGui::SliderFloat("Value", &value, 0.0f, 1.0f);

        if (ImGui::Button("Press Me"))
        {
            std::cout << "Button clicked\n";
        }

        ImGui::End();
    }

    void rendering()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

};