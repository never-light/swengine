#pragma once

#include <optional>

#include "Modules/ECS/ECS.h"
#include "LevelsManager.h"

struct SpawnGameObjectCommandEvent {
 public:
  SpawnGameObjectCommandEvent(std::string objectSpawnName, const glm::vec3& position, const glm::vec3& direction,
    const std::optional<std::string>& objectName = {});

  [[nodiscard]] const std::string& getObjectSpawnName() const;
  [[nodiscard]] const glm::vec3& getPosition() const;
  [[nodiscard]] const glm::vec3& getDirection() const;
  [[nodiscard]] const std::optional<std::string>& getObjectName() const;

 private:
  std::string m_objectSpawnName;
  glm::vec3 m_position;
  glm::vec3 m_direction;
  const std::optional<std::string>& m_objectName;
};

class GameObjectsSpawnSystem : public GameSystem,
                               public EventsListener<SpawnGameObjectCommandEvent> {
 public:
  explicit GameObjectsSpawnSystem(std::shared_ptr<LevelsManager> levelsManager);

  void activate() override;
  void deactivate() override;

  EventProcessStatus receiveEvent(const SpawnGameObjectCommandEvent& event) override;

 private:
  std::shared_ptr<LevelsManager> m_levelsManager;
};
