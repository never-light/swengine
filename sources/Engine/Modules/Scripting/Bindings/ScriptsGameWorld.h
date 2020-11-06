#pragma once

#include <glm/vec3.hpp>
#include "Modules/ECS/ECS.h"

class ScriptsGameWorld {
 public:
  explicit ScriptsGameWorld(std::shared_ptr<GameWorld> gameWorld);
  ~ScriptsGameWorld();

  GameObject findGameObject(const std::string& objectName);

  void spawnGameObject(const std::string& spawnName, const glm::vec3& position, const glm::vec3& direction);
  void spawnUniqueGameObject(const std::string& spawnName, const std::string& objectName,
    const glm::vec3& position, const glm::vec3& direction);

 private:
  std::shared_ptr<GameWorld> m_gameWorld;
};
