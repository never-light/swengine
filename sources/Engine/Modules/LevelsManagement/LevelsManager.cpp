#include "precompiled.h"

#pragma hdrstop

#include "LevelsManager.h"

#include <utility>

#include "Modules/Graphics/GraphicsSystem/GraphicsSceneManagementSystem.h"
#include "Modules/Math/MathUtils.h"
#include "Utility/files.h"

LevelsManager::LevelsManager(const std::shared_ptr<GameWorld>& gameWorld,
  const std::shared_ptr<ResourcesManager>& resourceManager)
  : m_gameWorld(gameWorld),
    m_resourceManager(resourceManager),
    m_gameObjectsLoader(gameWorld, resourceManager)
{

}

LevelsManager::~LevelsManager()
{
  unloadLevel();
}

void LevelsManager::unloadLevel()
{
  if (m_isLevelLoaded) {
    m_gameWorld->emitEvent<UnloadSceneCommandEvent>(UnloadSceneCommandEvent{});

    for (GameObject object : m_gameWorld->all()) {
      m_gameWorld->removeGameObject(object);
    }

    m_isLevelLoaded = false;
  }
}

void LevelsManager::loadLevelStaticObjects(
  const std::string& levelName,
  std::vector<GameObject>& objectsIds)
{
  spdlog::info("Load level static objects: {}", levelName);

  auto levelDescriptionDocument = openLevelDescriptionFile(levelName,
    "level_static",
    "objects");

  pugi::xml_node levelDescription = levelDescriptionDocument->child("objects");

  for (pugi::xml_node& objectNode : levelDescription.children("object")) {
    auto transformNode = objectNode.child("transform");

    if (transformNode) {
      if (transformNode.attribute("static")) {
        THROW_EXCEPTION(EngineRuntimeException, "Level static objects shouldn't use static attribute");
      }

      transformNode.append_attribute("static") = "true";
    }
  }

  for (const pugi::xml_node& objectNode : levelDescription.children("object")) {
    std::string gameObjectSpawnName = m_gameObjectsLoader.loadGameObject(objectNode);

    GameObject gameObject = m_gameObjectsLoader.buildGameObject(gameObjectSpawnName);
    objectsIds.push_back(gameObject);
  }
}

void LevelsManager::loadLevelDynamicObjects(
  const std::string& levelName,
  std::vector<GameObject>& objects)
{
  spdlog::info("Load level dynamic objects: {}", levelName);

  auto levelDescriptionDocument = openLevelDescriptionFile(levelName,
    "level_spawn",
    "objects");

  pugi::xml_node levelDescription = levelDescriptionDocument->child("objects");

  for (pugi::xml_node& objectNode : levelDescription.children("object")) {
    auto transformNode = objectNode.child("transform");

    if (transformNode) {
      if (transformNode.attribute("static")) {
        THROW_EXCEPTION(EngineRuntimeException, "Level dynamic objects shouldn't use static attribute");
      }

      transformNode.append_attribute("static") = "false";
    }
  }

  for (const pugi::xml_node& objectNode : levelDescription.children("object")) {
    std::string gameObjectSpawnName = m_gameObjectsLoader.loadGameObject(objectNode);

    GameObject gameObject = m_gameObjectsLoader.buildGameObject(gameObjectSpawnName);
    objects.push_back(gameObject);
  }
}

void LevelsManager::loadLevel(const std::string& name)
{
  spdlog::info("Load level {}", name);

  std::vector<GameObject> levelStaticObjects;
  std::vector<GameObject> levelDynamicObjects;

  loadLevelStaticObjects(name, levelStaticObjects);
  loadLevelDynamicObjects(name, levelDynamicObjects);

  std::vector<GameObject> sceneObjects = levelStaticObjects;
  sceneObjects.insert(sceneObjects.end(), levelDynamicObjects.begin(), levelDynamicObjects.end());

  m_gameWorld->emitEvent<LoadSceneCommandEvent>(LoadSceneCommandEvent{.sceneObjects=sceneObjects});

  m_isLevelLoaded = true;

  spdlog::info("Level {} is loaded", name);
}

GameObjectsLoader& LevelsManager::getObjectsLoader()
{
  return m_gameObjectsLoader;
}

std::shared_ptr<pugi::xml_document> LevelsManager::openLevelDescriptionFile(const std::string& levelName,
  const std::string& descriptionFile,
  const std::string& descriptionNodeName)
{
  std::string levelPath = FileUtils::getLevelPath(levelName);

  if (!FileUtils::isDirExists(levelPath)) {
    THROW_EXCEPTION(EngineRuntimeException, "Level does not exists: " + levelPath);
  }

  std::string levelDescPath = levelPath + "/" + descriptionFile + ".xml";

  auto levelDescription = std::make_shared<pugi::xml_document>();
  levelDescription->reset(std::get<0>(XMLUtils::openDescriptionFile(levelDescPath, descriptionNodeName)));

  return levelDescription;
}

std::shared_ptr<GameWorld> LevelsManager::getGameWorld() const
{
  return m_gameWorld;
}

void LevelsManager::loadSpawnObjectsList(const std::string& spawnListName)
{
  spdlog::info("Load spawn objects list: {}", spawnListName);

  std::string spawnListPath = FileUtils::getSpawnListPath(spawnListName);
  auto levelDescriptionDocument = std::get<0>(XMLUtils::openDescriptionFile(spawnListPath, "objects"));

  pugi::xml_node spawnListDescription = levelDescriptionDocument.child("objects");

  for (pugi::xml_node& objectNode : spawnListDescription.children("object")) {
    auto transformNode = objectNode.child("transform");

    if (transformNode) {
      if (transformNode.attribute("static")) {
        THROW_EXCEPTION(EngineRuntimeException, "Spawn objects shouldn't use static attribute");
      }

      transformNode.append_attribute("static") = "false";
    }
  }

  for (const pugi::xml_node& objectNode : spawnListDescription.children("object")) {
    std::string gameObjectSpawnName = m_gameObjectsLoader.loadGameObject(objectNode);
    LOCAL_VALUE_UNUSED(gameObjectSpawnName);
  }
}
