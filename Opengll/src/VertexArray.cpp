#include "VertexArray.h"
#include "Renderer.h"
#include <iostream>

VertexArray::VertexArray()
{
	GL_CALL(glCreateVertexArrays(1, &m_RendererID));
	GL_CALL(glBindVertexArray(m_RendererID));
}

VertexArray::~VertexArray()
{
	GL_CALL(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	vb.Bind();
	const std::vector<VertexBufferElement>& elements = layout.GetElements();
	unsigned int offset = 0;

	//the index is the input id of the shader. When you write layout = 0 its taking the 0th index.
	//the count is how many vars of that type
	//the type is the webgl type GLuint GLfloat GLbyte
	//normalised is if you want it normalised
	//the stride is the amount of bytes between each vertex
	//the offset is the bytes between the start of the vertex to that attribute
	for (unsigned int i = 0; i < elements.size(); i++) {
		const VertexBufferElement& element = elements[i];

		GL_CALL(glEnableVertexAttribArray(i));
		GL_CALL(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));

		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	GL_CALL(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GL_CALL(glBindVertexArray(0));
}
