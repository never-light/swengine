#include <catch2/catch.hpp>

#include <Engine/Modules/ECS/ECS.h>
#include <Game/Game/Dynamic/DialoguesManager.h>
#include <Game/Game/PlayerComponent.h>
#include <Game/Saving/SavingSystem.h>
#include <Engine/Modules/Graphics/GraphicsSystem/TransformComponent.h>
#include <Engine/Modules/ECS/GameObjectsFactory.h>

#include "TestGameWorldGenerator.h"

//TEST_CASE("game_world_saving_loading", "[game]")
//{
//  auto gameWorld = GameWorld::createInstance();
//  auto resourcesManager = std::make_shared<ResourcesManager>();
//
//  gameWorld->registerComponentBinderFactory<TransformComponent>(
//    std::make_shared<GameObjectsComponentsGenericBindersFactory<TransformComponent, TransformComponentBinder>>());
//
//  std::shared_ptr<SavingSystem> savingSystem = std::make_shared<SavingSystem>();
//  gameWorld->getGameSystemsGroup()->addGameSystem(savingSystem);
//
//  GameObject playerObject = gameWorld->createGameObject("player");
//  auto playerTransformComponent = playerObject.addComponent<TransformComponent>();
//  playerTransformComponent->getTransform().setPosition(10, 20, 30);
//
//  GameObject enemyObject = gameWorld->createGameObject("enemy");
//  auto enemyTransformComponent = enemyObject.addComponent<TransformComponent>();
//  enemyTransformComponent->getTransform().setPosition(-10, -20, -30);
//
//  gameWorld->emitEvent<SaveCommandTriggerEvent>(SaveCommandTriggerEvent("test_save"));
//
//  for (GameObject gameObject : gameWorld->all()) {
//    gameWorld->removeGameObject(gameObject);
//  }
//
//  gameWorld->emitEvent<LoadCommandTriggerEvent>(LoadCommandTriggerEvent("test_save"));
//
//  GameObject loadedPlayerObject = gameWorld->findGameObject("player");
//  REQUIRE(loadedPlayerObject.isAlive());
//  REQUIRE(MathUtils::isEqual(loadedPlayerObject.getComponent<TransformComponent>()->getTransform().getPosition(),
//    glm::vec3(10, 20, 30)));
//
//  GameObject loadedEnemyObject = gameWorld->findGameObject("enemy");
//  REQUIRE(loadedEnemyObject.isAlive());
//  REQUIRE(MathUtils::isEqual(loadedEnemyObject.getComponent<TransformComponent>()->getTransform().getPosition(),
//    glm::vec3(-10, -20, -30)));
//}
