#include "precompiled.h"

#pragma hdrstop

#include "ScriptingSystem.h"
#include "Exceptions/exceptions.h"

#include <utility>

ScriptingSystem::ScriptingSystem(std::shared_ptr<GameWorld> gameWorld)
  : m_scriptsExecutor(std::make_shared<ScriptsExecutor>(std::move(gameWorld)))
{

}

void ScriptingSystem::configure()
{
  auto gameWorld = getGameWorld();
  gameWorld->subscribeEventsListener<ExecuteScriptSimpleActionCommand>(this);
  gameWorld->subscribeEventsListener<ExecuteScriptUndirectedActionCommand>(this);
  gameWorld->subscribeEventsListener<ExecuteScriptDirectedActionCommand>(this);
  gameWorld->subscribeEventsListener<ExecuteScriptParametricActionCommand>(this);
}

void ScriptingSystem::unconfigure()
{
  auto gameWorld = getGameWorld();

  gameWorld->unsubscribeEventsListener<ExecuteScriptSimpleActionCommand>(this);
  gameWorld->unsubscribeEventsListener<ExecuteScriptUndirectedActionCommand>(this);
  gameWorld->unsubscribeEventsListener<ExecuteScriptDirectedActionCommand>(this);
  gameWorld->unsubscribeEventsListener<ExecuteScriptParametricActionCommand>(this);
}

EventProcessStatus ScriptingSystem::receiveEvent(const ExecuteScriptDirectedActionCommand& event)
{
  auto actionHandler = findActionHandler(event.actionName);
  validateActionCallResult(actionHandler(event.initiator.getName(), event.target.getName()));

  return EventProcessStatus::Processed;
}

EventProcessStatus ScriptingSystem::receiveEvent(const ExecuteScriptSimpleActionCommand& event)
{
  auto actionHandler = findActionHandler(event.actionName);
  validateActionCallResult(actionHandler());

  return EventProcessStatus::Processed;
}

EventProcessStatus ScriptingSystem::receiveEvent(const ExecuteScriptUndirectedActionCommand& event)
{
  auto actionHandler = findActionHandler(event.actionName);
  validateActionCallResult(actionHandler(event.initiator.getName()));

  return EventProcessStatus::Processed;
}

sol::protected_function ScriptingSystem::findActionHandler(const std::string& handlerName)
{
  auto actionHandlerFindExpression = m_scriptsExecutor->getLuaState().load("return " + handlerName);
  auto actionFindingResult = actionHandlerFindExpression();

  return static_cast<sol::protected_function>(actionFindingResult);
}

EventProcessStatus ScriptingSystem::receiveEvent(const ExecuteScriptParametricActionCommand& event)
{
  auto actionHandler = findActionHandler(event.getActionName());
  validateActionCallResult(event.apply(actionHandler));

  return EventProcessStatus::Processed;
}

void ScriptingSystem::validateActionCallResult(const sol::protected_function_result& result)
{
  if (!result.valid()) {
    sol::error error = result;

    spdlog::error("Lua action error: " + std::string(error.what()));

    THROW_EXCEPTION(EngineRuntimeException, "Lua action error: " + std::string(error.what()));
  }
}

std::shared_ptr<ScriptsExecutor> ScriptingSystem::getScriptsExecutor() const
{
  return m_scriptsExecutor;
}
