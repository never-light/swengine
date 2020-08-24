#pragma once

#include "Modules/ECS/ECS.h"
#include "BulletBackend/BulletPhysicsBackend.h"

#include "RigidBodyComponent.h"
#include "CollisionShapesFactory.h"


class PhysicsSystem : public GameSystem {
 public:
  PhysicsSystem();
  ~PhysicsSystem() override;

  void configure(GameWorld* gameWorld) override;
  void unconfigure(GameWorld* gameWorld) override;

  void update(GameWorld* gameWorld, float delta) override;

  void setGravity(const glm::vec3& gravity);
  [[nodiscard]] glm::vec3 getGravity() const;

 private:
  std::unique_ptr<PhysicsBackend> m_physicsBackend;
};