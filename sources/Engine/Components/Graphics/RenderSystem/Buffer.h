#pragma once

#include <Engine\types.h>

class Buffer {
public:
	enum class Type {
		Vertex, Index
	};

	enum class Usage {
		StaticDraw, StaticRead, DynamicDraw, DynamicRead
	};
public:
	Buffer(Type type, Usage usage);
	virtual ~Buffer();

	virtual void create() = 0;
	virtual void destroy() = 0;

	virtual void bind() = 0;
	virtual void unbind() = 0;
	
	virtual void allocateMemory(size_t size) = 0;

	virtual void setData(size_t length, const std::byte* data) = 0;
	virtual void setData(size_t offset, size_t length, const std::byte* data) = 0;

protected:
	Type m_type;
	Usage m_usage;
};