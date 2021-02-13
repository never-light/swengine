#pragma once

#include "Modules/ResourceManagement/ResourcesManagement.h"

#include "BaseBackend/RigidBodyComponentBackend.h"
#include "BaseBackend/KinematicCharacterComponentBackend.h"
#include "BaseBackend/PhysicsSystemBackend.h"

#include "CollisionShapes.h"

class PhysicsBackendFactory {
 public:
  PhysicsBackendFactory() = delete;

 public:
  static std::shared_ptr<PhysicsSystemBackend> createPhysicsSystem(GameWorld* gameWorld);

  static std::shared_ptr<RigidBodyComponentBackend> createRigidBodyComponent(float mass,
    ResourceHandle<CollisionShape> collisionShape);

  static std::shared_ptr<KinematicCharacterComponentBackend> createKinematicCharacterComponent(
    ResourceHandle<CollisionShape> collisionShape);

};
