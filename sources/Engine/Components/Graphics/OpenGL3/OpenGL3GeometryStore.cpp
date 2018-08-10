#include "OpenGL3GeometryStore.h"

#include "OpenGL3Errors.h"

OpenGL3GeometryStore::OpenGL3GeometryStore()
	: GeometryStore(), m_VAO(0)
{
}

OpenGL3GeometryStore::~OpenGL3GeometryStore()
{
	if (m_VAO != 0)
		destroy();
}

OpenGL3GeometryStore::BufferId OpenGL3GeometryStore::requireBuffer(BufferType bufferType, BufferUsage bufferUsage, size_t size)
{
	Buffer::Type glBufferType = (bufferType == BufferType::Vertex) ? Buffer::Type::Vertex : Buffer::Type::Index;
	OpenGL3Buffer* buffer = new OpenGL3Buffer(glBufferType, bufferUsage);

	buffer->create();
	buffer->bind();
	buffer->allocateMemory(size);
	
	m_buffers.push_back(buffer);

	return m_buffers.size() - 1;
}

void OpenGL3GeometryStore::setBufferData(BufferId bufferId, size_t offset, size_t length, const std::byte * data)
{
	m_buffers[bufferId]->bind();
	m_buffers[bufferId]->setData(offset, length, data);
}

void OpenGL3GeometryStore::setVertexLayoutAttribute(size_t index, BufferId bufferId, size_t size, VertexLayoutAttributeBaseType type, bool shouldNormalize, size_t stride, size_t offset)
{
	m_vertexLayoutDescription.push_back(VertexLayoutAttribute{ index, bufferId, size, type, shouldNormalize, stride, offset });
}

void OpenGL3GeometryStore::create()
{
	OPENGL3_CALL_BLOCK_BEGIN();
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
	OPENGL3_CALL_BLOCK_END();

	OPENGL3_CALL_BLOCK_BEGIN();

	for (const auto& attribute : m_vertexLayoutDescription) {
		m_buffers[attribute.bufferId]->bind();

		GLboolean shouldNormalize = attribute.shouldNormalize ? GL_TRUE : GL_FALSE;
		GLenum baseType;

		switch (attribute.type) {
		case VertexLayoutAttributeBaseType::Float:
			baseType = GL_FLOAT;
			break;

		case VertexLayoutAttributeBaseType::UnsignedByte:
			baseType = GL_UNSIGNED_BYTE;
			break;

		case VertexLayoutAttributeBaseType::UnsignedShort:
			baseType = GL_UNSIGNED_SHORT;
			break;

		case VertexLayoutAttributeBaseType::UnsignedInt:
			baseType = GL_UNSIGNED_INT;
			break;

		case VertexLayoutAttributeBaseType::Int:
			baseType = GL_INT;
			break;

		}

		glEnableVertexAttribArray(attribute.index);

		if (baseType == GL_FLOAT) {
			glVertexAttribPointer(attribute.index, attribute.size, baseType, shouldNormalize, attribute.stride, (GLvoid*)attribute.offset);
		}
		else {
			glVertexAttribIPointer(attribute.index, attribute.size, baseType, attribute.stride, (GLvoid*)attribute.offset);
		}
	}

	OPENGL3_CALL_BLOCK_END();

	for (OpenGL3Buffer* buffer : m_buffers)
		buffer->bind();

	glBindVertexArray(0);
}

void OpenGL3GeometryStore::destroy()
{
	if (m_VAO != 0) {
		for (OpenGL3Buffer* buffer : m_buffers)
			delete buffer;

		m_buffers.clear();
		m_vertexLayoutDescription.clear();

		OPENGL3_CALL(glDeleteVertexArrays(1, &m_VAO));
		m_VAO = 0;
	}
}

void OpenGL3GeometryStore::bind()
{
	OPENGL3_CALL(glBindVertexArray(m_VAO));
}

void OpenGL3GeometryStore::unbind()
{
	OPENGL3_CALL(glBindVertexArray(0));
}

void OpenGL3GeometryStore::drawArrays(DrawType drawType, size_t offset, size_t count) {
	GLenum drawMode;

	if (drawType == DrawType::Triangles)
		drawMode = GL_TRIANGLES;
	
	OPENGL3_CALL(glDrawArrays(drawMode, offset, count));
}

void OpenGL3GeometryStore::drawElements(DrawType drawType, size_t offset, size_t count, IndicesType indicesType) {
	GLenum drawMode;

	if (drawType == DrawType::Triangles)
		drawMode = GL_TRIANGLES;

	GLenum glIndicesType;
	size_t offsetMultiplier;

	if (indicesType == IndicesType::UnsignedInt) {
		glIndicesType = GL_UNSIGNED_INT;
		offsetMultiplier = sizeof(unsigned int);
	}

	OPENGL3_CALL(glDrawElements(drawMode, count, glIndicesType, (const void*)(offset * offsetMultiplier)));
}