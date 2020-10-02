#include <catch2/catch.hpp>

#include <Engine/Modules/ECS/ECS.h>
#include <Engine/Modules/LevelsManagement/LevelsManager.h>
#include <Engine/Modules/ResourceManagement/ResourceManager.h>

#include <Game/Game/Dynamic/GameLogicConditionsManager.h>
#include <Game/Game/PlayerComponent.h>
#include <Game/Game/Inventory/InventoryComponent.h>
#include <Game/Game/Inventory/InventoryControlSystem.h>
#include <Game/Game/Dynamic/ActorComponent.h>
#include <Game/Game/Dynamic/DialoguesManager.h>
#include <Game/Game/Dynamic/InfoportionsSystem.h>

#include "Tests/utility/ecsUtility.h"

//
static std::shared_ptr<GameWorld> createTestGameWorld()
{
  auto gameWorld = GameWorld::createInstance();
  auto resourcesManager = std::make_shared<ResourceManager>();
  auto levelsManager = std::make_shared<LevelsManager>(gameWorld, resourcesManager);

  auto infoportionsSystem = std::make_shared<InfoportionsSystem>();
  gameWorld->getGameSystemsGroup()->addGameSystem(infoportionsSystem);

  auto inventoryControlSystem = std::make_shared<InventoryControlSystem>(levelsManager);
  gameWorld->getGameSystemsGroup()->addGameSystem(inventoryControlSystem);

  infoportionsSystem->addInfoportion("test_infoportion_1");
  infoportionsSystem->addInfoportion("test_infoportion_2");

  GameObject testInventoryItem = gameWorld->createGameObject();
  testInventoryItem.addComponent<InventoryItemComponent>(nullptr, "test_item_id", "test_item_name");

  GameObject player = gameWorld->createGameObject("player");
  player.addComponent<PlayerComponent>(1.0f);
  auto playerInventoryComponent = player.addComponent<InventoryComponent>();
  playerInventoryComponent->addItem(testInventoryItem);

  auto playerActorComponent = player.addComponent<ActorComponent>();
  playerActorComponent->setName("Player");

  GameObject npc1 = gameWorld->createGameObject("npc1");
  auto npcActorComponent1 = npc1.addComponent<ActorComponent>();
  npcActorComponent1->setName("npc1");

  auto npcInventoryComponent = npc1.addComponent<InventoryComponent>();
  LOCAL_VALUE_UNUSED(npcInventoryComponent);

  return gameWorld;
}

TEST_CASE("game_logic_infoportions_management", "[game]")
{
  bool addInfoEventReceived = false;

  auto addInfoportionListener = ECSUtility::makeEventsListener<AddInfoportionEvent>(
    [&addInfoEventReceived](const AddInfoportionEvent& event) {
      REQUIRE(event.getInfoportionName() == "test_infoportion_1");
      REQUIRE(event.getActor().getName() == "player");

      addInfoEventReceived = true;
    });

  bool removeInfoEventReceived = false;

  auto removeInfoportionListener = ECSUtility::makeEventsListener<RemoveInfoportionEvent>(
    [&removeInfoEventReceived](const RemoveInfoportionEvent& event) {
      REQUIRE(event.getInfoportionName() == "test_infoportion_1");
      REQUIRE(event.getActor().getName() == "player");

      removeInfoEventReceived = true;
    });

  auto gameWorld = createTestGameWorld();

  gameWorld->subscribeEventsListener<AddInfoportionEvent>(addInfoportionListener.get());
  gameWorld->subscribeEventsListener<RemoveInfoportionEvent>(removeInfoportionListener.get());

  auto playerObject = gameWorld->findGameObject("player");
  auto playerActorComponent = playerObject.getComponent<ActorComponent>();

  gameWorld->emitEvent(AddInfoportionCommandEvent(playerObject, "test_infoportion_1"));

  REQUIRE(addInfoEventReceived);
  REQUIRE(playerActorComponent->getInfoportionsStorage().hasInfoportion("test_infoportion_1"));

  gameWorld->emitEvent(RemoveInfoportionCommandEvent(playerObject, "test_infoportion_1"));

  REQUIRE(removeInfoEventReceived);
  REQUIRE(!playerActorComponent->getInfoportionsStorage().hasInfoportion("test_infoportion_1"));
}

TEST_CASE("game_logic_has_infoportion_condition", "[game]")
{
  auto gameWorld = createTestGameWorld();
  auto conditionsManager = std::make_shared<GameLogicConditionsManager>(gameWorld);

  auto playerObject = gameWorld->findGameObject("player");

  auto hasInfoConditionPositive = std::make_shared<GameLogicConditionHasInfoportion>(conditionsManager.get(),
    "test_infoportion_1");
  hasInfoConditionPositive->setActor(playerObject);

  auto hasInfoConditionNegative = std::make_shared<GameLogicConditionHasInfoportion>(conditionsManager.get(),
    "test_infoportion_2");
  hasInfoConditionNegative->setActor(playerObject);

  auto hasNotInfoConditionPositive = std::make_shared<GameLogicConditionHasNotInfoportion>(conditionsManager.get(),
    "test_infoportion_2");
  hasNotInfoConditionPositive->setActor(playerObject);

  auto hasNotInfoConditionNegative = std::make_shared<GameLogicConditionHasNotInfoportion>(conditionsManager.get(),
    "test_infoportion_1");
  hasNotInfoConditionNegative->setActor(playerObject);

  auto addInfoportionAction = std::make_shared<GameLogicActionAddInfoportion>(conditionsManager.get(),
    "test_infoportion_1");
  addInfoportionAction->setActor(playerObject);

  addInfoportionAction->execute();

  REQUIRE(hasInfoConditionPositive->calculateValue());
  REQUIRE_FALSE(hasInfoConditionNegative->calculateValue());

  REQUIRE(hasNotInfoConditionPositive->calculateValue());
  REQUIRE_FALSE(hasNotInfoConditionNegative->calculateValue());

  auto removeInfoportionAction = std::make_shared<GameLogicActionRemoveInfoportion>(conditionsManager.get(),
    "test_infoportion_1");
  removeInfoportionAction->setActor(playerObject);

  removeInfoportionAction->execute();

  REQUIRE_FALSE(hasInfoConditionPositive->calculateValue());
  REQUIRE_FALSE(hasInfoConditionNegative->calculateValue());

  REQUIRE(hasNotInfoConditionPositive->calculateValue());
  REQUIRE(hasNotInfoConditionNegative->calculateValue());
}

TEST_CASE("game_logic_inventory_transfer_item_action", "[game]")
{
  auto gameWorld = createTestGameWorld();
  auto conditionsManager = std::make_shared<GameLogicConditionsManager>(gameWorld);

  bool dropItemEventReceived = false;
  bool takeItemEventReceived = false;

  auto inventoryItemActionEventListener = ECSUtility::makeEventsListener<InventoryItemActionEvent>(
    [&dropItemEventReceived, &takeItemEventReceived](const InventoryItemActionEvent& event) {
      if (event.triggerType == InventoryItemActionTriggerType::DropFromInventory) {
        REQUIRE_FALSE(dropItemEventReceived);
        REQUIRE(event.inventoryOwner.getName() == "player");
        REQUIRE(GameObject(event.item).getComponent<InventoryItemComponent>()->getId() == "test_item_id");

        dropItemEventReceived = true;
      }
      else if (event.triggerType == InventoryItemActionTriggerType::RelocateToInventory) {
        REQUIRE_FALSE(takeItemEventReceived);
        REQUIRE(event.inventoryOwner.getName() == "npc1");
        REQUIRE(GameObject(event.item).getComponent<InventoryItemComponent>()->getId() == "test_item_id");

        takeItemEventReceived = true;
      }
      else {
        REQUIRE(false);
      }

    });

  gameWorld->subscribeEventsListener(inventoryItemActionEventListener.get());

  auto playerObject = gameWorld->findGameObject("player");
  auto playerInventory = playerObject.getComponent<InventoryComponent>();

  auto npcObject = gameWorld->findGameObject("npc1");
  auto npcInventory = npcObject.getComponent<InventoryComponent>();

  auto transferItemAction = std::make_shared<GameLogicActionTransferItem>(conditionsManager.get(),
    "test_item_id");
  transferItemAction->setInitiator(playerObject);
  transferItemAction->setTarget(npcObject);

  transferItemAction->execute();

  REQUIRE(dropItemEventReceived);
  REQUIRE(takeItemEventReceived);

  auto playerHasObjectCondition = std::make_shared<GameLogicConditionHasObject>(conditionsManager.get(),
    "test_item_id");
  playerHasObjectCondition->setActor(playerObject);

  auto npcHasObjectCondition = std::make_shared<GameLogicConditionHasObject>(conditionsManager.get(),
    "test_item_id");
  npcHasObjectCondition->setActor(npcObject);

  REQUIRE_FALSE(playerHasObjectCondition->calculateValue());
  REQUIRE_FALSE(playerInventory->hasItem("test_item_id"));

  REQUIRE(npcHasObjectCondition->calculateValue());
  REQUIRE(npcInventory->hasItem("test_item_id"));
}
