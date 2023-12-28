/*
    Created by Janis.
    2023
*/

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

    // initialize ImGui
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();

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

    // error handling
    glfwSetErrorCallback([](int error, const char *description)
                         { fprintf(stderr, "GLFW Error %d: %s\n", error, description); });

    // handle resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // handle mouse input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    // glfwSetMouseButtonCallback(window, mouse_button_callback);

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
    VertexBufferLayout CubeLayout;
    VertexBufferLayout LightCubeLayout;

    vao.Bind();
    vbo.Bind();
    ibo.Bind();

    // Textures
    Texture diffuseMap("../res/textures/container2.png", false,
                    GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 0);
    Texture specularMap("../res/textures/container2_specular.png", false,
                    GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 0);

    diffuseMap.Bind(0);
    specularMap.Bind(1);

    // Positions
    CubeLayout.Push<float>(3, GL_FALSE);
    // Colors
    CubeLayout.Push<float>(3, GL_FALSE);
    // Textures
    CubeLayout.Push<float>(2, GL_FALSE);
    // Normal Vectors
    CubeLayout.Push<float>(3, GL_FALSE);

    vao.AddBuffer(vbo, CubeLayout);

    vao.Unbind();
    vbo.Unbind();

    VertexArray lightVao;
    VertexBuffer lightVbo(LightCubeVertices, sizeof(LightCubeVertices));
    lightVao.Bind();
    lightVbo.Bind();

    // Positions
    LightCubeLayout.Push<float>(3, GL_FALSE);

    lightVao.AddBuffer(lightVbo, LightCubeLayout);

    Renderer renderer;

    // unbind everything
    lightVao.Unbind();
    lightVbo.Unbind();
    ibo.Unbind();
    lightningShader.Unbind();
    lightCubeShader.Unbind();
    diffuseMap.Unbind();
    specularMap.Unbind();

    // main loop
    while (!glfwWindowShouldClose(window))
    {
        // set light position
        float lightX = 2.0f * sin(glfwGetTime());
        float lightY = 2.0f * sin(glfwGetTime());
        float lightZ = 1.5f * cos(glfwGetTime());
        glm::vec3 lightPos = glm::vec3(lightX, lightY, lightZ);

        // calculate the light color
        glm::vec3 lightColor = glm::vec3(1.0f);

        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

        // calculate time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // check for input
        renderer.processInput(window);

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
        diffuseMap.Bind(0);
        specularMap.Bind(1);
        lightningShader.Bind();
        lightningShader.SetUniform1i("material.diffuse", 0);
        lightningShader.SetUniform1i("material.specular", 1);
        lightningShader.SetUniform1f("mixValue", mixValue);

        lightningShader.SetUniformMat4("view", view);
        lightningShader.SetUniformMat4("projection", proj);

        lightningShader.SetUniform3f("viewPos", camera.Position);

        lightningShader.SetUniform1f("material.shininess", 64.0f);

        lightningShader.SetUniform3f("light.position", lightPos);
        lightningShader.SetUniform3f("light.ambient", ambientColor);
        lightningShader.SetUniform3f("light.diffuse", diffuseColor);
        lightningShader.SetUniform3f("light.specular", {1.0f, 1.0f, 1.0f});

        glm::mat4 model = glm::mat4(1.0f);
        lightningShader.SetUniformMat4("model", model);
        renderer.Draw(vao, lightningShader, GL_TRIANGLES, 36);

        // Lamp
        lightCubeShader.Bind();
        lightCubeShader.SetUniformMat4("view", view);
        lightCubeShader.SetUniformMat4("projection", proj);

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightCubeShader.SetUniformMat4("model", model);

        renderer.Draw(lightVao, lightCubeShader, GL_TRIANGLES, 36);

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
    lightVbo.Delete();
    ibo.Delete();
    lightningShader.Delete();
    lightCubeShader.Delete();
    diffuseMap.Delete();
    specularMap.Delete();

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}
