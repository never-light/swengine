#pragma once

#include <Engine/Modules/ECS/ECS.h>
#include "InventoryItemComponent.h"

enum class InventoryItemActionTriggerType {
  Use,
  RelocateToInventory,
  DropFromInventory,
  //Show
};

struct InventoryItemActionEvent {
 public:
  InventoryItemActionEvent(GameObject inventoryOwner,
    InventoryItemActionTriggerType triggerType,
    GameObject item)
    : inventoryOwner(inventoryOwner),
      triggerType(triggerType),
      item(item)
  {

  }

 public:
  GameObject inventoryOwner;
  InventoryItemActionTriggerType triggerType;

  GameObject item;
};

struct InventoryItemActionCommandEvent : public InventoryItemActionEvent {
 public:
  InventoryItemActionCommandEvent(GameObject inventoryOwner,
    InventoryItemActionTriggerType triggerType,
    GameObject item)
    : InventoryItemActionEvent(inventoryOwner, triggerType, item)
  {

  }
};

struct InventoryItemTransferEvent {
 public:
  InventoryItemTransferEvent(
    GameObject initiator,
    GameObject target,
    GameObject item)
    : initiator(initiator),
      target(target),
      item(item)
  {

  }

 public:
  GameObject initiator;
  GameObject target;
  GameObject item;
};

struct InventoryItemTransferCommandEvent : public InventoryItemTransferEvent {
 public:
  InventoryItemTransferCommandEvent(
    GameObject initiator,
    GameObject target,
    GameObject item)
    : InventoryItemTransferEvent(initiator, target, item)
  {

  }
};

class InventoryComponent {
 public:
  InventoryComponent() = default;

  void addItem(GameObject item);
  void removeItem(GameObject item);

  [[nodiscard]] const std::vector<GameObject>& getItems() const;

  [[nodiscard]] bool hasItem(const std::string& itemId) const;
  [[nodiscard]] GameObject getItem(const std::string& itemId) const;

 private:
  std::vector<GameObject> m_inventoryItems;
};
