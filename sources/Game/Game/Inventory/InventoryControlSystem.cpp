#include "InventoryControlSystem.h"

#include <Engine/Modules/Graphics/GraphicsSystem/MeshRendererComponent.h>
#include <Engine/Modules/Physics/RigidBodyComponent.h>

InventoryControlSystem::InventoryControlSystem(std::shared_ptr<LevelsManager> levelsManager)
  : m_levelsManager(levelsManager) {

}

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

  auto& inventoryItemComponent = *GameObject(event.item).getComponent<InventoryItemComponent>().get();

  switch (event.triggerType) {
    case InventoryItemActionTriggerType::RelocateToInventory:
      relocateObjectToInventory(event.inventoryOwner, event.item);
      break;

    case InventoryItemActionTriggerType::DropFromInventory:
      dropObjectFromInventory(event.inventoryOwner, event.item);
      break;

    case InventoryItemActionTriggerType::Use: {
      const auto& useCallback = inventoryItemComponent.getUseCallback();

      if (useCallback) {
        useCallback(event.inventoryOwner, event.item);
      }

      break;
    }
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

  SW_ASSERT(!inventoryItemComponent.getOwner().isFormed());
  inventoryItemComponent.setOwner(inventoryOwner);

  if (objectToRelocate.hasComponent<MeshRendererComponent>()) {
    objectToRelocate.removeComponent<MeshRendererComponent>();
    inventoryItemComponent.addComponentToRestore<MeshRendererComponent>();
  }

  if (objectToRelocate.hasComponent<RigidBodyComponent>()) {
    objectToRelocate.removeComponent<RigidBodyComponent>();
    inventoryItemComponent.addComponentToRestore<RigidBodyComponent>();
  }

  inventoryComponent.addItem(objectToRelocate);

  const auto& takeCallback = inventoryItemComponent.getTakeCallback();

  if (takeCallback) {
    takeCallback(inventoryOwner, objectToRelocate);
  }
}

void InventoryControlSystem::dropObjectFromInventory(
  GameObject inventoryOwner,
  GameObject objectToDrop)
{
  auto& inventoryComponent = *inventoryOwner.getComponent<InventoryComponent>().get();
  auto& inventoryItemComponent = *objectToDrop.getComponent<InventoryItemComponent>().get();

  inventoryItemComponent.setOwner(GameObject());

  if (inventoryItemComponent.shouldComponentBeRestored<MeshRendererComponent>()) {
    m_levelsManager->loadGameObjectComponent<MeshRendererComponent>(objectToDrop);
    inventoryItemComponent.removeComponentToRestore<MeshRendererComponent>();
  }

  if (inventoryItemComponent.shouldComponentBeRestored<RigidBodyComponent>()) {
    m_levelsManager->loadGameObjectComponent<RigidBodyComponent>(objectToDrop);
    inventoryItemComponent.removeComponentToRestore<RigidBodyComponent>();
  }

  // TODO: consider creation component like EntityLifeComponent
  //  for storing Time To Live property. Set the object life start timestamp here.

  inventoryComponent.removeItem(objectToDrop);

  const auto& dropCallback = inventoryItemComponent.getDropCallback();

  if (dropCallback) {
    dropCallback(inventoryOwner, objectToDrop);
  }
}
