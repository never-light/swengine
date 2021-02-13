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
  unloadSpawnLists();
}

void LevelsManager::unloadLevel()
{
  if (m_isLevelLoaded) {
    m_gameWorld->emitEvent<UnloadSceneCommandEvent>(UnloadSceneCommandEvent{});

    for (GameObject object : m_gameWorld->all()) {
      m_gameWorld->removeGameObject(object);
    }

    m_isLevelLoaded = false;
    m_loadedLevelName = "";
  }
}

void LevelsManager::loadLevelStaticObjects(
  const std::string& levelName,
  std::vector<std::string>& objectsIds)
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
        THROW_EXCEPTION(EngineRuntimeException, "Level static objects shouldn't use \"static\" attribute");
      }

      transformNode.append_attribute("static") = "true";

      if (transformNode.attribute("online")) {
        THROW_EXCEPTION(EngineRuntimeException, "Level static objects shouldn't use \"online\" attribute");
      }

      transformNode.append_attribute("online") = "true";
    }
  }

  for (const pugi::xml_node& objectNode : levelDescription.children("object")) {
    std::string gameObjectSpawnName = m_gameObjectsLoader.loadGameObject(objectNode);

    objectsIds.push_back(gameObjectSpawnName);
  }
}

void LevelsManager::loadLevel(const std::string& name)
{
  SW_ASSERT(!m_isLevelLoaded);

  spdlog::info("Load level {}", name);

  std::vector<std::string> levelStaticObjects;
  loadLevelStaticObjects(name, levelStaticObjects);

  std::vector<std::string> sceneObjectsNames = levelStaticObjects;

  std::vector<GameObject> sceneObjects;

  for (const std::string& objectSpawnName : sceneObjectsNames) {
    GameObject gameObject = m_gameObjectsLoader.buildGameObject(objectSpawnName);

    sceneObjects.push_back(gameObject);

    if (gameObject.hasComponent<TransformComponent>()) {
      gameObject.getComponent<TransformComponent>()->setLevelId(name);
    }
  }

  m_gameWorld->emitEvent<LoadSceneCommandEvent>(LoadSceneCommandEvent{.sceneObjects=sceneObjects});

  m_isLevelLoaded = true;
  m_loadedLevelName = name;

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
  loadSpawnList(FileUtils::getSpawnListPath(spawnListName));
}

bool LevelsManager::isLevelLoaded() const
{
  return m_isLevelLoaded;
}

void LevelsManager::loadLevelSpawnList(const std::string& levelName)
{
  std::string levelPath = FileUtils::getLevelPath(levelName);
  std::string spawnListPath = levelPath + "/level_spawn.xml";
  loadSpawnList(spawnListPath);
}

void LevelsManager::loadSpawnList(const std::string& path)
{
  spdlog::info("Load spawn list: {}", path);

  auto levelDescriptionDocument = std::get<0>(XMLUtils::openDescriptionFile(path, "objects"));

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

void LevelsManager::loadLevelsSpawnLists()
{
  for (const std::string& levelName : FileUtils::listDirectories(std::string(FileUtils::LEVELS_PATH))) {
    loadLevelSpawnList(levelName);
  }
}

void LevelsManager::unloadSpawnLists()
{
  m_gameObjectsLoader.resetLoadedObjects();
}

const std::string& LevelsManager::getLoadedLevelName() const
{
  SW_ASSERT(isLevelLoaded());
  return m_loadedLevelName;
}
