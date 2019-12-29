#include "MeshRenderingSystem.h"

#include "Modules/ECS/ECS.h"

#include "TransformComponent.h"
#include "MeshRendererComponent.h"

MeshRenderingSystem::MeshRenderingSystem(std::shared_ptr<GLGraphicsContext> graphicsContext,
                                         std::shared_ptr<SharedGraphicsState> sharedGraphicsState)
    : m_graphicsContext(graphicsContext),
      m_sharedGraphicsState(sharedGraphicsState)
{

}

MeshRenderingSystem::~MeshRenderingSystem()
{

}

void MeshRenderingSystem::configure(GameWorld* gameWorld)
{
    ARG_UNUSED(gameWorld);
}

void MeshRenderingSystem::unconfigure(GameWorld* gameWorld)
{
    ARG_UNUSED(gameWorld);
}

void MeshRenderingSystem::update(GameWorld* gameWorld, float delta)
{
    ARG_UNUSED(gameWorld);
    ARG_UNUSED(delta);
}

void MeshRenderingSystem::render(GameWorld* gameWorld)
{
    m_graphicsContext->disableWireframeRendering();
    m_graphicsContext->enableDepthTest();
    m_graphicsContext->enableFaceCulling();

    for (const GameObject* obj : gameWorld->allWith<MeshRendererComponent, TransformComponent>()) {
        const auto& meshComponent = obj->getComponent<MeshRendererComponent>();
        Mesh* mesh = meshComponent->getMeshInstance().get();

        SW_ASSERT(mesh != nullptr);

        Transform* transform = obj->getComponent<TransformComponent>()->getTransform();

        const size_t subMeshesCount = mesh->getSubMeshesCount();

        SW_ASSERT(subMeshesCount != 0);

        for (size_t subMeshIndex = 0; subMeshIndex < subMeshesCount; subMeshIndex++) {
            const Material* material = meshComponent->getMaterialInstance(subMeshIndex).get();

            SW_ASSERT(material != nullptr);

            GLShadersPipeline* shadersPipeline = material->getShadersPipeline().get();

            SW_ASSERT(shadersPipeline != nullptr);

            GLShader* vertexShader = shadersPipeline->getShader(GL_VERTEX_SHADER);

            if (transform != nullptr) {
                if (vertexShader->hasParameter("transform.localToWorld")) {
                    vertexShader->setParameter("transform.localToWorld", transform->getTransformationMatrix());
                }
            }

            Camera* camera = m_sharedGraphicsState->getActiveCamera().get();

            if (camera != nullptr) {
                if (vertexShader->hasParameter("scene.worldToCamera")) {
                    vertexShader->setParameter("scene.worldToCamera", camera->getViewMatrix());
                    vertexShader->setParameter("scene.cameraToProjection", camera->getProjectionMatrix());
                }
            }

            m_graphicsContext->executeRenderTask({
                mesh->getGeometryStore(),
                shadersPipeline,
                mesh->getSubMeshIndicesOffset(subMeshIndex),
                mesh->getSubMeshIndicesCount(subMeshIndex)
            });
        }
    }
}
