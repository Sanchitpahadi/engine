#define STBI_MALLOC(sz) malloc(sz)
#define STBI_FREE(p) free(p)
#define STBI_REALLOC(p, sz) realloc(p, sz)
#define STBI_NO_FAILURE_STRINGS
#define STBI_NO_SIMD 
#define STB_IMAGE_IMPLEMENTATION
#include "std_img.h"
#include"Engine.h"
#include "sphere.h"
#include "PhysicsSystem.h"
#include "vertices.h"
#include "OrbitComponent.h"

int main()
 {
    Verticese ver;

    std::vector rectVertices = ver.rectVertices;
    std::vector rectIndices = ver.rectIndices;

    std::vector cubeVertices = ver.cubeVertices;
    std::vector cubeIndices = ver.cubeIndices;

    std::vector<float>        sphereVerts   = GenerateSphereVertices(36, 18, 1.0f);
    std::vector<unsigned int> sphereIndices = GenerateSphereIndices(36, 18);

    Engine e(1920,1080,"SANC");

    glEnable(GL_FRAMEBUFFER_SRGB);

    Scene scene;
    Mesh cubeMesh;
    Material groundMaterial;
    Mesh sphereMesh;
    Material sphereMaterial;
    Shader shader;

    shader.Init("Resources/shader.vs", "Resources/shader.fs");

    Entity ground = scene.CreateEntity();
    sphereMaterial.shader = &shader;
    sphereMaterial.color = glm::vec3(0.2f, 0.6f, 1.0f);  // blue

    scene.names[ground] = "Ground";

    e.initEverything(scene);

    cubeMesh.Initc(cubeVertices, cubeIndices);
    sphereMesh.Initc(sphereVerts, sphereIndices);

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

    Entity sun = scene.CreateEntity("Sun");
    scene.meshRenderers[sun] = MeshRendererComponent{};
    scene.meshRenderers[sun].mesh     = &sphereMesh;
    scene.meshRenderers[sun].material = &sphereMaterial; // give it its own bright material later
    scene.transforms[sun].position    = glm::vec3(0.0f, 5.0f, 0.0f);
    scene.transforms[sun].scale       = glm::vec3(2.0f, 2.0f, 2.0f); // bigger ball
    scene.colliders[ground] = BoxColliderComponent{ glm::vec3(1.0f) };   // <-- add this
    Entity ball = scene.CreateEntity("Planet");
    scene.meshRenderers[ball] = MeshRendererComponent{};
    scene.meshRenderers[ball].mesh     = &sphereMesh;
    scene.meshRenderers[ball].material = &sphereMaterial;
    scene.transforms[ball].scale       = glm::vec3(0.5f, 0.5f, 0.5f); // smaller
    scene.physics[sun].MakeStatic();
    scene.physics[ground].MakeStatic();

    scene.orbits[ball] = OrbitComponent{ sun, 6.0f, 1.0f, 0.0f };
    scene.ground = ground;

    e.loop(scene);
    return 0;
}