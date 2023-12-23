#ifdef _WIN32
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
#endif

#include "Includes.h"

int main(int argc, char **argv)
{
    GLFWwindow *window;

    /* Initialize GLFW */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
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

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // handle resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // handle mouse input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    // handle scrolling input
    glfwSetScrollCallback(window, scroll_callback);

    // uncomment to set window fullscreen resolution
    // glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, 1920, 1080, 280);

    // wait for vsync
    glfwSwapInterval(1);

    // Set Window Icon
#ifdef _WIN32
    SetWindowIcon(window, "../res/icons/window.png");
#endif

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

    /* start coding */

    // GLCall(glEnable(GL_BLEND));
    // GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    GLCall(glEnable(GL_DEPTH_TEST));

    // Shaders
    Shader lightningShader("../res/shaders/shader.vert", "../res/shaders/shader.frag", "");
    Shader lightCubeShader("../res/shaders/light_cube.vert", "../res/shaders/light_cube.frag", "");
    lightningShader.Bind();
    lightCubeShader.Bind();

    // VertexArray, VertexBuffer, IndexBuffer
    VertexArray vao;
    VertexBuffer vbo(vertices, sizeof(vertices));
    IndexBuffer ibo(indices, sizeof(indices));
    VertexBufferLayout layout;

    vao.Bind();
    vbo.Bind();
    ibo.Bind();

    // Textures
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

    VertexArray lightVao;
    lightVao.Bind();
    lightVao.AddBuffer(vbo, layout);

    Renderer renderer;

    // unbind everything
    vao.Unbind();
    lightVao.Unbind();
    vbo.Unbind();
    ibo.Unbind();
    lightningShader.Unbind();
    lightCubeShader.Unbind();
    texture1.Unbind();
    texture2.Unbind();

    float mixValue = 0.2f;

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    // main loop
    while (!glfwWindowShouldClose(window))
    {
        // calculate time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // check for input
        renderer.processInput(window, mixValue);

        // init ImGui
        ImGui_ImplGlfwGL3_NewFrame();

        /* start rendering */
        // clear background
        renderer.Clear(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

        // calculate position
        // view matrix
        glm::mat4 view = glm::mat4(1.0f);
        view = camera.GetViewMatrix();

        // projection matrix
        glm::mat4 proj = glm::mat4(1.0f);
        proj = glm::perspective(glm::radians(camera.Zoom),
                                static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT),
                                0.1f, 100.0f);

        // render screen
        texture1.Bind(0);
        texture2.Bind(1);
        lightningShader.Bind();
        lightningShader.SetUniform1i("texture1", 0);
        lightningShader.SetUniform1i("texture2", 1);
        lightningShader.SetUniform1f("mixValue", mixValue);
        lightningShader.SetUniformMat4("view", view);
        lightningShader.SetUniformMat4("projection", proj);
        lightningShader.SetUniform3f("objectColor", {1.0f, 0.5f, 0.31f});
        lightningShader.SetUniform3f("lightColor", {1.0f, 1.0f, 1.0f});

        glm::mat4 model = glm::mat4(1.0f);
        lightningShader.SetUniformMat4("model", model);
        renderer.Draw(vao, lightningShader, 36);

        lightCubeShader.Bind();
        lightCubeShader.SetUniform1i("texture1", 0);
        lightCubeShader.SetUniform1i("texture2", 1);
        lightCubeShader.SetUniform1f("mixValue", mixValue);
        lightCubeShader.SetUniformMat4("view", view);
        lightCubeShader.SetUniformMat4("projection", proj);

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.5f)); 
        lightCubeShader.SetUniformMat4("model", model);

        renderer.Draw(lightVao, lightCubeShader, 36);

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
    lightVao.Delete();
    vbo.Delete();
    ibo.Delete();
    lightningShader.Delete();
    lightCubeShader.Delete();
    texture1.Delete();
    texture2.Delete();

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}
