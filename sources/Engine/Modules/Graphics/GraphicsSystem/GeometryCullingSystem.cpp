#include "precompiled.h"

#pragma hdrstop

#include "GeometryCullingSystem.h"

#include <utility>

#include "Modules/ECS/ECS.h"

#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "DebugPainter.h"

GeometryCullingSystem::GeometryCullingSystem(std::shared_ptr<GLGraphicsContext> graphicsContext,
  std::shared_ptr<SharedGraphicsState> sharedGraphicsState)
  : RenderingSystem(std::move(graphicsContext), std::move(sharedGraphicsState))
{

}

GeometryCullingSystem::~GeometryCullingSystem() = default;

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

    bool isVisible = false;

    if (meshComponent.getAttributes().isStatic) {
      isVisible = GeometryUtils::isAABBFrustumIntersecting(meshComponent.getBoundingBox(),
        activeCamera->getFrustum());
    }
    else {
      isVisible = GeometryUtils::isSphereFrustumIntersecting(meshComponent.getBoundingSphere(),
        activeCamera->getFrustum());
    }

    if (!isVisible) {
      meshComponent.cull();

      m_sharedGraphicsState->getFrameStats()
        .increaseCulledSubMeshesCount(meshComponent.getMeshInstance()->getSubMeshesCount());
    }

    // TODO: draw bounds only for visible objects
    if (meshComponent.getAttributes().isStatic) {
      DebugPainter::renderAABB(meshComponent.getBoundingBox());
    }
    else {
      DebugPainter::renderSphere(meshComponent.getBoundingSphere());
    }
  }
}

void GeometryCullingSystem::afterRender(GameWorld* gameWorld)
{
  for (auto obj : gameWorld->allWith<MeshRendererComponent>()) {
    auto& rendererComponent = obj->getComponent<MeshRendererComponent>();
    rendererComponent.cull(false);
  }
}

