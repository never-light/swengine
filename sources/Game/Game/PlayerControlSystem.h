#pragma once

#include <Engine/Modules/ECS/ECS.h>
#include <Engine/Modules/Application/GameConsole.h>
#include <Engine/Modules/Graphics/GraphicsSystem/CameraComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/GraphicsScene.h>
#include <Engine/Modules/Input/InputModule.h>

#include "Game/Inventory/InventoryUI.h"
#include "Game/Dynamic/DialoguesUI.h"
#include "Game/Dynamic/ActorComponent.h"

#include "PlayerComponent.h"

struct PlayerUILayout {
  std::shared_ptr<GUILayout> playerUILayout;
  std::shared_ptr<InventoryUI> inventoryUI;
  std::shared_ptr<GUILayout> interactionUI;
  std::shared_ptr<GUIText> interactionUIText;
  std::shared_ptr<DialoguesUI> dialoguesUI;
};

class PlayerControlSystem : public GameSystem,
                            public EventsListener<MouseWheelEvent>,
                            public EventsListener<InputActionToggleEvent>,
                            public EventsListener<KeyboardEvent>,
                            public EventsListener<ActorTalkTriggerCommandEvent> {
 public:
  explicit PlayerControlSystem(
    std::shared_ptr<InputModule> inputModule,
    std::shared_ptr<GraphicsScene> graphicsScene,
    PlayerUILayout uiLayout);
  ~PlayerControlSystem() override = default;

  void configure() override;
  void unconfigure() override;

  void activate() override;
  void deactivate() override;

  void fixedUpdate(float delta) override;
  void update(float delta) override;

  void render() override;

  EventProcessStatus receiveEvent(const MouseWheelEvent& event) override;
  EventProcessStatus receiveEvent(const InputActionToggleEvent& event) override;
  EventProcessStatus receiveEvent(const KeyboardEvent& event) override;
  EventProcessStatus receiveEvent(const ActorTalkTriggerCommandEvent& event) override;

  void disableMovementControl();
  void enableMovementControl();

 private:
  [[nodiscard]] Camera& getPlayerCamera();

  void showGUIWindow(std::shared_ptr<GUILayout> window);
  void hideGUIWindow();

  [[nodiscard]] bool isGUIWindowModeActive() const;

  void processNearestInteractiveObjects(const Transform& playerTransform);
  [[nodiscard]] GameObject findNearestInteractiveObject(const Transform& playerTransform);

  void performInteractiveAction();

 private:
  GameObject m_playerObject;

  int16_t m_walkAnimationStateId = -1;
  int16_t m_idleAnimationStateId = -1;

  std::shared_ptr<InputModule> m_inputModule;
  std::shared_ptr<GraphicsScene> m_graphicsScene;

  PlayerUILayout m_uiLayout;

  std::shared_ptr<GUILayout> m_activeGUIWindow;

  bool m_isMovementControlEnabled{};
};

