#pragma once

#include <Engine/Modules/Scripting/ScriptsExecutor.h>
#include <Engine/Modules/ECS/ECS.h>
#include "Game/Dynamic/ActorComponent.h"

class GameScriptingActorProxy {
 public:
  explicit GameScriptingActorProxy(std::shared_ptr<GameWorld> gameWorld,
    GameObject actorObject);
  ~GameScriptingActorProxy() = default;

  // TODO: this API is not const-correct, so fix it after fixing getComponent function of GameObject
  [[nodiscard]] const std::string& getName();

  [[nodiscard]] float getHealth();
  void setHealth(float health);

  [[nodiscard]] float getHealthLimit();
  void setHealthLimit(float limit);

  void hit(float hitValue);

  void giveInfoportion(const std::string& infoportionName);
  void deleteInfoportion(const std::string& infoportionName);
  [[nodiscard]] bool hasInfoportion(const std::string& infoportionName);

 private:
  [[nodiscard]] inline GameObjectComponentHandle<ActorComponent> getActorComponent()
  {
    return m_actorObject.getComponent<ActorComponent>();
  }

 private:
  std::shared_ptr<GameWorld> m_gameWorld;
  GameObject m_actorObject;
};

class GameplayScriptingContext {
 public:
  GameplayScriptingContext(std::shared_ptr<ScriptsExecutor> scriptsExecutor,
    std::shared_ptr<GameWorld> gameWorld);
  ~GameplayScriptingContext() = default;

 private:
  void registerActorsProxy();

 private:
  std::shared_ptr<ScriptsExecutor> m_scriptsExecutor;
  std::shared_ptr<GameWorld> m_gameWorld;
};
