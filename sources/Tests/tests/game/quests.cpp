#include <catch2/catch.hpp>

#include <Engine/Modules/ECS/ECS.h>
#include <Engine/Modules/LevelsManagement/LevelsManager.h>
#include <Engine/Modules/ResourceManagement/ResourcesManagement.h>

#include <Game/Game/Dynamic/GameLogicConditionsManager.h>
#include <Game/Game/PlayerComponent.h>
#include <Game/Game/Inventory/InventoryComponent.h>
#include <Game/Game/Inventory/InventoryControlSystem.h>
#include <Game/Game/Dynamic/ActorComponent.h>
#include <Game/Game/Dynamic/DialoguesManager.h>
#include <Game/Game/Dynamic/InfoportionsSystem.h>
#include <Game/Game/Dynamic/QuestsSystem.h>

#include "Tests/utility/ecsUtility.h"

#include "TestGameWorldGenerator.h"

TEST_CASE("game_logic_quests_simple_quest", "[game]")
{
  auto gameWorld = TestGameWorldGenerator::buildTestGameWorld();
  auto playerObject = gameWorld->findGameObject("player");
  auto playerActorComponent = playerObject.getComponent<ActorComponent>();
  auto conditionsManager = gameWorld->getGameSystemsGroup()->getGameSystem<QuestsSystem>()->getConditionsManager();

  REQUIRE(playerActorComponent->getQuestState("quest_id_find_medkit").getState() == QuestState::NotStarted);

  auto questStartedListener = ECSUtility::makeEventsListener<QuestStartedEvent>();
  auto questCompletedListener = ECSUtility::makeEventsListener<QuestCompletedEvent>();
  auto questTaskStartedListener = ECSUtility::makeEventsListener<QuestTaskStartedEvent>();
  auto questTaskCompletedListener = ECSUtility::makeEventsListener<QuestTaskCompletedEvent>();
  auto questTaskCancelledListener = ECSUtility::makeEventsListener<QuestTaskCancelledEvent>();

  gameWorld->subscribeEventsListener<QuestStartedEvent>(questStartedListener.get());
  gameWorld->subscribeEventsListener<QuestCompletedEvent>(questCompletedListener.get());
  gameWorld->subscribeEventsListener<QuestTaskStartedEvent>(questTaskStartedListener.get());
  gameWorld->subscribeEventsListener<QuestTaskCompletedEvent>(questTaskCompletedListener.get());
  gameWorld->subscribeEventsListener<QuestTaskCancelledEvent>(questTaskCancelledListener.get());

  // Start quest
  auto addStartQuestInfoportionAction = std::make_shared<GameLogicActionAddInfoportion>(
    conditionsManager.get(),
    "infoportion_id_quest_find_medkit_started");
  addStartQuestInfoportionAction->setActor(playerObject);

  addStartQuestInfoportionAction->execute();

  REQUIRE(questStartedListener->isReceived());
  REQUIRE(questStartedListener->getEvent().getActor() == playerObject);
  REQUIRE(questStartedListener->getEvent().getQuestId() == "quest_id_find_medkit");
  questStartedListener->resetReceivedState();

  REQUIRE(questTaskStartedListener->isReceived());
  REQUIRE(questTaskStartedListener->getEvent().getActor() == playerObject);
  REQUIRE(questTaskStartedListener->getEvent().getQuestId() == "quest_id_find_medkit");
  REQUIRE(questTaskStartedListener->getEvent().getTaskId() == "quest_id_find_medkit_task_id_find");
  questTaskStartedListener->resetReceivedState();

  REQUIRE(playerActorComponent->getQuestState("quest_id_find_medkit").getState() == QuestState::Started);
  REQUIRE(playerActorComponent->getQuestState("quest_id_find_medkit").getCurrentTaskId()
    == "quest_id_find_medkit_task_id_find");
  REQUIRE(playerActorComponent->getQuestState("quest_id_find_medkit").getTaskState(
    "quest_id_find_medkit_task_id_find").getState() == QuestTaskState::Started);
  REQUIRE(playerActorComponent->getQuestState("quest_id_find_medkit").getTaskState(
    "quest_id_find_medkit_task_id_transfer").getState() == QuestTaskState::NotStarted);

  // Add medkit to inventory

  gameWorld->emitEvent<InventoryItemActionCommandEvent>(InventoryItemActionCommandEvent(playerObject,
    InventoryItemActionTriggerType::RelocateToInventory, gameWorld->findGameObject("medkit")));

  REQUIRE_FALSE(questStartedListener->isReceived());
  REQUIRE(questTaskStartedListener->isReceived());
  REQUIRE(questTaskStartedListener->getEvent().getTaskId() == "quest_id_find_medkit_task_id_transfer");
  questTaskStartedListener->resetReceivedState();

  REQUIRE(questTaskCompletedListener->isReceived());
  REQUIRE(questTaskCompletedListener->getEvent().getTaskId() == "quest_id_find_medkit_task_id_find");
  questTaskCompletedListener->resetReceivedState();

  REQUIRE(playerActorComponent->getQuestState("quest_id_find_medkit").getState() == QuestState::Started);
  REQUIRE(playerActorComponent->getQuestState("quest_id_find_medkit").getCurrentTaskId()
    == "quest_id_find_medkit_task_id_transfer");
  REQUIRE(playerActorComponent->getQuestState("quest_id_find_medkit").getTaskState(
    "quest_id_find_medkit_task_id_find").getState() == QuestTaskState::Completed);
  REQUIRE(playerActorComponent->getQuestState("quest_id_find_medkit").getTaskState(
    "quest_id_find_medkit_task_id_transfer").getState() == QuestTaskState::Started);

  // Drop medkit to switch between quest tasks

  gameWorld->emitEvent<InventoryItemActionCommandEvent>(InventoryItemActionCommandEvent(playerObject,
    InventoryItemActionTriggerType::DropFromInventory, gameWorld->findGameObject("medkit")));

  REQUIRE_FALSE(questStartedListener->isReceived());

  REQUIRE(questTaskCancelledListener->isReceived());
  REQUIRE(questTaskCancelledListener->getEventsCount() == 2);
  REQUIRE(questTaskCancelledListener->getEvent(0).getTaskId() == "quest_id_find_medkit_task_id_transfer");
  REQUIRE(questTaskCancelledListener->getEvent(1).getTaskId() == "quest_id_find_medkit_task_id_find");
  questTaskCancelledListener->resetReceivedState();

  REQUIRE(questTaskStartedListener->isReceived());
  REQUIRE(questTaskStartedListener->getEvent().getTaskId() == "quest_id_find_medkit_task_id_find");
  questTaskStartedListener->resetReceivedState();

  REQUIRE(playerActorComponent->getQuestState("quest_id_find_medkit").getCurrentTaskId()
    == "quest_id_find_medkit_task_id_find");

  // Take medkit again

  gameWorld->emitEvent<InventoryItemActionCommandEvent>(InventoryItemActionCommandEvent(playerObject,
    InventoryItemActionTriggerType::RelocateToInventory, gameWorld->findGameObject("medkit")));

  REQUIRE_FALSE(questStartedListener->isReceived());
  REQUIRE(questTaskStartedListener->isReceived());
  REQUIRE(questTaskStartedListener->getEvent().getTaskId() == "quest_id_find_medkit_task_id_transfer");
  questTaskStartedListener->resetReceivedState();

  REQUIRE(questTaskCompletedListener->isReceived());
  REQUIRE(questTaskCompletedListener->getEvent().getTaskId() == "quest_id_find_medkit_task_id_find");
  questTaskCompletedListener->resetReceivedState();

  // Add infoportion about quest completion
  auto addCompleteQuestInfoportionAction = std::make_shared<GameLogicActionAddInfoportion>(
    conditionsManager.get(),
    "infoportion_id_quest_find_medkit_completed");
  addCompleteQuestInfoportionAction->setActor(playerObject);

  addCompleteQuestInfoportionAction->execute();

  REQUIRE(questTaskCompletedListener->isReceived());
  REQUIRE(questTaskCompletedListener->getEvent().getTaskId() == "quest_id_find_medkit_task_id_transfer");
  questTaskCompletedListener->resetReceivedState();

  REQUIRE(questCompletedListener->isReceived());
  REQUIRE(questCompletedListener->getEvent().getActor() == playerObject);
  REQUIRE(questCompletedListener->getEvent().getQuestId() == "quest_id_find_medkit");
  questCompletedListener->resetReceivedState();

  REQUIRE(playerActorComponent->getQuestState("quest_id_find_medkit").getState() == QuestState::Completed);
  REQUIRE(playerActorComponent->getQuestState("quest_id_find_medkit").getTaskState(
    "quest_id_find_medkit_task_id_find").getState() == QuestTaskState::Completed);
  REQUIRE(playerActorComponent->getQuestState("quest_id_find_medkit").getTaskState(
    "quest_id_find_medkit_task_id_transfer").getState() == QuestTaskState::Completed);

  // Transfer medkit to NPC

  auto transferMedkitAction = std::make_shared<GameLogicActionTransferItem>(
    conditionsManager.get(),
    "objet_class_medkit");

  transferMedkitAction->setInitiator(playerObject);
  transferMedkitAction->setTarget(gameWorld->findGameObject("npc1"));

  transferMedkitAction->execute();

  REQUIRE_FALSE(questStartedListener->isReceived());
  REQUIRE_FALSE(questCompletedListener->isReceived());
  REQUIRE_FALSE(questTaskStartedListener->isReceived());
  REQUIRE_FALSE(questTaskCompletedListener->isReceived());
  REQUIRE_FALSE(questTaskCancelledListener->isReceived());
}
