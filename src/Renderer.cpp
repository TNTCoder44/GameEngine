#include "Renderer.h"

void GlClearError()
{
    while (glGetError != GL_NO_ERROR);
}

bool GlLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGl Error] (" << error << "):" << function << " " << file << ": " << line << std::endl;
        return false;
    }

    return true;
}

void Renderer::Clear(const glm::vec4& color) const
{
    glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
	ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(const VertexArray& va, const Shader& shader, unsigned int count) const
{
    shader.Bind();
    va.Bind();
    GLCall(glDrawArrays(GL_TRIANGLES, 0, count));
}

void Renderer::processInput(GLFWwindow* window, float& mixValue)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
        mixValue += 0.001f;
        if (mixValue >= 1.0f)
            mixValue = 1.0f;
	}

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixValue -= 0.001f;
        if (mixValue <= 0.0f)
            mixValue = 0.0f;
    }
}
