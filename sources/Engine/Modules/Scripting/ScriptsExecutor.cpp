#include "precompiled.h"

#pragma hdrstop

#include "ScriptsExecutor.h"

ScriptsExecutor::ScriptsExecutor()
{
  lua.open_libraries(sol::lib::base);
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
