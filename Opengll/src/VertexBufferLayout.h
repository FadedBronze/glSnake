#pragma once
#include <vector>
#include <GL/glew.h>
#include <iostream>
#include "Renderer.h"
#include <stdexcept>

struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	bool normalized;

	static unsigned int GetSizeOfType(unsigned int type) {
		unsigned int size;
		
		switch (type)
		{
		case GL_BYTE:
			size = 1;
			break;
		case GL_UNSIGNED_INT:
			size = 4;
			break;
		case GL_FLOAT:
			size = 4;
			break;
		default:
			std::cout << "INVALID DATA TYPE ENTERED" << std::endl;
			ASSERT(false);
			size = 0;
			break;
		}

		return size;
	}
};

class VertexBufferLayout {
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout()
	: m_Stride(0) {}

	template<typename T>
	void Push(unsigned int count) {
		std::runtime_error(false);
	}

	template<>
	void Push<float>(unsigned int count) {
		VertexBufferElement vbe = { GL_FLOAT, count, false };
		m_Elements.push_back(vbe);
		m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void Push<unsigned int>(unsigned int count) {
		VertexBufferElement vbe = { GL_UNSIGNED_INT, count, false };
		m_Elements.push_back(vbe);
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<unsigned char>(unsigned int count) {
		VertexBufferElement vbe = { GL_BYTE, count, true };
		m_Elements.push_back(vbe);
		m_Stride += VertexBufferElement::GetSizeOfType(GL_BYTE) * count;
	}

	inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
	inline const unsigned int GetStride() const { return m_Stride; }
};