#pragma once

#include <Engine\Components\Graphics\RenderSystem\GraphicsContext.h>

#include "OpenGL3.h"

class OpenGL3GraphicsContext : public GraphicsContext {
public:
	OpenGL3GraphicsContext(Window* window, unsigned int viewportWidth, unsigned int viewportHeight);
	~OpenGL3GraphicsContext();

	virtual void enableDepthTest() override;
	virtual void disableDepthTest() override;

	virtual void setDepthTestFunction(DepthFunction function) override;

	virtual void enableFaceCulling() override;
	virtual void disableFaceCulling() override;

	virtual void setFaceCullingMode(FaceCullingMode mode) override;

	virtual void clear(const vector3& color) override;
	virtual void swapBuffers() override;
};