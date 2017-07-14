#pragma once

#include "OpenGL3Color.h"
#include "OpenGL3Window.h"
#include "OpenGL3Sprite.h"
#include "Math.h"

class OpenGL3Renderer {
public:
	OpenGL3Renderer(Window*);
	~OpenGL3Renderer();

	void drawSprite(Sprite*, const glm::vec2&, const glm::vec2&, float);
	void beginRendering(Color);
	void endRendering();

	void setViewMatrix(const glm::mat4&);
	void setProjectionMatrix(const glm::mat4&);
private:
	OpenGL3Renderer(OpenGL3Renderer&);

private:
	Window* m_window;

	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;
};

typedef OpenGL3Renderer Renderer;