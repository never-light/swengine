#pragma once

#include <Engine/Modules/ECS/ECS.h>
#include "InventoryItemComponent.h"

enum class InventoryItemActionTriggerType {
  Use,
  RelocateToInventory,
  DropFromInventory,
  //Show
};

struct InventoryItemActionTriggeredEvent {
 public:
  InventoryItemActionTriggeredEvent(GameObject inventoryOwner,
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

class InventoryComponent {
 public:
  InventoryComponent() = default;

  void addItem(GameObject item);
  void removeItem(GameObject item);

  [[nodiscard]] const std::vector<GameObject>& getItems() const;

 private:
  std::vector<GameObject> m_inventoryItems;
};
