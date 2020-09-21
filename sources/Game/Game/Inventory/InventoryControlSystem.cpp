#include "InventoryControlSystem.h"

InventoryControlSystem::InventoryControlSystem() = default;

InventoryControlSystem::~InventoryControlSystem() = default;

void InventoryControlSystem::configure()
{
}

void InventoryControlSystem::unconfigure()
{
}

void InventoryControlSystem::activate()
{
  getGameWorld()->subscribeEventsListener<InventoryItemActionTriggeredEvent>(this);
}

void InventoryControlSystem::deactivate()
{
  getGameWorld()->unsubscribeEventsListener<InventoryItemActionTriggeredEvent>(this);
}

EventProcessStatus InventoryControlSystem::receiveEvent(
  GameWorld* gameWorld,
  const InventoryItemActionTriggeredEvent& event)
{
  ARG_UNUSED(gameWorld);

  switch (event.triggerType) {
    case InventoryItemActionTriggerType::RelocateToInventory:
      relocateObjectToInventory(event.inventoryOwner, event.item);
      break;

    case InventoryItemActionTriggerType::DropFromInventory:
      dropObjectFromInventory(event.inventoryOwner, event.item);
      break;

    case InventoryItemActionTriggerType::Use:
      spdlog::debug("Use inventory object {}", event.item.getName());
      //dropObjectFromInventory(event.inventoryOwner, event.item);
      break;

    default:
      SW_ASSERT(false && "Unknown inventory action trigger type");
      break;
  }

  return EventProcessStatus::Processed;
}

void InventoryControlSystem::relocateObjectToInventory(
  GameObject inventoryOwner,
  GameObject objectToRelocate)
{
  auto& inventoryComponent = *inventoryOwner.getComponent<InventoryComponent>().get();
  auto& inventoryItemComponent = *objectToRelocate.getComponent<InventoryItemComponent>().get();

  SW_ASSERT(!inventoryItemComponent.getOwner().isAlive());
  inventoryItemComponent.setOwner(inventoryOwner);

  // TODO[WIP]: set ghost state for objectToRelocate

  inventoryComponent.addItem(objectToRelocate);
}

void InventoryControlSystem::dropObjectFromInventory(
  GameObject inventoryOwner,
  GameObject objectToDrop)
{
  auto& inventoryComponent = *inventoryOwner.getComponent<InventoryComponent>().get();
  auto& inventoryItemComponent = *objectToDrop.getComponent<InventoryItemComponent>().get();

  inventoryItemComponent.setOwner(GameObject());

  // TODO[WIP]: remove ghost state from objectToDrop

  // TODO: consider creation component like EntityLifeComponent
  //  for storing Time To Live property. Set the object life start timestamp here.

  inventoryComponent.removeItem(objectToDrop);
}
