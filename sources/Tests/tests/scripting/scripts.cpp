#include <catch2/catch.hpp>

#include <optional>
#include <utility>

#include <Engine/Modules/ECS/ECS.h>
#include <Engine/Modules/Scripting/ScriptsExecutor.h>

TEST_CASE("scripts_executor_loading_scripts", "[resources]")
{
  std::shared_ptr<GameWorld> gameWorld = GameWorld::createInstance();
  ScriptsExecutor scriptsExecutor(gameWorld);

  sol::function sumFunction = scriptsExecutor.getLuaState()["sum"];
  REQUIRE(static_cast<int>(sumFunction(45, 55)) == 100);
}
