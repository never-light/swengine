#include "precompiled.h"

#pragma hdrstop

#include "LevelsManager.h"

#include <utility>

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
  if (m_currentLevel.isAlive()) {
    unloadCurrentLevel();
  }
}

void LevelsManager::unloadCurrentLevel()
{
  SW_ASSERT(m_currentLevel.isAlive());

  m_gameWorld->emitEvent(LevelUnloadEvent(m_currentLevel));

  for (auto levelObject : m_currentLevel.getComponent<LevelComponent>()->getStaticObjectsList()) {
    m_gameWorld->removeGameObject(levelObject);
  }

  for (auto levelObject : m_currentLevel.getComponent<LevelComponent>()->getDynamicObjectsList()) {
    m_gameWorld->removeGameObject(levelObject);
  }

  m_gameWorld->removeGameObject(m_currentLevel);

  m_currentLevel = GameObject{};
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
  SW_ASSERT(!m_currentLevel.isAlive());

  spdlog::info("Load level {}", name);

  std::vector<GameObject> levelStaticObjects;
  std::vector<GameObject> levelDynamicObjects;

  auto staticObjectsData = loadLevelStaticObjects(name, levelStaticObjects);
  auto dynamicObjectsData = loadLevelDynamicObjects(name, levelDynamicObjects);

  GameObject levelGameObject = m_gameWorld->createGameObject();
  RETURN_VALUE_UNUSED(levelGameObject.addComponent<LevelComponent>(
    std::move(levelStaticObjects),
    std::move(levelDynamicObjects),
    staticObjectsData,
    dynamicObjectsData));

  m_currentLevel = levelGameObject;

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
