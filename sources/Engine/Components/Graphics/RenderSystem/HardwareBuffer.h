#pragma once

#include <vector>
#include "Vertex.h"

enum class VertexAttributeValueType {
	Real
};

struct VertexAttributeDescription {
	VertexAttributeDescription(size_t size, VertexAttributeValueType type, bool normalized, size_t stride, size_t offset) 
		: size(size), 
		type(type), 
		normalized(normalized), 
		stride(stride), 
		offset(offset) 
	{

	}

	~VertexAttributeDescription() = default;

	size_t offset;
	size_t size;
	size_t stride;
	VertexAttributeValueType type;
	bool normalized;
};

class HardwareBuffer {
public:
	HardwareBuffer() = default;
	virtual ~HardwareBuffer() = default;

	virtual void lock() = 0;
	virtual void unlock() = 0;

	virtual void setVertexFormat(VertexFormat format) = 0;
	virtual VertexFormat getVertexFormat() const = 0;
	virtual void addVertexAttribute(VertexAttributeDescription description) = 0;

	virtual void setVerticesData(size_t count, size_t typeSize, const void* data) = 0;
	virtual void setIndicesData(size_t count, size_t typeSize, const void* data) = 0;

	virtual size_t getIndicesCount() const = 0;
	virtual size_t getVerticesCount() const = 0;

	virtual bool hasIndicesData() const = 0;
};