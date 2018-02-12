#include "OpenGL3UniformBuffer.h"

OpenGL3UniformBuffer::OpenGL3UniformBuffer() 
	: m_uniformBufferPointer(0) {
	lock(true);
	unlock();
}

OpenGL3UniformBuffer::OpenGL3UniformBuffer(GLsizeiptr size)
	: m_uniformBufferPointer(0) {
	lock(true);
	setData(size, nullptr);
	unlock();
}

OpenGL3UniformBuffer::~OpenGL3UniformBuffer() {
	freeData();
}

void OpenGL3UniformBuffer::lock(bool replace) {
	if (replace) {
		freeData();

		glGenBuffers(1, &m_uniformBufferPointer);
	}

	glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferPointer);
}

void OpenGL3UniformBuffer::unlock() {
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OpenGL3UniformBuffer::setData(GLsizeiptr size, const void* data) {
	glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW);
}

void OpenGL3UniformBuffer::setDataPart(GLintptr offset, GLsizeiptr size, const void* data) {
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

GLuint OpenGL3UniformBuffer::getPointer() const {
	return m_uniformBufferPointer;
}

void OpenGL3UniformBuffer::freeData() {
	if (m_uniformBufferPointer)
		glDeleteBuffers(1, &m_uniformBufferPointer);
}