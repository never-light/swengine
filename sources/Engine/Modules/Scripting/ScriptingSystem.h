#pragma once

#include <memory>
#include <utility>

#include "Modules/ECS/ECS.h"
#include "ScriptsExecutor.h"

struct ExecuteScriptSimpleActionCommand {
  std::string actionName;
};

struct ExecuteScriptUndirectedActionCommand {
  GameObject initiator;
  std::string actionName;
};

struct ExecuteScriptDirectedActionCommand {
  GameObject initiator;
  GameObject target;
  std::string actionName;
};

struct ExecuteScriptParametricActionCommand {
 public:
  [[nodiscard]] sol::protected_function_result apply(sol::protected_function actionHandler) const {
    return m_actionApplier(std::move(actionHandler));
  }

  template<class TupleType>
  static ExecuteScriptParametricActionCommand create(const std::string& actionName, const TupleType& parameters) {
    ExecuteScriptParametricActionCommand actionCommand;

    actionCommand.m_actionName = actionName;

    actionCommand.m_actionApplier = [parameters] (sol::protected_function actionHandler) {
      return std::apply(actionHandler, parameters);
    };

    return actionCommand;
  }

  [[nodiscard]] inline const std::string& getActionName() const {
    return m_actionName;
  }

 private:
  ExecuteScriptParametricActionCommand() = default;

 private:
  std::string m_actionName;
  std::function<sol::protected_function_result(sol::protected_function actionHandler)> m_actionApplier;
};

class ScriptingSystem : public GameSystem,
                        public EventsListener<ExecuteScriptSimpleActionCommand>,
                        public EventsListener<ExecuteScriptUndirectedActionCommand>,
                        public EventsListener<ExecuteScriptDirectedActionCommand>,
                        public EventsListener<ExecuteScriptParametricActionCommand> {
 public:
  explicit ScriptingSystem(std::shared_ptr<GameWorld> gameWorld);
  ~ScriptingSystem() override = default;

  void configure() override;
  void unconfigure() override;

  EventProcessStatus receiveEvent(const ExecuteScriptSimpleActionCommand& event) override;
  EventProcessStatus receiveEvent(const ExecuteScriptUndirectedActionCommand& event) override;
  EventProcessStatus receiveEvent(const ExecuteScriptDirectedActionCommand& event) override;
  EventProcessStatus receiveEvent(const ExecuteScriptParametricActionCommand& event) override;

  [[nodiscard]] std::shared_ptr<ScriptsExecutor> getScriptsExecutor() const;

 private:
  [[nodiscard]] sol::protected_function findActionHandler(const std::string& handlerName);
  void validateActionCallResult(const sol::protected_function_result& result);

 private:
  std::shared_ptr<ScriptsExecutor> m_scriptsExecutor;
};

