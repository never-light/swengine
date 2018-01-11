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

class Renderer {
public:
	virtual Window* getWindow() const = 0;

	virtual void setCurrentCamera(Camera*) = 0;
	virtual Camera* getCurrentCamera() const = 0;

	virtual void drawSprite(Sprite*, const glm::vec2&, const glm::vec2&, float) = 0;
	virtual void drawModel(const Model*) = 0;
	virtual void drawMesh(const Mesh*) = 0;

	virtual void beginRendering(Color) = 0;
	virtual void endRendering() = 0;

	virtual void addLight(Light*) = 0;

	virtual void bindMaterial(const Material*) = 0;
	virtual void bindShader(const GpuProgram*) = 0;
	virtual void bindTexture(const Texture*, size_t) = 0;

	virtual Material* createMaterial() = 0;
	virtual Texture* createTexture(int width, int height, const unsigned char* data) = 0;
	virtual GpuProgram* createGpuProgram(const std::string& source) = 0;
	virtual Sprite* createSprite(Texture* texture, GpuProgram* gpuProram) = 0;
	virtual Mesh* createMesh() = 0;
	virtual Light* createLight(LightType type) = 0;
};