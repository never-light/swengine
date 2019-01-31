#pragma once

#include <Engine/Components/Graphics/RenderSystem/GeometryInstance.h>
#include "GL.h"
#include "GLBuffer.h"

#include <vector>
#include <optional>

class GLGeometryInstance : public GeometryInstance {
public:
	GLGeometryInstance();
	virtual ~GLGeometryInstance();
	
public:
	virtual bool hasAttribute(int attributeId) const override;

	virtual void setAttributeDesc(int attributeId, const GeometryAttributeDesc& description) override;
	virtual GeometryAttributeDesc getAttributeDesc(int attributeId) const override;

	virtual size_t getVerticesDataSize() const override;
	virtual size_t getIndicesDataSize() const override;

	virtual size_t getDataSize() const override;

	virtual bool isIndexed() const override;

	virtual void create() override;
	virtual void destroy() override;

	virtual void bind() override;
	virtual void unbind() override;

	virtual void setVerticesData(size_t count, size_t size, const std::byte* data, DataUsage usage) override;
	virtual void setVerticesDataPart(size_t offset, size_t size, const std::byte* data) override;
	
	virtual void setIndicesData(size_t count, size_t size, const std::byte* data, DataUsage usage) override;

	virtual void draw(DrawMode drawType, size_t offset, size_t count) override;
	virtual void drawIndexed(DrawMode drawType, size_t offset, size_t count) override;

	virtual void draw(DrawMode drawType) override;
	virtual void drawIndexed(DrawMode drawType) override;

	virtual size_t getVerticesCount() const override;
	virtual size_t getIndicesCount() const override;
protected:
	GLBuffer* createBuffer(GLBuffer::Type bufferType, GLBuffer::Usage bufferUsage, size_t capacity);

protected:
	GLuint m_VAO;

	GLBuffer* m_vertexBuffer;
	GLBuffer* m_indexBuffer;

	std::vector<std::optional<GeometryAttributeDesc>> m_vertexLayoutDescription;

	size_t m_verticesDataSize;
	size_t m_indicesDataSize;

	size_t m_verticesCount;
	size_t m_indicesCount;

	bool m_indexed;
};