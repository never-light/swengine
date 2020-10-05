#include "precompiled.h"

#pragma hdrstop
#include "LinearSceneStructure.h"

#include "Modules/Graphics/GraphicsSystem/TransformComponent.h"
#include "Modules/Graphics/GraphicsSystem/GraphicsSceneManagementSystem.h"
#include "Modules/Math/geometry.h"

void LinearSceneStructure::clear()
{
  m_staticObjects.clear();
  m_dynamicObjects.clear();
}

void LinearSceneStructure::addObject(GameObject object)
{
  if (object.getComponent<TransformComponent>()->isStatic()) {
    m_staticObjects.push_back(object);
  }
  else {
    m_dynamicObjects.push_back(object);
  }
}

void LinearSceneStructure::removeObject(GameObject object)
{
  if (object.getComponent<TransformComponent>()->isStatic()) {
    std::erase(m_staticObjects, object);
  }
  else {
    std::erase(m_dynamicObjects, object);
  }
}

void LinearSceneStructure::queryNearestDynamicNeighbors(
  const glm::vec3& origin,
  float radius,
  std::vector<GameObject>& result)
{
  for (GameObject& object : m_dynamicObjects) {
    glm::vec3 position = object.getComponent<TransformComponent>()->getTransform().getPosition();

    if (glm::length2(position - origin) <= radius * radius) {
      result.push_back(object);
    }
  }
}

void LinearSceneStructure::buildFromObjectsList(std::vector<GameObject>& objects)
{
  SW_ASSERT(!m_levelObject.isFormed());

  for (GameObject& object : objects) {
    auto transformComponent = object.getComponent<TransformComponent>();

    if (transformComponent->isStatic()) {
      m_staticObjects.push_back(object);
    }
    else {
      m_dynamicObjects.push_back(object);
    }
  }
}

void LinearSceneStructure::queryVisibleObjects(Camera& camera, std::vector<GameObject>& result)
{
  queryVisibleObjects(camera, m_staticObjects, result);
  queryVisibleObjects(camera, m_dynamicObjects, result);
}

size_t LinearSceneStructure::getObjectsCount() const
{
  return m_staticObjects.size() + m_dynamicObjects.size();
}

void LinearSceneStructure::queryVisibleObjects(Camera& camera,
  std::vector<GameObject>& objects,
  std::vector<GameObject>& result)
{
  for (GameObject& object : objects) {
    auto& sceneNodeComponent = *object.getComponent<ObjectSceneNodeComponent>().get();

    if (!sceneNodeComponent.isDrawable()) {
      continue;
    }

    auto& transformComponent = *object.getComponent<TransformComponent>().get();

    bool isVisible = false;

    if (transformComponent.isStatic()) {
      isVisible = GeometryUtils::isAABBFrustumIntersecting(transformComponent.getBoundingBox(),
        camera.getFrustum());
    }
    else {
      isVisible = GeometryUtils::isSphereFrustumIntersecting(transformComponent.getBoundingSphere(),
        camera.getFrustum());
    }

    if (isVisible) {
      result.push_back(object);
    }
  }
}
