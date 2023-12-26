﻿#pragma once

#include <vector>
#include <glad/glad.h>
#include "Renderer.h"

#include <type_traits>
#include <stdexcept>

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
		static_assert(std::is_same<T, float>::value ||
						  std::is_same<T, unsigned int>::value ||
						  std::is_same<T, unsigned char>::value,
					  "Unsupported type");

		unsigned int type = 0;

		if (std::is_same<T, float>::value)
			type = GL_FLOAT;
		else if (std::is_same<T, unsigned int>::value)
			type = GL_UNSIGNED_INT;
		else if (std::is_same<T, unsigned char>::value)
			type = GL_UNSIGNED_BYTE;

		m_Elements.push_back({type, count, normalized});
		m_Stride += count * VertexBufferElement::GetSizeOfType(type);
	}

	inline const std::vector<VertexBufferElement> GetElements() const & { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};