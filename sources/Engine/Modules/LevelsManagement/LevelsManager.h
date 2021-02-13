#pragma once

#include <tuple>
#include <utility>

#include "Modules/ECS/ECS.h"
#include "Modules/ResourceManagement/ResourcesManagement.h"

#include "Utility/xml.h"

#include "GameObjectsLoader.h"

class LevelsManager : public std::enable_shared_from_this<LevelsManager> {
 public:
  LevelsManager(const std::shared_ptr<GameWorld>& gameWorld,
    const std::shared_ptr<ResourcesManager>& resourceManager);
  ~LevelsManager();

  void loadLevel(const std::string& name);
  void unloadLevel();
  void unloadSpawnLists();

  template<class T>
  inline void buildGameObjectComponent(GameObject object)
  {
    m_gameObjectsLoader.buildGameObjectComponent<T>(object);
  }

  void loadSpawnObjectsList(const std::string& spawnListName);

  void loadLevelsSpawnLists();
  void loadLevelSpawnList(const std::string& levelName);

  GameObjectsLoader& getObjectsLoader();

  std::shared_ptr<GameWorld> getGameWorld() const;

  [[nodiscard]] bool isLevelLoaded() const;
  [[nodiscard]] const std::string& getLoadedLevelName() const;

 private:
  static std::shared_ptr<pugi::xml_document> openLevelDescriptionFile(const std::string& levelName,
    const std::string& descriptionFile,
    const std::string& descriptionNodeName);

  void loadLevelStaticObjects(const std::string& levelName, std::vector<std::string>& objects);
  void loadSpawnList(const std::string& path);

 private:
  std::shared_ptr<GameWorld> m_gameWorld;
  std::shared_ptr<ResourcesManager> m_resourceManager;

  GameObjectsLoader m_gameObjectsLoader;

  bool m_isLevelLoaded = false;
  std::string m_loadedLevelName;
};
