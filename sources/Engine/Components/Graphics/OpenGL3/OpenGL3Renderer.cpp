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

	m_viewport = new Viewport(window->getWidth(), window->getHeight());
	glViewport(0, 0, m_viewport->getWidth(), m_viewport->getHeight());

	m_frameUB = new OpenGL3UniformBuffer(sizeof(matrix4) * 2 + 16 * 2);
	m_objectUB = new OpenGL3UniformBuffer(sizeof(matrix4));

	m_lightsUB = new OpenGL3UniformBuffer();

	glBindBufferBase(GL_UNIFORM_BUFFER, m_frameUBBindingPoint, m_frameUB->getPointer());
	glBindBufferBase(GL_UNIFORM_BUFFER, m_objectUBBindingPoint, m_objectUB->getPointer());
	glBindBufferBase(GL_UNIFORM_BUFFER, m_lightsUBBindingPoint, m_lightsUB->getPointer());

	m_shaderDataStorage = new OpenGL3ShaderDataStorage();
	setShaderAutobindingData("object.ubo", (int)m_objectUBBindingPoint);
	setShaderAutobindingData("frame.ubo", (int)m_frameUBBindingPoint);
	setShaderAutobindingData("lighting.ubo", (int)m_lightsUBBindingPoint);
}

OpenGL3Renderer::~OpenGL3Renderer() {
	delete m_lightsUB;
	delete m_objectUB;
	delete m_frameUB;

	delete m_shaderDataStorage;

	delete m_viewport;
}

void OpenGL3Renderer::startFrame() {
	updateLightsUB();
	updateFrameUB();
}

void OpenGL3Renderer::endFrame() {

}

void OpenGL3Renderer::updateObjectUB(const matrix4& modelMatrix) {
	m_objectUB->lock();
	m_objectUB->setDataPart(0, sizeof(matrix4), glm::value_ptr(modelMatrix));
	m_objectUB->unlock();
}

void OpenGL3Renderer::updateFrameUB() {
	m_frameUB->lock();
	m_frameUB->setDataPart(0, sizeof(matrix4), glm::value_ptr(m_currentCamera->getViewMatrix()));
	m_frameUB->setDataPart(sizeof(matrix4), sizeof(matrix4), glm::value_ptr(m_currentCamera->getProjectionMatrix()));
	m_frameUB->setDataPart(sizeof(matrix4) * 2, sizeof(vector3), glm::value_ptr(m_currentCamera->getPosition()));
	m_frameUB->setDataPart(sizeof(matrix4) * 2 + 16, sizeof(vector3), glm::value_ptr(m_currentCamera->getFrontDirection()));
	m_frameUB->unlock();
}

void OpenGL3Renderer::updateLightsUB() {
	m_lightsUB->lock();
	m_lightsUB->setData(4 + m_lightDescriptionSize * m_lights.size());

	for (size_t i = 0; i < m_lights.size(); i++) {
		Light* light = m_lights[i];

		GLintptr offset = i * m_lightDescriptionSize;

		m_lightsUB->setDataPart(offset, sizeof(matrix4), glm::value_ptr(light->getSpaceMatrix()));
		m_lightsUB->setDataPart(offset + 64, sizeof(vector3), glm::value_ptr(light->getPosition()));
		m_lightsUB->setDataPart(offset + 80, sizeof(vector3), glm::value_ptr(light->getDirection()));

		m_lightsUB->setDataPart(offset + 96, sizeof(vector3), glm::value_ptr(light->getAmbientColor()));
		m_lightsUB->setDataPart(offset + 112, sizeof(vector3), glm::value_ptr(light->getDuffuseColor()));
		m_lightsUB->setDataPart(offset + 128, sizeof(vector3), glm::value_ptr(light->getSpecularColor()));

		float innerAngle = light->getInnerAngle();
		m_lightsUB->setDataPart(offset + 140, sizeof(float), &innerAngle);

		float outerAngle = light->getOuterAngle();
		m_lightsUB->setDataPart(offset + 144, sizeof(float), &outerAngle);

		float attenuationConstant = light->getAttenuationConstant();
		m_lightsUB->setDataPart(offset + 148, sizeof(float), &attenuationConstant);

		float attenuationLinear = light->getAttenuationLinear();
		m_lightsUB->setDataPart(offset + 152, sizeof(float), &attenuationLinear);

		float attenuationQuadratic = light->getAttenuationQuadratic();
		m_lightsUB->setDataPart(offset + 156, sizeof(float), &attenuationQuadratic);

		int type = static_cast<int>(light->getType());
		m_lightsUB->setDataPart(offset + 160, sizeof(int), &type);
	}

	int lightsCount = m_lights.size();
	m_lightsUB->setDataPart(m_lights.size() * m_lightDescriptionSize, sizeof(int), &lightsCount);

	m_lightsUB->unlock();
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
	m_lights.push_back(light);
}

void OpenGL3Renderer::clearCurrentRenderTarget(const Color& color, ClearRenderTargetMode mode) {
	GLint clearMode;

	if (mode == ClearRenderTargetMode::Color)
		clearMode = GL_COLOR_BUFFER_BIT;
	else if (mode == ClearRenderTargetMode::ColorDepth)
		clearMode = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	else if (mode == ClearRenderTargetMode::Depth)
		clearMode = GL_DEPTH_BUFFER_BIT;

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

	bindTextureUnit(glSprite->getTexture(), 0);

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
			drawMesh(subModel->getMesh(), subModel->getParent()->getParentSceneNode()->getTransformationMatrix(), subModel->getMaterial());
		}
	}
}

void OpenGL3Renderer::drawMesh(const Mesh* mesh, const matrix4& transform, const Material* material) {
	if (material != nullptr) {
		bindMaterial(material);
	}

	updateObjectUB(transform);
	bindShader(material->getGpuProgram());
	bindShaderData(material->getGpuProgram());

	bindGeometryBuffer(mesh->getGeometryBuffer());

	if (mesh->getGeometryBuffer()->hasIndicesData()) {
		drawIndexedPrimitives(DrawPrimitivesMode::Triangles, mesh->getGeometryBuffer()->getIndicesCount());
	}
	else {
		drawPrimitives(DrawPrimitivesMode::Triangles, 0, mesh->getGeometryBuffer()->getVerticesCount());
	}

	unbindGeometryBuffer();
}

void OpenGL3Renderer::drawNDCQuad(GpuProgram* program) {
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

	bindShader(program);
	bindShaderData(program);

	bindGeometryBuffer(buffer);
	drawPrimitives(DrawPrimitivesMode::TrianglesStrip, 0, 4);
	unbindGeometryBuffer();
}

void OpenGL3Renderer::bindShaderData(GpuProgram* program) {
	OpenGL3GpuProgram* glProgram = static_cast<OpenGL3GpuProgram*>(program);

	int freeTextureIndex = 0;

	for (auto& parameter : glProgram->getRequiredParameters()) {
		if (!m_shaderDataStorage->hasParameter(parameter.getLocation())) {
			continue;
		}

		const std::string& type = parameter.getType();
		const GpuProgram::Parameter& passingValue = m_shaderDataStorage->getParameter(parameter.getLocation());

		if (type == "UBO") {
			glProgram->attachUniformBlock(parameter.getName(), std::get<int>(passingValue));
		}
		else {
			if (type == "float")
				glProgram->setParameter(parameter.getName(), std::get<float>(passingValue));
			else if (type == "Color")
				glProgram->setParameter(parameter.getName(), std::get<vector3>(passingValue));
			else if (type == "int")
				glProgram->setParameter(parameter.getName(), std::get<int>(passingValue));
			else if (type == "bool")
				glProgram->setParameter(parameter.getName(), std::get<bool>(passingValue));
			else if (type == "Texture" || type == "CubeMap") {
				Texture* texture = std::get<Texture*>(passingValue);
				bindTextureUnit(texture, freeTextureIndex);

				glProgram->setParameter(parameter.getName(), freeTextureIndex);

				freeTextureIndex++;
			}
		}
	}
}

void OpenGL3Renderer::bindMaterial(const Material* material) {
	for (auto& parameterIt : material->getParameters()) {
		auto [name, value] = parameterIt;
		m_shaderDataStorage->setParameter("material." + name, value);
	}
}

void OpenGL3Renderer::bindTextureUnit(const Texture* texture, size_t unit) {
	const OpenGL3Texture* glTexture = static_cast<const OpenGL3Texture*>(texture);

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(glTexture->getBindTarget(), glTexture->getTexturePointer());
}

void OpenGL3Renderer::bindShader(GpuProgram* shader) {
	glUseProgram(static_cast<const OpenGL3GpuProgram*>(shader)->getShaderPointer());
}

void OpenGL3Renderer::bindGeometryBuffer(const HardwareBuffer* buffer) {
	glBindVertexArray(static_cast<const OpenGL3HardwareBuffer*>(buffer)->getVertexArrayObjectPointer());
}

void OpenGL3Renderer::unbindGeometryBuffer() {
	glBindVertexArray(0);
}

void OpenGL3Renderer::setViewport(int width, int height) {
	m_viewport->setWidth(width);
	m_viewport->setHeight(height);

	glViewport(0, 0, width, height);
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
void OpenGL3Renderer::setShaderAutobindingData(const std::string& name, const GpuProgram::Parameter& value) {
	m_shaderDataStorage->setParameter(name, value);
}