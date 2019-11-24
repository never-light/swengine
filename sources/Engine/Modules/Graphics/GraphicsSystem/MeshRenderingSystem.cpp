#include "MeshRenderingSystem.h"

#include "Modules/ECS/ECS.h"

#include "TransformComponent.h"
#include "MeshRendererComponent.h"

MeshRenderingSystem::MeshRenderingSystem(std::shared_ptr<GLGraphicsContext> graphicsContext)
    : m_graphicsContext(graphicsContext)
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
    for (const GameObject* obj : gameWorld->allWith<MeshRendererComponent, TransformComponent>()) {
        const auto& meshComponent = obj->getComponent<MeshRendererComponent>();
        Mesh* mesh = meshComponent->getMeshInstance().get();

        SW_ASSERT(mesh != nullptr);

        const size_t subMeshesCount = mesh->getSubMeshesCount();

        SW_ASSERT(subMeshesCount != 0);

        for (size_t subMeshIndex = 0; subMeshIndex < subMeshesCount; subMeshIndex++) {
            const Material* material = meshComponent->getMaterialInstance(subMeshIndex).get();

            SW_ASSERT(material != nullptr);

            const GLShadersPipeline* shadersPipeline = material->getShadersPipeline().get();

            SW_ASSERT(shadersPipeline != nullptr);

            m_graphicsContext->executeRenderTask({
                mesh->getGeometryStore(),
                shadersPipeline,
                mesh->getSubMeshIndicesOffset(subMeshIndex),
                mesh->getSubMeshIndicesCount(subMeshIndex)
            });
        }
    }
}
