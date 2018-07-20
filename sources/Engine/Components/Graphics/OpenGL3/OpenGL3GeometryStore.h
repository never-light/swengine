#pragma once

#include <Engine\Components\Graphics\RenderSystem\GeometryStore.h>
#include "OpenGL3.h"
#include "OpenGL3Buffer.h"

#include <vector>

class OpenGL3GeometryStore : public GeometryStore {
private:
	struct VertexLayoutAttribute {
		size_t index;
		GeometryStore::BufferId bufferId;
		size_t size;
		VertexLayoutAttributeBaseType type;
		bool shouldNormalize;
		size_t stride;
		size_t offset;
	};

public:
	OpenGL3GeometryStore();
	virtual ~OpenGL3GeometryStore();

	virtual BufferId requireBuffer(BufferType bufferType, size_t size) override;
	
	virtual void setBufferData(BufferId bufferId, size_t offset, size_t length, const std::byte * data) override;
	virtual void setVertexLayoutAttribute(size_t index, BufferId bufferId, size_t size, VertexLayoutAttributeBaseType type, bool shouldNormalize, size_t stride, size_t offset) override;
	
	virtual void create() override;
	virtual void destroy() override;
	
	virtual void bind() override;
	virtual void unbind() override;

	virtual void drawArrays(DrawType drawType, size_t offset, size_t count);
	virtual void drawElements(DrawType drawType, size_t offset, size_t count, IndicesType indicesType);
protected:
	GLuint m_VAO;

	std::vector<OpenGL3Buffer*> m_buffers;
	std::vector<VertexLayoutAttribute> m_vertexLayoutDescription;
};