#include "SavingSystem.h"

#include <Engine/Utility/files.h>
#include <Engine/Modules/Graphics/GraphicsSystem/TransformComponent.h>

#include <utility>

SavingSystem::SavingSystem(std::shared_ptr<ResourcesManager> resourcesManager)
  : m_resourcesManager(std::move(resourcesManager))
{

}

SavingSystem::~SavingSystem()
{

}

void SavingSystem::configure()
{
  GameSystem::configure();
}

void SavingSystem::unconfigure()
{
  GameSystem::unconfigure();
}

EventProcessStatus SavingSystem::receiveEvent(const SaveCommandTriggerEvent& event)
{
  saveGameState(event.getSaveName());

  return EventProcessStatus::Processed;
}

void SavingSystem::saveGameState(const std::string& saveName)
{
  GameWorld* gameWorld = getGameWorld();

  std::ofstream saveFileStream(FileUtils::getSavePath(saveName), std::ios::binary);
  OutputDataArchive saveArchive(saveFileStream, m_resourcesManager);

  for (GameObject object : gameWorld->all()) {
    auto transformComponent = object.getComponent<TransformComponent>();

    if (!transformComponent->isStatic()) {
      saveComponent<TransformComponent>(object, saveArchive);
    }
  }
}

template<class T>
void SavingSystem::saveComponent(GameObject gameObject, OutputDataArchive& outputArchive)
{
  if (gameObject.hasComponent<T>()) {
    outputArchive(*gameObject.getComponent<T>().get());
  }
}

void SavingSystem::activate()
{

}

void SavingSystem::deactivate()
{
}

GameObjectSaveWrapper::GameObjectSaveWrapper(GameObject gameObject)
  : m_gameObject(gameObject)
{

}

template<class Archive>
void GameObjectSaveWrapper::save(Archive& archive) const
{

}

template<class Archive>
void GameObjectSaveWrapper::load(Archive& archive)
{

}
