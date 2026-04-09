#include"Engine.h"

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

    cubeMesh.Initc(cubeVertices, cubeIndices);
    player.mesh = &cubeMesh;

    lightCube.Initc(cubeVertices, cubeIndices);
    light.mesh = &lightCube;

    render.Init();

    camera.Position = glm::vec3(0.0f, 2.0f, 5.0f);
    projection = glm::perspective(glm::radians(60.0f), 800.0f/600.0f, 0.1f, 1000.0f);

     render.Init();

 }
 void Engine::run()
 {

 }
 void Engine::loop()
 {
        while (!window->ShouldClose()) {
        render.SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);
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
  

        window->SwapBuffers();
        window->PollEvents();
    }
 }