#include "LevelRenderer.h"

#include <algorithm>

LevelRenderer::LevelRenderer(GraphicsContext * graphicsContext, GraphicsResourceFactory * graphicsResourceFactory)
	: m_graphicsContext(graphicsContext),
	m_graphicsResourceFactory(graphicsResourceFactory)
{
}

LevelRenderer::~LevelRenderer()
{
}

void LevelRenderer::addLightSource(const Light * lightSource)
{
	m_lightsSources.push_back(lightSource);
}

void LevelRenderer::removeLightSource(const Light * lightSource)
{
	m_lightsSources.erase(std::remove(m_lightsSources.begin(), m_lightsSources.end(), lightSource), m_lightsSources.end());
}

void LevelRenderer::setActiveCamera(const Camera * camera)
{
	m_activeCamera = camera;
}

void LevelRenderer::render()
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

	m_graphicsContext->enableDepthTest();

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
