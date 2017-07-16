#include "OpenGL3Renderer.h"
#include <Engine\Components\Debugging\Log.h>

OpenGL3Renderer::OpenGL3Renderer(Window* window) : m_window(window) {
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
	}
}

OpenGL3Renderer::~OpenGL3Renderer() {

}

void OpenGL3Renderer::beginRendering(OpenGL3Color clearColor) {
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGL3Renderer::endRendering() {
	glfwSwapBuffers(m_window->getWindowPointer());
}

void OpenGL3Renderer::drawSprite(OpenGL3Sprite* sprite, const glm::vec2& position, const glm::vec2& size, float rotation) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glm::mat4 transformationMatrix;
	transformationMatrix = glm::translate(transformationMatrix, glm::vec3(position, 0.0f));
	transformationMatrix = glm::rotate(transformationMatrix, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	transformationMatrix = glm::scale(transformationMatrix, glm::vec3(size, 1.0f));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sprite->getTexture()->getTexturePointer());

	glUseProgram(sprite->getShader()->getShaderPointer());
	sprite->getShader()->setInteger("spriteTexture", 0);
	sprite->getShader()->setMatrix4("projection", m_projectionMatrix);
	sprite->getShader()->setMatrix4("model", transformationMatrix);

	glBindVertexArray(sprite->getVertexArrayPointer());
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void OpenGL3Renderer::setViewMatrix(const glm::mat4& viewMatrix) {
	m_viewMatrix = viewMatrix;
}

void OpenGL3Renderer::setProjectionMatrix(const glm::mat4& projectionMatrix) {
	m_projectionMatrix = projectionMatrix;
}