#include "OpenGL3Renderer.h"
#include <Engine\Components\Debugging\Log.h>

#include <Engine\Components\SceneManager\SceneNode.h>

OpenGL3Renderer::OpenGL3Renderer(Window* window) : m_window(window) {
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
	}
}

OpenGL3Renderer::~OpenGL3Renderer() {

}

Window* OpenGL3Renderer::getWindow() const {
	return m_window;
}

void OpenGL3Renderer::setCurrentCamera(Camera* camera) {
	m_currentCamera = camera;
}

Camera* OpenGL3Renderer::getCurrentCamera() const {
	return m_currentCamera;
}

void OpenGL3Renderer::beginRendering(Color clearColor) {
	glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGL3Renderer::endRendering() {
	glfwSwapBuffers(m_window->getWindowPointer());
}

void OpenGL3Renderer::addLight(Light* light) {
	m_lights.push_back(static_cast<OpenGL3Light*>(light));
}

void OpenGL3Renderer::drawSprite(Sprite* sprite, const glm::vec2& position, const glm::vec2& size, float rotation) {
	OpenGL3Sprite* glSprite = static_cast<OpenGL3Sprite*>(sprite);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glm::mat4 transformationMatrix;
	transformationMatrix = glm::translate(transformationMatrix, glm::vec3(position, 0.0f));
	transformationMatrix = glm::rotate(transformationMatrix, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	transformationMatrix = glm::scale(transformationMatrix, glm::vec3(size, 1.0f));

	bindTexture(glSprite->getTexture(), 0);

	glSprite->getShader()->setParameter("spriteTexture", 0);
	glSprite->getShader()->setParameter("projection", m_currentCamera->getProjectionMatrix());
	glSprite->getShader()->setParameter("model", transformationMatrix);
	bindShader(glSprite->getShader());

	glBindVertexArray(glSprite->getVertexArrayPointer());
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

	GpuProgram* shader = subModel->getMaterial()->getGpuProgram();
	shader->setParameter("model", subModel->getParent()->getParentSceneNode()->getTransformationMatrix());
	shader->setParameter("view", m_currentCamera->getViewMatrix());
	shader->setParameter("projection", m_currentCamera->getProjectionMatrix());
	shader->setParameter("cameraPosition", m_currentCamera->getPosition());

	drawMesh(subModel->getMesh());
}

void OpenGL3Renderer::drawMesh(const Mesh* mesh) {
	const OpenGL3Mesh* glMesh = static_cast<const OpenGL3Mesh*>(mesh);

	if (!glMesh->hasPreparedVertexData()) {
		return;
	}

	glBindVertexArray(glMesh->getVertexArrayObjectPointer());

	if (glMesh->getIndicesCount() > 0) {
		glDrawElements(GL_TRIANGLES, glMesh->getIndicesCount(), GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, glMesh->getVerticesCount());
	}

	glBindVertexArray(0);
}

void OpenGL3Renderer::bindMaterial(const Material* material) {
	OpenGL3GpuProgram* mtlShader = static_cast<OpenGL3GpuProgram*>(material->getGpuProgram());

	bindShader(mtlShader);

	if (material->hasLightmap(LightmapType::Diffuse)) {
		bindTexture(material->getLightmap(LightmapType::Diffuse), 0);
		mtlShader->setParameter("material.lightmaps.diffuse", 0);
		mtlShader->setParameter("material.lightmaps.useDiffuseMap", 1);
	}
	else {
		mtlShader->setParameter("material.diffuseColor", material->getDuffuseColor());
		mtlShader->setParameter("material.ambientColor", material->getAmbientColor());
		mtlShader->setParameter("material.lightmaps.useDiffuseMap", 0);
	}

	if (material->hasLightmap(LightmapType::Specular)) {
		bindTexture(material->getLightmap(LightmapType::Specular), 1);
		mtlShader->setParameter("material.lightmaps.specular", 1);
		mtlShader->setParameter("material.lightmaps.useSpecularMap", true);
	}
	else {
		mtlShader->setParameter("material.specularColor", material->getSpecularColor());
		mtlShader->setParameter("material.lightmaps.useSpecularMap", false);
	}

	mtlShader->setParameter("material.shininess", material->getShininess());

	for (size_t i = 0; i < m_lights.size(); i++) {
		OpenGL3Light* light = m_lights.at(i);

		std::string lightIndex = "lights[" + std::to_string(i) + "]";

		mtlShader->setParameter(lightIndex + ".type", static_cast<int>(light->getType()));
		mtlShader->setParameter(lightIndex + ".ambientColor", light->getAmbientColor());
		mtlShader->setParameter(lightIndex + ".diffuseColor", light->getDuffuseColor());
		mtlShader->setParameter(lightIndex + ".specularColor", light->getSpecularColor());

		if (light->getType() == LightType::Directional) {
			mtlShader->setParameter(lightIndex + ".direction", light->getDirection());
		}
		else if (light->getType() == LightType::Point) {
			mtlShader->setParameter(lightIndex + ".position", light->getPosition());
			mtlShader->setParameter(lightIndex + ".attenuation.constant", light->getAttenuationConstant());
			mtlShader->setParameter(lightIndex + ".attenuation.linear", light->getAttenuationLinear());
			mtlShader->setParameter(lightIndex + ".attenuation.quadratic", light->getAttenuationQuadratic());
		}
		else if (light->getType() == LightType::Spotlight) {
			mtlShader->setParameter(lightIndex + ".position", light->getPosition());
			mtlShader->setParameter(lightIndex + ".direction", light->getDirection());

			mtlShader->setParameter(lightIndex + ".attenuation.constant", light->getAttenuationConstant());
			mtlShader->setParameter(lightIndex + ".attenuation.linear", light->getAttenuationLinear());
			mtlShader->setParameter(lightIndex + ".attenuation.quadratic", light->getAttenuationQuadratic());

			mtlShader->setParameter(lightIndex + ".innerCutOff", glm::cos(glm::radians(light->getInnerAngle())));
			mtlShader->setParameter(lightIndex + ".outerCutOff", glm::cos(glm::radians(light->getOuterAngle())));
		}
	}
}

void OpenGL3Renderer::bindTexture(const Texture* texture, size_t unit) {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, static_cast<const OpenGL3Texture*>(texture)->getTexturePointer());
}

void OpenGL3Renderer::bindShader(const GpuProgram* shader) {
	glUseProgram(static_cast<const OpenGL3GpuProgram*>(shader)->getShaderPointer());
}

Material* OpenGL3Renderer::createMaterial() {
	return new OpenGL3Material();
}

Texture* OpenGL3Renderer::createTexture(int width, int height, const unsigned char* data) {
	return new OpenGL3Texture(width, height, data);
}

GpuProgram* OpenGL3Renderer::createGpuProgram(const std::string& source) {
	return new OpenGL3GpuProgram(source);
}

Sprite* OpenGL3Renderer::createSprite(Texture* texture, GpuProgram* gpuProgram) {
	return new OpenGL3Sprite(texture, gpuProgram);
}

Mesh* OpenGL3Renderer::createMesh() {
	return new OpenGL3Mesh();
}