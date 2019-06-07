#pragma once

#include <Windows.h>
#include <Engine/Components/Graphics/RenderSystem/GraphicsDevice.h>
#include <Engine/Components/Platform/Base/BaseWindow.h>
#include "GLGeometryInstance.h"

class GLGraphicsDevice : public GraphicDevice {
public:
	GLGraphicsDevice(std::shared_ptr<sw::platform::base::Window> window, Logger* logger);
	virtual ~GLGraphicsDevice();

	GLGraphicsDevice(const GraphicDevice& device) = delete;
	GLGraphicsDevice(GraphicDevice&& device) = delete;

	std::shared_ptr<GraphicsContext> getImmediateContext() override;
	void swapBuffers() override;

	std::shared_ptr<GLGeometryInstance> createGeometryInstance() override;
	std::shared_ptr<Texture> createTexture() override;
	std::shared_ptr<GpuProgram> createGpuProgram() override;
	std::shared_ptr<RenderTarget> createRenderTarget() override;

private:
	std::shared_ptr<GraphicsContext> m_immediateContext;

	HGLRC m_renderingContext = nullptr;
	HDC m_deviceContext = nullptr;
};
