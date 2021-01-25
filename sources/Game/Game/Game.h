#pragma once

#include <Engine/Modules/Input/InputModule.h>
#include <Engine/Modules/Graphics/OpenGL/GLGraphicsContext.h>
#include <Engine/Modules/Graphics/GraphicsSystem/GraphicsScene.h>
#include <Engine/Modules/ResourceManagement/ResourcesManagement.h>
#include <Engine/Modules/ECS/GameSystemsGroup.h>
#include <Engine/Modules/LevelsManagement/LevelsManager.h>

#include "PlayerControlSystem.h"
#include "FreeCameraControlSystem.h"

#include "Game/Inventory/InventoryUI.h"
#include "Game/Inventory/InventoryControlSystem.h"
#include "Game/Dynamic/InteractiveObjectsControlSystem.h"
#include "Game/Dynamic/QuestsSystem.h"
#include "Game/Dynamic/InfoportionsSystem.h"

class Game : public EventsListener<GameConsoleCommandEvent> {
 public:
  Game(std::shared_ptr<GameWorld> gameWorld,
    std::shared_ptr<GameSystemsGroup> gameApplicationSystemsGroup,
    std::shared_ptr<InputModule> inputModule,
    std::shared_ptr<GLGraphicsContext> graphicsContext,
    std::shared_ptr<GraphicsScene> graphicsScene,
    std::shared_ptr<GUISystem> guiSystem,
    std::shared_ptr<ResourcesManager> resourceManager,
    std::shared_ptr<LevelsManager> levelsManager,
    std::shared_ptr<GUILayout> gameUILayout);

  ~Game() override = default;

  void activate();
  void deactivate();

  void enterConsoleMode();
  void leaveConsoleMode();

  void createNewGame(const std::string& levelName);
  void load(const std::string& levelName, LevelLoadingMode levelLoadingMode = LevelLoadingMode::AllData);
  void unload();

  void setupGameState();

  EventProcessStatus receiveEvent(const GameConsoleCommandEvent& event) override;

  [[nodiscard]] bool isLoaded() const;

 private:
  std::shared_ptr<GameWorld> m_gameWorld;
  std::shared_ptr<InputModule> m_inputModule;
  std::shared_ptr<GLGraphicsContext> m_graphicsContext;
  std::shared_ptr<GraphicsScene> m_graphicsScene;
  std::shared_ptr<GUISystem> m_guiSystem;
  std::shared_ptr<ResourcesManager> m_resourceManager;
  std::shared_ptr<LevelsManager> m_levelsManager;

  std::shared_ptr<GameSystemsGroup> m_gameApplicationSystems;
  std::shared_ptr<GameSystemsGroup> m_gameModeSystems;
  std::shared_ptr<PlayerControlSystem> m_playerControlSystem;
  std::shared_ptr<FreeCameraControlSystem> m_freeCameraControlSystem;

  std::shared_ptr<InventoryControlSystem> m_inventoryControlSystem;
  std::shared_ptr<InteractiveObjectsControlSystem> m_interactiveObjectsControlSystem;
  std::shared_ptr<GameLogicConditionsManager> m_gameLogicConditionsManager;
  std::shared_ptr<QuestsStorage> m_questsStorage;
  std::shared_ptr<QuestsSystem> m_questsSystem;
  std::shared_ptr<InfoportionsSystem> m_infoportionsSystem;
  std::shared_ptr<DialoguesManager> m_dialoguesManager;

  std::shared_ptr<GameSystem> m_preservedCameraControlSystem;
  std::shared_ptr<GameSystem> m_activeCameraControlSystem;

  std::shared_ptr<GUILayout> m_gameUILayout;
  PlayerUILayout m_playerUILayout{};

  bool m_isGameLoaded = false;
  bool m_isGamePreloaded = false;
};
