#pragma once

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>
#include <Engine\Components\GUI\Window.h>
#include "OpenGL3.h"
#include "OpenGL3Color.h"
#include "OpenGL3Sprite.h"
#include "OpenGL3Mesh.h"
#include <Engine\Components\Graphics\Model.h>

class OpenGL3Renderer {
public:
	OpenGL3Renderer(Window*);
	~OpenGL3Renderer();

	void drawSprite(OpenGL3Sprite*, const glm::vec2&, const glm::vec2&, float);
	void drawModel(const Model*);

	void beginRendering(OpenGL3Color);
	void endRendering();

	void setViewMatrix(const glm::mat4&);
	void setProjectionMatrix(const glm::mat4&);

	void bindShader(const OpenGL3Shader*);
	void bindTexture(const OpenGL3Texture*, size_t);
	
private:
	OpenGL3Renderer(OpenGL3Renderer&);

private:
	Window* m_window;

	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;
};