#pragma once

#include <Engine/types.h>
#include "GL.h"

class GLBuffer {
public:
	enum class Type {
		Vertex, Index
	};

	enum class Usage {
		StaticDraw, StaticRead, DynamicDraw, DynamicRead
	};

public:
	GLBuffer(Type type, Usage usage);
	virtual ~GLBuffer();

	void create();
	void destroy();

	void bind();
	void unbind();

	void allocateMemory(size_t size);

	void setData(size_t length, const std::byte* data);
	void setData(size_t offset, size_t length, const std::byte* data);

	GLuint getRawPointer() const;

protected:
	Type m_type;
	Usage m_usage;

	GLuint m_bufferPointer;
	size_t m_size;

	GLenum m_glUsage;
	GLenum m_glTarget;
};