#pragma once

#include <glm/vec3.hpp>
#include <btBulletDynamicsCommon.h>

#include "Modules/ECS/ECS.h"

#include "Modules/Physics/BaseBackend/PhysicsSystemBackend.h"
#include "Modules/Physics/RigidBodyComponent.h"

#include "BulletCollisionDispatcher.h"

// TODO: fix possible circular dependency here, replace shared_ptr to GameWorld with weak_ptr

class BulletPhysicsSystemBackend :
  public PhysicsSystemBackend,
  public std::enable_shared_from_this<BulletPhysicsSystemBackend>,
  public EventsListener<GameObjectAddComponentEvent<RigidBodyComponent>>,
  public EventsListener<GameObjectRemoveComponentEvent<RigidBodyComponent>>,
  public EventsListener<GameObjectRemoveEvent> {
 public:
  explicit BulletPhysicsSystemBackend(std::shared_ptr<GameWorld> gameWorld);
  ~BulletPhysicsSystemBackend() override;

  void configure() override;
  void unconfigure() override;

  void setGravity(const glm::vec3& gravity) override;
  glm::vec3 getGravity() const override;

  void update(float delta) override;

  EventProcessStatus receiveEvent(GameWorld* gameWorld,
    const GameObjectAddComponentEvent<RigidBodyComponent>& event) override;

  EventProcessStatus receiveEvent(GameWorld* gameWorld,
    const GameObjectRemoveComponentEvent<RigidBodyComponent>& event) override;

  EventProcessStatus receiveEvent(GameWorld* gameWorld, const GameObjectRemoveEvent& event) override;

 private:
  bool isConfigured() const;

  void nearCallback(btBroadphasePair& collisionPair,
    btCollisionDispatcher& dispatcher, btDispatcherInfo& dispatchInfo);

 private:
  static void physicsNearCallback(btBroadphasePair& collisionPair,
    btCollisionDispatcher& dispatcher, btDispatcherInfo& dispatchInfo);

 private:
  std::shared_ptr<GameWorld> m_gameWorld;

  btDefaultCollisionConfiguration* m_collisionConfiguration = nullptr;
  BulletCollisionDispatcher* m_collisionDispatcher = nullptr;
  btBroadphaseInterface* m_broadphaseInterface = nullptr;
  btSequentialImpulseConstraintSolver* m_constraintSolver = nullptr;
  btDiscreteDynamicsWorld* m_dynamicsWorld = nullptr;
};
