#pragma once

#include <tuple>
#include <utility>

#include "Modules/ECS/ECS.h"
#include "Modules/ResourceManagement/ResourceManager.h"

#include "Utility/xml.h"

#include "GameObjectsLoader.h"

struct LevelLoadUnloadEvent {
 public:
  explicit LevelLoadUnloadEvent(GameObject levelObject)
    : m_levelObject(levelObject)
  {

  }

  [[nodiscard]] GameObject getObject() const
  {
    return m_levelObject;
  }

 private:
  GameObject m_levelObject;
};

struct LevelLoadEvent : public LevelLoadUnloadEvent {
 public:
  explicit LevelLoadEvent(GameObject levelObject) : LevelLoadUnloadEvent(levelObject)
  {}
};

struct LevelUnloadEvent : public LevelLoadUnloadEvent {
 public:
  explicit LevelUnloadEvent(GameObject levelObject) : LevelLoadUnloadEvent(levelObject)
  {}
};

struct LevelComponent {
 public:
  explicit LevelComponent(
    std::vector<GameObject> staticObjects,
    std::vector<GameObject> dynamicObjects,
    std::shared_ptr<pugi::xml_document> levelStaticData,
    std::shared_ptr<pugi::xml_document> levelSpawnData)
    : m_staticObjectsList(std::move(staticObjects)),
      m_dynamicObjectsList(std::move(dynamicObjects)),
      m_levelStaticData(std::move(levelStaticData)),
      m_levelSpawnData(std::move(levelSpawnData))
  {}

  ~LevelComponent() = default;

  [[nodiscard]] const std::vector<GameObject>& getStaticObjectsList() const
  {
    return m_staticObjectsList;
  }

  [[nodiscard]] const std::vector<GameObject>& getDynamicObjectsList() const
  {
    return m_dynamicObjectsList;
  }

 private:
  std::vector<GameObject> m_staticObjectsList;
  std::vector<GameObject> m_dynamicObjectsList;

  std::shared_ptr<pugi::xml_document> m_levelStaticData;
  std::shared_ptr<pugi::xml_document> m_levelSpawnData;
};

struct GameObjectDeclarationComponent {
 public:
  explicit GameObjectDeclarationComponent(const pugi::xml_node declarationNode)
    : m_declarationNode(declarationNode)
  {

  }

  [[nodiscard]] const pugi::xml_node& getDeclarationNode() const
  {
    return m_declarationNode;
  }

 private:
  pugi::xml_node m_declarationNode;
};

class LevelsManager : public std::enable_shared_from_this<LevelsManager> {
 public:
  LevelsManager(std::shared_ptr<GameWorld> gameWorld,
    std::shared_ptr<ResourceManager> resourceManager);
  ~LevelsManager();

  void loadLevel(const std::string& name);
  void unloadCurrentLevel();

  template<class T>
  inline void loadGameObjectComponent(GameObject object)
  {
    pugi::xml_node objectNode = object.getComponent<GameObjectDeclarationComponent>()->getDeclarationNode();

    m_gameObjectsLoader.loadGameObjectComponent<T>(objectNode, object);
  }

  GameObjectsLoader& getObjectsLoader();

 private:
  static std::shared_ptr<pugi::xml_document> openLevelDescriptionFile(const std::string& levelName,
    const std::string& descriptionFile,
    const std::string& descriptionNodeName);

  std::shared_ptr<pugi::xml_document> loadLevelStaticObjects(const std::string& levelName,
    std::vector<GameObject>& objects);
  std::shared_ptr<pugi::xml_document> loadLevelDynamicObjects(const std::string& levelName,
    std::vector<GameObject>& objects);

 private:
  std::shared_ptr<GameWorld> m_gameWorld;
  std::shared_ptr<ResourceManager> m_resourceManager;

  GameObjectsLoader m_gameObjectsLoader;

  GameObject m_currentLevel{};
};
