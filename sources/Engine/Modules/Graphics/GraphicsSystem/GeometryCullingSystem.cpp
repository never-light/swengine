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
  for (GameObject* obj : gameWorld->allWith<MeshRendererComponent, TransformComponent>()) {
    auto& meshComponent = obj->getComponent<MeshRendererComponent>();

    std::shared_ptr<Camera> activeCamera = m_sharedGraphicsState->getActiveCamera();

    bool isCulled = !GeometryUtils::isAABBFrustumIntersecting(meshComponent.getAABB(), activeCamera->getFrustum());

    if (isCulled) {
      meshComponent.cull();

      m_sharedGraphicsState->getFrameStats()
        .increaseCulledSubMeshesCount(meshComponent.getMeshInstance()->getSubMeshesCount());
    }
  }
}

void GeometryCullingSystem::afterRender(GameWorld* gameWorld)
{
  for (GameObject* obj : gameWorld->allWith<MeshRendererComponent, TransformComponent>()) {
    obj->getComponent<MeshRendererComponent>().cull(false);
  }
}

