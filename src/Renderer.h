#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"

#ifdef _WIN32
#define ASSERT(x) \
    if (!(x))     \
        __debugbreak();
#endif

#ifdef __APPLE__
#define ASSERT(x) \
    if (!(x))     \
        ;
#endif

#ifdef __linux__
#define ASSERT(x) \
    if (!(x))     \
        ;
#endif

#ifndef _DEBUG
#define GLCall(x)   \
    GlClearError(); \
    x;              \
    ASSERT(GlLogCall(#x, __FILE__, __LINE__));

#else
#define GLCall(x) x;
#endif

void GlClearError();
bool GlLogCall(const char *function, const char *file, int line);

inline void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

class Renderer
{
public:
    void Clear(const glm::vec4 &color) const;
    void Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const;
    void Draw(const VertexArray &va, const Shader &shader, unsigned int count) const;
    void OnImGuiRender();

    void processInput(GLFWwindow *window, float &mixValue);
};