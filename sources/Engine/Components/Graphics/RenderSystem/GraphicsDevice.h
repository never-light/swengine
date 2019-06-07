#pragma once

#include "GeometryInstance.h"
#include "GpuProgram.h"
#include "RenderTarget.h"
#include "GraphicsContext.h"
#include "Engine/Components/Graphics/OpenGL4/GLGeometryInstance.h"

class GraphicDevice {
public:
	GraphicDevice() = default;
	virtual ~GraphicDevice() = default;

	GraphicDevice(const GraphicDevice& device) = delete;
	GraphicDevice(GraphicDevice&& device) = delete;

	virtual std::shared_ptr<GraphicsContext> getImmediateContext() = 0;
	virtual void swapBuffers() = 0;

	virtual std::shared_ptr<GLGeometryInstance> createGeometryInstance() = 0;
	virtual std::shared_ptr<Texture> createTexture() = 0;
	virtual std::shared_ptr<GpuProgram> createGpuProgram() = 0;
	virtual std::shared_ptr<RenderTarget> createRenderTarget() = 0;
};
