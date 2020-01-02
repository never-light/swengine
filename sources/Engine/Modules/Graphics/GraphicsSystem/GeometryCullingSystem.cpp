#include "GeometryCullingSystem.h"

#include "Modules/ECS/ECS.h"

#include "TransformComponent.h"
#include "MeshRendererComponent.h"

GeometryCullingSystem::GeometryCullingSystem(std::shared_ptr<GLGraphicsContext> graphicsContext,
                                         std::shared_ptr<SharedGraphicsState> sharedGraphicsState)
    : m_graphicsContext(graphicsContext),
      m_sharedGraphicsState(sharedGraphicsState)
{

}

GeometryCullingSystem::~GeometryCullingSystem()
{

}

void GeometryCullingSystem::configure(GameWorld* gameWorld)
{
    ARG_UNUSED(gameWorld);
}

void GeometryCullingSystem::unconfigure(GameWorld* gameWorld)
{
    ARG_UNUSED(gameWorld);
}

void GeometryCullingSystem::update(GameWorld* gameWorld, float delta)
{
    ARG_UNUSED(gameWorld);
    ARG_UNUSED(delta);
}

void GeometryCullingSystem::beforeRender(GameWorld* gameWorld)
{
    for (const GameObject* obj : gameWorld->allWith<MeshRendererComponent, TransformComponent>()) {
        const auto& meshComponent = obj->getComponent<MeshRendererComponent>();
        Mesh* mesh = meshComponent->getMeshInstance().get();

        bool isMeshFrustumIntersecting = isSphereFrustumIntersecting(mesh->getAABB().toSphere(),
                                                                     m_sharedGraphicsState->getActiveCamera()->getFrustum());

        if (!isMeshFrustumIntersecting) {
            meshComponent->cull();
        }
    }
}

void GeometryCullingSystem::afterRender(GameWorld* gameWorld)
{
    for (const GameObject* obj : gameWorld->allWith<MeshRendererComponent, TransformComponent>()) {
        obj->getComponent<MeshRendererComponent>()->cull(false);
    }
}

