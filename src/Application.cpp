#ifdef _WIN32
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
#endif

#include "Includes.h"

constexpr int WINDOW_WIDTH = 1000;
constexpr int WINDOW_HEIGHT = 800;

int main(int argc, char **argv)
{
    GLFWwindow *window;

    /* Initialize GLFW */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // find primary monitor
    auto *monitor = glfwGetPrimaryMonitor();

    /* Create a windowed mode window and its OpenGL context */
    // change monitor variable to monitor to get fullscreen resolution
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGl", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // handle resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // uncomment to set window fullscreen resolution
    // glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, 1920, 1080, 280);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // wait for vsync
    glfwSwapInterval(0);

    // Set Window Icon
    SetWindowIcon(window, "../res/icons/window.png");

    // init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // initialize ImGui
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();

    // start coding

    GLfloat vertices[] = {
        // positions        // colors          // texture coords
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
    };

    GLuint indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    // GLCall(glEnable(GL_BLEND));
    // GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    Shader shader("../res/shaders/shader.vert", "../res/shaders/shader.frag", "");
    shader.Bind();

    VertexArray vao;
    VertexBuffer vbo(vertices, sizeof(vertices));
    IndexBuffer ibo(indices, sizeof(indices));
    VertexBufferLayout layout;

    vao.Bind();
    vbo.Bind();
    ibo.Bind();

    Texture texture1("../res/textures/container.jpg", false, GL_TEXTURE_2D,
                     GL_RGB, GL_REPEAT, GL_LINEAR, GL_LINEAR, 0);
    Texture texture2("../res/textures/awesomeface.png", true, GL_TEXTURE_2D,
                     GL_RGBA, GL_REPEAT, GL_LINEAR, GL_LINEAR, 0);

    texture1.Bind(0);
    texture2.Bind(1);

    // Positions
    layout.Push<float>(3, GL_FALSE);
    // Colors
    layout.Push<float>(3, GL_FALSE);
    // Textures
    layout.Push<float>(2, GL_FALSE);

    vao.AddBuffer(vbo, layout);

    Renderer renderer;

    // unbind everything
    vao.Unbind();
    vbo.Unbind();
    ibo.Unbind();
    shader.Unbind();
    texture1.Unbind();
    texture2.Unbind();

    float mixValue = 0.2f;

    // main loop
    while (!glfwWindowShouldClose(window))
    {
        // check for input
        renderer.processInput(window, mixValue);

        // init ImGui
        ImGui_ImplGlfwGL3_NewFrame();

        // transform position
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        transform = glm::rotate(transform, static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));

        /* start rendering */
        // clear background
        renderer.Clear(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

        // render screen
        texture1.Bind(0);
        texture2.Bind(1);
        shader.Bind();
        shader.SetUniform1i("texture1", 0);
        shader.SetUniform1i("texture2", 1);
        shader.SetUniform1f("mixValue", mixValue);
        shader.SetUniformMat4("transform", transform);
        renderer.Draw(vao, ibo, shader);

        // second transform
        transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f));
        float scaleAmount = static_cast<float>(sin(glfwGetTime()));
        transform = glm::scale(transform, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
        
        shader.SetUniformMat4("transform", transform);
        renderer.Draw(vao, ibo, shader);

        // draw ImGui
        // renderer.OnImGuiRender();
 
        // render ImGui
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        // do glfw stuff
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    vao.Delete();
    vbo.Delete();
    ibo.Delete();
    shader.Delete();
    texture1.Delete();
    texture2.Delete();

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}
