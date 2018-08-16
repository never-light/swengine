#pragma once

#include "OpenGL3RenderTarget.h"

class OpenGL3WindowRenderTarget : public OpenGL3RenderTarget {
public:
	OpenGL3WindowRenderTarget();
	virtual ~OpenGL3WindowRenderTarget();

	virtual void create() override;
	virtual void destroy() override;

	virtual void attachColorComponent(size_t index, Texture * texture) override;
	virtual void attachDepthStencilComponent(Texture* texture) override;
};