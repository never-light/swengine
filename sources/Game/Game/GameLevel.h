#pragma once

#include <Engine/Modules/ECS/ECS.h>
#include <Engine/Modules/Graphics/OpenGL/GLGraphicsContext.h>
#include <Engine/Modules/ResourceManagement/ResourceManager.h>
#include <Engine/Modules/LevelsManagement/LevelsManager.h>

class GameLevel {
 public:
  GameLevel(std::shared_ptr<GameWorld> gameWorld,
    std::shared_ptr<GLGraphicsContext> graphicsContext,
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<LevelsManager> levelsManager);

  [[nodiscard]] GameObject getPlayer() const;

 private:
  std::shared_ptr<GameWorld> m_gameWorld;
  std::shared_ptr<GLGraphicsContext> m_graphicsContext;
  std::shared_ptr<ResourceManager> m_resourceManager;
  std::shared_ptr<LevelsManager> m_levelsManager;

  GameObject m_player;
};

