#pragma once

#include <vector>
#include "Vertex.h"

class HardwareBuffer {
public:
	HardwareBuffer() { }
	HardwareBuffer(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices) { }

	virtual ~HardwareBuffer() { }

	virtual void lock() = 0;
	virtual void unlock() = 0;

	virtual void setVertices(const std::vector<Vertex>& vertices) = 0;
	virtual const std::vector<Vertex>& getVertices() = 0;

	virtual void setIndices(const std::vector<size_t>& indices) = 0;
	virtual const std::vector<size_t>& getIndices() = 0;

	virtual void addVertex(const Vertex& vertex) = 0;
	virtual void addIndex(size_t index) = 0;

	virtual size_t getVerticesCount() const = 0;
	virtual size_t getIndicesCount() const = 0;
};