#pragma once

DISABLE_WARNINGS()
#define SOL_ALL_SAFETIES_ON 1

#include <sol/sol.hpp>
ENABLE_WARNINGS()

class ScriptsExecutor {
 public:
  ScriptsExecutor();
  ~ScriptsExecutor();

  [[nodiscard]] const sol::state& getLuaState() const;
  [[nodiscard]] sol::state& getLuaState();

 private:
  sol::state m_luaState;
};
