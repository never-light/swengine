#include "precompiled.h"
#pragma hdrstop

#include "GeometryCullingSystem.h"

#include "Modules/ECS/ECS.h"

#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "DebugPainter.h"

GeometryCullingSystem::GeometryCullingSystem(std::shared_ptr<GLGraphicsContext> graphicsContext,
                                         std::shared_ptr<SharedGraphicsState> sharedGraphicsState)
    : RenderingSystem(graphicsContext, sharedGraphicsState)
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

        //DebugPainter::renderSphere(meshComponent->getAABB().toSphere(), glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f }, true);

        /*DebugPainter::renderAABB(meshComponent->getAABB().getMin(), meshComponent->getAABB().getMax(),
                                 glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f }, true);*/

        bool isMeshFrustumIntersecting = isAABBFrustumIntersecting(meshComponent->getAABB(),
                                                                    m_sharedGraphicsState->getActiveCamera()->getFrustum());

        if (!isMeshFrustumIntersecting) {
            meshComponent->cull();

            m_sharedGraphicsState->getFrameStats().increaseCulledSubMeshesCount(meshComponent->getMeshInstance()->getSubMeshesCount());
        }
    }
}

void GeometryCullingSystem::afterRender(GameWorld* gameWorld)
{
    for (const GameObject* obj : gameWorld->allWith<MeshRendererComponent, TransformComponent>()) {
        obj->getComponent<MeshRendererComponent>()->cull(false);
    }
}

