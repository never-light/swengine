#pragma once

#include <vector>

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>
#include <Engine\Components\GUI\Window.h>
#include "OpenGL3.h"
#include "OpenGL3Color.h"
#include "OpenGL3Texture.h"
#include "OpenGL3Lightmap.h"
#include "OpenGL3Shader.h"
#include "OpenGL3Sprite.h"
#include "OpenGL3Mesh.h"
#include "OpenGL3Material.h"
#include "OpenGL3Light.h"
#include "OpenGL3DirectionalLight.h"
#include "OpenGL3PointLight.h"
#include "OpenGL3Spotlight.h"
#include "OpenGL3Camera.h"
#include <Engine\Components\Graphics\Model.h>

class OpenGL3Renderer {
public:
	OpenGL3Renderer(Window*);
	~OpenGL3Renderer();

	void setCurrentCamera(OpenGL3Camera*);
	OpenGL3Camera* getCurrentCamera() const;

	void drawSprite(OpenGL3Sprite*, const glm::vec2&, const glm::vec2&, float);
	void drawModel(const Model*);
	void drawMesh(const Mesh*);

	void beginRendering(OpenGL3Color);
	void endRendering();

	void addLight(OpenGL3Light*);

	void bindMaterial(const OpenGL3Material*);
	void bindShader(const OpenGL3Shader*);
	void bindTexture(const OpenGL3Texture*, size_t);
private:
	void drawSubModel(const SubModel*);
	OpenGL3Renderer(OpenGL3Renderer&);

private:
	Window* m_window;
	OpenGL3Camera* m_currentCamera;

	std::vector<OpenGL3Light*> m_lights;
};