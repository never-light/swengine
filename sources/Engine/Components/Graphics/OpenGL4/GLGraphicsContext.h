#pragma once

#include <Engine/Components/Graphics/RenderSystem/GraphicsContext.h>
#include "GL.h"

#include <Engine/Components/Graphics/RenderSystem/RenderSystemException.h>
#include "GLGeometryInstance.h"

class GLGraphicsContext : public GraphicsContext {
public:
	GLGraphicsContext(Window* window, unsigned int viewportWidth, unsigned int viewportHeight, RenderTarget* windowRenderTarget, Logger* logger);
	~GLGraphicsContext();

	virtual void enableDepthTest() override;
	virtual void disableDepthTest() override;

	virtual void setDepthTestFunction(DepthFunction function) override;

	virtual void enableWritingToDepthBuffer() override;
	virtual void disableWritingToDepthBuffer() override;

	virtual void enableFaceCulling() override;
	virtual void disableFaceCulling() override;

	virtual void setFaceCullingMode(FaceCullingMode mode) override;

	virtual void enableBlending() override;
	virtual void disableBlending() override;

	virtual void setBlendingMode(BlendingMode sourceAffect, BlendingMode destinationAffect) override;
	virtual void setBlendingEquation(BlendingEquation equation);

	virtual void enableScissorTest() override;
	virtual void disableScissorTest() override;

	virtual void enableWireframeRendering() override;
	virtual void disableWireframeRendering() override;

	virtual void setScissorRectangle(const Rect& rectangle) override;

	virtual void drawFullScreenQuad();

	virtual void swapBuffers() override;

	virtual GeometryInstance* getNDCQuadInstance() const override;

	virtual GeometryInstance* createGeometryInstance() override;
	virtual Texture* createTexture() override;
	virtual GpuProgram* createGpuProgram() override;
	virtual RenderTarget* createRenderTarget() override;

private:
	GLGeometryInstance* m_ndcQuadInstance;

private:
	static void APIENTRY debugOutputCallback(GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		GLvoid* userParam);
};