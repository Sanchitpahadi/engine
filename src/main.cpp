#include"Engine.h"

struct AABB
{
    glm::vec3 min;
    glm::vec3 max;
};

AABB GetAABB(const TransformComponent& t, glm::vec3 size)
{
    glm::vec3 half = (size * t.scale) * 0.5f;

    return {
        t.position - half,
        t.position + half
    };
}

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

    Material cubeMaterial;
    
    Shader shader;

    shader.Init("Resources/shader.vs", "Resources/shader.fs");
    
    e.initEverything();

    cubeMesh.Initc(cubeVertices, cubeIndices);

    cubeMaterial.shader = &shader;
    cubeMaterial.color = glm::vec3(0.2f, 0.0f, 0.5f);

    Entity ground = scene.CreateEntity();
    Entity player = scene.CreateEntity();

    scene.transforms[ground] = TransformComponent{};
    scene.meshRenderers[ground] = MeshRendererComponent{};


    
    scene.transforms[player] = TransformComponent{};
    scene.meshRenderers[player] = MeshRendererComponent{};

    scene.transforms[ground].position = glm::vec3(0.0f, -1.0f, 0.0f);

    scene.transforms[ground].scale = glm::vec3(10.0f, 0.5f, 10.0f);
    
    scene.transforms[player].position = glm::vec3(0.0f);

    scene.meshRenderers[player].mesh = &cubeMesh;
    scene.meshRenderers[player].material = &cubeMaterial;

    
    scene.meshRenderers[ground].mesh = &cubeMesh;
    scene.meshRenderers[ground].material = &cubeMaterial;
    
    e.loop(scene);
    
    return 0;
}