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

  std::ofstream saveFileStream(FileUtils::getSavePath(saveName));
  cereal::XMLOutputArchive saveArchive(saveFileStream);

  saveArchive(*gameWorld);
}

void SavingSystem::activate()
{

}

void SavingSystem::deactivate()
{
}
