#include "InventoryControlSystem.h"

#include <utility>

#include <Engine/Modules/Graphics/GraphicsSystem/MeshRendererComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/TransformComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/GraphicsSceneManagementSystem.h>
#include <Engine/Modules/Physics/RigidBodyComponent.h>

InventoryControlSystem::InventoryControlSystem(std::shared_ptr<LevelsManager> levelsManager)
  : m_levelsManager(std::move(levelsManager))
{

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
  getGameWorld()->subscribeEventsListener<InventoryItemActionCommandEvent>(this);
  getGameWorld()->subscribeEventsListener<InventoryItemTransferCommandEvent>(this);
}

void InventoryControlSystem::deactivate()
{
  getGameWorld()->unsubscribeEventsListener<InventoryItemTransferCommandEvent>(this);
  getGameWorld()->unsubscribeEventsListener<InventoryItemActionCommandEvent>(this);
}

EventProcessStatus InventoryControlSystem::receiveEvent(
  const InventoryItemActionCommandEvent& event)
{
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

      getGameWorld()->emitEvent<InventoryItemActionEvent>(InventoryItemActionEvent(event.inventoryOwner,
        InventoryItemActionTriggerType::Use, event.item));

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

  getGameWorld()->emitEvent<InventoryItemActionEvent>(InventoryItemActionEvent(inventoryOwner,
    InventoryItemActionTriggerType::RelocateToInventory, objectToRelocate));

  if (objectToRelocate.hasComponent<ObjectSceneNodeComponent>()) {
    getGameWorld()->emitEvent<RemoveObjectFromSceneCommandEvent>(
      RemoveObjectFromSceneCommandEvent{objectToRelocate});
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

  auto& inventoryOwnerTransform = *inventoryOwner.getComponent<TransformComponent>().get();

  if (objectToDrop.hasComponent<RigidBodyComponent>()) {
    auto& rigidBodyComponent = *objectToDrop.getComponent<RigidBodyComponent>().get();
    auto& objectToDropLastTransform = *objectToDrop.getComponent<TransformComponent>().get();

    Transform transform = objectToDropLastTransform.getTransform();
    transform.setPosition(inventoryOwnerTransform.getBoundingSphere().getOrigin());

    rigidBodyComponent.setTransform(transform);
  }
  else if (objectToDrop.hasComponent<TransformComponent>()) {
    objectToDrop.getComponent<TransformComponent>()->getTransform().setPosition(
      inventoryOwnerTransform.getBoundingSphere().getOrigin());
  }

  getGameWorld()->emitEvent<AddObjectToSceneCommandEvent>(
    AddObjectToSceneCommandEvent{objectToDrop});

  const auto& dropCallback = inventoryItemComponent.getDropCallback();

  if (dropCallback) {
    dropCallback(inventoryOwner, objectToDrop);
  }

  getGameWorld()->emitEvent<InventoryItemActionEvent>(InventoryItemActionEvent(inventoryOwner,
    InventoryItemActionTriggerType::DropFromInventory, objectToDrop));
}

EventProcessStatus InventoryControlSystem::receiveEvent(const InventoryItemTransferCommandEvent& event)
{
  GameObject initiator = event.initiator;
  GameObject target = event.target;
  GameObject item = event.item;

  auto& initiatorInventoryComponent = *initiator.getComponent<InventoryComponent>().get();
  auto& targetInventoryComponent = *target.getComponent<InventoryComponent>().get();
  auto& inventoryItemComponent = *item.getComponent<InventoryItemComponent>().get();

  SW_ASSERT(initiatorInventoryComponent.hasItem(inventoryItemComponent.getId()));

  const auto& dropCallback = inventoryItemComponent.getDropCallback();

  if (dropCallback) {
    dropCallback(initiator, item);
  }

  initiatorInventoryComponent.removeItem(item);

  getGameWorld()->emitEvent<InventoryItemActionEvent>(InventoryItemActionEvent(initiator,
    InventoryItemActionTriggerType::DropFromInventory, item));

  targetInventoryComponent.addItem(item);

  getGameWorld()->emitEvent<InventoryItemActionEvent>(InventoryItemActionEvent(target,
    InventoryItemActionTriggerType::RelocateToInventory, item));

  const auto& takeCallback = inventoryItemComponent.getTakeCallback();

  if (takeCallback) {
    takeCallback(initiator, item);
  }

  getGameWorld()->emitEvent<InventoryItemTransferEvent>(InventoryItemTransferEvent(initiator, target, item));

  return EventProcessStatus::Processed;
}
