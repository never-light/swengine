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
  if (m_currentLevelId != GameObjectNone) {
    unloadCurrentLevel();
  }
}

void LevelsManager::unloadCurrentLevel()
{
  SW_ASSERT(m_currentLevelId != GameObjectNone);

  auto levelObject = m_gameWorld->findGameObject(m_currentLevelId);

  m_gameWorld->emitEvent(LevelUnloadEvent(m_currentLevelId));

  // TODO: add function to remove game object by id to GameWorld and use it here
  for (auto levelObjectId : levelObject->getComponent<LevelComponent>().getStaticObjectsList()) {
    m_gameWorld->removeGameObject(m_gameWorld->findGameObject(levelObjectId));
  }

  m_gameWorld->removeGameObject(levelObject);

  m_currentLevelId = GameObjectNone;
}

void LevelsManager::loadLevelStaticObjects(const std::string& levelName, std::vector<GameObjectId>& objectsIds)
{
  spdlog::info("Load level static objects: {}", levelName);

  auto[levelDescriptionDocument, levelDescription] = openLevelDescriptionFile(levelName,
    "level_static",
    "level");

  LOCAL_VALUE_UNUSED(levelDescriptionDocument);

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
    auto& gameObject = m_gameObjectsLoader.loadGameObject(objectNode);

    objectsIds.push_back(gameObject.getId());
  }
}

void LevelsManager::loadLevelDynamicObjects(const std::string& levelName, std::vector<GameObjectId>& objectsIds)
{
  spdlog::info("Load level dynamic objects: {}", levelName);

  auto[levelDescriptionDocument, levelDescription] = openLevelDescriptionFile(levelName,
    "level_spawn",
    "level");

  LOCAL_VALUE_UNUSED(levelDescriptionDocument);

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
    auto& gameObject = m_gameObjectsLoader.loadGameObject(objectNode);

    objectsIds.push_back(gameObject.getId());
  }
}

void LevelsManager::loadLevel(const std::string& name)
{
  SW_ASSERT(m_currentLevelId == GameObjectNone);

  spdlog::info("Load level {}", name);

  std::vector<GameObjectId> gameObjectsIds;

  loadLevelStaticObjects(name, gameObjectsIds);
  loadLevelDynamicObjects(name, gameObjectsIds);

  auto levelGameObject = m_gameWorld->createGameObject();
  RETURN_VALUE_UNUSED(levelGameObject->addComponent<LevelComponent>(std::move(gameObjectsIds)));

  m_currentLevelId = levelGameObject->getId();

  spdlog::info("Level {} is loaded", name);
}

GameObjectsLoader& LevelsManager::getObjectsLoader()
{
  return m_gameObjectsLoader;
}

std::tuple<pugi::xml_document, pugi::xml_node> LevelsManager::openLevelDescriptionFile(const std::string& levelName,
  const std::string& descriptionFile,
  const std::string& descriptionNodeName)
{
  std::string levelPath = FileUtils::getLevelPath(levelName);

  if (!FileUtils::isDirExists(levelPath)) {
    THROW_EXCEPTION(EngineRuntimeException, "Level does not exists: " + levelPath);
  }

  std::string levelDescPath = levelPath + "/" + descriptionFile + ".xml";

  if (!FileUtils::isFileExists(levelDescPath)) {
    THROW_EXCEPTION(EngineRuntimeException,
      fmt::format("Level description file doesn't exist: {}", levelDescPath));
  }

  pugi::xml_document levelDescDocument;
  pugi::xml_parse_result levelDescParseResult = levelDescDocument.load_file(levelDescPath.c_str());

  if (!levelDescParseResult) {
    THROW_EXCEPTION(EngineRuntimeException,
      fmt::format("Level description file has invalid format: {}", levelDescPath));
  }

  pugi::xml_node levelDescriptionNode = levelDescDocument.child(descriptionNodeName.c_str());

  if (!levelDescriptionNode) {
    THROW_EXCEPTION(EngineRuntimeException,
      fmt::format("Level description file hasn't description node {}: {}", descriptionNodeName, levelDescPath));
  }

  return std::tuple<pugi::xml_document, pugi::xml_node>(std::move(levelDescDocument), levelDescriptionNode);
}
