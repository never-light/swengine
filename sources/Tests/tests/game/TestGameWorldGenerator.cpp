#include "TestGameWorldGenerator.h"

#include <Engine/Modules/Graphics/GraphicsSystem/TransformComponent.h>
#include <Engine/Modules/LevelsManagement/LevelsManager.h>
#include <Engine/Modules/ResourceManagement/ResourceManager.h>

#include <Game/Game/Inventory/InventoryControlSystem.h>
#include <Game/Game/Dynamic/QuestsSystem.h>

#include <Game/Game/Dynamic/GameLogicConditionsManager.h>
#include <Game/Game/Dynamic/ActorComponent.h>
#include <Game/Game/Inventory/InventoryComponent.h>
#include <Game/Game/PlayerComponent.h>

std::shared_ptr<GameWorld> TestGameWorldGenerator::buildTestGameWorld()
{
  // Initialize game world and general gameplay systems
  auto gameWorld = GameWorld::createInstance();
  auto resourcesManager = std::make_shared<ResourceManager>();
  auto levelsManager = std::make_shared<LevelsManager>(gameWorld, resourcesManager);

  auto infoportionsSystem = std::make_shared<InfoportionsSystem>();
  gameWorld->getGameSystemsGroup()->addGameSystem(infoportionsSystem);

  auto inventoryControlSystem = std::make_shared<InventoryControlSystem>(levelsManager);
  gameWorld->getGameSystemsGroup()->addGameSystem(inventoryControlSystem);

  auto conditionsManager = std::make_shared<GameLogicConditionsManager>(gameWorld);
  auto questsSystem = std::make_shared<QuestsSystem>(conditionsManager);
  gameWorld->getGameSystemsGroup()->addGameSystem(questsSystem);

  // Initialize player

  GameObject player = gameWorld->createGameObject("player");
  player.addComponent<TransformComponent>();
  player.addComponent<PlayerComponent>(1.0f);
  player.addComponent<InventoryComponent>();

  auto playerActorComponent = player.addComponent<ActorComponent>();
  playerActorComponent->setName("Player");

  // Initialize NCP

  GameObject npc1 = gameWorld->createGameObject("npc1");
  npc1.addComponent<TransformComponent>();
  auto npcActorComponent1 = npc1.addComponent<ActorComponent>();
  npcActorComponent1->setName("npc1");

  npc1.addComponent<InventoryComponent>();

  // Initialize misc game objects
  GameObject medkit = gameWorld->createGameObject("medkit");
  medkit.addComponent<TransformComponent>();
  medkit.addComponent<InventoryItemComponent>(nullptr, "objet_class_medkit", "medkit");

  // Initialize infoportions
  infoportionsSystem->addInfoportion("infoportion_id_test_1");
  infoportionsSystem->addInfoportion("infoportion_id_test_2");

  infoportionsSystem->addInfoportion("infoportion_id_quest_find_medkit_started");
  infoportionsSystem->addInfoportion("infoportion_id_quest_find_medkit_completed");

  // Initialize quests

  // Find a medkit quest
  Quest findMedkitQuest("quest_id_find_medkit", "find_medkit", "find_medkit_desc");

  QuestTask findMedkitTask("quest_id_find_medkit_task_id_find", "find_medkit", "find_medkit_desc");

  findMedkitTask.setAutostartCondition(std::make_shared<GameLogicConditionHasInfoportion>(
    conditionsManager.get(), "infoportion_id_quest_find_medkit_started"));
  findMedkitTask.setActiveCondition(std::make_shared<GameLogicConditionHasNotObject>(
    conditionsManager.get(), "objet_class_medkit"));
  findMedkitTask.setCompleteCondition(std::make_shared<GameLogicConditionHasObject>(
    conditionsManager.get(), "objet_class_medkit"));

  findMedkitQuest.addTask(findMedkitTask);

  QuestTask transferMedkitTask("quest_id_find_medkit_task_id_transfer", "transfer_medkit",
    "transfer_medkit_desc");
  transferMedkitTask.setActiveCondition(std::make_shared<GameLogicConditionHasObject>(
    conditionsManager.get(), "objet_class_medkit"));
  transferMedkitTask.setCompleteCondition(std::make_shared<GameLogicConditionHasInfoportion>(
    conditionsManager.get(), "infoportion_id_quest_find_medkit_completed"));

  findMedkitQuest.addTask(transferMedkitTask);

  questsSystem->addQuest(findMedkitQuest);

  return gameWorld;
}
