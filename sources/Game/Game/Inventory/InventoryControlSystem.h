#pragma once

#include "InventoryComponent.h"

class InventoryControlSystem : public GameSystem,
                               public EventsListener<InventoryItemActionTriggeredEvent> {
 public:
  InventoryControlSystem();
  ~InventoryControlSystem() override;

  void configure() override;
  void unconfigure() override;

  void activate() override;
  void deactivate() override;

  EventProcessStatus receiveEvent(GameWorld* gameWorld, const InventoryItemActionTriggeredEvent& event) override;

 private:
  void relocateObjectToInventory(GameObject inventoryOwner, GameObject objectToRelocate);
  void dropObjectFromInventory(GameObject inventoryOwner, GameObject objectToDrop);
};
