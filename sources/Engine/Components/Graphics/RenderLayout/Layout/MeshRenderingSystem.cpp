#include "MeshRenderingSystem.h"

#include <Engine/Components/ECS/GameWorld.h>
#include <Engine/Components/Math/Transform.h>

#include "MeshComponent.h"


MeshRenderingSystem::MeshRenderingSystem(GraphicsPipeline* graphicsPipeline, SceneEnvironment* sceneEnvironment,
	GpuProgram* geometryPassProgram, GpuProgram* lightingPassProgram)
		: m_graphicsPipeline(graphicsPipeline), 
	m_sceneEnvironment(sceneEnvironment),
	m_geometryPassProgram(geometryPassProgram),
	m_lightingPassProgram(lightingPassProgram)
{

}

MeshRenderingSystem::~MeshRenderingSystem()
{
}

void MeshRenderingSystem::render(GameWorld * gameWorld)
{
	Camera* activeCamera = m_sceneEnvironment->getActiveCamera();
	GraphicsContext* graphicsContext = m_graphicsPipeline->getGraphicsContext();

	if (activeCamera == nullptr)
		return;

	m_geometryPassProgram->bind();
	m_geometryPassProgram->setParameter("scene.viewTransform", activeCamera->getViewMatrix());
	m_geometryPassProgram->setParameter("scene.projectionTransform", activeCamera->getProjectionMatrix());

	graphicsContext->enableWritingToDepthBuffer();
	graphicsContext->enableDepthTest();

	graphicsContext->enableFaceCulling();
	graphicsContext->setFaceCullingMode(GraphicsContext::FaceCullingMode::Back);

	m_graphicsPipeline->getGBuffer()->bind();
	m_graphicsPipeline->getGBuffer()->setClearColor(0.0f, 0.0f, 0.0f);
	m_graphicsPipeline->getGBuffer()->clear(RenderTarget::CLEAR_COLOR | RenderTarget::CLEAR_DEPTH);

	for (GameObject* gameObject : gameWorld->allWith<Transform, MeshComponent>()) {
		ComponentHandle<MeshComponent> meshHandle = gameObject->getComponent<MeshComponent>();
		ComponentHandle<Transform> transformHandle = gameObject->getComponent<Transform>();

		bool transformDataPassed = false;

		for (const SubMesh* subMesh : meshHandle->getMesh()->getSubMeshes()) {
			PBRMaterial* material = dynamic_cast<PBRMaterial*>(subMesh->getMaterial());

			if (material == nullptr)
				continue;

			if (!transformDataPassed)
				m_geometryPassProgram->setParameter("transform.localToWorld", transformHandle->getTransformationMatrix());
		
			bindPBRMaterialParameters(material);

			subMesh->getGeometryInstance()->bind();

			if (subMesh->getGeometryInstance()->isIndexed())
				subMesh->getGeometryInstance()->drawIndexed(GeometryInstance::DrawMode::Triangles);
			else
				subMesh->getGeometryInstance()->draw(GeometryInstance::DrawMode::Triangles);
		}
	}

	//showGBuffer();
	m_graphicsPipeline->getGBuffer()->unbind();

	return;

	graphicsContext->disableWritingToDepthBuffer();
	graphicsContext->disableDepthTest();

	// Lighting pass
	m_lightingPassProgram->bind();
	m_lightingPassProgram->setParameter("g_cameraPosition",
		activeCamera->getTransform()->getPosition());

	m_graphicsPipeline->getGBufferAttachment0()->bind(0);
	m_graphicsPipeline->getGBufferAttachment1()->bind(1);
	m_graphicsPipeline->getGBufferAttachment2()->bind(2);

	m_lightingPassProgram->setParameter("g_position", 0);
	m_lightingPassProgram->setParameter("g_albedo", 1);
	m_lightingPassProgram->setParameter("g_normal", 2);

	graphicsContext->getNDCQuadInstance()->draw(GeometryInstance::DrawMode::TrianglesStrip);
}

void MeshRenderingSystem::showGBuffer()
{
	unsigned int viewportWidth = m_graphicsPipeline->getGraphicsContext()->getViewportWidth();
	unsigned int viewportHeight = m_graphicsPipeline->getGraphicsContext()->getViewportHeight();

	unsigned int halfWidth = viewportWidth / 2;
	unsigned int halfHeight = viewportHeight / 2;

	m_graphicsPipeline->getGBuffer()->copyColorComponentData(0, nullptr, 0,
		Rect(0, 0, viewportWidth, viewportHeight), Rect(0, 0, halfWidth, halfHeight),
		RenderTarget::CopyFilter::Linear);

	m_graphicsPipeline->getGBuffer()->copyColorComponentData(1, nullptr, 0,
		Rect(0, 0, viewportWidth, viewportHeight), Rect(0, halfHeight, halfWidth, halfHeight),
		RenderTarget::CopyFilter::Linear);

	m_graphicsPipeline->getGBuffer()->copyColorComponentData(2, nullptr, 0,
		Rect(0, 0, viewportWidth, viewportHeight), Rect(halfWidth, halfHeight, halfWidth, halfHeight),
		RenderTarget::CopyFilter::Linear);
}


void MeshRenderingSystem::bindPBRMaterialParameters(PBRMaterial* material)
{
	// Albedo
	if (material->hasColorMap()) {
		material->getColorMap()->bind(ALBEDO_TEXTURE_UNIT);
		m_geometryPassProgram->setParameter("material.albedoTexture", (int)ALBEDO_TEXTURE_UNIT);
	}
	else {
		m_geometryPassProgram->setParameter("material.albedoValue", material->getColorValue());
	}

	m_geometryPassProgram->setParameter("material.useAlbedoTexture", material->hasColorMap());

	// Metallic

	if (material->hasMetallicMap()) {
		material->getMetallicMap()->bind(METALLIC_TEXTURE_UNIT);
		m_geometryPassProgram->setParameter("material.metallicTexture", (int)METALLIC_TEXTURE_UNIT);
	}
	else {
		m_geometryPassProgram->setParameter("material.metallicValue", material->getMetallicValue());
	}

	m_geometryPassProgram->setParameter("material.useMetallicTexture", material->hasMetallicMap());

	// Roughness

	if (material->hasRoughnessMap()) {
		material->getRoughnessMap()->bind(ROUGHNESS_TEXTURE_UNIT);
		m_geometryPassProgram->setParameter("material.roughnessTexture", (int)ROUGHNESS_TEXTURE_UNIT);
	}
	else {
		m_geometryPassProgram->setParameter("material.roughnessValue", material->getRoughnessValue());
	}

	m_geometryPassProgram->setParameter("material.useRoughnessTexture", material->hasRoughnessMap());

	// AO

	if (material->hasAOMap()) {
		material->getAOMap()->bind(AO_TEXTURE_UNIT);
		m_geometryPassProgram->setParameter("material.AOTexture", (int)AO_TEXTURE_UNIT);
	}
	else {
		m_geometryPassProgram->setParameter("material.AOValue", material->getAOValue());
	}

	m_geometryPassProgram->setParameter("material.useAOTexture", material->hasAOMap());

	// Normal

	if (material->hasNormalMap()) {
		material->getNormalMap()->bind(NORMAL_TEXTURE_UNIT);
		m_geometryPassProgram->setParameter("material.normalTexture", (int)NORMAL_TEXTURE_UNIT);
	}

	m_geometryPassProgram->setParameter("material.useNormalTexture", material->hasNormalMap());
}

void MeshRenderingSystem::update(GameWorld * gameWorld, float delta)
{

}
