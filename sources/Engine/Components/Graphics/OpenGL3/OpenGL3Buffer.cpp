#include "OpenGL3Buffer.h"

#include <string>
#include "OpenGL3Errors.h"

OpenGL3Buffer::OpenGL3Buffer(Buffer::Type type, Buffer::Usage usage)
	: Buffer(type, usage), m_bufferPointer(0)
{
	if (usage == Buffer::Usage::StaticDraw)
		m_glUsage = GL_STATIC_DRAW;
	else if (usage == Buffer::Usage::StaticRead)
		m_glUsage = GL_STATIC_READ;
	else if (usage == Buffer::Usage::DynamicDraw)
		m_glUsage = GL_DYNAMIC_DRAW;
	else if (usage == Buffer::Usage::DynamicRead)
		m_glUsage = GL_DYNAMIC_READ;

	if (type == Buffer::Type::Index)
		m_glTarget = GL_ELEMENT_ARRAY_BUFFER;
	else if (type == Buffer::Type::Vertex)
		m_glTarget = GL_ARRAY_BUFFER;
}

OpenGL3Buffer::~OpenGL3Buffer()
{
	if (m_bufferPointer != 0)
		destroy();
}

void OpenGL3Buffer::create() 
{
	OPENGL3_CALL(glGenBuffers(1, &m_bufferPointer));
}

void OpenGL3Buffer::destroy()
{
	if (m_bufferPointer != 0) {
		OPENGL3_CALL(glDeleteBuffers(1, &m_bufferPointer));
		m_bufferPointer = 0;
	}
}

void OpenGL3Buffer::bind()
{
	OPENGL3_CALL(glBindBuffer(m_glTarget, m_bufferPointer));
}

void OpenGL3Buffer::unbind()
{
	OPENGL3_CALL(glBindBuffer(m_glTarget, 0));
}

void OpenGL3Buffer::allocateMemory(size_t size)
{
	m_size = size;
	OPENGL3_CALL(glBufferData(m_glTarget, size, 0, m_glUsage));
}

void OpenGL3Buffer::setData(size_t length, const std::byte* data)
{
	OPENGL3_CALL(glBufferData(m_glTarget, length, (data == nullptr) ? 0 : data, m_glUsage));
}

void OpenGL3Buffer::setData(size_t offset, size_t length, const std::byte* data)
{
	OPENGL3_CALL(glBufferSubData(m_glTarget, offset, length, data));
}

GLuint OpenGL3Buffer::getBufferPointer() const
{
	return m_bufferPointer;
}