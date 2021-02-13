#pragma once

#include "ECS.h"

struct ChangeGameObjectOnlineStatusCommandEvent {
  GameObject gameObject;
  bool makeOnline;
};

struct GameObjectOnlineStatusChangeEvent {
  GameObject gameObject;
  bool makeOnline;
};

class OnlineManagementSystem : public GameSystem,
                               public EventsListener<ChangeGameObjectOnlineStatusCommandEvent> {
 public:
  OnlineManagementSystem() = default;
  ~OnlineManagementSystem() override = default;

  void configure() override;
  void unconfigure() override;

  EventProcessStatus receiveEvent(const ChangeGameObjectOnlineStatusCommandEvent& event) override;
};
