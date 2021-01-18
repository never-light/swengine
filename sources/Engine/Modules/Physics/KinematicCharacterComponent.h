#pragma once

#include <memory>

#include "Modules/ResourceManagement/ResourcesManagement.h"
#include "Modules/Graphics/GraphicsSystem/Transform.h"
#include "Modules/ECS/GameObjectsFactory.h"
#include "BaseBackend/KinematicCharacterComponentBackend.h"

#include "CollisionShapes.h"
#include "PhysicsCollisions.h"

class KinematicCharacterComponentBindingParameters {
 public:
  glm::vec3 originOffset{};
  float capsuleHeight{};
  float capsuleRadius{};

  template<class Archive>
  void serialize(Archive& archive)
  {
    archive(
      cereal::make_nvp("origin_offset", originOffset),
      cereal::make_nvp("capsule_height", capsuleHeight),
      cereal::make_nvp("capsule_radius", capsuleRadius));
  };
};

struct KinematicCharacterComponent {
 public:
  static constexpr bool s_isSerializable = true;
  using BindingParameters = KinematicCharacterComponentBindingParameters;

 public:
  explicit KinematicCharacterComponent(ResourceHandle<CollisionShape> collisionShape);

  void setTransform(const Transform& transform);

  void setPositionIncrement(const glm::vec3& increment);

  void jump(const glm::vec3& jumpVector);
  [[nodiscard]] bool isOnGround() const;

  void setOriginOffset(const glm::vec3& offset);
  [[nodiscard]] glm::vec3 getOriginOffset() const;

  void setCollisionCallback(CollisionCallback callback);
  [[nodiscard]] CollisionCallback getCollisionCallback() const;

  [[nodiscard]] const KinematicCharacterComponentBackend& getBackend() const;
  [[nodiscard]] KinematicCharacterComponentBackend& getBackend();

  [[nodiscard]] ResourceHandle<CollisionShape> getCollisionShape() const;

  [[nodiscard]] BindingParameters getBindingParameters() const;

  void resetBackend();

 private:
  ResourceHandle<CollisionShape> m_collisionShape;
  std::shared_ptr<KinematicCharacterComponentBackend> m_backend;
  CollisionCallback m_collisionCallback;
};

class KinematicCharacterComponentBinder : public GameObjectsComponentBinder<KinematicCharacterComponent> {
 public:
  explicit KinematicCharacterComponentBinder(const ComponentBindingParameters& componentParameters,
    std::shared_ptr<ResourcesManager> resourcesManager);

  void bindToObject(GameObject& gameObject) override;

 private:
  ComponentBindingParameters m_bindingParameters;
  std::shared_ptr<ResourcesManager> m_resourcesManager;
};
