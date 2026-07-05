#pragma once
#include <Glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class GridRenderer
{
public:
    void Init();
    void Draw(const glm::mat4& view, const glm::mat4& proj, float nearPlane, float farPlane);
    void Shutdown();

private:
    unsigned int m_VAO = 0, m_VBO = 0;
    unsigned int m_Shader = 0;

    unsigned int CompileShader(const char* vert, const char* frag);
};