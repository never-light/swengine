#pragma once

#include <Engine/types.h>
#include "GeometryAttributeDesc.h"

class GeometryInstance {
public:
	enum class CommonAttrs {
		Position = 0, Normal, Tangent, Bitangent, UV, JointsIds, JointsWeights, Color, _End
	};

	enum class DataUsage {
		StaticDraw, StaticRead, DynamicDraw, DynamicRead
	};

	enum class DrawMode {
		Triangles, TrianglesStrip
	};

public:
	GeometryInstance() = default;
	virtual ~GeometryInstance() = default;

	virtual void create() = 0;
	virtual void destroy() = 0;

	virtual void bind() = 0;
	virtual void unbind() = 0;

	virtual void setVerticesData(size_t count, size_t size, const std::byte* data, DataUsage usage) = 0;
	virtual void setVerticesDataPart(size_t offset, size_t size, const std::byte* data) = 0;

	virtual size_t getVerticesDataSize() const = 0;
	virtual size_t getIndicesDataSize() const = 0;
	
	virtual size_t getDataSize() const = 0;

	virtual bool hasAttribute(int attributeId) const = 0;
	bool hasAttribute(CommonAttrs attribute) const;

	virtual void setAttributeDesc(int attributeId, const GeometryAttributeDesc& description) = 0;
	virtual GeometryAttributeDesc getAttributeDesc(int attributeId) const = 0;

	void setAttributeDesc(CommonAttrs attributeId, const GeometryAttributeDesc& description);
	GeometryAttributeDesc getAttributeDesc(CommonAttrs attributeId) const;

	virtual bool isIndexed() const = 0;
	virtual void setIndicesData(size_t count, size_t size, const std::byte* data, DataUsage usage) = 0;

	virtual void draw(DrawMode drawType, size_t offset, size_t count) = 0;
	virtual void drawIndexed(DrawMode drawType, size_t offset, size_t count) = 0;

	virtual void draw(DrawMode drawType) = 0;
	virtual void drawIndexed(DrawMode drawType) = 0;

	virtual size_t getVerticesCount() const = 0;
	virtual size_t getIndicesCount() const = 0;
};