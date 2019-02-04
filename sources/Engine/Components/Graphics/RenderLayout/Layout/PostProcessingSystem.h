#pragma once

#include <Engine/Components/ECS/ECS.h>
#include <Engine/Components/Graphics/RenderSystem/GraphicsContext.h>
#include <Engine/Components/Graphics/RenderLayout/SceneEnvironment.h>
#include <Engine/Components/Graphics/RenderLayout/Materials/PBRMaterial.h>

#include "GraphicsPipeline.h"

class PostProcessingSystem : public GameSystem {
public:
	PostProcessingSystem(GraphicsPipeline* graphicsPipeline, SceneEnvironment* sceneEnvironment,
		GpuProgram* postProcessingProgram);
	
	virtual ~PostProcessingSystem();

	virtual void update(GameWorld* gameWorld, float delta) override;
	virtual void render(GameWorld * gameWorld) override;

private:
	GraphicsPipeline* m_graphicsPipeline;
	SceneEnvironment* m_sceneEnvironment;

	GpuProgram* m_postProcessingProgram;
};