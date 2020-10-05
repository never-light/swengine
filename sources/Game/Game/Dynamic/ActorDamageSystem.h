#pragma once

#include <string>
#include <unordered_set>

#include <Engine/Modules/ECS/ECS.h>

// TODO: consider separation events and systems and move events structs into own files

struct HitActorEvent {
 public:
  HitActorEvent(GameObject actor, float hitValue);

  [[nodiscard]] GameObject getActor() const;
  [[nodiscard]] float getHitValue() const;

 private:
  GameObject m_actor;
  float m_hitValue;
};

struct HitActorCommandEvent : public HitActorEvent {
 public:
  HitActorCommandEvent(GameObject actor, float hitValue);
};

// TODO: choose events naming convention (for example, separate them into signals and commands)
//  and rename all events

/**
 * @brief The signal about death of some actor
 *
 * Actor object will be destroyed and removed after
 * death. It is possible to process this event to
 * handle death before object destruction.
 */
struct ActorDeathSignalEvent {
 public:
  explicit ActorDeathSignalEvent(GameObject actor);

  [[nodiscard]] GameObject getActor() const;

 private:
  GameObject m_actor;
};

// TODO: implement garbage collection for dead actors

class ActorDamageSystem : public GameSystem,
                           public EventsListener<HitActorCommandEvent> {
 public:
  ActorDamageSystem() = default;

  void activate() override;
  void deactivate() override;

  EventProcessStatus receiveEvent(const HitActorCommandEvent& event) override;

 private:
  void destroyActor(GameObject actor);

 private:
  std::unordered_set<std::string> m_storage;
};
