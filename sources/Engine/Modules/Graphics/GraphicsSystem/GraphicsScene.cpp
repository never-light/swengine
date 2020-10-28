#include "precompiled.h"

#pragma hdrstop

#include "GraphicsScene.h"

#include <utility>

#include "Culling/LinearSceneStructure.h"
#include "MeshRendererComponent.h"
#include "MeshRenderingSystem.h"

GraphicsScene::GraphicsScene()
  : m_accelerationStructure(std::make_unique<LinearSceneStructure>())
{

}

void GraphicsScene::buildFromObjectsList(std::vector<GameObject>& objects)
{
  for (GameObject& object : objects) {
    addSceneNodeComponent(object);
  }

  m_accelerationStructure->buildFromObjectsList(objects);
}

void GraphicsScene::addObject(GameObject object)
{
  SW_ASSERT(!object.hasComponent<ObjectSceneNodeComponent>());

  addSceneNodeComponent(object);

  m_accelerationStructure->addObject(object);
}

void GraphicsScene::removeObject(GameObject object)
{
  SW_ASSERT(object.hasComponent<ObjectSceneNodeComponent>());

  auto& sceneNodeComponent = *object.getComponent<ObjectSceneNodeComponent>().get();

  if (sceneNodeComponent.isDrawable()) {
    m_drawableObjectsCount--;
  }

  m_accelerationStructure->removeObject(object);

  object.removeComponent<ObjectSceneNodeComponent>();
}

void GraphicsScene::queryNearestDynamicNeighbors(
  const glm::vec3& origin,
  float radius,
  std::vector<GameObject>& result)
{
  m_accelerationStructure->queryNearestDynamicNeighbors(origin, radius, result);
}

void GraphicsScene::queryVisibleObjects(Camera& camera, std::vector<GameObject>& result)
{
  m_accelerationStructure->queryVisibleObjects(camera, result);
}

void GraphicsScene::clearObjects()
{
  m_accelerationStructure->clear();
}

void GraphicsScene::setActiveCamera(std::shared_ptr<Camera> camera)
{
  m_activeCamera = std::move(camera);
}

std::shared_ptr<Camera> GraphicsScene::getActiveCamera() const
{
  return m_activeCamera;
}

size_t GraphicsScene::getObjectsCount() const
{
  return m_accelerationStructure->getObjectsCount();
}

[[nodiscard]] size_t GraphicsScene::getDrawableObjectsCount() const
{
  return m_drawableObjectsCount;
}

void GraphicsScene::queryVisibleObjects(std::vector<GameObject>& result)
{
  queryVisibleObjects(*m_activeCamera, result);
}

void GraphicsScene::addSceneNodeComponent(GameObject& object)
{
  bool isDrawable = isObjectDrawable(object);

  object.addComponent<ObjectSceneNodeComponent>(isDrawable);

  if (isDrawable) {
    m_drawableObjectsCount++;
  }
}

bool GraphicsScene::isObjectDrawable(GameObject& object)
{
  return object.hasComponent<MeshRendererComponent>();
}

const FrameStats& GraphicsScene::getFrameStats() const
{
  return m_frameStats;
}

FrameStats& GraphicsScene::getFrameStats()
{
  return m_frameStats;
}
