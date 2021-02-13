#pragma once

#include <memory>

#include "Camera.h"
#include "Modules/ECS/GameObjectsFactory.h"

class CameraComponentBindingParameters {
 public:
  glm::vec3 position{};
  glm::vec3 lookAtPoint{};

  float nearDistance = 0.1f;
  float farDistance = 100.0f;
  float fov = 60.0f;

  template<class Archive>
  void serialize(Archive& archive)
  {
    archive(
      cereal::make_nvp("position", position),
      cereal::make_nvp("look_at", lookAtPoint),
      cereal::make_nvp("near_dist", nearDistance),
      cereal::make_nvp("far_dist", farDistance),
      cereal::make_nvp("fov", fov));
  };

};

class CameraComponent {
 public:
  static constexpr bool s_isSerializable = true;
  using BindingParameters = CameraComponentBindingParameters;

 public:
  CameraComponent();

  void setCamera(std::shared_ptr<Camera> camera);
  [[nodiscard]] std::shared_ptr<Camera> getCamera() const;

  [[nodiscard]] BindingParameters getBindingParameters() const;

 private:
  std::shared_ptr<Camera> m_camera;
};

class CameraComponentBinder : public GameObjectsComponentBinder<CameraComponent> {
 public:
  explicit CameraComponentBinder(const ComponentBindingParameters& componentParameters);

  void bindToObject(GameObject& gameObject) override;

 private:
  ComponentBindingParameters m_bindingParameters;
};
