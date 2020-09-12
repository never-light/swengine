#pragma once

#include <utility>

#include "Modules/ECS/ECS.h"
#include "Modules/ResourceManagement/ResourceManager.h"

#include "Utility/xml.h"

#include "GameObjectsLoader.h"

struct LevelLoadUnloadEvent {
 public:
  explicit LevelLoadUnloadEvent(GameObjectId levelObjectId) : m_levelObjectId(levelObjectId)
  {

  }

  [[nodiscard]] GameObjectId getObjectId() const
  {
    return m_levelObjectId;
  }

 private:
  GameObjectId m_levelObjectId;
};

struct LevelLoadEvent : public LevelLoadUnloadEvent {
 public:
  explicit LevelLoadEvent(GameObjectId levelObjectId) : LevelLoadUnloadEvent(levelObjectId)
  {}
};

struct LevelUnloadEvent : public LevelLoadUnloadEvent {
 public:
  explicit LevelUnloadEvent(GameObjectId levelObjectId) : LevelLoadUnloadEvent(levelObjectId)
  {}
};

class LevelComponent {
 public:
  explicit LevelComponent(std::vector<GameObjectId> staticObjects)
    : m_staticObjectsList(std::move(staticObjects))
  {}

  ~LevelComponent() = default;

  [[nodiscard]] const std::vector<GameObjectId>& getStaticObjectsList() const
  {
    return m_staticObjectsList;
  }

 private:
  std::vector<GameObjectId> m_staticObjectsList;
};

class LevelsManager : public std::enable_shared_from_this<LevelsManager> {
 public:
  LevelsManager(std::shared_ptr<GameWorld> gameWorld,
    std::shared_ptr<ResourceManager> resourceManager);
  ~LevelsManager();

  void loadLevel(const std::string& name);
  void unloadCurrentLevel();

  GameObjectsLoader& getObjectsLoader();

 private:
  std::shared_ptr<GameWorld> m_gameWorld;
  std::shared_ptr<ResourceManager> m_resourceManager;

  GameObjectsLoader m_gameObjectsLoader;

  GameObjectId m_currentLevelId = GameObjectNone;
};
