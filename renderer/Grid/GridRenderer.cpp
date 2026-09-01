#include "GridRenderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

static const float s_QuadVerts[] = {
    -1.f, -1.f,
     1.f, -1.f,
     1.f,  1.f,
    -1.f, -1.f,
     1.f,  1.f,
    -1.f,  1.f,
};

static const char* s_GridVert = R"(
#version 330 core
layout(location = 0) in vec2 aPos;

out vec3 nearPoint;
out vec3 farPoint;

uniform mat4 uView;
uniform mat4 uProj;

vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 proj)
{
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(proj);
    vec4 unprojected = viewInv * projInv * vec4(x, y, z, 1.0);
    return unprojected.xyz / unprojected.w;
}

void main()
{
    nearPoint   = UnprojectPoint(aPos.x, aPos.y, 0.0, uView, uProj);
    farPoint    = UnprojectPoint(aPos.x, aPos.y, 1.0, uView, uProj);
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

static const char* s_GridFrag = R"(
#version 330 core

in  vec3 nearPoint;
in  vec3 farPoint;
out vec4 fragColor;

uniform mat4  uView;
uniform mat4  uProj;
uniform float uNear;
uniform float uFar;

vec4 Grid(vec3 pos, float scale, bool drawAxis)
{
    vec2 coord = pos.xz * scale;
    vec2 deriv = fwidth(coord);
    vec2 grid  = abs(fract(coord - 0.5) - 0.5) / deriv;
    float line = min(grid.x, grid.y);
    float alpha = 1.0 - min(line, 1.0);

    vec4 color = vec4(0.5, 0.5, 0.5, alpha);

    if (drawAxis) {
        if (abs(pos.z) < 0.05)
            color = vec4(0.9, 0.2, 0.2, 1.0);
        if (abs(pos.x) < 0.05)
            color = vec4(0.2, 0.85, 0.35, 1.0);
    }
    return color;
}

void main()
{
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    if (t <= 0.0) discard;

    vec3 pos = nearPoint + t * (farPoint - nearPoint);

    float dist = length(pos.xz);
    float fade = 1.0 - clamp(dist / (uFar * 0.5), 0.0, 1.0);
    fade = fade * fade;
    if (fade < 0.005) discard;

    vec4 coarse = Grid(pos, 1.0,  true);
    vec4 fine   = Grid(pos, 10.0, false) * 0.4;
    vec4 color  = coarse + fine;
    color.a    *= fade;

    if (color.a < 0.01) discard;
    fragColor = color;
}
)";

static unsigned int CompileStage(GLenum type, const char* src)
{
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    int ok;
    glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetShaderInfoLog(id, 512, nullptr, log);
        std::cerr << "[GridRenderer] Shader error: " << log << "\n";
    }
    return id;
}

unsigned int GridRenderer::CompileShader(const char* vert, const char* frag)
{
    unsigned int vs   = CompileStage(GL_VERTEX_SHADER,   vert);
    unsigned int fs   = CompileStage(GL_FRAGMENT_SHADER, frag);
    unsigned int prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return prog;
}

void GridRenderer::Init()
{
    m_Shader = CompileShader(s_GridVert, s_GridFrag);

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(s_QuadVerts), s_QuadVerts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void GridRenderer::Draw(const glm::mat4& view, const glm::mat4& proj,
                        float nearPlane, float farPlane)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    glUseProgram(m_Shader);
    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "uView"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "uProj"), 1, GL_FALSE, glm::value_ptr(proj));
    glUniform1f(glGetUniformLocation(m_Shader, "uNear"), nearPlane);
    glUniform1f(glGetUniformLocation(m_Shader, "uFar"),  farPlane);

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

void GridRenderer::Shutdown()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteProgram(m_Shader);
}