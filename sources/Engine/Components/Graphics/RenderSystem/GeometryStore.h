#pragma once

#include "Buffer.h"

class GeometryStore {
public:
	using BufferId = size_t;

	enum class BufferType {
		Vertex, Index
	};

	enum class VertexLayoutAttributeBaseType {
		Float, UnsignedByte, UnsignedShort, Int, UnsignedInt
	};

	enum class DrawType {
		Triangles, TrianglesStrip
	};

	enum class IndicesType {
		UnsignedInt
	};

	using BufferUsage = Buffer::Usage;
public:
	GeometryStore();
	virtual ~GeometryStore();

	virtual BufferId requireBuffer(BufferType bufferType, BufferUsage bufferUsage, size_t size) = 0;
	virtual void setBufferData(BufferId bufferId, size_t offset, size_t length, const std::byte* data) = 0;

	virtual void setVertexLayoutAttribute(size_t index, BufferId bufferId, size_t size, VertexLayoutAttributeBaseType type, 
		bool shouldNormalize, size_t stride, size_t offset) = 0;

	virtual void create() = 0;
	virtual void destroy() = 0;

	virtual void bind() = 0;
	virtual void unbind() = 0;

	virtual void drawArrays(DrawType drawType, size_t offset, size_t count) = 0;
	virtual void drawElements(DrawType drawType, size_t offset, size_t count, IndicesType indicesType) = 0;
};