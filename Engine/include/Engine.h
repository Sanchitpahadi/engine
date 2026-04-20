#include "Glad/glad.h"  // First, include Glad
#include <GLFW/glfw3.h>  // Then include GLFW (if you're using it)

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

class Engine
{
private:

    int height,width;
    const char*title;
    Window *window;

    Shader shader, lightShader;

    Light light;

    SceneObject player;


    Mesh cubeMesh,lightCube;
    
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
        // pos(x,y,z)        uv(u,v)     normal(nx,ny,nz)
        // Front face  (normal: 0, 0, 1)
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





public:
    Engine(int w, int h, const char* title);
    ~Engine();
    void shaderInit();
    void initEverything();
    void run();
    void loop();


};

Engine::Engine(int w, int h, const char* ti)
    :width(w),height(h),title(ti),
    window(new Window(w,h,ti))
{
}

Engine::~Engine()
{
}
 void Engine::shaderInit()
 {
    shader.Init("Resources/shader.vs", "Resources/shader.fs");      
    lightShader.Init("Resources/light.vs", "Resources/light.fs");
    light.Init();
 }

 void Engine::initEverything()
 {
    ui.init(window->GetNativeWindow());
    cubeMesh.Initc(cubeVertices, cubeIndices);
    player.mesh = &cubeMesh;
    
    lightCube.Initc(cubeVertices, cubeIndices);
    light.mesh = &lightCube;

    render.Init();

    camera.Position = glm::vec3(0.0f, 2.0f, 5.0f);
    projection = glm::perspective(glm::radians(60.0f), 800.0f/600.0f, 0.1f, 1000.0f);


 }

 void Engine::loop()
 {
    while (!window->ShouldClose()) {
        ui.newFrame();


        ui.basic();
        

        render.SetClearColor(0.2f, 0.f, 0.5f, 1.0f);
        render.Clear();


            float deltaTime = t.Delta();
            camera.ProcessKeyboard(window->GetNativeWindow(), deltaTime);

            glm::mat4 view = camera.GetViewMatrix();


            shader.use();
            shader.setMat4("view", view);
            shader.setMat4("projection", projection);
            shader.setMat4("model", glm::mat4(1.f));
            shader.setVec3("objectColor", glm::vec3(0.2f, 0.f, 0.5f));
            shader.setInt("texture0", 0);
            light.UploadToShader(shader, camera.Position);

            


            cubeMesh.Draw();
            player.movement(*window, deltaTime);


            light.Render(view, projection);
  
        ui.rendering();

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