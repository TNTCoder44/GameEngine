#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include "Renderer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "Globals.h"

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

inline Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

/*inline void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{

    float xoffset = static_cast<float>(xpos) - lastX;
    float yoffset = lastY - static_cast<float>(ypos); // reversed since y-coordinates go from bottom to top
    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}*/

inline void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    float lastX = static_cast<float>(WINDOW_WIDTH / 2);
    float lastY = static_cast<float>(WINDOW_HEIGHT / 2);

    float xPos = static_cast<float>(xpos);
    float yPos = static_cast<float>(ypos);

    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xoffset = xPos - lastX;
    float yoffset = lastY - yPos; // reversed since y-coordinates go from bottom to top

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

inline void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

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