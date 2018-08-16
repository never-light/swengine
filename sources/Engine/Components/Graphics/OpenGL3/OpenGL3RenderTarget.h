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

	virtual void attachColorComponent(size_t index, Texture* texture) override;
	virtual void attachDepthStencilComponent(Texture* texture) override;

	virtual void copyColorComponentData(
		size_t sourceComponentIndex,
		RenderTarget* destination,
		size_t destinationComponentIndex,
		const Rect& sourceArea, const Rect& destinationArea,
		CopyFilter copyFilter = CopyFilter::Linear
	) override;

	virtual void copyDepthStencilComponentData(
		RenderTarget* destination,
		const Rect& sourceArea, const Rect& destinationArea,
		CopyFilter copyFilter = CopyFilter::Linear
	) override;

	GLuint getFrameBufferPointer() const;

protected:
	GLuint m_frameBuffer;

private:
	const static size_t MAX_COLOR_ATTACHMENTS = 8;

	size_t m_freeColorAttachment;
	GLenum* m_colorAttachments;

	bool m_isComplete;
};