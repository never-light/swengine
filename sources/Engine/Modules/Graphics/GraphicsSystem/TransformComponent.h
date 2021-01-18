#pragma once

#include <memory>

#include "Transform.h"
#include "Modules/ECS/GameObjectsFactory.h"
#include "Modules/Math/geometry.h"
#include "Modules/Math/MathUtils.h"

struct TransformComponentBindingParameters {
  glm::vec3 position{};
  glm::vec3 scale{};
  glm::vec3 frontDirection{};
  bool isStatic{};

  template<class Archive>
  void serialize(Archive& archive)
  {
    archive(
      cereal::make_nvp("position", position),
      cereal::make_nvp("scale", scale),
      cereal::make_nvp("front_direction", frontDirection),
      cereal::make_nvp("is_static", isStatic));
  };
};

class TransformComponent {
 public:
  static constexpr bool s_isSerializable = true;
  using BindingParameters = TransformComponentBindingParameters;

 public:
  TransformComponent();

  [[nodiscard]] Transform& getTransform();
  [[nodiscard]] const Transform& getTransform() const;

  [[nodiscard]] std::shared_ptr<Transform> getTransformPtr() const;

  // TODO: move static mode settings to constructor parameters
  //  and do not allow to change it after component creation
  void setStaticMode(bool isStatic);
  [[nodiscard]] bool isStatic() const;

  void updateBounds(const glm::mat4& transformation);
  void updateBounds(const glm::vec3& origin, const glm::quat& orientation);

  [[nodiscard]] const AABB& getBoundingBox() const;
  [[nodiscard]] const Sphere& getBoundingSphere() const;

  void setBounds(const AABB& bounds);
  [[nodiscard]] const AABB& getOriginalBounds() const;

  [[nodiscard]] BindingParameters getBindingParameters() const;

 private:
  std::shared_ptr<Transform> m_transform;

  bool m_isStatic = false;

  AABB m_boundingBox;
  Sphere m_boundingSphere;

  AABB m_originalBounds;
};

class TransformComponentBinder : public GameObjectsComponentBinder<TransformComponent> {
 public:
  explicit TransformComponentBinder(const ComponentBindingParameters& componentParameters);

  void bindToObject(GameObject& gameObject) override;

 private:
  ComponentBindingParameters m_bindingParameters;
};