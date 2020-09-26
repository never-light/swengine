#pragma once

#include <Engine/Modules/ECS/ECS.h>
#include <Engine/Modules/Application/GameConsole.h>
#include <Engine/Modules/Graphics/GraphicsSystem/CameraComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/GraphicsScene.h>
#include <Engine/Modules/Input/InputModule.h>

#include "Game/Inventory/InventoryUI.h"

#include "PlayerComponent.h"

class PlayerControlSystem : public GameSystem,
                            public EventsListener<MouseWheelEvent>,
                            public EventsListener<InputActionToggleEvent>,
                            public EventsListener<KeyboardEvent> {
 public:
  explicit PlayerControlSystem(
    std::shared_ptr<InputModule> inputModule,
    std::shared_ptr<GraphicsScene> graphicsScene,
    std::shared_ptr<GUILayout> playerUILayout,
    std::shared_ptr<InventoryUI> inventoryUILayout);
  ~PlayerControlSystem() override = default;

  void configure() override;
  void unconfigure() override;

  void activate() override;
  void deactivate() override;

  void fixedUpdate(float delta) override;
  void update(float delta) override;

  void render() override;

  EventProcessStatus receiveEvent(GameWorld* gameWorld, const MouseWheelEvent& event) override;
  EventProcessStatus receiveEvent(GameWorld* gameWorld, const InputActionToggleEvent& event) override;
  EventProcessStatus receiveEvent(GameWorld* gameWorld, const KeyboardEvent& event) override;

  void disableMovementControl();
  void enableMovementControl();

 private:
  [[nodiscard]] Camera& getPlayerCamera();

  void showGUIWindow(std::shared_ptr<GUILayout> window);
  void hideGUIWindow(std::shared_ptr<GUILayout> window);

 private:
  GameObject m_playerObject;

  int16_t m_walkAnimationStateId = -1;
  int16_t m_idleAnimationStateId = -1;

  std::shared_ptr<InputModule> m_inputModule;
  std::shared_ptr<GraphicsScene> m_graphicsScene;

  std::shared_ptr<GUILayout> m_uiLayout;
  std::shared_ptr<InventoryUI> m_inventoryUI;

  bool m_isMovementControlEnabled{};
  bool m_isUIModeActive{};
};

