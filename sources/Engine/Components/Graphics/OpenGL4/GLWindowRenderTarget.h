#pragma once

#include "GLRenderTarget.h"

class GLWindowRenderTarget : public GLRenderTarget {
public:
	GLWindowRenderTarget();
	virtual ~GLWindowRenderTarget();

	virtual void create() override;
	virtual void destroy() override;

	virtual void attachColorComponent(size_t index, Texture * texture) override;
	virtual void attachDepthStencilComponent(Texture* texture) override;
};