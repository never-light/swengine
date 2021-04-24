#include "GameplayScriptingContext.h"
#include "Game/Dynamic/ActorDamageSystem.h"

#include <utility>

GameplayScriptingContext::GameplayScriptingContext(std::shared_ptr<ScriptsExecutor> scriptsExecutor,
  std::shared_ptr<GameWorld> gameWorld)
  : m_scriptsExecutor(std::move(scriptsExecutor)),
    m_gameWorld(std::move(gameWorld))
{
  spdlog::info(">> Gameplay scripting context initialization is started");
  registerActorsProxy();
  spdlog::info("<< Gameplay scripting context initialization is finished");
}

void GameplayScriptingContext::registerActorsProxy()
{
  auto& luaState = m_scriptsExecutor->getLuaState();

  luaState.new_usertype<GameScriptingActorProxy>("GameActor",
    "name", sol::property(&GameScriptingActorProxy::getName),
    "health", sol::property(&GameScriptingActorProxy::getHealth, &GameScriptingActorProxy::setHealth),
    "healthLimit", sol::property(&GameScriptingActorProxy::getHealthLimit, &GameScriptingActorProxy::setHealthLimit),
    "hit", &GameScriptingActorProxy::hit,
    "give_infoportion", &GameScriptingActorProxy::giveInfoportion,
    "delete_infoportion", &GameScriptingActorProxy::deleteInfoportion,
    "has_infoportion", &GameScriptingActorProxy::hasInfoportion
  );

  auto worldContext = luaState["world"].get_or_create<sol::table>();

  worldContext.set_function("find_actor", [this](const std::string& actorObjectName) -> std::shared_ptr<GameScriptingActorProxy> {
    spdlog::info("[script] >> Find actor {}", actorObjectName);

    GameObject actorObject = m_gameWorld->findGameObject(actorObjectName);

    if (actorObject.isAlive() && actorObject.hasComponent<ActorComponent>()) {
      spdlog::info("[script] << Actor {} is found, name is {}", actorObjectName, actorObject.getComponent<ActorComponent>()->getName());

      return std::make_shared<GameScriptingActorProxy>(m_gameWorld, actorObject);
    }
    else {
      spdlog::info("[script] << Actor {} is not found");

      return nullptr;
    }
  });
}

GameScriptingActorProxy::GameScriptingActorProxy(std::shared_ptr<GameWorld> gameWorld,
  GameObject actorObject)
  : m_gameWorld(std::move(gameWorld)),
    m_actorObject(actorObject)
{

}

const std::string& GameScriptingActorProxy::getName()
{
  return getActorComponent()->getName();
}

float GameScriptingActorProxy::getHealth()
{
  return getActorComponent()->getHealth();
}

void GameScriptingActorProxy::setHealth(float health)
{
  SW_ASSERT(health >= 0);

  m_gameWorld->emitEvent(HitActorCommandEvent(m_actorObject,
    getActorComponent()->getHealth() - health));
}

float GameScriptingActorProxy::getHealthLimit()
{
  return getActorComponent()->getHealthLimit();
}

void GameScriptingActorProxy::setHealthLimit(float limit)
{
  getActorComponent()->setHealthLimit(limit);

  // Hit actor with zero value to recalculate current health
  m_gameWorld->emitEvent(HitActorCommandEvent(m_actorObject, 0.0f));
}

void GameScriptingActorProxy::hit(float hitValue)
{
  m_gameWorld->emitEvent(HitActorCommandEvent(m_actorObject, hitValue));
}

bool GameScriptingActorProxy::hasInfoportion(const std::string& infoportionName)
{
  return getActorComponent()->getInfoportionsStorage().hasInfoportion(infoportionName);
}

void GameScriptingActorProxy::giveInfoportion(const std::string& infoportionName)
{
  m_gameWorld->emitEvent(AddInfoportionCommandEvent(m_actorObject, infoportionName));
}

void GameScriptingActorProxy::deleteInfoportion(const std::string& infoportionName)
{
  m_gameWorld->emitEvent(RemoveInfoportionCommandEvent(m_actorObject, infoportionName));
}
