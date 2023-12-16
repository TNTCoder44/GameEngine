#include "Shader.h"
#include "Renderer.h"

#ifdef _WIN32
#include <malloc.h>
#endif

#ifdef __APPLE__
#include <malloc/malloc.h>
#endif

#ifdef __linux__
#include <malloc/malloc.h>
#endif

Shader::Shader(const std::string &vertexSource, const std::string &fragmentSource, const std::string &geometrySource)
    : m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(vertexSource, fragmentSource, geometrySource);
    m_RendererID = CreateShader(source.vertexShader, source.fragmentShader, source.geometryShader);
}

Shader::~Shader()
{
}

ShaderProgramSource Shader::ParseShader(const std::string &vertexFile, const std::string &fragmentFile, const std::string &geometryFile)
{
    std::ifstream vertStream(vertexFile);
    std::ifstream fragStream(fragmentFile);

    if (!vertStream.is_open())
    {
        throw std::runtime_error("Could not open file: " + std::string(vertexFile));
    }

    if (!fragStream.is_open())
    {
        throw std::runtime_error("Could not open file: " + std::string(fragmentFile));
    }

    std::ostringstream vertOStream;
    std::ostringstream fragOStream;
    std::ostringstream geomOStream;

    vertOStream << vertStream.rdbuf();
    fragOStream << fragStream.rdbuf();

    if (!geometryFile.empty())
    {
        std::ifstream geometryStream(geometryFile);

        if (!geometryStream.is_open())
        {
            throw std::runtime_error("Could not open file: " + std::string(geometryFile));
        }
        geomOStream << geometryStream.rdbuf();
        geometryStream.close();
    }

    vertStream.close();
    fragStream.close();

    return {vertOStream.str(), fragOStream.str(), geomOStream.str()};
}

unsigned int Shader::CompileShader(unsigned int type, const std::string &source)
{
    GLCall(unsigned int id = glCreateShader(type));
    const char *src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char *message = static_cast<char *>(alloca(length * sizeof(char)));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!";
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));

        return 1;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader, const std::string &geometryShader)
{
    GLCall(unsigned int program = glCreateProgram());
    GLCall(unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader));
    GLCall(unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader));

    if (!geometryShader.empty())
    {
        GLCall(unsigned int gs = CompileShader(GL_GEOMETRY_SHADER, geometryShader));
        GLCall(glAttachShader(program, gs));
    }

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    int success;
    char infoLog[512];

    GLCall(glGetProgramiv(program, GL_LINK_STATUS, &success));
    if (success == GL_FALSE)
    {
        GLCall(glGetProgramInfoLog(program, 512, NULL, infoLog));
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::Delete() const
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::SetUniform1i(const std::string &name, int value)
{
    // GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string &name, float value)
{
    // GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform2f(const std::string &name, const glm::vec2 &value)
{
    GLCall(glUniform2f(GetUniformLocation(name), value.x, value.y));
}

void Shader::SetUniform3f(const std::string &name, const glm::vec3 &value)
{
    GLCall(glUniform3f(GetUniformLocation(name), value.x, value.y, value.z));
}

void Shader::SetUniform4f(const std::string &name, const glm::vec4 &value)
{
    GLCall(glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w));
}

void Shader::SetUniformMat3(const std::string &name, const glm::mat3 &matrix)
{
    GLCall(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniformMat4(const std::string &name, const glm::mat4 &matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

GLint Shader::GetUniformLocation(const std::string &name) const
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    {
        return m_UniformLocationCache[name];
    }

    GLCall(GLint location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
    {
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    }

    m_UniformLocationCache[name] = location;

    return location;
}
