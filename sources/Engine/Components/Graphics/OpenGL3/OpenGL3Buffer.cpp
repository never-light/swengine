#include "OpenGL3Buffer.h"

#include <string>
#include <Engine\Components\Debugging\Log.h>

OpenGL3Buffer::OpenGL3Buffer(Buffer::Type type)
	: Buffer(type), m_bufferPointer(0)
{

}

OpenGL3Buffer::~OpenGL3Buffer()
{
	if (m_bufferPointer != 0)
		destroy();
}

void OpenGL3Buffer::create() 
{
	glGenBuffers(1, &m_bufferPointer);
}

void OpenGL3Buffer::destroy()
{
	glDeleteBuffers(1, &m_bufferPointer);
	m_bufferPointer = 0;
}

void OpenGL3Buffer::bind()
{
	GLenum target = (m_type == Type::Vertex) ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER;
	glBindBuffer(target, m_bufferPointer);
}

void OpenGL3Buffer::unbind()
{
	GLenum target = (m_type == Type::Vertex) ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER;
	glBindBuffer(target, 0);
}

void OpenGL3Buffer::setData(size_t length, const std::byte* data)
{
	GLenum target = (m_type == Type::Vertex) ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER;
	glBufferData(target, length, (data == nullptr) ? 0 : data, GL_STATIC_DRAW);
}

void OpenGL3Buffer::setData(size_t offset, size_t length, const std::byte* data)
{
	GLenum target = (m_type == Type::Vertex) ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER;
	glBufferSubData(target, offset, length, data);
}

GLuint OpenGL3Buffer::getBufferPointer() const
{
	return m_bufferPointer;
}
