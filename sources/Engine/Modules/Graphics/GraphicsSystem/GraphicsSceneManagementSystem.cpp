#include "precompiled.h"

#pragma hdrstop

#include "GraphicsSceneManagementSystem.h"

#include <utility>

#include "Modules/ECS/ECS.h"

#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "DebugPainter.h"

GraphicsSceneManagementSystem::GraphicsSceneManagementSystem(
  std::shared_ptr<GraphicsScene> graphicsScene)
  : m_graphicsScene(std::move(graphicsScene))
{

}

GraphicsSceneManagementSystem::~GraphicsSceneManagementSystem() = default;

void GraphicsSceneManagementSystem::configure()
{
  GameWorld* gameWorld = getGameWorld();

  gameWorld->subscribeEventsListener<LoadSceneCommandEvent>(this);
  gameWorld->subscribeEventsListener<UnloadSceneCommandEvent>(this);
  gameWorld->subscribeEventsListener<AddObjectToSceneCommandEvent>(this);
  gameWorld->subscribeEventsListener<RemoveObjectFromSceneCommandEvent>(this);
  gameWorld->subscribeEventsListener<GameObjectAddEvent>(this);
  gameWorld->subscribeEventsListener<GameObjectRemoveEvent>(this);
}

void GraphicsSceneManagementSystem::unconfigure()
{
  GameWorld* gameWorld = getGameWorld();

  gameWorld->unsubscribeEventsListener<LoadSceneCommandEvent>(this);
  gameWorld->unsubscribeEventsListener<UnloadSceneCommandEvent>(this);
  gameWorld->unsubscribeEventsListener<AddObjectToSceneCommandEvent>(this);
  gameWorld->unsubscribeEventsListener<RemoveObjectFromSceneCommandEvent>(this);
  gameWorld->unsubscribeEventsListener<GameObjectAddEvent>(this);
  gameWorld->unsubscribeEventsListener<GameObjectRemoveEvent>(this);
}

EventProcessStatus GraphicsSceneManagementSystem::receiveEvent(const LoadSceneCommandEvent& event)
{
  SW_ASSERT(m_graphicsScene->getObjectsCount() == 0);

  std::vector<GameObject> objectsList = event.sceneObjects;
  m_graphicsScene->buildFromObjectsList(objectsList);

  return EventProcessStatus::Processed;
}

EventProcessStatus GraphicsSceneManagementSystem::receiveEvent(const UnloadSceneCommandEvent& event)
{
  ARG_UNUSED(event);

  m_graphicsScene->clearObjects();

  return EventProcessStatus::Processed;
}

EventProcessStatus GraphicsSceneManagementSystem::receiveEvent(const AddObjectToSceneCommandEvent& event)
{
  m_graphicsScene->addObject(event.object);
  return EventProcessStatus::Processed;
}

EventProcessStatus GraphicsSceneManagementSystem::receiveEvent(const RemoveObjectFromSceneCommandEvent& event)
{
  m_graphicsScene->removeObject(event.object);
  return EventProcessStatus::Processed;
}

EventProcessStatus GraphicsSceneManagementSystem::receiveEvent(const GameObjectAddEvent& event)
{
  ARG_UNUSED(event);

  return EventProcessStatus::Processed;
}

EventProcessStatus GraphicsSceneManagementSystem::receiveEvent(const GameObjectRemoveEvent& event)
{
  GameObject object = event.gameObject;

  if (object.hasComponent<ObjectSceneNodeComponent>()) {
    m_graphicsScene->removeObject(object);
  }

  return EventProcessStatus::Processed;
}
