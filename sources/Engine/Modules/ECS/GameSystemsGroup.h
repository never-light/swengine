#pragma once

#include <vector>
#include <memory>

#include "GameSystem.h"

class GameWorld;

class GameSystemsGroup : public GameSystem {
 public:
  GameSystemsGroup();
  explicit GameSystemsGroup(GameWorld* gameWorld);

  ~GameSystemsGroup() override;

  void configure() override;
  void unconfigure() override;
  [[nodiscard]] bool isConfigured() const;

  void beforeRender() override;
  void render() override;
  void afterRender() override;

  void fixedUpdate(float delta) override;
  void update(float delta) override;

  void activate() override;
  void deactivate() override;

  virtual void addGameSystem(std::shared_ptr<GameSystem> system);
  virtual void removeGameSystem(std::shared_ptr<GameSystem> system);

  template<class T>
  std::shared_ptr<T> getGameSystem() const;

  [[nodiscard]] const std::vector<std::shared_ptr<GameSystem>>& getGameSystems() const;

 private:
  std::vector<std::shared_ptr<GameSystem>> m_gameSystems;
  bool m_isConfigured = false;
};

template<class T>
std::shared_ptr<T> GameSystemsGroup::getGameSystem() const
{
  static_assert(std::is_base_of_v<GameSystem, T>);

  for (std::shared_ptr<GameSystem> gameSystem : m_gameSystems) {
    std::shared_ptr<T> foundGameSystem = std::dynamic_pointer_cast<T>(gameSystem);

    if (foundGameSystem != nullptr) {
      return foundGameSystem;
    }
  }

  return nullptr;
}

