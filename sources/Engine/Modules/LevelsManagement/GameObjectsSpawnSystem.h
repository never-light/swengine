#pragma once

#include <optional>

#include "Modules/ECS/ECS.h"
#include "LevelsManager.h"

struct SpawnGameObjectCommandEvent {
 public:
  SpawnGameObjectCommandEvent(std::string objectSpawnName, const std::optional<glm::vec3>& position,
    const std::optional<glm::vec3>& direction,
    std::optional<std::string>  objectName = {},
    std::optional<std::string>  levelName = {});

  [[nodiscard]] const std::string& getObjectSpawnName() const;
  [[nodiscard]] const std::optional<glm::vec3>& getPosition() const;
  [[nodiscard]] const std::optional<glm::vec3>& getDirection() const;
  [[nodiscard]] const std::optional<std::string>& getObjectName() const;
  [[nodiscard]] const std::optional<std::string>& getLevelName() const;

 private:
  std::string m_objectSpawnName;
  std::optional<glm::vec3> m_position;
  std::optional<glm::vec3> m_direction;
  std::optional<std::string> m_objectName;
  std::optional<std::string> m_levelName;
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
