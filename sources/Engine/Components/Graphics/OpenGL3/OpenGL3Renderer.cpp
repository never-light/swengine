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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

	bindTexture(sprite->getTexture(), 0);

	sprite->getShader()->setInteger("spriteTexture", 0);
	sprite->getShader()->setMatrix4("projection", m_projectionMatrix);
	sprite->getShader()->setMatrix4("model", transformationMatrix);
	bindShader(sprite->getShader());

	glBindVertexArray(sprite->getVertexArrayPointer());
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void OpenGL3Renderer::drawModel(const Model* model) {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	bindTexture(model->getTexture(), 0);

	model->getShader()->setMatrix4("model", model->getTransformationMatrix());
	model->getShader()->setMatrix4("view", m_viewMatrix);
	model->getShader()->setMatrix4("projection", m_projectionMatrix);
	model->getShader()->setInteger("tex", 0);

	bindShader(model->getShader());

	glBindVertexArray(model->getMesh()->getVertexArrayObjectPointer());

	if (model->getMesh()->getIndicesCount() > 0) {
		glDrawElements(GL_TRIANGLES, model->getMesh()->getIndicesCount(), GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, model->getMesh()->getVerticesCount());
	}

	glBindVertexArray(0);
}

void OpenGL3Renderer::bindTexture(const OpenGL3Texture* texture, size_t unit) {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, texture->getTexturePointer());
}

void OpenGL3Renderer::bindShader(const OpenGL3Shader* shader) {
	glUseProgram(shader->getShaderPointer());
}

void OpenGL3Renderer::setViewMatrix(const glm::mat4& viewMatrix) {
	m_viewMatrix = viewMatrix;
}

void OpenGL3Renderer::setProjectionMatrix(const glm::mat4& projectionMatrix) {
	m_projectionMatrix = projectionMatrix;
}