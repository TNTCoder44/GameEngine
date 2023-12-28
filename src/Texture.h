#pragma once

#include "Renderer.h"

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP, m_DesiredChannels;

public:
	Texture(const std::string& path, bool flipVertically, GLuint drawMethod, 
			GLuint minFilter, GLuint magFilter, int desiredChannels);
	~Texture();

	void Bind(unsigned int slot) const;
	void Unbind() const;
	void Delete() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};