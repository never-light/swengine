#pragma once

#include <Engine/Modules/ECS/ECS.h>
#include <Engine/Modules/Application/GameConsole.h>
#include <Engine/Modules/Graphics/GraphicsSystem/CameraComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/SharedGraphicsState.h>
#include <Engine/Modules/Input/InputModule.h>

#include "PlayerComponent.h"

class PlayerControlSystem : public GameSystem,
                            public EventsListener<MouseWheelEvent>,
                            public EventsListener<InputActionToggleEvent> {
 public:
  explicit PlayerControlSystem(std::shared_ptr<InputModule> inputModule,
    std::shared_ptr<SharedGraphicsState> sharedGraphicsState);
  ~PlayerControlSystem() override = default;

  void configure(GameWorld* gameWorld) override;
  void unconfigure(GameWorld* gameWorld) override;

  void update(GameWorld* gameWorld, float delta) override;
  void render(GameWorld* gameWorld) override;

  EventProcessStatus receiveEvent(GameWorld* gameWorld, const MouseWheelEvent& event) override;
  EventProcessStatus receiveEvent(GameWorld* gameWorld, const InputActionToggleEvent& event) override;

 private:
  [[nodiscard]] Camera* getPlayerCamera() const;
  [[nodiscard]] Transform* getPlayerTransform() const;

  void updateViewParameters(const glm::vec2& mouseDelta, float delta);
  void updatePlayerAndCameraPosition(float delta);

 private:
  GameObject* m_playerObject = nullptr;
  int16_t m_runningAnimationStateId = -1;
  int16_t m_idleAnimationStateId = -1;

  std::shared_ptr<InputModule> m_inputModule;
  std::shared_ptr<SharedGraphicsState> m_sharedGraphicsState;
};

