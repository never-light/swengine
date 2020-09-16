#pragma once

#include "Modules/ECS/ECS.h"

#include "BaseBackend/PhysicsSystemBackend.h"

#include "RigidBodyComponent.h"
#include "KinematicCharacterComponent.h"

#include "CollisionShapesFactory.h"


class PhysicsSystem : public GameSystem {
 public:
  PhysicsSystem();
  ~PhysicsSystem() override;

  void configure() override;
  void unconfigure() override;

  void render() override;
  void update(float delta) override;

  void enableDebugDrawing(bool enable);
  bool isDebugDrawingEnabled();

  void setGravity(const glm::vec3& gravity);
  [[nodiscard]] glm::vec3 getGravity() const;

  void setUpdateStepCallback(std::function<void(float)> callback);

 private:
  std::shared_ptr<PhysicsSystemBackend> m_physicsBackend;
};
