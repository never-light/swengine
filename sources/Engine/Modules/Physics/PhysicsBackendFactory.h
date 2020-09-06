#pragma once

#include "BaseBackend/RigidBodyComponentBackend.h"
#include "BaseBackend/KinematicCharacterComponentBackend.h"
#include "BaseBackend/PhysicsSystemBackend.h"

#include "CollisionShapes.h"

class PhysicsBackendFactory {
 public:
  PhysicsBackendFactory() = delete;

 public:
  static std::shared_ptr<PhysicsSystemBackend> createPhysicsSystem(std::shared_ptr<GameWorld> gameWorld);

  static std::shared_ptr<RigidBodyComponentBackend> createRigidBodyComponent(float mass,
    std::shared_ptr<CollisionShape> collisionShape);

  static std::shared_ptr<KinematicCharacterComponentBackend> createKinematicCharacterComponent(
    std::shared_ptr<CollisionShape> collisionShape);

};
