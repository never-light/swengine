#pragma once

#include <Engine\Components\Graphics\RenderSystem\HardwareBuffer.h>
#include <Engine\types.h>
#include "OpenGL3.h"

class OpenGL3HardwareBuffer : public HardwareBuffer {
public:
	OpenGL3HardwareBuffer();
	~OpenGL3HardwareBuffer();

	void lock() override;
	void unlock() override;

	void setVertexFormat(VertexFormat format) override;
	VertexFormat getVertexFormat() const override;
	void addVertexAttribute(VertexAttributeDescription description) override;

	void setVerticesData(size_t count, size_t typeSize, const void* data) override;
	void setIndicesData(size_t count, size_t typeSize, const void* data) override;

	size_t getIndicesCount() const override;
	size_t getVerticesCount() const override;

	bool hasIndicesData() const override;

	GLuint getVertexArrayObjectPointer() const;
private:
	void freeData();

private:
	VertexFormat m_vertexFormat;
	std::vector<VertexAttributeDescription> m_vertexAttributes;

	size_t m_verticesCount;
	size_t m_indicesCount;
private:
	GLuint m_vertexArrayObject;
	GLuint m_vertexBufferObject;
	GLuint m_elementBufferObject;
};