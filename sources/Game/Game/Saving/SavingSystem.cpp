#include "SavingSystem.h"

#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>

#include <Engine/Utility/files.h>

SavingSystem::SavingSystem()
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
  cereal::BinaryOutputArchive saveArchive(saveFileStream);

  for (GameObject object : gameWorld->all()) {

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
