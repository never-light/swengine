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

void OpenGL3Renderer::setCurrentCamera(OpenGL3Camera* camera) {
	m_currentCamera = camera;
}

OpenGL3Camera* OpenGL3Renderer::getCurrentCamera() const {
	return m_currentCamera;
}

void OpenGL3Renderer::beginRendering(OpenGL3Color clearColor) {
	glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGL3Renderer::endRendering() {
	glfwSwapBuffers(m_window->getWindowPointer());
}

void OpenGL3Renderer::addLight(OpenGL3Light* light) {
	m_lights.push_back(light);
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
	sprite->getShader()->setMatrix4("projection", m_currentCamera->getProjectionMatrix());
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

	if (model->countSubModels()) {
		for (const SubModel* subModel : model->getSubModels()) {
			drawSubModel(subModel);
		}
	}
}

void OpenGL3Renderer::drawSubModel(const SubModel* subModel) {
	bindMaterial(subModel->getMaterial());

	Shader* shader = subModel->getMaterial()->getShader();
	shader->setParameter<matrix4>("model", subModel->getParent()->getTransformationMatrix());
	shader->setParameter<matrix4>("view", m_currentCamera->getViewMatrix());
	shader->setParameter<matrix4>("projection", m_currentCamera->getProjectionMatrix());
	shader->setParameter<vector3>("cameraPosition", m_currentCamera->getPosition());

	drawMesh(subModel->getMesh());
}

void OpenGL3Renderer::drawMesh(const Mesh* mesh) {
	glBindVertexArray(mesh->getVertexArrayObjectPointer());

	if (mesh->getIndicesCount() > 0) {
		glDrawElements(GL_TRIANGLES, mesh->getIndicesCount(), GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, mesh->getVerticesCount());
	}

	glBindVertexArray(0);
}

void OpenGL3Renderer::bindMaterial(const OpenGL3Material* material) {
	OpenGL3Shader* mtlShader = material->getShader();

	bindShader(mtlShader);

	if (material->isLightmapExists(LightmapType::Diffuse)) {
		bindTexture(material->getLightmap(LightmapType::Diffuse), 0);
		mtlShader->setParameter<LightmapId>("material.lightmaps.diffuse", 0);
		mtlShader->setParameter<int>("material.lightmaps.useDiffuseMap", 1);
	}
	else {
		mtlShader->setParameter<Color>("material.diffuseColor", material->getDuffuseColor());
		mtlShader->setParameter<Color>("material.ambientColor", material->getAmbientColor());
		mtlShader->setParameter<int>("material.lightmaps.useDiffuseMap", 0);
	}

	if (material->isLightmapExists(LightmapType::Specular)) {
		bindTexture(material->getLightmap(LightmapType::Specular), 1);
		mtlShader->setParameter<LightmapId>("material.lightmaps.specular", 1);
		mtlShader->setParameter<bool>("material.lightmaps.useSpecularMap", true);
	}
	else {
		mtlShader->setParameter<Color>("material.specularColor", material->getSpecularColor());
		mtlShader->setParameter<bool>("material.lightmaps.useSpecularMap", false);
	}

	mtlShader->setParameter<float32>("material.shininess", material->getShininess());

	for (size_t i = 0; i < m_lights.size(); i++) {
		OpenGL3Light* light = m_lights.at(i);

		std::string lightIndex = "lights[" + std::to_string(i) + "]";

		mtlShader->setParameter<int>(lightIndex + ".type", static_cast<int>(light->getType()));
		mtlShader->setParameter<Color>(lightIndex + ".ambientColor", light->getAmbientColor());
		mtlShader->setParameter<Color>(lightIndex + ".diffuseColor", light->getDuffuseColor());
		mtlShader->setParameter<Color>(lightIndex + ".specularColor", light->getSpecularColor());

		if (light->getType() == LightType::Directional) {
			auto directionalLight = dynamic_cast<OpenGL3DirectionalLight*>(light);

			mtlShader->setParameter<vector3>(lightIndex + ".direction", directionalLight->getDirection());
		}
		else if (light->getType() == LightType::Point) {
			auto pointLight = dynamic_cast<OpenGL3PointLight*>(light);

			mtlShader->setParameter<vector3>(lightIndex + ".position", pointLight->getPosition());
			mtlShader->setParameter<float32>(lightIndex + ".attenuation.constant", pointLight->getAttenuationConstant());
			mtlShader->setParameter<float32>(lightIndex + ".attenuation.linear", pointLight->getAttenuationLinear());
			mtlShader->setParameter<float32>(lightIndex + ".attenuation.quadratic", pointLight->getAttenuationQuadratic());
		}
		else if (light->getType() == LightType::Spotlight) {
			auto spotlight = dynamic_cast<OpenGL3Spotlight*>(light);

			mtlShader->setParameter<vector3>(lightIndex + ".position", spotlight->getPosition());
			mtlShader->setParameter<vector3>(lightIndex + ".direction", spotlight->getDirection());

			mtlShader->setParameter<float32>(lightIndex + ".attenuation.constant", spotlight->getAttenuationConstant());
			mtlShader->setParameter<float32>(lightIndex + ".attenuation.linear", spotlight->getAttenuationLinear());
			mtlShader->setParameter<float32>(lightIndex + ".attenuation.quadratic", spotlight->getAttenuationQuadratic());

			mtlShader->setParameter<float32>(lightIndex + ".innerCutOff", glm::cos(glm::radians(spotlight->getInnerAngle())));
			mtlShader->setParameter<float32>(lightIndex + ".outerCutOff", glm::cos(glm::radians(spotlight->getOuterAngle())));
		}
	}
}

void OpenGL3Renderer::bindTexture(const OpenGL3Texture* texture, size_t unit) {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, texture->getTexturePointer());
}

void OpenGL3Renderer::bindShader(const OpenGL3Shader* shader) {
	glUseProgram(shader->getShaderPointer());
}