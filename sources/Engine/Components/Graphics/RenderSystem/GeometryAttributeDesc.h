#pragma once

enum class GeometryAttributeType {
	Float, UnsignedByte, UnsignedShort, Int, UnsignedInt
};

struct GeometryAttributeDesc {
public:
	GeometryAttributeDesc(GeometryAttributeType type, size_t offset, size_t size, size_t stride)
		: type(type), offset(offset), size(size), stride(stride)
	{ }

	~GeometryAttributeDesc() = default;

public:
	GeometryAttributeType type;
	size_t offset;
	size_t size;
	size_t stride;
};