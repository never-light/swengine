#pragma once

#include <Engine\Components\Graphics\RenderSystem\HardwareBuffer.h>
#include <Engine\types.h>
#include "OpenGL3.h"

class OpenGL3HardwareBuffer : public HardwareBuffer {
public:
	OpenGL3HardwareBuffer();
	OpenGL3HardwareBuffer(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices);

	~OpenGL3HardwareBuffer();

	void lock() override;
	void unlock() override;

	void setVertices(const std::vector<Vertex>& vertices) override;
	const std::vector<Vertex>& getVertices() override;

	void setIndices(const std::vector<size_t>& indices) override;
	const std::vector<size_t>& getIndices() override;

	void addVertex(const Vertex& vertex) override;
	void addIndex(size_t index) override;

	size_t getVerticesCount() const override;
	size_t getIndicesCount() const override;

	GLuint getVertexArrayObjectPointer() const;
private:
	void freeData();

private:
	std::vector<Vertex> m_vertices;
	std::vector<size_t> m_indices;

private:
	GLuint m_vertexArrayObject;
	GLuint m_vertexBufferObject;
	GLuint m_elementBufferObject;
};