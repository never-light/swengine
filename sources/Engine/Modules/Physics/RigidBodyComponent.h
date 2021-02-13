#pragma once

#include <memory>

#include "Modules/ResourceManagement/ResourcesManagement.h"
#include "Modules/Graphics/GraphicsSystem/Transform.h"
#include "Modules/ECS/GameObjectsFactory.h"
#include "BaseBackend/RigidBodyComponentBackend.h"

#include "CollisionShapes.h"
#include "PhysicsCollisions.h"

class RigidBodyComponentBindingParameters {
 public:
  std::string collisionModelResourceName;
  float mass{};

  template<class Archive>
  void serialize(Archive& archive)
  {
    archive(
      cereal::make_nvp("collision_model_resource", collisionModelResourceName),
      cereal::make_nvp("mass", mass));
  };
};

struct RigidBodyComponent {
 public:
  static constexpr bool s_isSerializable = true;
  using BindingParameters = RigidBodyComponentBindingParameters;

 public:
  RigidBodyComponent(float mass, ResourceHandle<CollisionShape> collisionShape);

  void setMass(float mass);
  [[nodiscard]] float getMass() const;

  void setTransform(const Transform& transform);

  void setLinearVelocity(const glm::vec3& velocity);
  [[nodiscard]] glm::vec3 getLinearVelocity() const;

  void setCollisionCallback(CollisionCallback callback);
  [[nodiscard]] CollisionCallback getCollisionCallback() const;

  void setAngularFactor(const glm::vec3& factor);
  [[nodiscard]] glm::vec3 getAngularFactor() const;

  void setLinearFactor(const glm::vec3& factor);
  [[nodiscard]] glm::vec3 getLinearFactor() const;

  void enableSimulation(bool enable);
  [[nodiscard]] bool isSimulationEnabled() const;

  [[nodiscard]] const RigidBodyComponentBackend& getBackend() const;
  [[nodiscard]] RigidBodyComponentBackend& getBackend();

  [[nodiscard]] BindingParameters getBindingParameters() const;

  void resetBackend();

 private:
  ResourceHandle<CollisionShape> m_collisionShape;
  std::shared_ptr<RigidBodyComponentBackend> m_backend;
  CollisionCallback m_collisionCallback;
};

class RigidBodyComponentBinder : public GameObjectsComponentBinder<RigidBodyComponent> {
 public:
  explicit RigidBodyComponentBinder(const ComponentBindingParameters& componentParameters,
    std::shared_ptr<ResourcesManager> resourcesManager);

  void bindToObject(GameObject& gameObject) override;

 private:
  ComponentBindingParameters m_bindingParameters;
  std::shared_ptr<ResourcesManager> m_resourcesManager;
};
