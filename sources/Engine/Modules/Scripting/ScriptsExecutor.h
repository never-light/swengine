#pragma once

#include "Modules/ECS/ECS.h"

DISABLE_WARNINGS()
#define SOL_ALL_SAFETIES_ON 1
#define SOL_EXCEPTIONS_SAFE_PROPAGATION 1

#include <sol/sol.hpp>
#include <utility>

ENABLE_WARNINGS()

#include "Bindings/ScriptsGameWorld.h"
#include "Modules/LevelsManagement/LevelsManager.h"
#include "Modules/ResourceManagement/ResourcesManager.h"

class ScriptingContext {
 public:
  ScriptingContext(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<LevelsManager> levelsManager,
    std::shared_ptr<ResourcesManager> resourcesManager)
      : m_gameWorld(std::move(gameWorld)),
      m_levelsManager(std::move(levelsManager)),
      m_resourcesManager(std::move(resourcesManager))
  {

  }

  [[nodiscard]] inline std::shared_ptr<GameWorld> getGameWorld() const
  {
    return m_gameWorld;
  }

  [[nodiscard]] inline std::shared_ptr<LevelsManager> getLevelsManager() const
  {
    return m_levelsManager;
  }

  [[nodiscard]] inline std::shared_ptr<ResourcesManager> getResourcesManager() const
  {
    return m_resourcesManager;
  }

 private:
  std::shared_ptr<GameWorld> m_gameWorld;
  std::shared_ptr<LevelsManager> m_levelsManager;
  std::shared_ptr<ResourcesManager> m_resourcesManager;
};

class ScriptsExecutor {
 public:
  explicit ScriptsExecutor(std::shared_ptr<ScriptingContext> scriptingContext);
  ~ScriptsExecutor();

  [[nodiscard]] const sol::state& getLuaState() const;
  [[nodiscard]] sol::state& getLuaState();

 private:
  void registerCommonTypes();
  void registerGameWorld();
  void registerResourcesManagementFunctions();

 private:
  std::shared_ptr<ScriptingContext> m_scriptingContext;

  sol::state m_luaState;

  std::shared_ptr<ScriptsGameWorld> m_scriptsGameWorld;
};
