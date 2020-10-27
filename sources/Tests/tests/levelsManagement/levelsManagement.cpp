#include <catch2/catch.hpp>
#include <Engine/Modules/LevelsManagement/LevelsManager.h>
#include <Engine/Modules/ResourceManagement/ResourceManagementModule.h>
#include <Engine/Modules/LevelsManagement/GameObjectsGenericClassLoader.h>

TEST_CASE("loading_levels_game_objects", "[levels_management]")
{
    std::shared_ptr<GameWorld> m_gameWorld = GameWorld::createInstance();

    std::shared_ptr<ResourceManagementModule> m_resourceManagementModule = std::make_shared<ResourceManagementModule>();
    std::shared_ptr<ResourcesManager> m_resourceManager = m_resourceManagementModule->getResourceManager();

    std::shared_ptr<LevelsManager> m_levelsManager =
            std::make_shared<LevelsManager>(m_gameWorld, m_resourceManagementModule->getResourceManager());
    m_levelsManager->getObjectsLoader().registerClassLoader("generic", std::make_unique<GameObjectsGenericClassLoader>(m_levelsManager));

    m_levelsManager->loadLevel("test");

    REQUIRE(m_gameWorld->findGameObject("player").isAlive());
    REQUIRE(m_gameWorld->findGameObject("city_environment").isAlive());
}