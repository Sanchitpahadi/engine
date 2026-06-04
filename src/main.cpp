#include"Engine.h"



int main()
 {

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
        0,  1,  2,  2,  3,  0,  
        4,  5,  6,  6,  7,  4,  
        8,  9, 10, 10, 11,  8,  
        12, 13, 14, 14, 15, 12, 
        16, 17, 18, 18, 19, 16, 
        20, 21, 22, 22, 23, 20, 
    };

    Engine e(1200,800,"SuckMyBrain");

    Scene scene;
    
    Mesh cubeMesh;

    Material groundMaterial;
    
    Shader shader;

    shader.Init("Resources/shader.vs", "Resources/shader.fs");

    Entity ground = scene.CreateEntity();



    scene.names[ground] = "Ground";


    e.initEverything(scene);


    cubeMesh.Initc(cubeVertices, cubeIndices);

    groundMaterial.shader = &shader;
    groundMaterial.color = glm::vec3(1.0f, 1.0f, 0.0f);


    // initializing its material and transform material etc
    scene.transforms[ground] = TransformComponent{};
    scene.meshRenderers[ground] = MeshRendererComponent{};

    scene.meshRenderers[ground].mesh = &cubeMesh;
    scene.meshRenderers[ground].material = &groundMaterial;
    
    // Aplying the initialized material 
    scene.transforms[ground].position = glm::vec3(0.0f, -1.0f, 0.0f);
    scene.transforms[ground].scale = glm::vec3(100.0f, 0.5f, 100.0f);


    scene.ground = ground;



    e.loop(scene);
    
    return 0;
}