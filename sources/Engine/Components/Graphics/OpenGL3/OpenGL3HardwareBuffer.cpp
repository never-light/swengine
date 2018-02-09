#include "OpenGL3HardwareBuffer.h"
#include <Engine\Components\Debugging\Log.h>

OpenGL3HardwareBuffer::OpenGL3HardwareBuffer() 
	: m_verticesCount(0), m_indicesCount(0) {

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

	m_verticesCount = 0;
	m_indicesCount = 0;
}

void OpenGL3HardwareBuffer::lock() {
	freeData();

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);
}

void OpenGL3HardwareBuffer::setVerticesData(size_t count, size_t typeSize, const void* data) {
	m_verticesCount = count;

	glGenBuffers(1, &m_vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, count * typeSize, data, GL_STATIC_DRAW);
}

void OpenGL3HardwareBuffer::setIndicesData(size_t count, size_t typeSize, const void* data) {
	m_indicesCount = count;

	glGenBuffers(1, &m_elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * typeSize, data, GL_STATIC_DRAW);
}
	
void OpenGL3HardwareBuffer::unlock() {
	for (size_t i = 0; i < m_vertexAttributes.size(); i++) {
		VertexAttributeDescription& description = m_vertexAttributes[i];

		GLenum vertexAttributeType;

		if (description.type == VertexAttributeValueType::Real)
			vertexAttributeType = GL_FLOAT;

		GLboolean normalized = (description.normalized) ? GL_TRUE : GL_FALSE;

		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, description.size, vertexAttributeType, normalized, description.stride, (void*)description.offset);
	}

	glBindVertexArray(0);
}

void OpenGL3HardwareBuffer::setVertexFormat(VertexFormat format) {
	m_vertexFormat = format;

	m_vertexAttributes.clear();

	if (format == VertexFormat::P1) {
		// Position
		m_vertexAttributes.push_back(VertexAttributeDescription(
			3, VertexAttributeValueType::Real, false, sizeof(VertexP1), 0
		));
	}
	else if (format == VertexFormat::P1UV) {
		// Position
		m_vertexAttributes.push_back(VertexAttributeDescription(
			3, VertexAttributeValueType::Real, false, sizeof(VertexP1UV), 0
		));
		// UV
		m_vertexAttributes.push_back(VertexAttributeDescription(
			2, VertexAttributeValueType::Real, false, sizeof(VertexP1UV), offsetof(VertexP1UV, uv)
		));
	}
	else if (format == VertexFormat::P1N1UV) {
		// Position
		m_vertexAttributes.push_back(VertexAttributeDescription(
			3, VertexAttributeValueType::Real, false, sizeof(VertexP1N1UV), 0
		));
		// UV
		m_vertexAttributes.push_back(VertexAttributeDescription(
			3, VertexAttributeValueType::Real, false, sizeof(VertexP1N1UV), offsetof(VertexP1N1UV, normal)
		));
		// Normal
		m_vertexAttributes.push_back(VertexAttributeDescription(
			2, VertexAttributeValueType::Real, false, sizeof(VertexP1N1UV), offsetof(VertexP1N1UV, uv)
		));
	}
	else if (format == VertexFormat::P1N1UVT1) {
		// Position
		m_vertexAttributes.push_back(VertexAttributeDescription(
			3, VertexAttributeValueType::Real, false, sizeof(VertexP1N1UVT1), 0
		));
		// UV
		m_vertexAttributes.push_back(VertexAttributeDescription(
			3, VertexAttributeValueType::Real, false, sizeof(VertexP1N1UVT1), offsetof(VertexP1N1UVT1, normal)
		));
		// Normal
		m_vertexAttributes.push_back(VertexAttributeDescription(
			2, VertexAttributeValueType::Real, false, sizeof(VertexP1N1UVT1), offsetof(VertexP1N1UVT1, uv)
		));
		// Tangent
		m_vertexAttributes.push_back(VertexAttributeDescription(
			3, VertexAttributeValueType::Real, false, sizeof(VertexP1N1UVT1), offsetof(VertexP1N1UVT1, tangent)
		));
	}
}

VertexFormat OpenGL3HardwareBuffer::getVertexFormat() const {
	return m_vertexFormat;
}

void OpenGL3HardwareBuffer::addVertexAttribute(VertexAttributeDescription description) {
	m_vertexAttributes.push_back(description);
}

size_t OpenGL3HardwareBuffer::getIndicesCount() const {
	return m_indicesCount;
}

size_t OpenGL3HardwareBuffer::getVerticesCount() const {
	return m_verticesCount;
}

bool OpenGL3HardwareBuffer::hasIndicesData() const {
	return m_indicesCount != 0;
}

GLuint OpenGL3HardwareBuffer::getVertexArrayObjectPointer() const {
	return m_vertexArrayObject;
}