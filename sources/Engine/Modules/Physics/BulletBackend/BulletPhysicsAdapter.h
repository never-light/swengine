#pragma once

#include <glm/vec3.hpp>
#include <btBulletDynamicsCommon.h>

#include "Modules/ECS/GameWorld.h"

// TODO: create adapters interfaces and break cyclic-dependency, do not use forward declarations here
struct RigidBodyComponent;

class BulletPhysicsAdapter : public std::enable_shared_from_this<BulletPhysicsAdapter>,
                             public EventsListener<GameObjectAddComponentEvent<RigidBodyComponent>>,
                             public EventsListener<GameObjectRemoveComponentEvent<RigidBodyComponent>>,
                             public EventsListener<GameObjectRemoveEvent> {
 public:
  BulletPhysicsAdapter(std::weak_ptr<GameWorld> gameWorld);
  ~BulletPhysicsAdapter();

  void configure();
  void unconfigure();

  void setGravity(const glm::vec3& gravity);
  glm::vec3 getGravity() const;

  void update(float delta);

  EventProcessStatus receiveEvent(GameWorld* gameWorld,
    const GameObjectAddComponentEvent<RigidBodyComponent>& event) override;

  EventProcessStatus receiveEvent(GameWorld* gameWorld,
    const GameObjectRemoveComponentEvent<RigidBodyComponent>& event) override;

  EventProcessStatus receiveEvent(GameWorld* gameWorld, const GameObjectRemoveEvent& event) override;

 private:
  bool isConfigured() const;

 private:
  std::weak_ptr<GameWorld> m_gameWorld;

  btDefaultCollisionConfiguration* m_collisionConfiguration = nullptr;
  btCollisionDispatcher* m_collisionDispatcher = nullptr;
  btBroadphaseInterface* m_broadphaseInterface = nullptr;
  btSequentialImpulseConstraintSolver* m_constraintSolver = nullptr;
  btDiscreteDynamicsWorld* m_dynamicsWorld = nullptr;
};
