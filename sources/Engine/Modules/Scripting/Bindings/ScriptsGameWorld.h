#pragma once

#include <optional>
#include <glm/vec3.hpp>
#include "Modules/ECS/ECS.h"

class ScriptsGameWorld {
 public:
  explicit ScriptsGameWorld(std::shared_ptr<GameWorld> gameWorld);
  ~ScriptsGameWorld();

  GameObject findGameObject(const std::string& objectName);

  void spawnGameObject(const std::string& spawnName,
    const std::optional<std::string>& objectName,
    const std::optional<glm::vec3>& position,
    const std::optional<glm::vec3>& direction,
    const std::optional<std::string> levelName);

 private:
  std::shared_ptr<GameWorld> m_gameWorld;
};
