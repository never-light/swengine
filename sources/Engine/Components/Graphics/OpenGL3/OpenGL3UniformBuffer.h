#pragma once

#include "OpenGL3.h"

class OpenGL3UniformBuffer {
public:
	OpenGL3UniformBuffer();
	OpenGL3UniformBuffer(GLsizeiptr size);
	~OpenGL3UniformBuffer();

	void lock(bool replace = false);
	void unlock();

	void setData(GLsizeiptr size, const void* data = nullptr);
	void setDataPart(GLintptr offset, GLsizeiptr size, const void* data = nullptr);

	GLuint getPointer() const;

private:
	void freeData();

private:
	GLuint m_uniformBufferPointer;
};