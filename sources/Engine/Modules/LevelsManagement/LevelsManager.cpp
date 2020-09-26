#include "precompiled.h"

#pragma hdrstop

#include "LevelsManager.h"

#include <utility>

#include "Modules/Graphics/GraphicsSystem/GraphicsSceneManagementSystem.h"
#include "Modules/Math/MathUtils.h"
#include "Utility/files.h"

LevelsManager::LevelsManager(std::shared_ptr<GameWorld> gameWorld,
  std::shared_ptr<ResourceManager> resourceManager)
  : m_gameWorld(gameWorld),
    m_resourceManager(resourceManager),
    m_gameObjectsLoader(gameWorld, resourceManager)
{

}

LevelsManager::~LevelsManager()
{
  if (m_currentLevelData != nullptr) {
    unloadLevel();
  }
}

void LevelsManager::unloadLevel()
{
  SW_ASSERT(m_currentLevelData != nullptr);

  m_gameWorld->emitEvent<UnloadSceneCommandEvent>(UnloadSceneCommandEvent{});

  for (GameObject object : m_gameWorld->all()) {
    m_gameWorld->removeGameObject(object);
  }

  m_currentLevelData.reset();
}

std::shared_ptr<pugi::xml_document> LevelsManager::loadLevelStaticObjects(
  const std::string& levelName,
  std::vector<GameObject>& objectsIds)
{
  spdlog::info("Load level static objects: {}", levelName);

  auto levelDescriptionDocument = openLevelDescriptionFile(levelName,
    "level_static",
    "level");

  pugi::xml_node levelDescription = levelDescriptionDocument->child("level");

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
    auto gameObject = m_gameObjectsLoader.loadGameObject(objectNode);

    gameObject.addComponent<GameObjectDeclarationComponent>(objectNode);
    objectsIds.push_back(gameObject);
  }

  return levelDescriptionDocument;
}

std::shared_ptr<pugi::xml_document> LevelsManager::loadLevelDynamicObjects(
  const std::string& levelName,
  std::vector<GameObject>& objects)
{
  spdlog::info("Load level dynamic objects: {}", levelName);

  auto levelDescriptionDocument = openLevelDescriptionFile(levelName,
    "level_spawn",
    "level");

  pugi::xml_node levelDescription = levelDescriptionDocument->child("level");

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
    auto gameObject = m_gameObjectsLoader.loadGameObject(objectNode);

    gameObject.addComponent<GameObjectDeclarationComponent>(objectNode);
    objects.push_back(gameObject);
  }

  return levelDescriptionDocument;
}

void LevelsManager::loadLevel(const std::string& name)
{
  SW_ASSERT(m_currentLevelData == nullptr);

  spdlog::info("Load level {}", name);

  std::vector<GameObject> levelStaticObjects;
  std::vector<GameObject> levelDynamicObjects;

  auto staticObjectsData = loadLevelStaticObjects(name, levelStaticObjects);
  auto dynamicObjectsData = loadLevelDynamicObjects(name, levelDynamicObjects);

  m_currentLevelData = std::make_shared<LevelData>(staticObjectsData, dynamicObjectsData);

  std::vector<GameObject> sceneObjects = levelStaticObjects;
  sceneObjects.insert(sceneObjects.end(), levelDynamicObjects.begin(), levelDynamicObjects.end());

  m_gameWorld->emitEvent<LoadSceneCommandEvent>(LoadSceneCommandEvent{.sceneObjects=sceneObjects});

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
