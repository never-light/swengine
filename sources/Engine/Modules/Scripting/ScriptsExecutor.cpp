#include "precompiled.h"

#pragma hdrstop

#include "ScriptsExecutor.h"

#include <spdlog/spdlog.h>

#include "Utility/files.h"

ScriptsExecutor::ScriptsExecutor(std::shared_ptr<GameWorld> gameWorld)
  : m_gameWorld(gameWorld),
    m_scriptsGameWorld(std::make_shared<ScriptsGameWorld>(gameWorld))
{
  m_luaState.open_libraries(sol::lib::base);

  registerCommonTypes();
  registerGameWorld();

  std::vector<std::string> scriptsList = FileUtils::getScriptsList();

  for (const std::string& scriptName : scriptsList) {
    m_luaState.script_file(FileUtils::getScriptPath(scriptName));
  }
}

ScriptsExecutor::~ScriptsExecutor()
{

}

const sol::state& ScriptsExecutor::getLuaState() const
{
  return m_luaState;
}

sol::state& ScriptsExecutor::getLuaState()
{
  return m_luaState;
}

void ScriptsExecutor::registerCommonTypes()
{
  auto logging = m_luaState["logging"].get_or_create<sol::table>();

  logging.set_function("debug", [] (const std::string& message) -> void {
    spdlog::debug("[script] " + message);
  });

  logging.set_function("info", [] (const std::string& message) -> void {
    spdlog::info("[script] " + message);
  });

  m_luaState.new_usertype<glm::vec3>("vec3",
    sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>(),
    "x", &glm::vec3::x,
    "y", &glm::vec3::y,
    "z", &glm::vec3::z
  );
}

void ScriptsExecutor::registerGameWorld()
{
//  sol::usertype<ScriptsGameWorld> gameWorld = m_luaState.new_usertype<ScriptsGameWorld>("game_world",
//    sol::constructors<>());
//
//  gameWorld["spawnGameObject"] = &ScriptsGameWorld::spawnGameObject;
//  gameWorld["spawnUniqueGameObject"] = &ScriptsGameWorld::spawnUniqueGameObject;
//  gameWorld["findGameObject"] = &ScriptsGameWorld::findGameObject;
//
//  m_luaState[sol::update_if_empty]["world"] = [this] () -> std::shared_ptr<ScriptsGameWorld> {
//    return m_scriptsGameWorld;
//  };
}
