#pragma once

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>


struct ShaderProgramSource
{
	std::string vertexShader;
	std::string fragmentShader;
	std::string geometryShader;
};

class Shader
{
private:
	unsigned int m_RendererID;
	mutable std::unordered_map<std::string, GLint> m_UniformLocationCache;

public:
	Shader(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource);
	~Shader();

	void Bind() const;
	void Unbind() const;
	void Delete() const;

	// Set uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform2f(const std::string& name, const glm::vec2& value);
	void SetUniform3f(const std::string& name, const glm::vec3& value);
	void SetUniform4f(const std::string& name, const glm::vec4& value);

	void SetUniformMat3(const std::string& name, const glm::mat3& matrix);
	void SetUniformMat4(const std::string& name, const glm::mat4& matrix);
	
private:
	ShaderProgramSource ParseShader(const std::string& vertexFile, const std::string& fragmentFile, const std::string& geometryFile);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader);

	GLint GetUniformLocation(const std::string& name) const;
};
