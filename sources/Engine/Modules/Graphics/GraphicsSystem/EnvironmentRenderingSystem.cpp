#include "EnvironmentRenderingSystem.h"
#include "Modules/ECS/ECS.h"

EnvironmentComponent::EnvironmentComponent()
{

}

void EnvironmentComponent::setEnvironmentMaterial(const std::shared_ptr<Material> material)
{
    m_environmentMaterial = material;
}

Material* EnvironmentComponent::getEnvironmentMaterial() const
{
    return m_environmentMaterial.get();
}

EnvironmentRenderingSystem::EnvironmentRenderingSystem(std::shared_ptr<GLGraphicsContext> graphicsContext,
                                                       std::shared_ptr<SharedGraphicsState> sharedGraphicsState,
                                                       std::shared_ptr<Mesh> indentityBox)
    : m_graphicsContext(graphicsContext),
      m_sharedGraphicsState(sharedGraphicsState),
      m_identityBox(indentityBox)
{

}

void EnvironmentRenderingSystem::configure(GameWorld* gameWorld)
{
    ARG_UNUSED(gameWorld);
}

void EnvironmentRenderingSystem::unconfigure(GameWorld* gameWorld)
{
    ARG_UNUSED(gameWorld);
}

void EnvironmentRenderingSystem::update(GameWorld* gameWorld, float delta)
{
    ARG_UNUSED(gameWorld);
    ARG_UNUSED(delta);
}

void EnvironmentRenderingSystem::render(GameWorld* gameWorld)
{
    GameObject* environmentObject = gameWorld->findGameObject([] (GameObject* obj) {
        return obj->hasComponent<EnvironmentComponent>();
    });

    SW_ASSERT(environmentObject != nullptr);

    Material* material = environmentObject->getComponent<EnvironmentComponent>()->getEnvironmentMaterial();

    m_graphicsContext->executeRenderTask(RenderTask{ &material->getGpuMaterial(),
                                                     m_identityBox->getGeometryStore(),
                                                     0, m_identityBox->getSubMeshIndicesCount(0) });
}
