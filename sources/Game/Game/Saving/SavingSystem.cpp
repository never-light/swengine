#include "SavingSystem.h"

#include <Engine/Utility/files.h>
#include <Engine/Modules/Graphics/GraphicsSystem/TransformComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/GraphicsScene.h>
#include <Engine/Modules/Graphics/GraphicsSystem/GraphicsSceneManagementSystem.h>

#include <utility>

SavingSystem::SavingSystem()
{

}

SavingSystem::~SavingSystem()
{

}

void SavingSystem::configure()
{
  GameSystem::configure();

  getGameWorld()->subscribeEventsListener<SaveCommandTriggerEvent>(this);
  getGameWorld()->subscribeEventsListener<LoadCommandTriggerEvent>(this);
}

void SavingSystem::unconfigure()
{
  GameSystem::unconfigure();

  getGameWorld()->unsubscribeEventsListener<LoadCommandTriggerEvent>(this);
  getGameWorld()->unsubscribeEventsListener<SaveCommandTriggerEvent>(this);
}

EventProcessStatus SavingSystem::receiveEvent(const SaveCommandTriggerEvent& event)
{
  saveGameState(event.getSaveName());

  return EventProcessStatus::Processed;
}

EventProcessStatus SavingSystem::receiveEvent(const LoadCommandTriggerEvent& event)
{
  loadGameState(event.getSaveName());

  return EventProcessStatus::Processed;
}

void SavingSystem::saveGameState(const std::string& saveName)
{
  GameWorld* gameWorld = getGameWorld();

  std::ofstream saveFileStream(FileUtils::getSavePath(saveName));
  cereal::XMLOutputArchive saveArchive(saveFileStream);

  saveArchive(*gameWorld);
}

void SavingSystem::loadGameState(const std::string& saveName)
{
  GameWorld* gameWorld = getGameWorld();

  // Unload existing game world data

  for (GameObject gameObject : gameWorld->all()) {
    gameWorld->removeGameObject(gameObject);
  }

//  gameWorld->update(1.0f / 30.0f);

  std::ifstream loadFileStream(FileUtils::getSavePath(saveName));
  cereal::XMLInputArchive loadArchive(loadFileStream);

  loadArchive(*gameWorld);

  std::vector<GameObject> sceneObjects;

  for (GameObject gameObject : gameWorld->all()) {
    if (gameObject.hasComponent<ObjectSceneNodeComponent>()) {
      gameObject.removeComponent<ObjectSceneNodeComponent>();

      sceneObjects.push_back(gameObject);
    }
  }

  gameWorld->emitEvent<LoadSceneCommandEvent>(LoadSceneCommandEvent{.sceneObjects=sceneObjects});
}

void SavingSystem::activate()
{

}

void SavingSystem::deactivate()
{
}
