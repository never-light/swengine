#pragma once

#include <Engine/Modules/Input/InputModule.h>
#include <Engine/Modules/Graphics/OpenGL/GLGraphicsContext.h>
#include <Engine/Modules/Graphics/GraphicsSystem/SharedGraphicsState.h>
#include <Engine/Modules/ResourceManagement/ResourceManager.h>
#include <Engine/Modules/ECS/GameSystemsGroup.h>

#include "GameLevel.h"
#include "PlayerControlSystem.h"
#include "FreeCameraControlSystem.h"

class Game : public EventsListener<GameConsoleCommandEvent> {
 public:
  Game(std::shared_ptr<GameWorld> gameWorld,
    std::shared_ptr<GameSystemsGroup> gameApplicationSystemsGroup,
    std::shared_ptr<InputModule> inputModule,
    std::shared_ptr<GLGraphicsContext> graphicsContext,
    std::shared_ptr<SharedGraphicsState> sharedGraphicsState,
    std::shared_ptr<ResourceManager> resourceManager);

  ~Game() = default;

  void activate();
  void deactivate();

  void enterConsoleMode();
  void leaveConsoleMode();

  EventProcessStatus receiveEvent(GameWorld* gameWorld, const GameConsoleCommandEvent& event) override;

 private:
  std::shared_ptr<GameWorld> m_gameWorld;
  std::shared_ptr<InputModule> m_inputModule;
  std::shared_ptr<GLGraphicsContext> m_graphicsContext;
  std::shared_ptr<SharedGraphicsState> m_sharedGraphicsState;
  std::shared_ptr<ResourceManager> m_resourceManager;

  std::shared_ptr<GameLevel> m_level;

  std::shared_ptr<GameSystemsGroup> m_gameApplicationSystems;
  std::shared_ptr<GameSystemsGroup> m_gameModeSystems;
  std::shared_ptr<PlayerControlSystem> m_playerControlSystem;
  std::shared_ptr<FreeCameraControlSystem> m_freeCameraControlSystem;

  std::shared_ptr<GameSystem> m_preservedCameraControlSystem;
  std::shared_ptr<GameSystem> m_activeCameraControlSystem;
};
