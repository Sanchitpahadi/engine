#include <iostream>

#include "Glad/glad.h" 
#include <GLFW/glfw3.h>

#define STBI_MALLOC(sz) malloc(sz)
#define STBI_FREE(p) free(p)
#define STBI_REALLOC(p, sz) realloc(p, sz)
#define STBI_NO_FAILURE_STRINGS
#define STBI_NO_SIMD 
#define STB_IMAGE_IMPLEMENTATION
#include "std_img.h"

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#include "Texture.h"
#include "TextRenderer.h"
#include "shader.h"

    
#include"Window.hpp"
#include"Renderer.h"
#include"Mesh.h"
#include"Camera.h"
#include"OBJLoader.h"
#include"Deimgui.h"
#include"scene.h"
#include "Timer.h"
#include "FileBrowser.h"

#include"transform.h"
#include"picking.h"

class Engine
{
private:

    

        std::vector<float> cubeVertices = {
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,

        // Back face   (normal: 0, 0,-1)
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f,-1.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f,-1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f,-1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, 0.0f,-1.0f,

        // Left face   (normal:-1, 0, 0)
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        // Right face  (normal: 1, 0, 0)
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,

        // Top face    (normal: 0, 1, 0)
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,

        // Bottom face (normal: 0,-1, 0)
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f,-1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,  0.0f,-1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,-1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,-1.0f, 0.0f,
    };

    std::vector<unsigned int> cubeIndices = {
        0,  1,  2,  2,  3,  0,  
        4,  5,  6,  6,  7,  4,  
        8,  9, 10, 10, 11,  8,  
        12, 13, 14, 14, 15, 12, 
        16, 17, 18, 18, 19, 16, 
        20, 21, 22, 22, 23, 20, 
    };

    int height, width;

    const char* title;

    Window* window;

    Shader shader;

    Renderer render;

    Camera camera;

    glm::mat4 projection = glm::mat4(1.0f);

    Timer t;

    Deimgui ui;

    PhysicsSystem physics;


    Mesh cubeMesh;

    Material groundMaterial,playerMaterial;
    
                    
    MeshRendererComponent mr;



    void UpdateSelectedObject();
    void ApplyGameStateLogic();

public:
    Engine(int w, int h, const char* title);
    ~Engine();
    void shaderInit();
    void initEverything(Scene& s);
    void loop(Scene &scene);


    Entity SpawnCube(Scene& scene,
                             const std::string& name,
                             glm::vec3 position,
                             glm::vec3 color)
    {
        Entity e = scene.CreateEntity(name);
    
        // Position it
        scene.transforms[e].position = position;
    
        // Wire up renderer — points to Engine-owned mesh and a per-entity material
        MeshRendererComponent mr;
        mr.mesh             = &cubeMesh;          // shared mesh, Engine owns it
        mr.material         = new Material();     // per-entity material
        mr.material->shader = &shader;     // shared shader, Engine owns it
        mr.material->color  = color;
        scene.meshRenderers[e] = mr;
    
        return e;
    }


};

Engine::Engine(int w, int h, const char* ti)
    : width(w), height(h), title(ti),
      window(new Window(w, h, ti))
{
}

Engine::~Engine()
{
}


void Engine::initEverything(Scene& s)
{

    render.Init();


    ui.init(window->GetNativeWindow());
    
    std::cout << "Scene pointer: " << &s << std::endl;
    std::cout << "Entity count: " << s.entities.size() << std::endl;
    for (Entity e : s.entities) {
        std::cout << "Entity " << e << " name: " << s.names[e] << std::endl;
    }

    ui.GetGameState().scene = &s;
    
    // Setup game state callbacks
    
    ui.GetGameState().onSave = [this]() { 
        std::cout << "Engine: Saving scene to " << ui.GetGameState().currentScenePath << std::endl;
        // after : Implement actual scene saving
    };
    
    ui.GetGameState().onLoad = [this]() { 
        std::cout << "Engine: Loading scene from " << ui.GetGameState().currentScenePath << std::endl;
        // after: Implement actual scene loading
    };
    
    ui.GetGameState().onNew = [this]() { 
        std::cout << "Engine: Creating new scene..." << std::endl;
        // Reset scene state
        // after: Reset all game objects
    };
    
    // Camera
    camera.Position = glm::vec3(0.0f, 2.0f, 5.0f);
    camera.SetProjection(60.0f, 800.0f/600.0f, 0.1f, 1000.0f);
    glViewport(0, 0, width, height);

    cubeMesh.Initc(cubeVertices, cubeIndices);

    shader.Init("Resources/shader.vs", "Resources/shader.fs");



}

// need to update
void Engine::UpdateSelectedObject()
{
    int selectedID = ui.GetSelectedObjectID();
    
    if (selectedID == 0) {
        std::cout << "Selected: Cube" << std::endl;

    } else if (selectedID == 1) {
        std::cout << "Selected: Light" << std::endl;

    } else if (selectedID == 2) {
        std::cout << "Selected: Camera" << std::endl;

    }
}

// ==================== HELPER: Apply game state changes ====================
void Engine::ApplyGameStateLogic()
{
    auto& gameState = ui.GetGameState();
    
    // Handle play/pause/stop states
    if (gameState.isPlaying) {

        // Only update simulation if not paused
        if (!gameState.isPaused) {
            // after it works: Update your game objects here
            float deltaTime = t.Delta();
           // player.movement(*window, deltaTime);
        } else {
            // Paused - the main simulation will only be paused
            std::cout << "Game paused" << std::endl;


        }
    } else {
        // Game stopped - reset state
        gameState.isPaused = false;
    }
}


void Engine::loop(Scene &scene)
{
    size_t knownEntityCount = scene.entities.size();
    static Entity selectedEntity = -1;

    while (!window->ShouldClose()) {
        
        render.SetClearColor(1.0f, 1.0f, 1.0f, 0.0f); // bright green
        render.Clear();

        float deltaTime = t.Delta();

        camera.ProcessKeyboard(window->GetNativeWindow(), deltaTime);


        
        glm::mat4 view = camera.GetViewMatrix();

        // GAME LOGIC
        if (ui.IsPlaying() && !ui.IsPaused()) {
            
            physics.Update(scene, deltaTime);

        }
        if (scene.entities.size() != knownEntityCount)
        {
            for (Entity e : scene.entities)
            {
                if (!scene.meshRenderers.count(e))
                {
                    mr.mesh             = &cubeMesh;
                    mr.material         = new Material();
                    mr.material->shader = &shader;
                    mr.material->color  = glm::vec3(0.4f, 0.6f, 1.0f); // default blue
                    scene.meshRenderers[e] = mr;
                }
            }
            knownEntityCount = scene.entities.size();
        }
 



    static bool wasPressed = false;

    if (glfwGetMouseButton(window->GetNativeWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        if (!wasPressed) // fire once per click, not every frame
        {
        double mx, my;
        glfwGetCursorPos(window->GetNativeWindow(), &mx, &my);

        // Don't pick when the mouse is over an ImGui window
        if (!ImGui::GetIO().WantCaptureMouse)
        {
            Ray ray = ScreenToRay(mx, my, width, height, camera.GetViewMatrix(),camera.GetProjection());
            
            selectedEntity = PickEntity(ray, scene);

        }

    }


    wasPressed = true;
    }
    else 
    {
         wasPressed = false; 
    }
    
        if (selectedEntity != -1)
            ui.SetSelectedEntity(selectedEntity);
        render.Draw(scene,camera,selectedEntity);

        // ui rendering one
        ui.newFrame();     
        ui.basic();        
        ui.rendering();    

        UpdateSelectedObject();

        ApplyGameStateLogic();

        window->SwapBuffers();
        window->PollEvents();
    }
}

