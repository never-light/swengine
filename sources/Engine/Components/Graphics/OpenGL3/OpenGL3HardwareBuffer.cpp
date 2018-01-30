#include "OpenGL3HardwareBuffer.h"

OpenGL3HardwareBuffer::OpenGL3HardwareBuffer() {

}

OpenGL3HardwareBuffer::OpenGL3HardwareBuffer(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices) 
{
	lock();

	m_vertices = vertices;
	m_indices = indices;

	unlock();
}

OpenGL3HardwareBuffer::~OpenGL3HardwareBuffer() {
	freeData();
}

void OpenGL3HardwareBuffer::freeData() {
	if (m_vertexArrayObject) {
		glDeleteVertexArrays(1, &m_vertexArrayObject);
	}

	if (m_vertexBufferObject) {
		glDeleteBuffers(1, &m_vertexBufferObject);
	}

	if (m_elementBufferObject) {
		glDeleteBuffers(1, &m_elementBufferObject);
	}
}

void OpenGL3HardwareBuffer::lock() {
	freeData();
}

void OpenGL3HardwareBuffer::unlock() {
	if (m_vertices.size() == 0) {
		freeData();
		return;
	}

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	glGenBuffers(1, &m_vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

	if (m_indices.size() > 0) {
		glGenBuffers(1, &m_elementBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(uint32), m_indices.data(), GL_STATIC_DRAW);
	}

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// vertex texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinates));

	glBindVertexArray(0);
}

void OpenGL3HardwareBuffer::setVertices(const std::vector<Vertex>& vertices) {
	m_vertices = vertices;
}

const std::vector<Vertex>& OpenGL3HardwareBuffer::getVertices() {
	return m_vertices;
}

void OpenGL3HardwareBuffer::setIndices(const std::vector<size_t>& indices) {
	m_indices = indices;
}

const std::vector<size_t>& OpenGL3HardwareBuffer::getIndices() {
	return m_indices;
}

void OpenGL3HardwareBuffer::addVertex(const Vertex& vertex) {
	m_vertices.push_back(vertex);
}

void OpenGL3HardwareBuffer::addIndex(size_t index) {
	m_indices.push_back(index);
}

size_t OpenGL3HardwareBuffer::getVerticesCount() const {
	return m_vertices.size();
}

size_t OpenGL3HardwareBuffer::getIndicesCount() const {
	return m_indices.size();
}

GLuint OpenGL3HardwareBuffer::getVertexArrayObjectPointer() const {
	return m_vertexArrayObject;
}