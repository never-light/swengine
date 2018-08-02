#pragma once

#include <Engine\Components\Graphics\RenderSystem\Buffer.h>
#include "OpenGL3.h"

class OpenGL3Buffer : public Buffer {
public:
	OpenGL3Buffer(Buffer::Type type, Buffer::Usage usage);
	virtual ~OpenGL3Buffer();

	virtual void create() override;
	virtual void destroy() override;

	virtual void bind() override;
	virtual void unbind() override;

	virtual void allocateMemory(size_t size) override;

	virtual void setData(size_t length, const std::byte* data) override;
	virtual void setData(size_t offset, size_t length, const std::byte* data);

	GLuint getBufferPointer() const;

protected:
	GLuint m_bufferPointer;
	size_t m_size;

	GLenum m_glUsage;
	GLenum m_glTarget;
};