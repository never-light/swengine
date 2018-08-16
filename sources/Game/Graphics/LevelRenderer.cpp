#include "LevelRenderer.h"

#include <algorithm>

#include <Engine\assertions.h>

LevelRenderer::LevelRenderer(GraphicsContext * graphicsContext,
	GraphicsResourceFactory * graphicsResourceFactory,
	GpuProgram* deferredLightingProgram)
	: m_graphicsContext(graphicsContext),
	m_graphicsResourceFactory(graphicsResourceFactory),
	m_deferredLightingProgram(deferredLightingProgram),
	m_ndcQuad(new NDCQuadPrimitive(graphicsResourceFactory)),
	m_sphere(new SpherePrimitive(graphicsResourceFactory))
{
	initializeRenderTarget();

	enableGammaCorrection();
	setGamma(2.2);
}

LevelRenderer::~LevelRenderer()
{
	delete m_ndcQuad;
	delete m_sphere;

	delete m_gBufferAlbedo;
	delete m_gBufferNormals;
	delete m_gBufferPosition;
	delete m_gBufferUV;

	delete m_gBufferTarget;
}

void LevelRenderer::registerLightSource(Light * lightSource)
{
	_assert(lightSource->getId() == -1);

	lightSource->setId(m_lightsSources.size());
	m_lightsSources.push_back(lightSource);

	updateLightSource(lightSource);
}

void LevelRenderer::updateLightSource(const Light * lightSource)
{
	_assert(lightSource->getId() != -1);

	passLightSourceDataToGpuProgram(lightSource->getId(), lightSource, m_deferredLightingProgram);
}

void LevelRenderer::removeLightSource(const Light * lightSource)
{
	m_lightsSources.erase(std::remove(m_lightsSources.begin(), m_lightsSources.end(), lightSource), m_lightsSources.end());
}

void LevelRenderer::setActiveCamera(const Camera * camera)
{
	m_activeCamera = camera;
}

void LevelRenderer::prepareBaseMaterials()
{
	for (const BaseMaterial* baseMaterial : m_baseMaterials) {
		GpuProgram* gpuProgram = baseMaterial->getGpuProgram();
		gpuProgram->bind();

		if (baseMaterial->isTransformsDataRequired()) {
			gpuProgram->setParameter("scene.viewTransform", m_activeCamera->getViewMatrix());
			gpuProgram->setParameter("scene.projectionTransform", m_activeCamera->getProjectionMatrix());
		}

		if (baseMaterial->isLightsDataRequired()) {
			// TODO: pass lights data to GPU program
		}
	}
}

void LevelRenderer::render()
{
	prepareBaseMaterials();

	m_graphicsContext->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	m_graphicsContext->clear(RenderTarget::CLEAR_COLOR | RenderTarget::CLEAR_DEPTH);

	m_graphicsContext->enableWritingToDepthBuffer();
	m_graphicsContext->enableDepthTest();

	m_graphicsContext->enableFaceCulling();
	m_graphicsContext->setFaceCullingMode(GraphicsContext::FaceCullingMode::Back);

	m_gBufferTarget->bind();
	m_gBufferTarget->clear(RenderTarget::CLEAR_COLOR | RenderTarget::CLEAR_DEPTH);


	BaseMaterial* currentBaseMaterial = nullptr;

	for (Renderable* renderableObject : m_renderableObjects) {
		if (currentBaseMaterial != renderableObject->getBaseMaterial()) {
			currentBaseMaterial = renderableObject->getBaseMaterial();
			currentBaseMaterial->bind();

			const GraphicsPipelineState& requiredPipelineState =
				currentBaseMaterial->getRequiredGraphicsPipelineState();

			// TODO: change pipeline state if needed through graphics context
		}

		renderableObject->render();
	}

	//showGBuffer();
	m_gBufferTarget->unbind();

	m_graphicsContext->disableWritingToDepthBuffer();
	m_graphicsContext->disableDepthTest();

	// Lighting passes
	m_deferredLightingProgram->bind();
	m_deferredLightingProgram->setParameter("g_cameraPosition", m_activeCamera->getTransform()->getPosition());

	m_gBufferPosition->bind(POSITION_BUFFER_INDEX);
	m_gBufferAlbedo->bind(ALBEDO_BUFFER_INDEX);
	m_gBufferNormals->bind(NORMALS_BUFFER_INDEX);
	m_gBufferUV->bind(UV_BUFFER_INDEX);

	m_deferredLightingProgram->setParameter("g_position", (int)POSITION_BUFFER_INDEX);
	m_deferredLightingProgram->setParameter("g_albedo", (int)ALBEDO_BUFFER_INDEX);
	m_deferredLightingProgram->setParameter("g_normal", (int)NORMALS_BUFFER_INDEX);
	m_deferredLightingProgram->setParameter("g_uv", (int)UV_BUFFER_INDEX);

	m_ndcQuad->render();

	m_deferredLightingProgram->unbind();
}

void LevelRenderer::showGBuffer()
{
	unsigned int viewportWidth = m_graphicsContext->getViewportWidth();
	unsigned int viewportHeight = m_graphicsContext->getViewportHeight();

	unsigned int halfWidth = viewportWidth / 2;
	unsigned int halfHeight = viewportHeight / 2;

	m_gBufferTarget->copyColorComponentData(ALBEDO_BUFFER_INDEX, nullptr, 0,
		Rect(0, 0, viewportWidth, viewportHeight), Rect(0, 0, halfWidth, halfHeight),
		RenderTarget::CopyFilter::Linear);

	m_gBufferTarget->copyColorComponentData(NORMALS_BUFFER_INDEX, nullptr, 0,
		Rect(0, 0, viewportWidth, viewportHeight), Rect(0, halfHeight, halfWidth, halfHeight),
		RenderTarget::CopyFilter::Linear);

	m_gBufferTarget->copyColorComponentData(POSITION_BUFFER_INDEX, nullptr, 0,
		Rect(0, 0, viewportWidth, viewportHeight), Rect(halfWidth, halfHeight, halfWidth, halfHeight),
		RenderTarget::CopyFilter::Linear);

	m_gBufferTarget->copyColorComponentData(UV_BUFFER_INDEX, nullptr, 0,
		Rect(0, 0, viewportWidth, viewportHeight), Rect(halfWidth, 0, halfWidth, halfHeight),
		RenderTarget::CopyFilter::Linear);
}

float LevelRenderer::calculateLightSourceSphereRadius(const Light * light) const
{
	vector3 lightColor = light->getColor();

	float maxChannel = fmax(fmax(lightColor.r, lightColor.y), lightColor.z);
	float attenuationLinear = light->getAttenuationLinear();
	float attenuationExp = light->getAttenuationQuadratic();

	return (-attenuationLinear + std::sqrtf(attenuationLinear * attenuationLinear - 4 * attenuationExp * (attenuationExp - (256.0 / 5.0) * maxChannel)))
		/ (2 * attenuationExp);

}

void LevelRenderer::passLightSourceDataToGpuProgram(size_t index, const Light* light, GpuProgram* gpuProgram) const {
	gpuProgram->bind();

	std::string item = "g_lights[" + std::to_string(index) + "]";

	gpuProgram->setParameter(item + ".parameters.color", light->getColor());
	gpuProgram->setParameter(item + ".parameters.ambientIntensity", light->getAmbientIntensity());
	gpuProgram->setParameter(item + ".parameters.diffuseIntensity", light->getDiffuseIntensity());

	gpuProgram->setParameter(item + ".attenuation.constant", light->getAttenuationConstant());
	gpuProgram->setParameter(item + ".attenuation.linear", light->getAttenuationLinear());
	gpuProgram->setParameter(item + ".attenuation.exponent", light->getAttenuationQuadratic());

	gpuProgram->setParameter(item + ".position", light->getPosition());

	float boundingRadius = calculateLightSourceSphereRadius(light);
	gpuProgram->setParameter(item + ".boundingRadius", boundingRadius);
}

void LevelRenderer::registerBaseMaterial(BaseMaterial * baseMaterial)
{
	m_baseMaterials.push_back(baseMaterial);
}

void LevelRenderer::addRenderableObject(Renderable * object)
{
	m_renderableObjects.push_back(object);
}

void LevelRenderer::removeRenderableObject(Renderable * object)
{
	m_renderableObjects.erase(std::remove(m_renderableObjects.begin(), m_renderableObjects.end(), object),
		m_renderableObjects.end());
}

void LevelRenderer::enableGammaCorrection()
{
	m_isGammaCorrectionEnabled = true;

	m_deferredLightingProgram->bind();
	m_deferredLightingProgram->setParameter("g_isGammaCorrectionEnabled", true);
}

void LevelRenderer::disableGammaCorrection()
{
	m_isGammaCorrectionEnabled = false;

	m_deferredLightingProgram->bind();
	m_deferredLightingProgram->setParameter("g_isGammaCorrectionEnabled", false);
}

bool LevelRenderer::isGammaCorrectionEnabled()
{
	return m_isGammaCorrectionEnabled;
}

void LevelRenderer::setGamma(float gamma)
{
	m_gamma = gamma;

	m_deferredLightingProgram->bind();
	m_deferredLightingProgram->setParameter("g_gamma", m_gamma);
}

float LevelRenderer::getGamma() const
{
	return m_gamma;
}

void LevelRenderer::initializeRenderTarget()
{
	m_gBufferAlbedo = createGBufferColorTexture();
	m_gBufferNormals = createGBufferColorTexture();
	m_gBufferPosition = createGBufferColorTexture();
	m_gBufferUV = createGBufferColorTexture();

	m_gBufferDepthStencil = createGBufferDepthStencilTexture();

	m_gBufferTarget = m_graphicsResourceFactory->createRenderTarget();
	m_gBufferTarget->create();

	m_gBufferTarget->bind();
	m_gBufferTarget->attachColorComponent(ALBEDO_BUFFER_INDEX, m_gBufferAlbedo);
	m_gBufferTarget->attachColorComponent(NORMALS_BUFFER_INDEX, m_gBufferNormals);
	m_gBufferTarget->attachColorComponent(POSITION_BUFFER_INDEX, m_gBufferPosition);
	m_gBufferTarget->attachColorComponent(UV_BUFFER_INDEX, m_gBufferUV);
	m_gBufferTarget->attachDepthStencilComponent(m_gBufferDepthStencil);
	m_gBufferTarget->unbind();
}

Texture* LevelRenderer::createGBufferColorTexture()
{
	Texture* texture = m_graphicsResourceFactory->createTexture();

	texture->setTarget(Texture::Target::_2D);
	texture->setInternalFormat(Texture::InternalFormat::RGBA32F);
	texture->setSize(m_graphicsContext->getViewportWidth(), m_graphicsContext->getViewportHeight());
	texture->create();

	texture->bind();
	texture->setData(Texture::PixelFormat::RGBA, Texture::PixelDataType::Float, nullptr);
	texture->setMinificationFilter(Texture::Filter::Linear);
	texture->setMagnificationFilter(Texture::Filter::Linear);

	return texture;
}

Texture * LevelRenderer::createGBufferDepthStencilTexture()
{
	Texture* texture = m_graphicsResourceFactory->createTexture();

	texture->setTarget(Texture::Target::_2D);
	texture->setInternalFormat(Texture::InternalFormat::Depth24Stencil8);
	texture->setSize(m_graphicsContext->getViewportWidth(), m_graphicsContext->getViewportHeight());
	texture->create();

	texture->bind();
	texture->setData(Texture::PixelFormat::DepthStencil, Texture::PixelDataType::UnsignedInt24_8, nullptr);

	return texture;
}
