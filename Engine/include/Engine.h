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

#include <iostream>
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
#include"light.h"
#include"SceneObject.h"
#include "Timer.h"
#include "FileBrowser.h"

#include"transform.h"

class Engine
{
private:

    int height, width;

    const char* title;

    Window* window;

    Shader shader, lightShader;

    // Light light;

    SceneObject player,light;

    Mesh cubeMesh, lightCube;

    Material cubeMaterial;
    Material lightMaterial;
    
    Renderer render;

    Camera camera;

    glm::mat4 projection = glm::mat4(1.0f);

    Timer t;

    Deimgui ui;

    std::vector<float> rectVertices = 
    {
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f
    };

    std::vector<unsigned int> rectIndices = 
    {
        0, 1, 2,
        2, 3, 0
    };

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
        0,  1,  2,  2,  3,  0,  // Front
        4,  5,  6,  6,  7,  4,  // Back
        8,  9, 10, 10, 11,  8,  // Left
        12, 13, 14, 14, 15, 12,  // Right
        16, 17, 18, 18, 19, 16,  // Top
        20, 21, 22, 22, 23, 20,  // Bottom
    };

    void UpdateSelectedObject();
    void ApplyGameStateLogic();

public:
    Engine(int w, int h, const char* title);
    ~Engine();
    void shaderInit();
    void initEverything();
    void run();
    void loop();
};

Engine::Engine(int w, int h, const char* ti)
    : width(w), height(h), title(ti),
      window(new Window(w, h, ti))
{
}

Engine::~Engine()
{
}

void Engine::shaderInit()
{
    shader.Init("Resources/shader.vs", "Resources/shader.fs");      
    lightShader.Init("Resources/light.vs", "Resources/light.fs");
  //  light.Init();

}

void Engine::initEverything()
{
    ui.init(window->GetNativeWindow());
    
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
        player.mesh = nullptr;
        // after: Reset all game objects
    };
    
    // Mesh
    cubeMesh.Initc(cubeVertices, cubeIndices);
    lightCube.Initc(cubeVertices, cubeIndices);



    // Materials
    cubeMaterial.shader = &shader;
    cubeMaterial.color = glm::vec3(0.2f, 0.0f, 0.5f);

    lightMaterial.shader = &lightShader;
    lightMaterial.color = glm::vec3(1.0f);

    // Scene Objects
    player.mesh = &cubeMesh;
    player.material = &cubeMaterial;
    player.transform.position = glm::vec3(0.0f, 0.0f, 0.0f);

    light.mesh = &lightCube;
    light.material = &lightMaterial;
    light.transform.position = glm::vec3(2.0f, 2.0f, 2.0f);

    // Renderer
    render.Init();

    // Camera
    camera.Position = glm::vec3(0.0f, 2.0f, 5.0f);
    camera.SetProjection(60.0f, 800.0f/600.0f, 0.1f, 1000.0f);
    glViewport(0, 0, width, height);

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

void Engine::loop()
{
    while (!window->ShouldClose()) {
        
        render.SetClearColor(0.0f, 1.0f, 0.0f, 1.0f); // bright green
        render.Clear();

        float deltaTime = t.Delta();

        // camera
        camera.ProcessKeyboard(window->GetNativeWindow(), deltaTime);
        glm::mat4 view = camera.GetViewMatrix();

        // GAME LOGIC
        if (ui.IsPlaying() && !ui.IsPaused()) {
            player.transform.position.x += 1.0f * deltaTime; // test movement
        }
        shader.use();


        glm::mat4 model = glm::mat4(1.0f);

        shader.setMat4("model", model);

        shader.setVec3("objectColor", glm::vec3(0.5f, 0.0f, 1.0f));
        shader.setVec3("lightColor", glm::vec3(1.0f));
        shader.setVec3("lightPos", light.transform.position);
        shader.setVec3("viewPos", camera.Position);

        shader.setMat4("view", camera.GetViewMatrix());
        shader.setMat4("projection", camera.GetProjection());
        

        cubeMesh.Bind();
        render.Draw(cubeMesh);
       
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

void Engine::run()
{
    shaderInit();
    initEverything();
    loop();
}