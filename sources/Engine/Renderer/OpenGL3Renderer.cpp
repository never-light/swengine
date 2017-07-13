#include "OpenGL3Renderer.h"
#include "OpenGL3Window.h"
#include "../utils/utils.h"

OpenGL3Renderer::OpenGL3Renderer(Window* window) : m_window(window) {

}

OpenGL3Renderer::~OpenGL3Renderer() {

}

void OpenGL3Renderer::beginRendering(Color clearColor) {
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGL3Renderer::endRendering() {
	glfwSwapBuffers(m_window->getWindowPointer());
}

void OpenGL3Renderer::drawSprite(Sprite* sprite) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sprite->getTexture()->getTexturePointer());

	glUseProgram(sprite->getShader()->getShaderPointer());
	sprite->getShader()->setInteger("spriteTexture", 0);
	sprite->getShader()->setMatrix4("projection", m_projectionMatrix);
	sprite->getShader()->setMatrix4("model", sprite->getTransformationMatrix());

	glBindVertexArray(sprite->getVertexArrayPointer());
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void OpenGL3Renderer::setViewMatrix(const glm::mat4& viewMatrix) {
	m_viewMatrix = viewMatrix;
}

void OpenGL3Renderer::setProjectionMatrix(const glm::mat4& projectionMatrix) {
	m_projectionMatrix = projectionMatrix;
}