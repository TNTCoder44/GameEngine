/*
    Created by Janis.
    2023
*/

#ifdef _WIN32
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
#endif

#ifdef __APPLE__
#define OS_MACOS
#elif __linux__
#define OS_LINUX
#elif _WIN32
#define OS_WINDOWS
#endif

#include "Includes.h"

int main(int argc, char **argv)
{
    /* Initialize GLFW */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
#ifdef OS_MACOS
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // find primary monitor
    auto *monitor = glfwGetPrimaryMonitor();

    // Create window
    // Create a windowed mode window and its OpenGL context 
    // change monitor variable to monitor to get fullscreen resolution
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGl", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // initialize ImGui
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();

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
    Texture emissionMap("../res/textures/mat.png", false,
                        GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 0);

    diffuseMap.Bind(0);
    specularMap.Bind(1);
    emissionMap.Bind(2);

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
    emissionMap.Unbind();

    // main loop
    while (!glfwWindowShouldClose(window))
    {
        // set light position
        float lightX = 2.0f * sin(glfwGetTime());
        float lightY = 2.0f * sin(glfwGetTime());
        float lightZ = 1.5f * cos(glfwGetTime());

        glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

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

        // model matrix
        glm::mat4 model = glm::mat4(1.0f);

        // render screen
        diffuseMap.Bind(0);
        specularMap.Bind(1);
        emissionMap.Bind(2);
        lightningShader.Bind();
        lightningShader.SetUniform1i("material.diffuse", 0);
        lightningShader.SetUniform1i("material.specular", 1);
        lightningShader.SetUniform1i("material.emission", 2);
        lightningShader.SetUniform1f("mixValue", mixValue);

        lightningShader.SetUniformMat4("view", view);
        lightningShader.SetUniformMat4("projection", proj);

        lightningShader.SetUniform3f("viewPos", camera.Position);

        lightningShader.SetUniform1f("material.shininess", 64.0f);

        lightningShader.SetUniform3f("light.position", camera.Position);
        lightningShader.SetUniform3f("light.direction", camera.Front);
        lightningShader.SetUniform1f("light.cutOff", glm::cos(glm::radians(12.5f)));
        lightningShader.SetUniform1f("light.outerCutOff", glm::cos(glm::radians(17.5f)));
        lightningShader.SetUniform3f("light.ambient", ambientColor);
        lightningShader.SetUniform3f("light.diffuse", diffuseColor);
        lightningShader.SetUniform3f("light.specular", {1.0f, 1.0f, 1.0f});
        lightningShader.SetUniform1f("light.constant", 1.0f);
        lightningShader.SetUniform1f("light.linear", 0.09f);
        lightningShader.SetUniform1f("light.quadratic", 0.032f);

        for (unsigned int i = 0; i < 10; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightningShader.SetUniformMat4("model", model);

            renderer.Draw(vao, lightningShader, GL_TRIANGLES, 36);
        }

        // Lamp
        /*
        lightCubeShader.Bind();
        lightCubeShader.SetUniformMat4("projection", proj);
        lightCubeShader.SetUniformMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightCubeShader.SetUniformMat4("model", model);
        */

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
    emissionMap.Delete();

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}
