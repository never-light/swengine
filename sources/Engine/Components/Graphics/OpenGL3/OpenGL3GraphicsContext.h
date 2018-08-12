#pragma once

#include <Engine\Components\Graphics\RenderSystem\GraphicsContext.h>
#include "OpenGL3.h"

#include <Engine\Components\Graphics\RenderSystem\RenderSystemException.h>

class OpenGL3GraphicsContext : public GraphicsContext {
public:
	OpenGL3GraphicsContext(Window* window, unsigned int viewportWidth, unsigned int viewportHeight, RenderTarget* windowRenderTarget, Logger* logger);
	~OpenGL3GraphicsContext();

	virtual void enableDepthTest() override;
	virtual void disableDepthTest() override;

	virtual void setDepthTestFunction(DepthFunction function) override;

	virtual void enableFaceCulling() override;
	virtual void disableFaceCulling() override;

	virtual void setFaceCullingMode(FaceCullingMode mode) override;

	virtual void enableBlending() override;
	virtual void disableBlending() override;

	virtual void setBlendingMode(BlendingMode sourceAffect, BlendingMode destinationAffect) override;

	virtual void enableScissorTest() override;
	virtual void disableScissorTest() override;

	virtual void enableWireframeRendering() override;
	virtual void disableWireframeRendering() override;

	virtual void setScissorRectangle(const Rect& rectangle) override;

	virtual void swapBuffers() override;
private:
	static void APIENTRY debugOutputCallback(GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		GLvoid* userParam);
};