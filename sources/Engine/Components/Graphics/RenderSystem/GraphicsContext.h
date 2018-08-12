#pragma once

#include <Engine\Components\Debugging\Logger.h>

#include <Engine\Components\GUI\Window.h>
#include <Engine\Components\Math\types.h>
#include <Engine\Components\Math\Geometry\Rect.h>

#include "RenderTarget.h"

class GraphicsContext {
public:
	enum class DepthFunction {
		Less, LessEqual
	};

	enum class FaceCullingMode {
		Front, Back, FrontBack
	};

	enum class BlendingMode {
		Zero, One, SrcAlpha, OneMinusSrcAlpha
	};
public:
	GraphicsContext(Window* window, unsigned int viewportWidth, unsigned int viewportHeight, RenderTarget* windowRenderTarget, Logger* logger);
	virtual ~GraphicsContext();

	virtual void enableDepthTest() = 0;
	virtual void disableDepthTest() = 0;

	virtual void setDepthTestFunction(DepthFunction function) = 0;

	virtual void enableFaceCulling() = 0;
	virtual void disableFaceCulling() = 0;

	virtual void setFaceCullingMode(FaceCullingMode mode) = 0;

	virtual void enableBlending() = 0;
	virtual void disableBlending() = 0;

	virtual void enableScissorTest() = 0;
	virtual void disableScissorTest() = 0;

	virtual void setScissorRectangle(const Rect& rectangle) = 0;

	virtual void setBlendingMode(BlendingMode sourceAffect, BlendingMode destinationAffect) = 0;

	virtual void swapBuffers() = 0;

	virtual void enableWireframeRendering() = 0;
	virtual void disableWireframeRendering() = 0;

	unsigned int getViewportWidth() const;
	unsigned int getViewportHeight() const;

	RenderTarget* getWindowRenderTarget() const;

	void setClearColor(float r, float g, float b, float a = 1.0f);
	void setClearColor(const vector3& color);
	void setClearColor(const vector4& color);

	void setDepthClearValue(float depthValue);
	void setStencilClearValue(int stencilValue);

	void clear(unsigned int mode);
protected:
	Window* m_window;

	unsigned int m_viewportWidth;
	unsigned int m_viewportHeight;

	RenderTarget* m_windowRenderTarget;

	Logger* m_logger;
};