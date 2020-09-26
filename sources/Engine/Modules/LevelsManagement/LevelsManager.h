#pragma once

#include <tuple>
#include <utility>

#include "Modules/ECS/ECS.h"
#include "Modules/ResourceManagement/ResourceManager.h"

#include "Utility/xml.h"

#include "GameObjectsLoader.h"

struct LevelData {
 public:
  explicit LevelData(
    std::shared_ptr<pugi::xml_document> levelStaticData,
    std::shared_ptr<pugi::xml_document> levelSpawnData)
    :
    m_levelStaticData(std::move(levelStaticData)),
    m_levelSpawnData(std::move(levelSpawnData))
  {}

  ~LevelData() = default;

 private:
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
  void unloadLevel();

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

  std::shared_ptr<LevelData> m_currentLevelData;
};
