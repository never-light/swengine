#include <catch2/catch.hpp>
#include <Engine/Modules/LevelsManagement/LevelsManager.h>
#include <Engine/Modules/ResourceManagement/ResourceManagementModule.h>
#include <Engine/Modules/Graphics/GraphicsSystem/TransformComponent.h>
#include <Engine/Modules/LevelsManagement/GameObjectsGenericClassLoader.h>
#include <Engine/Modules/LevelsManagement/GameObjectsSpawnSystem.h>

TEST_CASE("loading_levels_game_objects", "[levels_management]")
{
  std::shared_ptr<GameWorld> gameWorld = GameWorld::createInstance();

  std::shared_ptr<ResourceManagementModule> resourceManagementModule = std::make_shared<ResourceManagementModule>();
  std::shared_ptr<ResourcesManager> resourceManager = resourceManagementModule->getResourceManager();

  std::shared_ptr<LevelsManager> levelsManager =
    std::make_shared<LevelsManager>(gameWorld, resourceManagementModule->getResourceManager());
  levelsManager->getObjectsLoader()
    .registerClassLoader("generic", std::make_unique<GameObjectsGenericClassLoader>(levelsManager));

  levelsManager->loadLevel("test");

  REQUIRE(gameWorld->findGameObject("player").isAlive());
  REQUIRE(gameWorld->findGameObject("city_environment").isAlive());
}

TEST_CASE("spawn_game_objects", "[levels_management]")
{
  std::shared_ptr<GameWorld> gameWorld = GameWorld::createInstance();

  std::shared_ptr<ResourceManagementModule> resourceManagementModule = std::make_shared<ResourceManagementModule>();
  std::shared_ptr<ResourcesManager> resourceManager = resourceManagementModule->getResourceManager();

  std::shared_ptr<LevelsManager> levelsManager =
    std::make_shared<LevelsManager>(gameWorld, resourceManagementModule->getResourceManager());
  levelsManager->getObjectsLoader()
    .registerClassLoader("generic", std::make_unique<GameObjectsGenericClassLoader>(levelsManager));

  std::shared_ptr<GameObjectsSpawnSystem> spawnSystem =
    std::make_shared<GameObjectsSpawnSystem>(levelsManager);

  gameWorld->getGameSystemsGroup()->addGameSystem(spawnSystem);

  levelsManager->loadLevel("test");
  levelsManager->loadSpawnObjectsList("test_list");

  gameWorld->emitEvent(SpawnGameObjectCommandEvent(
    "test_spawned_object", glm::vec3{10.0f, 20.0f, 30.0f}, MathUtils::AXIS_Z));

  GameObject spawnedObject = gameWorld->findGameObject("test_spawned_object");

  REQUIRE(spawnedObject.isAlive());
  REQUIRE_FALSE(spawnedObject.getComponent<TransformComponent>()->isStatic());
  REQUIRE(MathUtils::isEqual(spawnedObject.getComponent<TransformComponent>()->getTransform().getPosition(),
    glm::vec3{10.0f, 20.0f, 30.0f}));
}