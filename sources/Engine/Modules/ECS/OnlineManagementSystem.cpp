#include "precompiled.h"

#pragma hdrstop

#include "OnlineManagementSystem.h"
#include "Modules/Graphics/GraphicsSystem/TransformComponent.h"

void OnlineManagementSystem::configure()
{
  getGameWorld()->subscribeEventsListener<ChangeGameObjectOnlineStatusCommandEvent>(this);
}

void OnlineManagementSystem::unconfigure()
{
  getGameWorld()->unsubscribeEventsListener<ChangeGameObjectOnlineStatusCommandEvent>(this);
}

EventProcessStatus OnlineManagementSystem::receiveEvent(const ChangeGameObjectOnlineStatusCommandEvent& event)
{
  SW_ASSERT(event.gameObject.isAlive() && event.gameObject.hasComponent<TransformComponent>());

  GameObject affectedObject = event.gameObject;
  auto transformComponent = affectedObject.getComponent<TransformComponent>();

  SW_ASSERT(event.makeOnline && !transformComponent->isOnline() ||
    !event.makeOnline && transformComponent->isOnline());

  transformComponent->setOnlineMode(event.makeOnline);

  getGameWorld()->emitEvent(ChangeGameObjectOnlineStatusCommandEvent{.gameObject = affectedObject,
    .makeOnline = event.makeOnline});

  if (event.makeOnline) {
    spdlog::debug("Object {} goes into online", affectedObject.getName());
  }
  else {
    spdlog::debug("Object {} goes into offline", affectedObject.getName());
  }

  return EventProcessStatus::Processed;
}
