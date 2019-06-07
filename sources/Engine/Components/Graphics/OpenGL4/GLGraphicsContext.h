#pragma once

#include <Engine/Components/Graphics/RenderSystem/GraphicsContext.h>
#include "GL.h"

#include <Engine/Components/Graphics/RenderSystem/RenderSystemException.h>
#include "GLGeometryInstance.h"

class GLGraphicsContext : public GraphicsContext {
public:
	GLGraphicsContext(std::shared_ptr<sw::platform::base::Window> window, RenderTarget* windowRenderTarget, Logger* logger);
	~GLGraphicsContext();

	void enableDepthTest() override;
	void disableDepthTest() override;

	void setDepthTestFunction(DepthFunction function) override;

	void enableWritingToDepthBuffer() override;
	void disableWritingToDepthBuffer() override;

	void enableFaceCulling() override;
	void disableFaceCulling() override;

	void setFaceCullingMode(FaceCullingMode mode) override;

	void enableBlending() override;
	void disableBlending() override;

	void setBlendingMode(BlendingMode sourceAffect, BlendingMode destinationAffect) override;
	void setBlendingEquation(BlendingEquation equation) override;

	void enableScissorTest() override;
	void disableScissorTest() override;

	void enableWireframeRendering() override;
	void disableWireframeRendering() override;

	void setScissorRectangle(const Rect& rectangle) override;

	void drawFullScreenQuad() override;

	GeometryInstance* getNDCQuadInstance() const override;

	GeometryInstance* createGeometryInstance() override;
	Texture* createTexture() override;
	GpuProgram* createGpuProgram() override;
	RenderTarget* createRenderTarget() override;

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