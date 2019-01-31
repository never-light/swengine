#pragma once

#include <Engine/Components/ECS/ECS.h>
#include <Engine/Components/Graphics/RenderSystem/GraphicsContext.h>
#include <Engine/Components/Graphics/RenderLayout/SceneEnvironment.h>
#include <Engine/Components/Graphics/RenderLayout/Materials/PBRMaterial.h>

#include "GraphicsPipeline.h"

class MeshRenderingSystem : public GameSystem {
public:
	MeshRenderingSystem(GraphicsPipeline* graphicsPipeline, SceneEnvironment* sceneEnvironment,
		GpuProgram* geometryPassProgram, GpuProgram* lightingPassProgram);
	virtual ~MeshRenderingSystem();

	virtual void update(GameWorld* gameWorld, float delta) override;
	virtual void render(GameWorld * gameWorld) override;

private:
	void bindPBRMaterialParameters(PBRMaterial* material);
	void showGBuffer();

private:
	static const size_t ALBEDO_TEXTURE_UNIT = 0;
	static const size_t METALLIC_TEXTURE_UNIT = 1;
	static const size_t ROUGHNESS_TEXTURE_UNIT = 2;
	static const size_t AO_TEXTURE_UNIT = 3;
	static const size_t NORMAL_TEXTURE_UNIT = 4;

private:
	GraphicsPipeline* m_graphicsPipeline;
	SceneEnvironment* m_sceneEnvironment;

	GpuProgram* m_geometryPassProgram;
	GpuProgram* m_lightingPassProgram;
};