#pragma once

#include <Engine/Modules/Input/InputModule.h>
#include <Engine/Modules/Graphics/OpenGL/GLGraphicsContext.h>
#include <Engine/Modules/Graphics/GraphicsSystem/GraphicsScene.h>
#include <Engine/Modules/ResourceManagement/ResourceManager.h>
#include <Engine/Modules/ECS/GameSystemsGroup.h>
#include <Engine/Modules/LevelsManagement/LevelsManager.h>

#include "GameLevel.h"
#include "PlayerControlSystem.h"
#include "FreeCameraControlSystem.h"

#include "Game/Inventory/InventoryUI.h"
#include "Game/Inventory/InventoryControlSystem.h"
#include "Game/Dynamic/InteractiveObjectsControlSystem.h"

class Game : public EventsListener<GameConsoleCommandEvent> {
 public:
  Game(std::shared_ptr<GameWorld> gameWorld,
    std::shared_ptr<GameSystemsGroup> gameApplicationSystemsGroup,
    std::shared_ptr<InputModule> inputModule,
    std::shared_ptr<GLGraphicsContext> graphicsContext,
    std::shared_ptr<GraphicsScene> graphicsScene,
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<LevelsManager> levelsManager,
    std::shared_ptr<GUILayout> gameUILayout,
    const PlayerUILayout& playerUILayout);

  ~Game() override = default;

  void activate();
  void deactivate();

  void enterConsoleMode();
  void leaveConsoleMode();

  EventProcessStatus receiveEvent(GameWorld* gameWorld, const GameConsoleCommandEvent& event) override;

 private:
  std::shared_ptr<GameWorld> m_gameWorld;
  std::shared_ptr<InputModule> m_inputModule;
  std::shared_ptr<GLGraphicsContext> m_graphicsContext;
  std::shared_ptr<GraphicsScene> m_graphicsScene;
  std::shared_ptr<ResourceManager> m_resourceManager;
  std::shared_ptr<LevelsManager> m_levelsManager;

  std::shared_ptr<GameLevel> m_level;

  std::shared_ptr<GameSystemsGroup> m_gameApplicationSystems;
  std::shared_ptr<GameSystemsGroup> m_gameModeSystems;
  std::shared_ptr<PlayerControlSystem> m_playerControlSystem;
  std::shared_ptr<FreeCameraControlSystem> m_freeCameraControlSystem;

  std::shared_ptr<InventoryControlSystem> m_inventoryControlSystem;
  std::shared_ptr<InteractiveObjectsControlSystem> m_interactiveObjectsControlSystem;

  std::shared_ptr<GameSystem> m_preservedCameraControlSystem;
  std::shared_ptr<GameSystem> m_activeCameraControlSystem;

  std::shared_ptr<GUILayout> m_gameUILayout;
  PlayerUILayout m_playerUILayout;
};
