#include "InteractiveObjectsControlSystem.h"

#include <Engine/Modules/Graphics/GraphicsSystem/MeshRendererComponent.h>
#include <Engine/Modules/Physics/RigidBodyComponent.h>

#include "Game/Inventory/InventoryComponent.h"
#include "Game/Dynamic/ActorComponent.h"

InteractiveObjectsControlSystem::InteractiveObjectsControlSystem() = default;

InteractiveObjectsControlSystem::~InteractiveObjectsControlSystem() = default;

void InteractiveObjectsControlSystem::configure()
{
}

void InteractiveObjectsControlSystem::unconfigure()
{
}

void InteractiveObjectsControlSystem::activate()
{
  getGameWorld()->subscribeEventsListener<InteractiveObjectActionTriggeredEvent>(this);
}

void InteractiveObjectsControlSystem::deactivate()
{
  getGameWorld()->unsubscribeEventsListener<InteractiveObjectActionTriggeredEvent>(this);
}

EventProcessStatus InteractiveObjectsControlSystem::receiveEvent(
  GameWorld* gameWorld,
  const InteractiveObjectActionTriggeredEvent& event)
{
  ARG_UNUSED(gameWorld);

  switch (event.triggerType) {
    case InteractiveObjectActionType::Take:
      handleTakeObjectAction(event.initiator, event.target);
      break;

    case InteractiveObjectActionType::Use:
      handleUseObjectAction(event.initiator, event.target);
      break;

    case InteractiveObjectActionType::Talk:
      handleTalkWithObjectAction(event.initiator, event.target);
      break;

    default:
      SW_ASSERT(false && "Unknown interactive action trigger type");
      break;
  }

  return EventProcessStatus::Processed;
}

void InteractiveObjectsControlSystem::handleTakeObjectAction(GameObject initiator, GameObject target)
{
  auto& interactiveObjectComponent = *target.getComponent<InteractiveObjectComponent>().get();

  //SW_ASSERT(interactiveObjectComponent.isTakeable());

  const auto& takeCallback = interactiveObjectComponent.getTakeCallback();

  if (takeCallback) {
    takeCallback(initiator, target);
  }

  getGameWorld()->emitEvent(InventoryItemActionTriggeredEvent(
    initiator,
    InventoryItemActionTriggerType::RelocateToInventory,
    target));
}

void InteractiveObjectsControlSystem::handleUseObjectAction(GameObject initiator, GameObject target)
{
  auto& interactiveObjectComponent = *target.getComponent<InteractiveObjectComponent>().get();

  //SW_ASSERT(interactiveObjectComponent.isUsable());

  const auto& useCallback = interactiveObjectComponent.getUseCallback();

  if (useCallback) {
    useCallback(initiator, target);
  }
}

void InteractiveObjectsControlSystem::handleTalkWithObjectAction(GameObject initiator, GameObject target)
{
  auto& interactiveObjectComponent = *target.getComponent<InteractiveObjectComponent>().get();

  SW_ASSERT(interactiveObjectComponent.isTalkable());
  SW_ASSERT(target.hasComponent<ActorComponent>());

  getGameWorld()->emitEvent(ActorTalkTriggerCommandEvent(initiator, target));
}
