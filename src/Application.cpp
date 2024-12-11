/*
    Created by Janis.
    2023
*/

#include "Renderer.h"
#ifdef _WIN32
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
#endif

#include "Includes.h"

int main(int argc, char **argv)
{
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

    // Create window
    // Create a windowed mode window and its OpenGL context
    // change monitor variable to monitor to get fullscreen resolution
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGl", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create a GLFW window!\n";
        glfwTerminate();
        return -1;
    }

    // init imgui
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
    glfwSetMouseButtonCallback(window, mouse_button_callback);

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

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }

    /* start coding */

    // GLCall(glEnable(GL_BLEND));
    // GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    GLCall(glEnable(GL_DEPTH_TEST));

    // Shaders
    Shader lightningShader("../res/shaders/lighting/shader.vert", "../res/shaders/lighting/shader.frag", "");
    Shader lightCubeShader("../res/shaders/lighting/light_cube.vert", "../res/shaders/lighting/light_cube.frag", "");
    Shader modelShader("../res/shaders/modeling/model.vert", "../res/shaders/modeling/model.frag", "");

    stbi_set_flip_vertically_on_load(1);
    Model backpack("../res/textures/backpack/backpack.obj");
    stbi_set_flip_vertically_on_load(0);

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
    Texture emissionMap("../res/textures/matrix.jpg", false,
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
    modelShader.Unbind();
    diffuseMap.Unbind();
    specularMap.Unbind();
    emissionMap.Unbind();
    
    // main loop
    while (!glfwWindowShouldClose(window))
    {
        // set light position
        float lightX = 2.0f * static_cast<float>(sin(glfwGetTime()));
        float lightY = 2.0f * static_cast<float>(sin(glfwGetTime()));
        float lightZ = 1.5f * static_cast<float>(cos(glfwGetTime()));

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

        // Model
        modelShader.Bind();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.0f, 0.0f, 7.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        modelShader.SetUniformMat4("view", view);
        modelShader.SetUniformMat4("projection", proj);
        modelShader.SetUniformMat4("model", model);

        modelShader.SetUniform3f("viewPos", camera.Position);

        modelShader.SetUniform3f("pointLight.position", pointLightPositions[0]);
        modelShader.SetUniform1f("pointLight.constant", 1.0f);
        modelShader.SetUniform1f("pointLight.linear", 0.09f);
        modelShader.SetUniform1f("pointLight.quadratic", 0.032f);
        modelShader.SetUniform3f("pointLight.ambient", (pointLightColors[0] * glm::vec3(0.1f) - 0.03f));
        modelShader.SetUniform3f("pointLight.diffuse", pointLightColors[0]);
        modelShader.SetUniform3f("pointLight.specular", {1.0f, 1.0f, 1.0f});

        modelShader.SetUniform3f("spotLight.position", camera.Position);
        modelShader.SetUniform3f("spotLight.direction", camera.Front);
        modelShader.SetUniform1f("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        modelShader.SetUniform1f("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
        modelShader.SetUniform1f("spotLight.constant", 1.0f);
        modelShader.SetUniform1f("spotLight.linear", 0.09f);
        modelShader.SetUniform1f("spotLight.quadratic", 0.032f);
        modelShader.SetUniform3f("spotLight.ambient", {0.0f, 0.0f, 0.0f});
        modelShader.SetUniform3f("spotLight.diffuse", {1.0f, 1.0f, 1.0f});
        modelShader.SetUniform3f("spotLight.specular", {1.0f, 1.0f, 1.0f});

        modelShader.SetUniform3f("dirLight.direction", {-0.2f, -1.0f, -0.3f});
        modelShader.SetUniform3f("dirLight.ambient", {0.05f, 0.05f, 0.05f});
        modelShader.SetUniform3f("dirLight.diffuse", {0.4f, 0.4f, 0.4f});
        modelShader.SetUniform3f("dirLight.specular", {0.5f, 0.5f, 0.5f});
        backpack.Draw(modelShader);

        // Cubes
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
        lightningShader.SetUniform1f("time", static_cast<float>(glfwGetTime()));

        lightningShader.SetUniform3f("viewPos", camera.Position);

        lightningShader.SetUniform1f("material.shininess", 64.0f);

        lightningShader.SetUniform3f("dirLight.direction", {-0.2f, -1.0f, -0.3f});
        lightningShader.SetUniform3f("dirLight.ambient", {0.05f, 0.05f, 0.05f});
        lightningShader.SetUniform3f("dirLight.diffuse", {0.4f, 0.4f, 0.4f});
        lightningShader.SetUniform3f("dirLight.specular", {0.5f, 0.5f, 0.5f});

        // point light
        for (GLuint i = 0; i < 4; i++)
        {
            std::string number = std::to_string(i);

            lightningShader.SetUniform3f("pointLights[" + number + "].position", pointLightPositions[i]);
            lightningShader.SetUniform3f("pointLights[" + number + "].ambient", (pointLightColors[i] * glm::vec3(0.1f) - 0.03f));
            lightningShader.SetUniform3f("pointLights[" + number + "].diffuse", pointLightColors[i]);
            lightningShader.SetUniform3f("pointLights[" + number + "].specular", {1.0f, 1.0f, 1.0f});
            lightningShader.SetUniform1f("pointLights[" + number + "].constant", 1.0f);
            lightningShader.SetUniform1f("pointLights[" + number + "].linear", 0.09f);
            lightningShader.SetUniform1f("pointLights[" + number + "].quadratic", 0.032f);
        }

        // spotLight
        lightningShader.SetUniform3f("spotLight.position", camera.Position);
        lightningShader.SetUniform3f("spotLight.direction", camera.Front);
        lightningShader.SetUniform3f("spotLight.ambient", {0.0f, 0.0f, 0.0f});
        lightningShader.SetUniform3f("spotLight.diffuse", {1.0f, 1.0f, 1.0f});
        lightningShader.SetUniform3f("spotLight.specular", {1.0f, 1.0f, 1.0f});
        lightningShader.SetUniform1f("spotLight.constant", 1.0f);
        lightningShader.SetUniform1f("spotLight.linear", 0.09f);
        lightningShader.SetUniform1f("spotLight.quadratic", 0.032f);
        lightningShader.SetUniform1f("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightningShader.SetUniform1f("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

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
        lightCubeShader.Bind();
        lightCubeShader.SetUniformMat4("projection", proj);
        lightCubeShader.SetUniformMat4("view", view);

        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lightCubeShader.SetUniformMat4("model", model);
            renderer.Draw(lightVao, lightCubeShader, GL_TRIANGLES, 36);
        }

        // draw ImGui
        renderer.OnImGuiRender();

        // render imgui
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

    glfwTerminate();

    return 0;
}
