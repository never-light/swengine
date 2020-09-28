#pragma once

#include <Engine/Modules/LevelsManagement/LevelsManager.h>
#include "InteractiveObjectComponent.h"

class InteractiveObjectsControlSystem : public GameSystem,
                                        public EventsListener<InteractiveObjectActionTriggeredEvent> {
 public:
  explicit InteractiveObjectsControlSystem();
  ~InteractiveObjectsControlSystem() override;

  void configure() override;
  void unconfigure() override;

  void activate() override;
  void deactivate() override;

  EventProcessStatus receiveEvent(
    GameWorld* gameWorld,
    const InteractiveObjectActionTriggeredEvent& event) override;

 private:
  void handleTakeObjectAction(GameObject initiator, GameObject target);
  void handleUseObjectAction(GameObject initiator, GameObject target);
  void handleTalkWithObjectAction(GameObject initiator, GameObject target);
};
