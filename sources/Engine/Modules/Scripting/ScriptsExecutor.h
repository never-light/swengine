#pragma once

#include "Modules/ECS/ECS.h"

DISABLE_WARNINGS()
#define SOL_ALL_SAFETIES_ON 1

#include <sol/sol.hpp>
ENABLE_WARNINGS()

#include "Bindings/ScriptsGameWorld.h"

class ScriptsExecutor {
 public:
  explicit ScriptsExecutor(std::shared_ptr<GameWorld> gameWorld);
  ~ScriptsExecutor();

  [[nodiscard]] const sol::state& getLuaState() const;
  [[nodiscard]] sol::state& getLuaState();

 private:
  void registerCommonTypes();
  void registerGameWorld();

 private:
  std::shared_ptr<GameWorld> m_gameWorld;

  sol::state m_luaState;

  std::shared_ptr<ScriptsGameWorld> m_scriptsGameWorld;
};
