#include"Mesh.h"
#include"shader.h"
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 
#include <fstream>
#include <sstream>
#include <string>
class Light {
public:
    glm::vec3 position = glm::vec3(1.f, 20.f, 1.f);
    glm::vec3 color    = glm::vec3(1.f, 1.f, 1.f);
    Mesh*   mesh;
    Shader    lightShader;

    Light()
    {

    }
    void Init() {
        lightShader.Init("Resources/light.vs", "Resources/light.fs");
    }

void UploadToShader(Shader& shader, glm::vec3& viewPos) {
    shader.setVec3("lightPos", position);           // vec3, not mat4
    shader.setVec3("lightColor", color);            // vec3, not mat4
    shader.setVec3("viewPos", viewPos);             // vec3, not mat4
}

void Render(glm::mat4& view, glm::mat4& projection) {
    if (!mesh) return;
    lightShader.use();
    glm::mat4 m = glm::translate(glm::mat4(1.f), position);
    m = glm::scale(m, glm::vec3(0.2f));
    
    lightShader.setMat4("model", m);
    lightShader.setMat4("view", view);
    lightShader.setMat4("projection", projection);  // ← ADD THIS
    lightShader.setVec3("color", color);
    
    mesh->Draw();
}
};