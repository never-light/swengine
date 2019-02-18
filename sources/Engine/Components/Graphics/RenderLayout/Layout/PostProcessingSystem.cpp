#include "PostProcessingSystem.h"

PostProcessingSystem::PostProcessingSystem(GraphicsPipeline * graphicsPipeline, 
	SceneEnvironment * sceneEnvironment, 
	GpuProgram* brightFilterProgram, GpuProgram* blurProgram, GpuProgram* postProcessingProgram)
	: m_graphicsPipeline(graphicsPipeline),
	m_sceneEnvironment(sceneEnvironment),
	m_brightFilterProgram(brightFilterProgram),
	m_blurProgram(blurProgram),
	m_postProcessingProgram(postProcessingProgram)
{
}

PostProcessingSystem::~PostProcessingSystem()
{
}

void PostProcessingSystem::update(GameWorld * gameWorld, float delta)
{
}

void PostProcessingSystem::render(GameWorld * gameWorld)
{
	GraphicsContext* graphicsContext = m_graphicsPipeline->getGraphicsContext();

	unsigned int viewportWidth = m_graphicsPipeline->getGraphicsContext()->getViewportWidth();
	unsigned int viewportHeight = m_graphicsPipeline->getGraphicsContext()->getViewportHeight();

	// Bright filter
	m_graphicsPipeline->getHDRTexture()->bind(0);

	m_brightFilterProgram->bind();
	m_graphicsPipeline->getBrightFilterRenderTarget()->bind();

	graphicsContext->getNDCQuadInstance()->bind();
	graphicsContext->getNDCQuadInstance()->draw(GeometryInstance::DrawMode::TrianglesStrip);
	
	// Blur
	m_graphicsPipeline->getBrightFilterTexture()->bind(0);

	m_graphicsPipeline->getBlurRenderTarget1()->bind();
	
	m_blurProgram->bind();

	m_blurProgram->setParameter("g_viewport_width", (float)viewportWidth);
	m_blurProgram->setParameter("g_viewport_height", (float)viewportHeight);

	m_blurProgram->setParameter("g_direction", 1);
	graphicsContext->getNDCQuadInstance()->draw(GeometryInstance::DrawMode::TrianglesStrip);

	m_graphicsPipeline->getBlurRenderTarget2()->bind();
	m_graphicsPipeline->getBlurredBrightTexture1()->bind(0);

	m_blurProgram->setParameter("g_direction", 1);
	graphicsContext->getNDCQuadInstance()->draw(GeometryInstance::DrawMode::TrianglesStrip);

	// Final post-processing
	graphicsContext->getWindowRenderTarget()->bind();

	m_graphicsPipeline->getHDRTexture()->bind(0);
	m_graphicsPipeline->getBlurredBrightTexture2()->bind(1);
	m_postProcessingProgram->bind();

	graphicsContext->getNDCQuadInstance()->draw(GeometryInstance::DrawMode::TrianglesStrip);

	m_graphicsPipeline->getBlurRenderTarget2()->copyColorComponentData(0, nullptr, 0,
		Rect(0, 0, viewportWidth / 4, viewportHeight / 4), Rect(0, 0, viewportWidth / 2, viewportHeight / 2),
		RenderTarget::CopyFilter::Linear);

}