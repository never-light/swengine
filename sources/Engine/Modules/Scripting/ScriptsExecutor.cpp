#include "precompiled.h"

#pragma hdrstop

#include "ScriptsExecutor.h"

#include <spdlog/spdlog.h>
#include <glm/gtx/string_cast.hpp>

#include "Exceptions/exceptions.h"

#include "Utility/files.h"

inline void luaStatePanic(sol::optional<std::string> errorMessage)
{
  std::string errorMessageText = errorMessage.value_or("no description");

  spdlog::error("Lua script execution error: {}", errorMessageText);

  THROW_EXCEPTION(EngineRuntimeException, errorMessageText);
}

ScriptsExecutor::ScriptsExecutor(std::shared_ptr<GameWorld> gameWorld)
  : m_gameWorld(gameWorld),
    m_luaState(sol::c_call<decltype(&luaStatePanic), &luaStatePanic>),
    m_scriptsGameWorld(std::make_shared<ScriptsGameWorld>(gameWorld))
{
  m_luaState.open_libraries(sol::lib::base, sol::lib::string);

  registerCommonTypes();
  registerGameWorld();

  std::vector<std::string> scriptsList = FileUtils::getScriptsList();

  for (const std::string& scriptName : scriptsList) {
    try {
      if (scriptName != "annotations") {
        spdlog::info("Load script {}", scriptName);
        m_luaState.script_file(FileUtils::getScriptPath(scriptName));
      }
    }
    catch (const sol::error& error) {
      THROW_EXCEPTION(EngineRuntimeException,
        fmt::format("Script can not be loaded, name {}, error {}", scriptName, error.what()));
    }
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

  logging.set_function("debug", [](const std::string& message) -> void {
    spdlog::debug("[script] " + message);
  });

  logging.set_function("info", [](const std::string& message) -> void {
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
  auto world = m_luaState["world"].get_or_create<sol::table>();

  world.set_function("spawn_object", [this](const std::string& spawnName,
    const sol::object& objectName,
    const sol::object& position,
    const sol::object& direction,
    const sol::object& level) -> void {

    spdlog::info("[script] spawn object {}", spawnName);

    m_scriptsGameWorld->spawnGameObject(spawnName,
      objectName == sol::nil ? std::optional<std::string>() : objectName.as<std::string>(),
      position == sol::nil ? std::optional<glm::vec3>() : position.as<glm::vec3>(),
      direction == sol::nil ? std::optional<glm::vec3>() : direction.as<glm::vec3>(),
      level == sol::nil ? std::optional<std::string>() : direction.as<std::string>());
  });
}
