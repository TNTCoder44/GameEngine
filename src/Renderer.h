#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include "VertexArray.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "Globals.h"
#ifndef _WIN32
#include "SoundEngine.h"
#endif
/*
    #define _DEBUG
*/

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

#ifndef __APPLE__
#ifndef _DEBUG
#define GLCall(x)   \
    GlClearError(); \
    x;              \
    ASSERT(GlLogCall(#x, __FILE__, __LINE__));
#endif
#else
#define GLCall(x) x;
#endif

void GlClearError();
bool GlLogCall(const char *function, const char *file, int line);

inline Camera camera(glm::vec3(0.0, 0.0, 3.0));

#ifndef _WIN32
inline SoundEngine soundEngine;
#endif

inline float lastX = static_cast<float>(WINDOW_WIDTH / 2);
inline float lastY = static_cast<float>(WINDOW_HEIGHT / 2);

#ifdef _DEBUG
inline bool debug_mode = true;
#else
inline bool debug_mode = false;
#endif

inline void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = static_cast<float>(xpos) - lastX;
    float yoffset = lastY - static_cast<float>(ypos); // reversed since y-coordinates go from bottom to top

    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    camera.ProcessMouseMovement(xoffset, yoffset);
}

static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
#ifndef _WIN32
        soundEngine.AsyncPlay("../res/sounds/sound.wav");
#else
        printf("Sadly no Sound in Windows yet..");
#endif
    }
}

inline void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

class Renderer
{
public:
    void Clear(const glm::vec4 &color) const;
    void ClearColor(const glm::vec4& color) const;
    void Draw(const VertexArray &va, const IndexBuffer &ib, unsigned int type, const Shader &shader) const;
    void Draw(const VertexArray &va, const Shader &shader, unsigned int type, unsigned int count) const;
    void OnImGuiRender();

    void processInput(GLFWwindow *window);
};
