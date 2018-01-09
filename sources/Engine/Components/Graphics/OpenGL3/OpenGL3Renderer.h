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
#include "OpenGL3Shader.h"
#include "OpenGL3Sprite.h"
#include "OpenGL3Mesh.h"
#include "OpenGL3Material.h"
#include "OpenGL3Light.h"

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

	void beginRendering(Color) override;
	void endRendering() override;

	void addLight(Light*) override;

	void bindMaterial(const Material*) override;
	void bindShader(const GpuProgram*) override;
	void bindTexture(const Texture*, size_t) override;
private:
	void drawSubModel(const SubModel*);
	OpenGL3Renderer(OpenGL3Renderer&);

private:
	Window* m_window;
	Camera* m_currentCamera;

	std::vector<OpenGL3Light*> m_lights;
};