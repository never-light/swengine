#include <catch2/catch.hpp>

#include <Engine/Modules/ECS/ECS.h>
#include <Game/Game/Dynamic/DialoguesManager.h>
#include <Game/Game/PlayerComponent.h>

static std::shared_ptr<GameWorld> createTestGameWorld()
{
  auto gameWorld = GameWorld::createInstance();

  GameObject player = gameWorld->createGameObject("player");
  player.addComponent<PlayerComponent>(1.0f);
  auto playerActorComponent = player.addComponent<ActorComponent>();
  playerActorComponent->setName("Player");

  GameObject npc1 = gameWorld->createGameObject("npc");
  auto npcActorComponent1 = npc1.addComponent<ActorComponent>();
  npcActorComponent1->setName("NPC1");

  npcActorComponent1->addDialogue(ActorDialogue("test_npc_hello_dialogue", true));
  npcActorComponent1->addDialogue(ActorDialogue("test_player_linear_dialogue", false));
  npcActorComponent1->addDialogue(ActorDialogue("test_player_branching_dialogue", false));

  GameObject npc2 = gameWorld->createGameObject("npc2");
  auto npcActorComponent2 = npc2.addComponent<ActorComponent>();
  npcActorComponent2->setName("NPC2");

  npcActorComponent2->addDialogue(ActorDialogue("test_player_linear_dialogue", false));
  npcActorComponent2->addDialogue(ActorDialogue("test_player_branching_dialogue", false));

  return gameWorld;
}

static DialoguesManager createTestDialoguesManager()
{
  DialoguesManager manager;
  manager.loadFromFile("test_dialogues");

  return manager;
}

TEST_CASE("game_dialogues_loading", "[game]")
{
  DialoguesManager dialoguesManager = createTestDialoguesManager();

  REQUIRE(dialoguesManager.hasDialogue("test_npc_hello_dialogue"));
  REQUIRE(dialoguesManager.hasDialogue("test_player_linear_dialogue"));
  REQUIRE(dialoguesManager.hasDialogue("test_player_branching_dialogue"));

  const Dialogue& npcHelloDialog = dialoguesManager.getDialogue("test_npc_hello_dialogue");

  REQUIRE(npcHelloDialog.hasPhrase("0"));

  const DialoguePhrase& npcHelloPhrase = npcHelloDialog.getPhrase("0");
  REQUIRE(npcHelloPhrase.getContent() == "npc 0");

  const Dialogue& playerLinearDialog = dialoguesManager.getDialogue("test_player_linear_dialogue");

  REQUIRE(playerLinearDialog.hasPhrase("player1"));
  REQUIRE(playerLinearDialog.hasPhrase("npc1"));

  REQUIRE(playerLinearDialog.getStartPhrase().getId() == "0");
}

TEST_CASE("game_dialogues_linear_started_by_npc", "[game]")
{
  DialoguesManager dialoguesManager = createTestDialoguesManager();
  std::shared_ptr<GameWorld> gameWorld = createTestGameWorld();

  GameObject player = gameWorld->findGameObject("player");
  GameObject npc = gameWorld->findGameObject("npc");

  DialogueState dialogueState;
  auto responsesList = dialoguesManager.startAnyDialogue(player, npc, dialogueState);

  // Validate dialogue state
  REQUIRE(dialogueState.isInitial());
  REQUIRE(dialogueState.hasLastPhrase());

  const Dialogue& npcDialogue = dialoguesManager.getDialogue("test_npc_hello_dialogue");

  REQUIRE(dialogueState.getDialogueId() == npcDialogue.getId());
  REQUIRE(dialogueState.getLastPhraseId() == npcDialogue.getStartPhrase().getId());
  REQUIRE(npcDialogue.getPhrase(dialogueState.getLastPhraseId()).getContent() == "npc 0");

  // Validate responses
  REQUIRE(responsesList.size() == 2);
  REQUIRE(responsesList[0].getDialogueId() == "test_player_linear_dialogue");
  REQUIRE(responsesList[0].getPhraseId() == "0");
  REQUIRE(responsesList[0].getInitiator().getName() == "player");
  REQUIRE(responsesList[0].getTarget().getName() == "npc");

  REQUIRE(responsesList[1].getDialogueId() == "test_player_branching_dialogue");
  REQUIRE(responsesList[1].getPhraseId() == "0");
  REQUIRE(responsesList[1].getInitiator().getName() == "player");
  REQUIRE(responsesList[1].getTarget().getName() == "npc");

  // Continue dialogue with linear dialogue

  auto secondResponsesList = dialoguesManager.continueDialogue(responsesList[0], dialogueState);

  // Validate dialogue state
  REQUIRE_FALSE(dialogueState.isInitial());
  REQUIRE(dialogueState.hasLastPhrase());

  const Dialogue& playerLinearDialogue = dialoguesManager.getDialogue("test_player_linear_dialogue");

  REQUIRE(dialogueState.getDialogueId() == playerLinearDialogue.getId());
  REQUIRE(dialogueState.getLastPhraseId() == "npc0");

  // Validate responses
  REQUIRE(secondResponsesList.size() == 1);
  REQUIRE(secondResponsesList[0].getDialogueId() == "test_player_linear_dialogue");
  REQUIRE(secondResponsesList[0].getPhraseId() == "player1");
  REQUIRE(secondResponsesList[0].getInitiator().getName() == "player");
  REQUIRE(secondResponsesList[0].getTarget().getName() == "npc");

  // Continue dialogue with linear dialogue
  auto thirdResponsesList = dialoguesManager.continueDialogue(secondResponsesList[0], dialogueState);

  // Validate dialogue state
  REQUIRE_FALSE(dialogueState.isInitial());
  REQUIRE(dialogueState.getLastPhraseId() == "npc1");

  // Validate responses
  REQUIRE(thirdResponsesList.size() == 1);
  REQUIRE(thirdResponsesList[0].getDialogueId() == "test_player_linear_dialogue");
  REQUIRE(thirdResponsesList[0].getPhraseId() == "player2");

  // Finish linear dialogue with the last player phrase
  auto fourthResponsesList = dialoguesManager.continueDialogue(thirdResponsesList[0], dialogueState);

  // Validate dialogue state
  REQUIRE(dialogueState.isInitial());
  REQUIRE_FALSE(dialogueState.hasLastPhrase());

  // Validate responses
  REQUIRE(responsesList.size() == 2);
  REQUIRE(responsesList[0].getDialogueId() == "test_player_linear_dialogue");
  REQUIRE(responsesList[0].getPhraseId() == "0");

  REQUIRE(responsesList[1].getDialogueId() == "test_player_branching_dialogue");
  REQUIRE(responsesList[1].getPhraseId() == "0");
}

TEST_CASE("game_dialogues_branching_started_by_player", "[game]")
{
  DialoguesManager dialoguesManager = createTestDialoguesManager();
  std::shared_ptr<GameWorld> gameWorld = createTestGameWorld();

  GameObject player = gameWorld->findGameObject("player");
  GameObject npc = gameWorld->findGameObject("npc2");

  DialogueState dialogueState;
  auto responsesList = dialoguesManager.startAnyDialogue(player, npc, dialogueState);

  // Validate dialogue state
  REQUIRE(dialogueState.isInitial());
  REQUIRE_FALSE(dialogueState.hasLastPhrase());

  // Validate responses
  REQUIRE(responsesList.size() == 2);
  REQUIRE(responsesList[0].getDialogueId() == "test_player_linear_dialogue");
  REQUIRE(responsesList[0].getPhraseId() == "0");

  REQUIRE(responsesList[1].getDialogueId() == "test_player_branching_dialogue");
  REQUIRE(responsesList[1].getPhraseId() == "0");

  auto responsesList2 = dialoguesManager.continueDialogue(responsesList[1], dialogueState);

  // Validate state
  REQUIRE_FALSE(dialogueState.isInitial());
  REQUIRE(dialogueState.hasLastPhrase());
  REQUIRE(dialogueState.getLastPhraseId() == "npc0");

  // Validate responses
  REQUIRE(responsesList2.size() == 2);
  REQUIRE(responsesList2[0].getPhraseId() == "player1");
  REQUIRE(responsesList2[1].getPhraseId() == "player2");

  auto responsesList3 = dialoguesManager.continueDialogue(responsesList2[0], dialogueState);

  // Validate state
  REQUIRE(dialogueState.isInitial());
  REQUIRE(dialogueState.hasLastPhrase());
  REQUIRE(dialogueState.getLastPhraseId() == "npc11");

  // Validate responses
  REQUIRE(responsesList3.size() == 2);
  REQUIRE(responsesList3[0].getDialogueId() == "test_player_linear_dialogue");
  REQUIRE(responsesList3[0].getPhraseId() == "0");

  REQUIRE(responsesList3[1].getDialogueId() == "test_player_branching_dialogue");
  REQUIRE(responsesList3[1].getPhraseId() == "0");
}