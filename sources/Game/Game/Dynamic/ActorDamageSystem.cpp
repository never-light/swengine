#include "ActorDamageSystem.h"

#include <utility>

#include <Engine/Modules/Math/MathUtils.h>

#include "ActorComponent.h"

HitActorEvent::HitActorEvent(GameObject actor, float hitValue)
  : m_actor(actor),
    m_hitValue(hitValue)
{

}

GameObject HitActorEvent::getActor() const
{
  return m_actor;
}

float HitActorEvent::getHitValue() const
{
  return m_hitValue;
}

HitActorCommandEvent::HitActorCommandEvent(GameObject actor, float hitValue)
  : HitActorEvent(actor, hitValue)
{

}

ActorDeathSignalEvent::ActorDeathSignalEvent(GameObject actor)
  : m_actor(actor)
{

}

GameObject ActorDeathSignalEvent::getActor() const
{
  return m_actor;
}

void ActorDamageSystem::activate()
{
  GameWorld* gameWorld = getGameWorld();

  gameWorld->subscribeEventsListener<HitActorCommandEvent>(this);
}

void ActorDamageSystem::deactivate()
{
  GameWorld* gameWorld = getGameWorld();

  gameWorld->unsubscribeEventsListener<HitActorCommandEvent>(this);
}

EventProcessStatus ActorDamageSystem::receiveEvent(const HitActorCommandEvent& event)
{
  GameObject actorObject = event.getActor();
  SW_ASSERT(actorObject.hasComponent<ActorComponent>());

  auto actorComponent = actorObject.getComponent<ActorComponent>();

  SW_ASSERT(!actorComponent->isDead());

  // Limit actor health between [0, limit]
  // Hit value can be also negative to heal a actor
  float newHealth = std::min(std::max(actorComponent->getHealth() - event.getHitValue(), 0.0f),
    actorComponent->getHealthLimit());
  actorComponent->setHealth(newHealth);

  getGameWorld()->emitEvent<HitActorEvent>(HitActorEvent(actorObject, event.getHitValue()));

  if (MathUtils::isZero(newHealth)) {
    actorComponent->setDead();

    EventProcessStatus deathSignalProcessStatus =
      getGameWorld()->emitEvent<ActorDeathSignalEvent>(ActorDeathSignalEvent(actorObject));

    // If event handlers did not handle event death by its own, just destroy the actor object
    if (deathSignalProcessStatus == EventProcessStatus::Skipped ||
      deathSignalProcessStatus == EventProcessStatus::Processed) {
      destroyActor(actorObject);
    }
  }

  return EventProcessStatus::Processed;
}

void ActorDamageSystem::destroyActor(GameObject actor)
{
  getGameWorld()->removeGameObject(actor);
}
