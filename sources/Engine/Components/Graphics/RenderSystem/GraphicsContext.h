#pragma once

#include <Engine/Components/Debugging/Logger.h>

#include <Engine/Components/Math/types.h>
#include <Engine/Components/Math/Geometry/Rect.h>
#include <Engine/Components/Platform/Base/BaseWindow.h>

#include "RenderTarget.h"
#include "GpuProgram.h"
#include "GeometryInstance.h"

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

	enum class BlendingEquation {
		Add, Subtract, ReverseSubtract, Min, Max
	};
public:
	GraphicsContext(std::shared_ptr<sw::platform::base::Window> window, RenderTarget* windowRenderTarget, Logger* logger);
	virtual ~GraphicsContext();

	virtual void enableDepthTest() = 0;
	virtual void disableDepthTest() = 0;

	virtual void setDepthTestFunction(DepthFunction function) = 0;

	virtual void enableWritingToDepthBuffer() = 0;
	virtual void disableWritingToDepthBuffer() = 0;

	virtual void enableFaceCulling() = 0;
	virtual void disableFaceCulling() = 0;

	virtual void setFaceCullingMode(FaceCullingMode mode) = 0;

	virtual void enableBlending() = 0;
	virtual void disableBlending() = 0;

	virtual void enableScissorTest() = 0;
	virtual void disableScissorTest() = 0;

	virtual void setScissorRectangle(const Rect& rectangle) = 0;

	virtual void setBlendingMode(BlendingMode sourceAffect, BlendingMode destinationAffect) = 0;
	virtual void setBlendingEquation(BlendingEquation equation) = 0;

	virtual void enableWireframeRendering() = 0;
	virtual void disableWireframeRendering() = 0;

	virtual GeometryInstance* getNDCQuadInstance() const = 0;

	virtual GeometryInstance* createGeometryInstance() = 0;
	virtual Texture* createTexture() = 0;
	virtual GpuProgram* createGpuProgram() = 0;

	virtual RenderTarget* createRenderTarget() = 0;

	virtual void drawFullScreenQuad() = 0;

	unsigned int getViewportWidth() const;
	unsigned int getViewportHeight() const;

	RenderTarget* getWindowRenderTarget() const;

	void setClearColor(float r, float g, float b, float a = 1.0f);
	void setClearColor(const vector3& color);
	void setClearColor(const vector4& color);

	void setDepthClearValue(float depthValue);
	void setStencilClearValue(int stencilValue);

	void clear(RTClearMode mode);

protected:
	std::shared_ptr<sw::platform::base::Window> m_window;

	RenderTarget* m_windowRenderTarget;

	Logger* m_logger;
};