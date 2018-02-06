#include "OpenGL3Renderer.h"
#include <Engine\Components\Debugging\Log.h>

#include <Engine\Components\SceneManager\SceneNode.h>
#include <Engine\ServiceLocator.h>

const std::unordered_map<Renderer::Option, GLenum> OpenGL3Renderer::m_enablingOptions{
	{ Renderer::Option::DepthTest, GL_DEPTH_TEST },
	{ Renderer::Option::FaceCulling, GL_CULL_FACE },
	{ Renderer::Option::MultiSample, GL_MULTISAMPLE }
};

const std::unordered_map<Renderer::OptionValue, GLenum> OpenGL3Renderer::m_optionsValues{
	{ Renderer::OptionValue::Less, GL_LESS },
	{ Renderer::OptionValue::LessEqual, GL_LEQUAL },
	{ Renderer::OptionValue::Front, GL_FRONT },
	{ Renderer::OptionValue::Back, GL_BACK },
	{ Renderer::OptionValue::FrontAndBack, GL_FRONT_AND_BACK },
	{ Renderer::OptionValue::Clockwise, GL_CW },
	{ Renderer::OptionValue::CounterClockwise, GL_CCW },
};

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

void OpenGL3Renderer::drawPrimitives(DrawPrimitivesMode mode, size_t start, size_t count) {
	GLenum drawMode;

	if (mode == DrawPrimitivesMode::Triangles)
		drawMode = GL_TRIANGLES;
	else if (mode == DrawPrimitivesMode::TrianglesStrip)
		drawMode = GL_TRIANGLE_STRIP;

	glDrawArrays(drawMode, start, count);
}

void OpenGL3Renderer::drawIndexedPrimitives(DrawPrimitivesMode mode, size_t count) {
	GLenum drawMode;

	if (mode == DrawPrimitivesMode::Triangles)
		drawMode = GL_TRIANGLES;
	else if (mode == DrawPrimitivesMode::TrianglesStrip)
		drawMode = GL_TRIANGLE_STRIP;

	glDrawElements(drawMode, count, GL_UNSIGNED_INT, 0);
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
	drawMesh(subModel->getMesh(), subModel->getParent()->getParentSceneNode()->getTransformationMatrix(), subModel->getMaterial());
}

void OpenGL3Renderer::drawMesh(const Mesh* mesh, const matrix4& transform, const Material* material) {
	m_cachedWorldMatrix = transform;
	bindMaterial(material);

	bindGeometryBuffer(mesh->getGeometryBuffer());

	if (mesh->getGeometryBuffer()->hasIndicesData()) {
		drawIndexedPrimitives(DrawPrimitivesMode::Triangles, mesh->getGeometryBuffer()->getIndicesCount());
	}
	else {
		drawPrimitives(DrawPrimitivesMode::Triangles, 0, mesh->getGeometryBuffer()->getVerticesCount());
	}

	unbindGeometryBuffer();
}

void OpenGL3Renderer::drawNDCQuad(GpuProgram* program, Framebuffer* framebuffer) {
	static OpenGL3HardwareBuffer* buffer = nullptr;

	if (buffer == nullptr) {
		std::vector<VertexP1UV> vertices {
			{ vector3(-1.0f,  1.0f, 0.0f), vector2(0.0f, 1.0f) },
			{ vector3(-1.0f, -1.0f, 0.0f), vector2(0.0f, 0.0f) },
			{ vector3(1.0f,  1.0f, 0.0f), vector2(1.0f, 1.0f) },
			{ vector3(1.0f, -1.0f, 0.0f), vector2(1.0f, 0.0f) }
		};

		buffer = new OpenGL3HardwareBuffer();
		buffer->lock();
		buffer->setVertexFormat(VertexFormat::P1UV);
		buffer->setVerticesData(vertices.size(), sizeof(VertexP1UV), vertices.data());
		buffer->unlock();
	}

	bindShader(program, framebuffer, nullptr);

	bindGeometryBuffer(buffer);
	drawPrimitives(DrawPrimitivesMode::TrianglesStrip, 0, 4);
	unbindGeometryBuffer();
}

void OpenGL3Renderer::bindMaterial(const Material* material) {
	bindShader(material->getGpuProgram(), nullptr, material);
}

void OpenGL3Renderer::bindShader(GpuProgram* program, const Framebuffer* framebuffer, const Material* material) {
	glUseProgram(static_cast<const OpenGL3GpuProgram*>(program)->getShaderPointer());

	bindShaderData(program, framebuffer, material);
}

void OpenGL3Renderer::bindShaderData(
	GpuProgram* program, 
	const Framebuffer* framebuffer, 
	const Material* material
) {
	if (program->hasRequiredParametersSection("transform")) {
		matrix4 mvp = m_currentCamera->getProjectionMatrix() *
			m_currentCamera->getViewMatrix() *
			m_cachedWorldMatrix;

		auto& transformParameters = program->getRequiredParametersSection("transform");

		GpuProgramParametersSection::const_iterator it;

		it = transformParameters.find("mvp");
		if (it != transformParameters.end())
			program->setParameter(it->second.getName(), mvp);

		it = transformParameters.find("model");
		if (it != transformParameters.end())
			program->setParameter(it->second.getName(), m_cachedWorldMatrix);

		it = transformParameters.find("view");
		if (it != transformParameters.end())
			program->setParameter(it->second.getName(), m_currentCamera->getViewMatrix());

		it = transformParameters.find("projection");
		if (it != transformParameters.end())
			program->setParameter(it->second.getName(), m_currentCamera->getProjectionMatrix());
	}

	if (framebuffer != nullptr && program->hasRequiredParametersSection("framebuffer")) {
		auto& framebufferParameters = program->getRequiredParametersSection("framebuffer");

		GpuProgramParametersSection::const_iterator it;

		it = framebufferParameters.find("colorAttachment0");
		if (it != framebufferParameters.end()) {
			bindTexture(framebuffer->getTextureAttachment(Framebuffer::Attachment::Color0), 0);
			program->setParameter(it->second.getName(), 0);
		}

		it = framebufferParameters.find("colorAttachment1");
		if (it != framebufferParameters.end()) {
			bindTexture(framebuffer->getTextureAttachment(Framebuffer::Attachment::Color1), 1);
			program->setParameter(it->second.getName(), 1);
		}

		it = framebufferParameters.find("colorAttachment2");
		if (it != framebufferParameters.end()) {
			bindTexture(framebuffer->getTextureAttachment(Framebuffer::Attachment::Color2), 2);
			program->setParameter(it->second.getName(), 2);
		}
	}

	if (program->hasRequiredParametersSection("config")) {
		auto& configParameters = program->getRequiredParametersSection("config");

		for (const auto& it : configParameters) {
			if (it.second.getType() == "int")
				program->setParameter(
					it.second.getName(),
					ServiceLocator::getConfigManager()->getOption<int>(it.first)
				);
			else if (it.second.getType() == "float")
				program->setParameter(
					it.second.getName(),
					ServiceLocator::getConfigManager()->getOption<real>(it.first)
				);
		}
	}

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

	if (material != nullptr && program->hasRequiredParametersSection("material")) {
		auto& materialParameters = program->getRequiredParametersSection("material");

		int freeTextureIndex = 0;

		for (auto& it : materialParameters) {
			if (!material->hasParameter(it.first))
				continue;

			MaterialParameter value = material->getParameter(it.first);

			const std::string& parameterType = it.second.getType();

			if (parameterType == "float")
				program->setParameter(it.second.getName(), std::get<real>(value));
			else if (parameterType == "Color")
				program->setParameter(it.second.getName(), std::get<Color>(value));
			else if (parameterType == "int")
				program->setParameter(it.second.getName(), std::get<int>(value));
			else if (parameterType == "bool")
				program->setParameter(it.second.getName(), std::get<bool>(value));
			else if (parameterType == "Texture" || parameterType == "CubeMap") {
				Texture* texture = std::get<Texture*>(value);
				bindTexture(texture, freeTextureIndex);

				program->setParameter(it.second.getName(), freeTextureIndex);

				freeTextureIndex++;
			}
		}
	}

	if (program->hasRequiredParametersSection("light")) {
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
}

void OpenGL3Renderer::bindTexture(const Texture* texture, size_t unit) {
	const OpenGL3Texture* glTexture = static_cast<const OpenGL3Texture*>(texture);

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(glTexture->getBindTarget(), glTexture->getTexturePointer());
}

void OpenGL3Renderer::bindShader(GpuProgram* shader) {
	bindShader(shader, nullptr, nullptr);
}

void OpenGL3Renderer::bindGeometryBuffer(const HardwareBuffer* buffer) {
	glBindVertexArray(static_cast<const OpenGL3HardwareBuffer*>(buffer)->getVertexArrayObjectPointer());
}

void OpenGL3Renderer::unbindGeometryBuffer() {
	glBindVertexArray(0);
}

void OpenGL3Renderer::setOption(Renderer::Option option, Renderer::OptionValue value) {
	auto optionIt = m_enablingOptions.find(option);

	if (optionIt != m_enablingOptions.end()) {
		if (value == Renderer::OptionValue::Enabled)
			glEnable(optionIt->second);
		else if (value == Renderer::OptionValue::Disabled)
			glDisable(optionIt->second);
	}
	else {
		if (option == Renderer::Option::DepthFunction)
			glDepthFunc(m_optionsValues.at(value));
		else if (option == Renderer::Option::FaceCullingMode)
			glCullFace(m_optionsValues.at(value));
		else if (option == Renderer::Option::FrontFace)
			glFrontFace(m_optionsValues.at(value));
	}
}