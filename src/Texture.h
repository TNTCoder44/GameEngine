#pragma once

#include "Renderer.h"

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
	GLuint m_Type;

public:
	Texture(const std::string& path, bool flipVertically,GLuint type, GLuint format, GLuint drawMethod, 
			GLuint minFilter, GLuint magFilter, const int& desiredChannels);
	~Texture();

	void Bind(unsigned int slot) const;
	void Unbind() const;
	void Delete() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};