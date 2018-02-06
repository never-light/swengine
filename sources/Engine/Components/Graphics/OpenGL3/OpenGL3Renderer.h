#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>

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

	void drawPrimitives(DrawPrimitivesMode mode, size_t start, size_t count) override;
	void drawIndexedPrimitives(DrawPrimitivesMode mode, size_t count) override;

	void drawNDCQuad(GpuProgram* program, Framebuffer* framebuffer) override;
	void drawSprite(Sprite*, const glm::vec2&, const glm::vec2&, float) override;
	void drawMesh(const Mesh* mesh, const matrix4& transform, const Material* material) override;
	void drawModel(const Model*) override;

	void swapBuffers() override;

	void clearCurrentRenderTarget(const Color& color, ClearRenderTargetMode mode) override;
	void setCurrentRenderTarget(Framebuffer* framebuffer) override;
	Framebuffer* getCurrentRenderTarget() const override;
	void resetRenderTarget() override;

	void copyFramebufferDataToDefaultBuffer(Framebuffer* framebuffer) override;

	void addLight(Light*) override;

	void bindTexture(const Texture*, size_t) override;
	void bindShader(GpuProgram* program) override;
	void bindMaterial(const Material*) override;
	
	void bindGeometryBuffer(const HardwareBuffer* buffer) override;
	void unbindGeometryBuffer() override;

	void setOption(Renderer::Option option, Renderer::OptionValue value) override;
private:
	void drawSubModel(const SubModel*);
	OpenGL3Renderer(OpenGL3Renderer&);

	void bindShaderData(GpuProgram* program, 
		const Framebuffer* framebuffer = nullptr, 
		const Material* material = nullptr
	);
	void bindShader(GpuProgram* program, const Framebuffer* framebuffer, const Material* material);
private:
	OpenGL3Framebuffer* m_renderTarget;

	Window* m_window;
	Camera* m_currentCamera;

	std::vector<OpenGL3Light*> m_lights;

	matrix4 m_cachedWorldMatrix;
private:
	static const std::unordered_map<Renderer::Option, GLenum> m_enablingOptions;
	static const std::unordered_map<Renderer::OptionValue, GLenum> m_optionsValues;
};