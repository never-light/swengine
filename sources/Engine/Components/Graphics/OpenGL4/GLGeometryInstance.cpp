#include "GLGeometryInstance.h"

#include "GLDebug.h"

GLGeometryInstance::GLGeometryInstance()
	: GeometryInstance(), 
	m_VAO(0),
	m_vertexBuffer(nullptr),
	m_indexBuffer(nullptr),
	m_verticesDataSize(0),
	m_indicesDataSize(0),
	m_indexed(false),
	m_verticesCount(0),
	m_indicesCount(0)
{
	m_vertexLayoutDescription.resize(16, std::optional<GeometryAttributeDesc>());
}

GLGeometryInstance::~GLGeometryInstance()
{
	if (m_VAO != 0)
		destroy();
}

GLBuffer* GLGeometryInstance::createBuffer(GLBuffer::Type bufferType, GLBuffer::Usage bufferUsage, size_t capacity)
{
	GLBuffer* buffer = new GLBuffer(bufferType, bufferUsage);

	buffer->create();
	buffer->bind();
	buffer->allocateMemory(capacity);

	return buffer;
}

void GLGeometryInstance::create()
{
	GL_CALL_BLOCK_BEGIN();
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
	GL_CALL_BLOCK_END();

	GL_CALL_BLOCK_BEGIN();

	if (m_indexBuffer != nullptr)
		m_indexBuffer->bind();
	
	m_vertexBuffer->bind();

	for (int attrIndex = 0; attrIndex < m_vertexLayoutDescription.size(); attrIndex++) {
		if (!m_vertexLayoutDescription[attrIndex].has_value())
			continue;

		const GeometryAttributeDesc& attribute = m_vertexLayoutDescription[attrIndex].value();

		GLboolean shouldNormalize = GL_FALSE;
		GLenum baseType;

		switch (attribute.type) {
		case GeometryAttributeType::Float:
			baseType = GL_FLOAT;
			break;

		case GeometryAttributeType::UnsignedByte:
			baseType = GL_UNSIGNED_BYTE;
			break;

		case GeometryAttributeType::UnsignedShort:
			baseType = GL_UNSIGNED_SHORT;
			break;

		case GeometryAttributeType::UnsignedInt:
			baseType = GL_UNSIGNED_INT;
			break;

		case GeometryAttributeType::Int:
			baseType = GL_INT;
			break;

		}

		glEnableVertexAttribArray(attrIndex);

		if (baseType == GL_FLOAT) {
			glVertexAttribPointer(attrIndex, attribute.size, baseType, shouldNormalize, attribute.stride, (GLvoid*)attribute.offset);
		}
		else {
			glVertexAttribIPointer(attrIndex, attribute.size, baseType, attribute.stride, (GLvoid*)attribute.offset);
		}
	}

	GL_CALL_BLOCK_END();

	if (m_indexBuffer != nullptr)
		m_indexBuffer->bind();

	m_vertexBuffer->bind();

	glBindVertexArray(0);
}

void GLGeometryInstance::destroy()
{
	if (m_VAO != 0) {
		delete m_vertexBuffer;
		m_vertexBuffer = nullptr;

		delete m_indexBuffer;
		m_indexBuffer = nullptr;

		m_vertexLayoutDescription.clear();
		m_vertexLayoutDescription.resize(16);

		GL_CALL(glDeleteVertexArrays(1, &m_VAO));
		m_VAO = 0;
	}
}

void GLGeometryInstance::bind()
{
	GL_CALL(glBindVertexArray(m_VAO));
}

void GLGeometryInstance::unbind()
{
	GL_CALL(glBindVertexArray(0));
}

void GLGeometryInstance::setVerticesData(size_t count, size_t size, const std::byte * data, DataUsage usage)
{
	if (m_vertexBuffer == nullptr)
		m_vertexBuffer = createBuffer(GLBuffer::Type::Vertex, (GLBuffer::Usage)usage, size);

	m_vertexBuffer->bind();
	m_vertexBuffer->setData(size, data);

	m_verticesCount = count;
}

void GLGeometryInstance::setVerticesDataPart(size_t offset, size_t size, const std::byte * data)
{
	m_vertexBuffer->bind();
	m_vertexBuffer->setData(offset, size, data);
}

void GLGeometryInstance::setIndicesData(size_t count, size_t size, const std::byte * data, DataUsage usage)
{
	if (m_indexBuffer == nullptr)
		m_indexBuffer = createBuffer(GLBuffer::Type::Index, (GLBuffer::Usage)usage, size);

	m_indexBuffer->bind();
	m_indexBuffer->setData(size, data);

	m_indicesCount = count;
	m_indexed = true;
}

void GLGeometryInstance::draw(DrawMode drawType, size_t offset, size_t count) {
	GLenum drawMode;

	if (drawType == DrawMode::Triangles)
		drawMode = GL_TRIANGLES;
	else if (drawType == DrawMode::TrianglesStrip)
		drawMode = GL_TRIANGLE_STRIP;
	
	GL_CALL(glDrawArrays(drawMode, offset, count));
}

void GLGeometryInstance::drawIndexed(DrawMode drawType, size_t offset, size_t count) {
	GLenum drawMode;

	if (drawType == DrawMode::Triangles)
		drawMode = GL_TRIANGLES;
	else if (drawType == DrawMode::TrianglesStrip)
		drawMode = GL_TRIANGLE_STRIP;

	GLenum glIndicesType = GL_UNSIGNED_INT;
	size_t offsetMultiplier = sizeof(unsigned int);

	GL_CALL(glDrawElements(drawMode, count, glIndicesType, (const void*)(offset * offsetMultiplier)));
}

void GLGeometryInstance::draw(DrawMode drawType)
{
	draw(drawType, 0, m_verticesCount);
}

void GLGeometryInstance::drawIndexed(DrawMode drawType)
{
	drawIndexed(drawType, 0, m_indicesCount);
}

size_t GLGeometryInstance::getVerticesCount() const
{
	return m_verticesCount;
}

size_t GLGeometryInstance::getIndicesCount() const
{
	return m_indicesCount;
}

bool GLGeometryInstance::hasAttribute(int attributeId) const
{
	return m_vertexLayoutDescription[attributeId].has_value();
}

void GLGeometryInstance::setAttributeDesc(int attributeId, const GeometryAttributeDesc & description)
{
	m_vertexLayoutDescription[attributeId] = description;
}

GeometryAttributeDesc GLGeometryInstance::getAttributeDesc(int attributeId) const
{
	return m_vertexLayoutDescription[attributeId].value();
}

size_t GLGeometryInstance::getVerticesDataSize() const
{
	return m_verticesDataSize;
}

size_t GLGeometryInstance::getIndicesDataSize() const
{
	return m_indicesDataSize;
}

size_t GLGeometryInstance::getDataSize() const
{
	return m_verticesDataSize + m_indicesDataSize;
}

bool GLGeometryInstance::isIndexed() const
{
	return m_indexed;
}
