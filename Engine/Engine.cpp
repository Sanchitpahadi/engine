#include"Engine.h"

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
        
        render.SetClearColor(1.0f, 1.0f, 1.0f, 1.0f); // bright green
        render.Clear();

        float deltaTime = t.Delta();

        camera.ProcessKeyboard(window->GetNativeWindow(), deltaTime);


        
        glm::mat4 view = camera.GetViewMatrix();

        // GAME LOGIC
        if (ui.IsPlaying() && !ui.IsPaused()) {
            
            physics.Update(scene, deltaTime);

        }
        // =============== just checks if there is new entity or not
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
 
        // ================= when we try to select the entities or objects
        static bool wasPressed = false;

        // ================= ray casting exampel when we use try to select the object
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
    // ======= select something it will be 0 , 1 2 i wanted to make smthg like blender so thoungt might be usefull =========
        if (selectedEntity != -1)
        {
            ui.SetSelectedEntity(selectedEntity);

        }

        //======= Draws everything in the 3d engine renderer ==========
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

