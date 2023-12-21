﻿#pragma once

#include <vector>
#include <glad/glad.h>
#include "Renderer.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:
			return 4;
		case GL_UNSIGNED_INT:
			return 4;
		case GL_UNSIGNED_BYTE:
			return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;

public:
	VertexBufferLayout()
		: m_Stride(0) {}

	template <typename T>
	void Push(unsigned int count, GLboolean normalized)
	{
		ASSERT(false);
	}

	template <>
	void Push<float>(unsigned int count, GLboolean normalized)
	{
		m_Elements.push_back({GL_FLOAT, count, normalized});
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template <>
	void Push<unsigned int>(unsigned int count, GLboolean normalized)
	{
		m_Elements.push_back({GL_UNSIGNED_INT, count, normalized});
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template <>
	void Push<unsigned char>(unsigned int count, GLboolean normalized)
	{
		m_Elements.push_back({GL_UNSIGNED_BYTE, count, normalized});
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement> GetElements() const & { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};
