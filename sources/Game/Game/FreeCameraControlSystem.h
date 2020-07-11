#pragma once

#include <Engine/Modules/ECS/ECS.h>
#include <Engine/Modules/Application/GameConsole.h>
#include <Engine/Modules/Graphics/GraphicsSystem/CameraComponent.h>
#include <Engine/Modules/Input/InputModule.h>

#include "PlayerComponent.h"

class FreeCameraControlSystem : public GameSystem,
                                public EventsListener<InputActionToggleEvent> {
 public:
  explicit FreeCameraControlSystem(std::shared_ptr<InputModule> inputModule,
    std::shared_ptr<SharedGraphicsState> sharedGraphicsState);
  ~FreeCameraControlSystem() override = default;

  void configure(GameWorld* gameWorld) override;
  void unconfigure(GameWorld* gameWorld) override;

  void update(GameWorld* gameWorld, float delta) override;

  EventProcessStatus receiveEvent(GameWorld* gameWorld, const InputActionToggleEvent& event) override;

 private:
  std::shared_ptr<InputModule> m_inputModule;
  std::shared_ptr<SharedGraphicsState> m_sharedGraphicsState;
  std::shared_ptr<Camera> m_freeCamera;

 private:
  static constexpr float MOVEMENT_SPEED = 3.5f;
};

