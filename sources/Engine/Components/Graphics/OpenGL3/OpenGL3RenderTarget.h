#pragma once

#include <Engine\Components\Graphics\RenderSystem\RenderTarget.h>

#include "OpenGL3.h"

class OpenGL3RenderTarget : public RenderTarget {
public:
	OpenGL3RenderTarget();
	virtual ~OpenGL3RenderTarget();

	virtual void create() override;
	virtual void destroy() override;

	virtual void bind() override;
	virtual void unbind() override;

	virtual void setClearColor(float r, float g, float b, float a = 1.0f) override;
	virtual void setClearColor(const vector3& color) override;
	virtual void setClearColor(const vector4& color) override;

	virtual void setDepthClearValue(float depthValue) override;
	virtual void setStencilClearValue(int stencilValue) override;

	virtual void clear(unsigned int mode) override;

	virtual void attachComponent(Component type, Texture * texture) override;

	virtual void copyComponentRawData(RenderTarget * destination, 
		const Rect & sourceArea, const Rect& destinationArea,
		unsigned int copyMode,
		CopyFilter copyFilter = CopyFilter::Linear) override;
	
	GLuint getFrameBufferPointer() const;

protected:
	GLuint m_frameBuffer;

private:
	size_t m_freeColorAttachment;
};