#pragma once

#include <Engine/Modules/LevelsManagement/LevelsManager.h>
#include "InventoryComponent.h"

class InventoryControlSystem : public GameSystem,
                               public EventsListener<InventoryItemActionTriggeredEvent> {
 public:
  explicit InventoryControlSystem(std::shared_ptr<LevelsManager> levelsManager);
  ~InventoryControlSystem() override;

  void configure() override;
  void unconfigure() override;

  void activate() override;
  void deactivate() override;

  EventProcessStatus receiveEvent(GameWorld* gameWorld, const InventoryItemActionTriggeredEvent& event) override;

 private:
  void relocateObjectToInventory(GameObject inventoryOwner, GameObject objectToRelocate);
  void dropObjectFromInventory(GameObject inventoryOwner, GameObject objectToDrop);

 private:
  std::shared_ptr<LevelsManager> m_levelsManager;
};
