#include "Engine.h"
#include "AssetManager.h"
#include "SceneSerializer.h"
#include <iostream>


Engine::Engine(int w, int h, const char* ti)
    : width(w), height(h), title(ti),
      window(new Window(w, h, ti))
{}

Engine::~Engine()
{
    delete window;
}

void Engine::initEverything(Scene& scene)
{
    render.Init();
    ui.init(window->GetNativeWindow());

    // Wire the scene into the UI
    ui.GetGameState().scene = &scene;

    // Scene operation callbacks
    ui.GetGameState().onSave = [this, &scene]() {
        const std::string& path = ui.GetGameState().currentScenePath;
        std::cout << "[Engine] Saving: " << path << "\n";
        if (!SceneSerializer::Save(scene, path))
            std::cout << "[Engine] Save FAILED: " << path << "\n";
    };
    ui.GetGameState().onLoad = [this, &scene]() {
        const std::string& path = ui.GetGameState().currentScenePath;
        std::cout << "[Engine] Loading: " << path << "\n";
        if (!SceneSerializer::Load(scene, path))
            std::cout << "[Engine] Load FAILED: " << path << "\n";
    };
    ui.GetGameState().onNew = [this, &scene]() {
        std::cout << "[Engine] New scene\n";
        scene.entities.clear();
        scene.names.clear();
        scene.transforms.clear();
        scene.meshRenderers.clear();
        scene.colliders.clear();
        scene.physics.clear();
        scene.orbits.clear();
        AssetManager::Get().Clear();
    };

    // Camera
    camera.Position = glm::vec3(0.0f, 2.0f, 5.0f);
    camera.SetProjection(60.0f, (float)width / (float)height, 0.1f, 1000.0f);
    glViewport(0, 0, width, height);

    // Default cube mesh + shader used for auto-assigned entities
    cubeMesh.Initc(cubeVertices, cubeIndices);
    shader.Init("Resources/shader.vs", "Resources/shader.fs");

    renderSystem.Init();
    // Asset panel: browses Resources/, spawns .obj files on screen, drives
    // the same save/load path as the UI's Save/Load buttons.
    assetPanel.Init("Resources", &shader);
}

// Any entity that appears without a MeshRenderer gets the default cube.

void Engine::AutoAssignMesh(Scene& scene, size_t& knownCount)
{
    if (scene.entities.size() == knownCount) return;

    for (Entity e : scene.entities) {
        if (!scene.meshRenderers.count(e)) {
            MeshRendererComponent defaultMR;
            defaultMR.mesh             = &cubeMesh;
            defaultMR.material         = new Material();
            defaultMR.material->shader = &shader;
            defaultMR.material->color  = glm::vec3(0.4f, 0.6f, 1.0f);
            scene.meshRenderers[e]     = defaultMR;
            scene.colliders[e] = BoxColliderComponent{ glm::vec3(1.0f) };
        }
    }
    knownCount = scene.entities.size();
}

// GAME STATE LOGIC
void Engine::ApplyGameStateLogic()
{
    auto& gs = ui.GetGameState();

    if (gs.isPlaying) {
        if (!gs.isPaused) {
            // Simulation tick happens in loop() via physics.Update
        } else {
            // Paused — nothing runs
        }
    } else {
        gs.isPaused = false;
    }
}

void Engine::loop(Scene& scene)
{
    size_t knownEntityCount = scene.entities.size();
    Entity selectedEntity   = (Entity)-1;
    bool   wasMousePressed  = false;

    while (!window->ShouldClose())
    {
        // Clear 
        render.SetClearColor(1.0f, 1.0f, 1.0f, 0.0f);
        render.Clear();

        float deltaTime = t.Delta();
        totalTime += deltaTime;

        //  Camera 
        camera.ProcessKeyboard(window->GetNativeWindow(), deltaTime);
        AutoAssignMesh(scene, knownEntityCount);
        //  Physics only while playing and not paused
        if (ui.IsPlaying() && !ui.IsPaused())
        {
            physicsSystem.Update(scene, deltaTime);
            orbitSystem.Update(scene, totalTime);
        }        

        //  Mouse picking (one-shot per click)
        bool mouseDown = glfwGetMouseButton(window->GetNativeWindow(),
                                            GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
        if (mouseDown && !wasMousePressed) {
            if (!ImGui::GetIO().WantCaptureMouse) {
                double mx, my;
                glfwGetCursorPos(window->GetNativeWindow(), &mx, &my);
                Ray ray = ScreenToRay(mx, my, width, height,camera.GetViewMatrix(),camera.GetProjection());
                selectedEntity = PickEntity(ray, scene);
            }
        }
        wasMousePressed = mouseDown;

        if (selectedEntity != (Entity)-1)
        {
            ui.SetSelectedEntity(selectedEntity);
        }

        renderSystem.Update(scene,render,camera, selectedEntity);
        //  UI render 
        ui.newFrame();
        ui.basic();
        assetPanel.Render(scene, selectedEntity);
        ui.rendering();

        // Game state side-effects 
        ApplyGameStateLogic();

        window->SwapBuffers();
        window->PollEvents();
    }
}