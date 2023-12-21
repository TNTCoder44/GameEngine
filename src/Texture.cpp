#include "Texture.h"

#include <stb/stb_image.h>

Texture::Texture(const std::string &path, bool flipVertically, GLuint type, GLuint format, GLuint drawMethod,
				 GLuint minFilter, GLuint magFilter, const int &desiredChannels)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr),
	  m_Width(0), m_Height(0), m_BPP(0), m_Type(type)
{
	if (flipVertically)
	{
		stbi_set_flip_vertically_on_load(1);
	}

	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, desiredChannels);

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(type, m_RendererID));

	GLCall(glTexParameteri(type, GL_TEXTURE_MIN_FILTER, minFilter));
	GLCall(glTexParameteri(type, GL_TEXTURE_MAG_FILTER, magFilter));
	GLCall(glTexParameteri(type, GL_TEXTURE_WRAP_S, drawMethod));
	
	GLCall(glTexParameteri(type, GL_TEXTURE_WRAP_T, drawMethod));

	if (type == GL_TEXTURE_2D)
	{
		GLCall(glTexImage2D(type, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, m_LocalBuffer));
	}

	glGenerateMipmap(type);

	if (m_LocalBuffer)
	{
		stbi_image_free(m_LocalBuffer);
	}
	else
	{
		std::cout << "\nError: Failed to load texture" << std::endl;
		std::cout << stbi_failure_reason() << std::endl;
	}
}

Texture::~Texture()
{
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(m_Type, m_RendererID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(m_Type, 0));
}

void Texture::Delete() const
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}
