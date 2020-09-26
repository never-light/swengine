#pragma once

#include <Engine/Modules/ECS/ECS.h>
#include <Engine/Modules/Application/GameConsole.h>
#include <Engine/Modules/Graphics/GraphicsSystem/CameraComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/GraphicsScene.h>
#include <Engine/Modules/Input/InputModule.h>

#include "PlayerComponent.h"

class FreeCameraControlSystem : public GameSystem,
                                public EventsListener<InputActionToggleEvent> {
 public:
  explicit FreeCameraControlSystem(
    std::shared_ptr<InputModule> inputModule,
    std::shared_ptr<GraphicsScene> graphicsScene);
  ~FreeCameraControlSystem() override = default;

  void configure() override;
  void unconfigure() override;

  void activate() override;
  void deactivate() override;

  void update(float delta) override;

  EventProcessStatus receiveEvent(GameWorld* gameWorld, const InputActionToggleEvent& event) override;

 private:
  std::shared_ptr<InputModule> m_inputModule;
  std::shared_ptr<GraphicsScene> m_graphicsScene;
  std::shared_ptr<Camera> m_freeCamera;

 private:
  static constexpr float MOVEMENT_SPEED = 3.5f;
};

