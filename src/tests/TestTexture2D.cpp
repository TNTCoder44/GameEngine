#include "TestTexture2D.h"

namespace test
{
	TestTexture2D::TestTexture2D()
		:
		m_VertexBufferLayout(), m_Renderer(),
		m_Proj(glm::ortho(0.f, 960.f, 0.f, 540.f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0)
	{
		float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.0f,    // 0
			 50.0f, -50.0f, 1.0f, 0.0f,    // 1
			 50.0f,  50.0f, 1.0f, 1.0f,    // 2
			-50.0f,  50.0f, 0.0f, 1.0f     // 3
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));

		m_VAO = std::make_unique<VertexArray>();

		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));

		m_VertexBufferLayout.Push<float>(2, GL_FALSE);
		m_VertexBufferLayout.Push<float>(2, GL_FALSE);

		m_VAO->AddBuffer(*m_VertexBuffer, m_VertexBufferLayout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

		m_Shader = std::make_unique<Shader>("../res/shaders/shader.vert", "../res/shaders/shader.fert", "");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", glm::vec4(0.8f, 0.3f, 0.8f, 1.0f));

		m_Texture = std::make_unique<Texture>("../res/textures/image.png", true, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, 4);
		m_Shader->SetUniform1i("u_Texture", 0);
	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{
		
	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		m_Texture->Bind(0);

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4("u_MVP", mvp);

			m_Renderer.Draw(*m_VAO, *m_IndexBuffer, GL_TRIANGLES, *m_Shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4("u_MVP", mvp);

			m_Renderer.Draw(*m_VAO, *m_IndexBuffer, GL_TRIANGLES, *m_Shader);
		}

	}

	void TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
