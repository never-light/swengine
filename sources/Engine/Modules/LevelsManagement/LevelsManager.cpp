#include "precompiled.h"

#pragma hdrstop

#include "LevelsManager.h"

#include <utility>

#include "Modules/Graphics/GraphicsSystem/TransformComponent.h"
#include "Modules/Graphics/GraphicsSystem/MeshRendererComponent.h"
#include "Modules/Graphics/Resources/MaterialResource.h"

#include "Modules/Physics/RigidBodyComponent.h"
#include "Modules/Physics/Resources/CollisionDataResource.h"

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

void LevelsManager::loadLevel(const std::string& name)
{
  SW_ASSERT(m_currentLevelId == GameObjectNone);

  std::string levelPath = FileUtils::getLevelPath(name);

  spdlog::info("Load level {}, path {}", name, levelPath);

  if (!FileUtils::isDirExists(levelPath)) {
    THROW_EXCEPTION(EngineRuntimeException, "Level does not exists: " + levelPath);
  }

  std::string levelStaticObjectsDescPath = levelPath + "/level_static.xml";

  if (!FileUtils::isFileExists(levelStaticObjectsDescPath)) {
    THROW_EXCEPTION(EngineRuntimeException, "Level static objects list on the path does not exists: " +
      levelStaticObjectsDescPath);
  }

  pugi::xml_document levelStaticObjects;
  pugi::xml_parse_result levelStaticObjectsParseResult =
    levelStaticObjects.load_file(levelStaticObjectsDescPath.c_str());

  if (!levelStaticObjectsParseResult) {
    THROW_EXCEPTION(EngineRuntimeException, "Level static objects list has invalid format: " +
      levelStaticObjectsDescPath);
  }

  pugi::xml_node levelDescription = levelStaticObjects.child("level");

  if (!levelDescription) {
    THROW_EXCEPTION(EngineRuntimeException, "Level static objects list hasn't level section: " +
      levelStaticObjectsDescPath);
  }

  std::vector<GameObjectId> gameObjectsIds;

  for (pugi::xml_node& objectNode : levelDescription.children("object")) {
    auto transformNode = objectNode.child("transform");

    if (transformNode) {
      if (transformNode.attribute("static")) {
        THROW_EXCEPTION(EngineRuntimeException, "Level static objects shouldn't use static attribute: " +
          levelStaticObjectsDescPath);
      }

      transformNode.append_attribute("static") = "true";
    }
  }

  for (const pugi::xml_node& objectNode : levelDescription.children("object")) {
    auto& gameObject = m_gameObjectsLoader.loadGameObject(objectNode);

    gameObjectsIds.push_back(gameObject.getId());
  }

  auto levelGameObject = m_gameWorld->createGameObject();
  RETURN_VALUE_UNUSED(levelGameObject->addComponent<LevelComponent>(std::move(gameObjectsIds)));

  m_currentLevelId = levelGameObject->getId();

  spdlog::info("Level {} is loaded", name);
}

GameObjectsLoader& LevelsManager::getObjectsLoader()
{
  return m_gameObjectsLoader;
}
