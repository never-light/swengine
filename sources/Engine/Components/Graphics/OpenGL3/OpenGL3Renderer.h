#pragma once

#include <vector>

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>
#include <Engine\Components\GUI\Window.h>

#include <Engine\Components\Graphics\RenderSystem\Model.h>
#include <Engine\Components\Graphics\RenderSystem\Color.h>
#include <Engine\Components\Graphics\RenderSystem\Renderer.h>

#include "OpenGL3.h"
#include "OpenGL3Texture.h"
#include "OpenGL3GpuProgram.h"
#include "OpenGL3Sprite.h"
#include "OpenGL3Material.h"
#include "OpenGL3Light.h"
#include "OpenGL3Framebuffer.h"
#include "OpenGL3Hardwarebuffer.h"

class OpenGL3Renderer : public Renderer {
public:
	OpenGL3Renderer(Window*);
	~OpenGL3Renderer();

	Window* getWindow() const override;

	void setCurrentCamera(Camera*) override;
	Camera* getCurrentCamera() const override;

	void drawSprite(Sprite*, const glm::vec2&, const glm::vec2&, float) override;
	void drawModel(const Model*) override;
	void drawMesh(const Mesh*) override;
	void drawNDCQuad(GpuProgram* program, Framebuffer* framebuffer) override;

	void beginRendering(Color) override;
	void endRendering() override;
	void swapBuffers() override;

	void addLight(Light*) override;

	void clearCurrentRenderTarget(const Color& color, ClearRenderTargetMode mode) override;
	void setCurrentRenderTarget(Framebuffer* framebuffer) override;
	Framebuffer* getCurrentRenderTarget() const override;
	void resetRenderTarget() override;

	void copyFramebufferDataToDefaultBuffer(Framebuffer* framebuffer) override;

	void bindMaterial(const Material*) override;
	void bindShader(const GpuProgram*) override;
	void bindTexture(const Texture*, size_t) override;
	void bindShaderLights(GpuProgram* program);

	Material* createMaterial() override;
	Texture* createTexture(int width, int height, const unsigned char* data) override;
	Texture* createTexture(int width, int height, TextureInternalFormat internalFormat, TextureFormat format, TextureDataType type) override;

	GpuProgram* createGpuProgram(const std::string& source) override;
	Sprite* createSprite(Texture* texture, GpuProgram* gpuProram) override;

	Light* createLight(LightType type) override;

	Framebuffer* createFramebuffer() override;
	
	HardwareBuffer* createHardwareBuffer() override;
	HardwareBuffer* createHardwareBuffer(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices) override;
private:
	void drawSubModel(const SubModel*);
	OpenGL3Renderer(OpenGL3Renderer&);

private:
	OpenGL3Framebuffer* m_renderTarget;

	Window* m_window;
	Camera* m_currentCamera;

	std::vector<OpenGL3Light*> m_lights;
};