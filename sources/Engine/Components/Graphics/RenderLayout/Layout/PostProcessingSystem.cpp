#include "PostProcessingSystem.h"

PostProcessingSystem::PostProcessingSystem(GraphicsPipeline * graphicsPipeline, 
	SceneEnvironment * sceneEnvironment, GpuProgram * postProcessingProgram)
	: m_graphicsPipeline(graphicsPipeline),
	m_sceneEnvironment(sceneEnvironment),
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
}