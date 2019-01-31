#include "GLBuffer.h"

#include <string>
#include "GLDebug.h"

GLBuffer::GLBuffer(Type type, Usage usage)
	: m_type(type), m_usage(usage), m_bufferPointer(0)
{
	if (usage == Usage::StaticDraw)
		m_glUsage = GL_STATIC_DRAW;
	else if (usage == Usage::StaticRead)
		m_glUsage = GL_STATIC_READ;
	else if (usage == Usage::DynamicDraw)
		m_glUsage = GL_DYNAMIC_DRAW;
	else if (usage == Usage::DynamicRead)
		m_glUsage = GL_DYNAMIC_READ;

	if (type == Type::Index)
		m_glTarget = GL_ELEMENT_ARRAY_BUFFER;
	else if (type == Type::Vertex)
		m_glTarget = GL_ARRAY_BUFFER;
}

GLBuffer::~GLBuffer()
{
	if (m_bufferPointer != 0)
		destroy();
}

void GLBuffer::create()
{
	GL_CALL(glGenBuffers(1, &m_bufferPointer));
}

void GLBuffer::destroy()
{
	if (m_bufferPointer != 0) {
		GL_CALL(glDeleteBuffers(1, &m_bufferPointer));
		m_bufferPointer = 0;
	}
}

void GLBuffer::bind()
{
	GL_CALL(glBindBuffer(m_glTarget, m_bufferPointer));
}

void GLBuffer::unbind()
{
	GL_CALL(glBindBuffer(m_glTarget, 0));
}

void GLBuffer::allocateMemory(size_t size)
{
	m_size = size;
	GL_CALL(glBufferData(m_glTarget, size, 0, m_glUsage));
}

void GLBuffer::setData(size_t length, const std::byte* data)
{
	GL_CALL(glBufferData(m_glTarget, length, (data == nullptr) ? 0 : data, m_glUsage));
}

void GLBuffer::setData(size_t offset, size_t length, const std::byte* data)
{
	GL_CALL(glBufferSubData(m_glTarget, offset, length, data));
}

GLuint GLBuffer::getRawPointer() const
{
	return m_bufferPointer;
}