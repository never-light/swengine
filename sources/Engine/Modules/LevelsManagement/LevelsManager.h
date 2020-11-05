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

  template<class T>
  inline void buildGameObjectComponent(GameObject object)
  {
    m_gameObjectsLoader.buildGameObjectComponent<T>(object);
  }

  void loadSpawnObjectsList(const std::string& spawnListName);

  GameObjectsLoader& getObjectsLoader();

  std::shared_ptr<GameWorld> getGameWorld() const;

 private:
  static std::shared_ptr<pugi::xml_document> openLevelDescriptionFile(const std::string& levelName,
    const std::string& descriptionFile,
    const std::string& descriptionNodeName);

  void loadLevelStaticObjects(const std::string& levelName, std::vector<GameObject>& objects);
  void loadLevelDynamicObjects(const std::string& levelName, std::vector<GameObject>& objects);

 private:
  std::shared_ptr<GameWorld> m_gameWorld;
  std::shared_ptr<ResourcesManager> m_resourceManager;

  GameObjectsLoader m_gameObjectsLoader;

  bool m_isLevelLoaded = false;
};
