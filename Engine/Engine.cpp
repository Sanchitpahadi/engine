#include "Engine.h"
#include <iostream>

// ─────────────────────────────────────────────────────────────────────────────
Engine::Engine(int w, int h, const char* ti)
    : width(w), height(h), title(ti),
      window(new Window(w, h, ti))
{}

Engine::~Engine()
{
    delete window;
}

// ─────────────────────────────────────────────────────────────────────────────
// INIT
// ─────────────────────────────────────────────────────────────────────────────
void Engine::initEverything(Scene& scene)
{
    render.Init();
    ui.init(window->GetNativeWindow());

    // Wire the scene into the UI
    ui.GetGameState().scene = &scene;

    // Scene operation callbacks
    ui.GetGameState().onSave = [this]() {
        std::cout << "[Engine] Saving: " << ui.GetGameState().currentScenePath << "\n";
        // TODO: implement serialisation
    };
    ui.GetGameState().onLoad = [this]() {
        std::cout << "[Engine] Loading: " << ui.GetGameState().currentScenePath << "\n";
        // TODO: implement deserialisation
    };
    ui.GetGameState().onNew = [this]() {
        std::cout << "[Engine] New scene\n";
        // TODO: clear all entity/component data
    };

    // Camera
    camera.Position = glm::vec3(0.0f, 2.0f, 5.0f);
    camera.SetProjection(60.0f, (float)width / (float)height, 0.1f, 1000.0f);
    glViewport(0, 0, width, height);

    // Default cube mesh + shader used for auto-assigned entities
    cubeMesh.Initc(cubeVertices, cubeIndices);
    shader.Init("Resources/shader.vs", "Resources/shader.fs");
}

// AUTO-ASSIGN MESH
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

// MAIN LOOP
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

        //  Camera 
        camera.ProcessKeyboard(window->GetNativeWindow(), deltaTime);

        //  Physics only while playing and not paused
        if (ui.IsPlaying() && !ui.IsPaused())
            physics.Update(scene, deltaTime);

        //  Auto-assign mesh to newly created entities 
        AutoAssignMesh(scene, knownEntityCount);

        //  Mouse picking (one-shot per click, skips ImGui windows) 
        bool mouseDown = glfwGetMouseButton(window->GetNativeWindow(),
                                            GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
        if (mouseDown && !wasMousePressed) {
            if (!ImGui::GetIO().WantCaptureMouse) {
                double mx, my;
                glfwGetCursorPos(window->GetNativeWindow(), &mx, &my);
                Ray ray = ScreenToRay(mx, my, width, height,
                                      camera.GetViewMatrix(),
                                      camera.GetProjection());
                selectedEntity = PickEntity(ray, scene);
            }
        }
        wasMousePressed = mouseDown;

        if (selectedEntity != (Entity)-1)
            ui.SetSelectedEntity(selectedEntity);

        //  3D render 
        render.Draw(scene, camera, selectedEntity);

        //  UI render 
        ui.newFrame();
        ui.basic();
        ui.rendering();

        // Game state side-effects 
        ApplyGameStateLogic();

        window->SwapBuffers();
        window->PollEvents();
    }
}
