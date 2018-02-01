#include "OpenGL3Renderer.h"
#include <Engine\Components\Debugging\Log.h>

#include <Engine\Components\SceneManager\SceneNode.h>

OpenGL3Renderer::OpenGL3Renderer(Window* window) 
	: m_window(window),
	m_renderTarget(nullptr) 
{
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

void OpenGL3Renderer::swapBuffers() {
	glfwSwapBuffers(m_window->getWindowPointer());
}

void OpenGL3Renderer::beginRendering(Color clearColor) {
	clearCurrentRenderTarget(clearColor, ClearRenderTargetMode::ColorDepth);
}

void OpenGL3Renderer::endRendering() {
	swapBuffers();
}

void OpenGL3Renderer::addLight(Light* light) {
	m_lights.push_back(static_cast<OpenGL3Light*>(light));
}

void OpenGL3Renderer::clearCurrentRenderTarget(const Color& color, ClearRenderTargetMode mode) {
	GLint clearMode;

	if (mode == ClearRenderTargetMode::Color)
		clearMode = GL_COLOR_BUFFER_BIT;
	else if (mode == ClearRenderTargetMode::ColorDepth)
		clearMode = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

	glClearColor(color.r, color.g, color.b, 1.0f);
	glClear(clearMode);

}

void OpenGL3Renderer::setCurrentRenderTarget(Framebuffer* framebuffer) {
	m_renderTarget = static_cast<OpenGL3Framebuffer*>(framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_renderTarget->getFramebufferPointer());
}

Framebuffer* OpenGL3Renderer::getCurrentRenderTarget() const {
	return m_renderTarget;
}

void OpenGL3Renderer::resetRenderTarget() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_renderTarget = nullptr;
}

void OpenGL3Renderer::copyFramebufferDataToDefaultBuffer(Framebuffer* framebuffer) {
	int width = m_window->getViewport()->getWidth();
	int height = m_window->getViewport()->getHeight();

	glBindFramebuffer(GL_READ_FRAMEBUFFER, static_cast<OpenGL3Framebuffer*>(framebuffer)->getFramebufferPointer());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

	matrix4 mvp = m_currentCamera->getProjectionMatrix() *
		m_currentCamera->getViewMatrix() *
		subModel->getParent()->getParentSceneNode()->getTransformationMatrix();

	if (shader->hasRequiredParametersSection("transform")) {
		auto& transformParameters = shader->getRequiredParametersSection("transform");

		GpuProgramParametersSection::const_iterator it;

		it = transformParameters.find("mvp");
		if (it != transformParameters.end())
			shader->setParameter(it->second.getName(), mvp);

		it = transformParameters.find("model");
		if (it != transformParameters.end())
			shader->setParameter(it->second.getName(), subModel->getParent()->getParentSceneNode()->getTransformationMatrix());

		it = transformParameters.find("view");
		if (it != transformParameters.end())
			shader->setParameter(it->second.getName(), m_currentCamera->getViewMatrix());

		it = transformParameters.find("projection");
		if (it != transformParameters.end())
			shader->setParameter(it->second.getName(), m_currentCamera->getProjectionMatrix());
	}

	if (shader->hasRequiredParametersSection("camera")) {
		auto& cameraParameters = shader->getRequiredParametersSection("camera");

		GpuProgramParametersSection::const_iterator it;

		it = cameraParameters.find("position");
		if (it != cameraParameters.end())
			shader->setParameter(it->second.getName(), m_currentCamera->getPosition());

		it = cameraParameters.find("direction");
		if (it != cameraParameters.end())
			shader->setParameter(it->second.getName(), m_currentCamera->getFrontDirection());
	}

	drawMesh(subModel->getMesh());
}

void OpenGL3Renderer::drawMesh(const Mesh* mesh) {
	OpenGL3HardwareBuffer* geometryBuffer = static_cast<OpenGL3HardwareBuffer*>(mesh->getGeometryBuffer());

	glBindVertexArray(geometryBuffer->getVertexArrayObjectPointer());

	if (geometryBuffer->getIndicesCount() > 0) {
		glDrawElements(GL_TRIANGLES, geometryBuffer->getIndicesCount(), GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, geometryBuffer->getVerticesCount());
	}

	glBindVertexArray(0);
}

void OpenGL3Renderer::drawNDCQuad(GpuProgram* program, Framebuffer* framebuffer) {
	static GLuint quadVAO = 0;
	static GLuint quadVBO;

	bindShader(program);

	if (program->hasRequiredParametersSection("camera")) {
		auto& cameraParameters = program->getRequiredParametersSection("camera");

		GpuProgramParametersSection::const_iterator it;

		it = cameraParameters.find("position");
		if (it != cameraParameters.end())
			program->setParameter(it->second.getName(), m_currentCamera->getPosition());

		it = cameraParameters.find("direction");
		if (it != cameraParameters.end())
			program->setParameter(it->second.getName(), m_currentCamera->getFrontDirection());
	}
	
	if (framebuffer != nullptr && program->hasRequiredParametersSection("framebuffer")) {
		auto& texturesList = framebuffer->getAttachedTextures();
		auto& framebufferParameters = program->getRequiredParametersSection("framebuffer");

		GpuProgramParametersSection::const_iterator it;

		it = framebufferParameters.find("colorAttachment0");
		if (it != framebufferParameters.end()) {
			bindTexture(framebuffer->getAttachedTexture(FramebufferTextureType::Color0), 0);
			program->setParameter(it->second.getName(), 0);
		}

		it = framebufferParameters.find("colorAttachment1");
		if (it != framebufferParameters.end()) {
			bindTexture(framebuffer->getAttachedTexture(FramebufferTextureType::Color1), 1);
			program->setParameter(it->second.getName(), 1);
		}

		it = framebufferParameters.find("colorAttachment2");
		if (it != framebufferParameters.end()) {
			bindTexture(framebuffer->getAttachedTexture(FramebufferTextureType::Color2), 2);
			program->setParameter(it->second.getName(), 2);
		}
	}

	if (program->hasRequiredParametersSection("light"))
		bindShaderLights(program);

	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void OpenGL3Renderer::bindMaterial(const Material* material) {
	GpuProgram* mtlShader = material->getGpuProgram();

	bindShader(mtlShader);

	int freeTextureIndex = 0;

	if (mtlShader->hasRequiredParametersSection("material")) {
		auto& materialParameters = mtlShader->getRequiredParametersSection("material");

		for (auto& it : materialParameters) {
			if (!material->hasParameter(it.first))
				continue;

			MaterialParameter value = material->getParameter(it.first);

			const std::string& parameterType = it.second.getType();

			if (parameterType == "float")
				mtlShader->setParameter(it.second.getName(), std::get<real>(value));
			else if (parameterType == "Color")
				mtlShader->setParameter(it.second.getName(), std::get<Color>(value));
			else if (parameterType == "int")
				mtlShader->setParameter(it.second.getName(), std::get<int>(value));
			else if (parameterType == "bool")
				mtlShader->setParameter(it.second.getName(), std::get<bool>(value));
			else if (parameterType == "Texture") {
				Texture* texture = std::get<Texture*>(value);
				bindTexture(texture, freeTextureIndex);

				mtlShader->setParameter(it.second.getName(), freeTextureIndex);

				freeTextureIndex++;
			}
		}
	}

	if (mtlShader->hasRequiredParametersSection("light"))
		bindShaderLights(mtlShader);
}

void OpenGL3Renderer::bindShaderLights(GpuProgram* program) {
	auto& lightParameters = program->getRequiredParametersSection("light");

	auto& sourcesParameterIt = lightParameters.find("sources");
	if (sourcesParameterIt == lightParameters.end())
		return;

	std::string lightSourcesArrayName = sourcesParameterIt->second.getName();

	for (size_t i = 0; i < m_lights.size(); i++) {
		OpenGL3Light* light = m_lights.at(i);

		std::string lightIndex = lightSourcesArrayName + "[" + std::to_string(i) + "]";

		Color diffuse = light->getDuffuseColor();

		program->setParameter(lightIndex + ".type", static_cast<int>(light->getType()));
		program->setParameter(lightIndex + ".ambientColor", light->getAmbientColor());
		program->setParameter(lightIndex + ".diffuseColor", diffuse);
		program->setParameter(lightIndex + ".specularColor", light->getSpecularColor());

		if (light->getType() == LightType::Directional) {
			program->setParameter(lightIndex + ".direction", light->getDirection());
		}
		else if (light->getType() == LightType::Point) {
			program->setParameter(lightIndex + ".position", light->getPosition());
			program->setParameter(lightIndex + ".attenuation.constant", light->getAttenuationConstant());
			program->setParameter(lightIndex + ".attenuation.linear", light->getAttenuationLinear());
			program->setParameter(lightIndex + ".attenuation.quadratic", light->getAttenuationQuadratic());

			const float maxBrightness = std::fmaxf(std::fmaxf(diffuse.r, diffuse.g), diffuse.b);
			float radius = (-light->getAttenuationLinear() + std::sqrt(light->getAttenuationLinear() * light->getAttenuationLinear() - 4 * light->getAttenuationQuadratic() * (light->getAttenuationConstant() - (256.0f / 5.0f) * maxBrightness))) / (2.0f * light->getAttenuationQuadratic());
		
			program->setParameter(lightIndex + ".radius", radius);
		}
		else if (light->getType() == LightType::Spotlight) {
			program->setParameter(lightIndex + ".position", light->getPosition());
			program->setParameter(lightIndex + ".direction", light->getDirection());

			program->setParameter(lightIndex + ".attenuation.constant", light->getAttenuationConstant());
			program->setParameter(lightIndex + ".attenuation.linear", light->getAttenuationLinear());
			program->setParameter(lightIndex + ".attenuation.quadratic", light->getAttenuationQuadratic());

			program->setParameter(lightIndex + ".innerCutOff", glm::cos(glm::radians(light->getInnerAngle())));
			program->setParameter(lightIndex + ".outerCutOff", glm::cos(glm::radians(light->getOuterAngle())));

			const float maxBrightness = std::fmaxf(std::fmaxf(diffuse.r, diffuse.g), diffuse.b);
			float radius = (-light->getAttenuationLinear() + std::sqrt(light->getAttenuationLinear() * light->getAttenuationLinear() - 4 * light->getAttenuationQuadratic() * (light->getAttenuationConstant() - (256.0f / 5.0f) * maxBrightness))) / (2.0f * light->getAttenuationQuadratic());

			program->setParameter(lightIndex + ".radius", radius);
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

Texture* OpenGL3Renderer::createTexture(int width, int height, TextureInternalFormat internalFormat, TextureFormat format, TextureDataType type) {
	return new OpenGL3Texture(width, height, internalFormat, format, type);
}

GpuProgram* OpenGL3Renderer::createGpuProgram(const std::string& source) {
	return new OpenGL3GpuProgram(source);
}

Sprite* OpenGL3Renderer::createSprite(Texture* texture, GpuProgram* gpuProgram) {
	return new OpenGL3Sprite(texture, gpuProgram);
}

Light* OpenGL3Renderer::createLight(LightType type) {
	return new OpenGL3Light(type);
}

Framebuffer* OpenGL3Renderer::createFramebuffer() {
	return new OpenGL3Framebuffer();
}

HardwareBuffer* OpenGL3Renderer::createHardwareBuffer() {
	return new OpenGL3HardwareBuffer();
}

HardwareBuffer* OpenGL3Renderer::createHardwareBuffer(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices) {
	return new OpenGL3HardwareBuffer(vertices, indices);
}