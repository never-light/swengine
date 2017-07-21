#pragma once

#include <vector>
#include <Engine\types.h>
#include <Engine\Components\Graphics\Vertex.h>

#include "OpenGL3.h"

class OpenGL3Mesh {
public:
	OpenGL3Mesh();
	OpenGL3Mesh(const std::vector<Vertex>&);
	OpenGL3Mesh(const std::vector<Vertex>&, const std::vector<uint32>&);
	virtual ~OpenGL3Mesh();

	void addVertex(const Vertex&);
	void addIndex(uint32);
	void updateBuffers();

	GLuint getVertexArrayObjectPointer() const;
	size_t getVerticesCount() const;
	size_t getIndicesCount() const;
protected:
	std::vector<Vertex> m_vertices;
	std::vector<uint32> m_indices;

private:
	GLuint m_vertexArrayObject;
	GLuint m_vertexBufferObject;
	GLuint m_elementBufferObject;
};