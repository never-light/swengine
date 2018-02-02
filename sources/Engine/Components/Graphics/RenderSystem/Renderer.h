#pragma once

#include <Engine\Components\GUI\Window.h>
#include <Engine\Components\Math\Math.h>

#include "Camera.h"
#include "Sprite.h"
#include "Mesh.h"
#include "Color.h"
#include "Material.h"
#include "GpuProgram.h"
#include "Texture.h"
#include "Light.h"
#include "Model.h"
#include "Framebuffer.h"
#include "HardwareBuffer.h"

enum class ClearRenderTargetMode {
	Color, ColorDepth
};

enum class DrawPrimitivesMode {
	Triangles, TrianglesStrip
};

class Renderer {
public:
	virtual Window* getWindow() const = 0;

	virtual void setCurrentCamera(Camera*) = 0;
	virtual Camera* getCurrentCamera() const = 0;

	virtual void drawPrimitives(DrawPrimitivesMode mode, size_t start, size_t count) = 0;
	virtual void drawIndexedPrimitives(DrawPrimitivesMode mode, size_t count) = 0;

	virtual void drawSprite(Sprite*, const glm::vec2&, const glm::vec2&, float) = 0;
	virtual void drawModel(const Model*) = 0;

	virtual void drawMesh(const Mesh* mesh) = 0;

	virtual void drawNDCQuad(GpuProgram* program, Framebuffer* framebuffer) = 0;
	 
	virtual void beginRendering(Color) = 0;
	virtual void endRendering() = 0;
	virtual void swapBuffers() = 0;

	virtual void addLight(Light*) = 0;

	virtual void clearCurrentRenderTarget(const Color& color, ClearRenderTargetMode mode) = 0;
	virtual void setCurrentRenderTarget(Framebuffer* framebuffer) = 0;
	virtual Framebuffer* getCurrentRenderTarget() const = 0;
	virtual void resetRenderTarget() = 0;

	virtual void copyFramebufferDataToDefaultBuffer(Framebuffer* framebuffer) = 0;

	virtual void bindMaterial(const Material* material) = 0;
	virtual void bindShader(const GpuProgram* gpuProgram) = 0;
	virtual void bindTexture(const Texture* texture, size_t unit) = 0;
	virtual void bindGeometryBuffer(const HardwareBuffer* buffer) = 0;
	virtual void unbindGeometryBuffer() = 0;
};