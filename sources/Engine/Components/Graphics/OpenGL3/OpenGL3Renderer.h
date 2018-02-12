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
#include "OpenGL3Framebuffer.h"
#include "OpenGL3Hardwarebuffer.h"
#include "OpenGL3UniformBuffer.h"

#include "OpenGL3ShaderDataStorage.h"

class OpenGL3Renderer : public Renderer {
public:
	OpenGL3Renderer(Window*);
	~OpenGL3Renderer();

	Window* getWindow() const override;

	void startFrame() override;
	void endFrame() override;

	void setCurrentCamera(Camera*) override;
	Camera* getCurrentCamera() const override;

	void drawPrimitives(DrawPrimitivesMode mode, size_t start, size_t count) override;
	void drawIndexedPrimitives(DrawPrimitivesMode mode, size_t count) override;

	void drawNDCQuad(GpuProgram* program) override;
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

	void bindTextureUnit(const Texture* texture, size_t unit) override;
	void bindShader(GpuProgram* program) override;
	void bindShaderData(GpuProgram* program) override;
	void bindMaterial(const Material* material) override;
	
	void bindGeometryBuffer(const HardwareBuffer* buffer) override;
	void unbindGeometryBuffer() override;
	 
	void setViewport(int width, int height) override;
	void setOption(Renderer::Option option, Renderer::OptionValue value) override;

	void setShaderAutobindingData(const std::string& name, const GpuProgram::Parameter& value) override;
private:
	void updateObjectUB(const matrix4& modelMatrix);
	void updateFrameUB();
	void updateLightsUB();

	OpenGL3Renderer(OpenGL3Renderer&);
private:
	OpenGL3Framebuffer* m_renderTarget;

	OpenGL3UniformBuffer* m_frameUB;
	OpenGL3UniformBuffer* m_objectUB;
	OpenGL3UniformBuffer* m_lightsUB;

	// Size of one light description
	const GLsizeiptr m_lightDescriptionSize = 176;

	const GLuint m_frameUBBindingPoint = 0;
	const GLuint m_objectUBBindingPoint = 1;
	const GLuint m_lightsUBBindingPoint = 2;

	OpenGL3ShaderDataStorage* m_shaderDataStorage;

	Viewport* m_viewport;
	Window* m_window;
	Camera* m_currentCamera;

	std::vector<Light*> m_lights;
private:
	static const std::unordered_map<Renderer::Option, GLenum> m_enablingOptions;
	static const std::unordered_map<Renderer::OptionValue, GLenum> m_optionsValues;
};