#include "Texture.h"

#include <stb/stb_image.h>

Texture::Texture(const std::string &path, bool flipVertically, GLuint drawMethod,
				 GLuint minFilter, GLuint magFilter, int desiredChannels)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr),
	  m_Width(0), m_Height(0), m_BPP(),
	  m_DesiredChannels(desiredChannels)
{
	if (flipVertically)
	{
		stbi_set_flip_vertically_on_load(1);
	}

	m_LocalBuffer = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_BPP, m_DesiredChannels);

	GLenum format;
	if (m_BPP == 1)
		format = GL_RED;
	else if (m_BPP == 3)
		format = GL_RGB;
	else if (m_BPP == 4)
		format = GL_RGBA;

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, drawMethod));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, drawMethod));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, m_LocalBuffer));
	
	glGenerateMipmap(GL_TEXTURE_2D);

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
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::Delete() const
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}
