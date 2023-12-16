#pragma once
#include <GLFW/glfw3.h>

inline void SetWindowIcon(GLFWwindow* window, const char* src)
{
    // load file image
    int width, height;
    int channels;
    unsigned char* pixels = stbi_load(src, &width, &height, &channels, 4);

    // change window icon
    GLFWimage images[1];
    images[0].width = width;
    images[0].height = height;
    images[0].pixels = pixels;

    glfwSetWindowIcon(window, 1, images);
}
