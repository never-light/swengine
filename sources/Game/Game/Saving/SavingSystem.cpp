#include "SavingSystem.h"

#include <Engine/Utility/files.h>
#include <Engine/Modules/Graphics/GraphicsSystem/TransformComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/GraphicsScene.h>
#include <Engine/Modules/Graphics/GraphicsSystem/GraphicsSceneManagementSystem.h>

#include <Engine/Exceptions/exceptions.h>

#include <utility>

SavingSystem::SavingSystem(std::shared_ptr<LevelsManager> levelsManager,
  std::shared_ptr<Game> game)
  : m_levelsManager(std::move(levelsManager)),
    m_game(std::move(game))
{

}

SavingSystem::~SavingSystem()
{

}

void SavingSystem::configure()
{
  GameSystem::configure();

  getGameWorld()->subscribeEventsListener<GameConsoleCommandEvent>(this);
  getGameWorld()->subscribeEventsListener<SaveCommandTriggerEvent>(this);
  getGameWorld()->subscribeEventsListener<LoadCommandTriggerEvent>(this);
}

void SavingSystem::unconfigure()
{
  GameSystem::unconfigure();

  getGameWorld()->unsubscribeEventsListener<GameConsoleCommandEvent>(this);
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
  std::string savePath = FileUtils::getSavePath(saveName);

  spdlog::info("Save game state to {}", savePath);

  GameWorld* gameWorld = getGameWorld();

  std::ofstream saveFileStream(savePath);
  cereal::XMLOutputArchive saveArchive(saveFileStream);

  GameObject playerObject = gameWorld->findGameObject("player");

  SaveHeader saveHeader{
    .saveName = saveName,
    .levelName = playerObject.getComponent<TransformComponent>()->getLevelId()
  };

  saveArchive(saveHeader);

  for (GameObject gameObject : gameWorld->all()) {
    SW_ASSERT(gameObject.hasComponent<TransformComponent>());

    auto transformComponent = gameObject.getComponent<TransformComponent>();

    // Serialize only dynamic level objects
    gameObject.markAsSerializable(!transformComponent->isStatic());
  }

  saveArchive(*gameWorld);
}

void SavingSystem::loadGameState(const std::string& saveName)
{
  std::string savePath = FileUtils::getSavePath(saveName);

  spdlog::info("Loading game state from {}", savePath);

  if (!FileUtils::isFileExists(savePath)) {
    THROW_EXCEPTION(EngineRuntimeException, fmt::format("Save file {} is not found", savePath));
  }

  if (m_game->isLoaded()) {
    m_game->unload();
  }

  GameWorld* gameWorld = getGameWorld();

  for (GameObject gameObject : gameWorld->all()) {
    ARG_UNUSED(gameObject);
    SW_ASSERT(false && "Game world should be empty after game unloading");
  }

  // Unload existing game world data

  std::ifstream loadFileStream(savePath);
  cereal::XMLInputArchive loadArchive(loadFileStream);

  SaveHeader saveHeader;
  loadArchive(saveHeader);

  m_game->createLoadedGame(saveHeader.levelName);

  // Only dynamic game objects are saved, so load them after static level objects loading
  loadArchive(*gameWorld);

  for (GameObject gameObject : gameWorld->all()) {
    if (gameObject.hasComponent<ObjectSceneNodeComponent>()
      && gameObject.getComponent<ObjectSceneNodeComponent>()->isGhost()) {
      gameObject.removeComponent<ObjectSceneNodeComponent>();

      gameWorld->emitEvent<AddObjectToSceneCommandEvent>(AddObjectToSceneCommandEvent{.object=gameObject});
    }
  }

  m_game->setupGameState(false);
}

void SavingSystem::activate()
{

}

void SavingSystem::deactivate()
{
}

EventProcessStatus SavingSystem::receiveEvent(const GameConsoleCommandEvent& event)
{
  if (event.command.starts_with("save")) {
    std::string saveName = StringUtils::split(event.command, ' ')[1];

    if (!saveName.empty()) {
      if (m_game->isLoaded()) {
        getGameWorld()->emitEvent<SaveCommandTriggerEvent>(SaveCommandTriggerEvent(saveName));
      }
    }

    return EventProcessStatus::Processed;
  }
  else if (event.command.starts_with("load")) {
    std::string saveName = StringUtils::split(event.command, ' ')[1];

    if (!saveName.empty()) {
      getGameWorld()->emitEvent<LoadCommandTriggerEvent>(LoadCommandTriggerEvent(saveName));
    }

    return EventProcessStatus::Processed;
  }

  return EventProcessStatus::Skipped;

}
