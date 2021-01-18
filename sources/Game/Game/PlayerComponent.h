#pragma once

#include <Engine/Modules/Graphics/GraphicsSystem/Camera.h>
#include <Engine/Modules/Graphics/OpenGL/Mesh.h>
#include <Engine/Modules/ECS/GameObjectsFactory.h>

struct PlayerComponentBindingParameters {
  float height{};
  float walkSpeed{};

  template<class Archive>
  void serialize(Archive& archive)
  {
    archive(
      cereal::make_nvp("height", height),
      cereal::make_nvp("walk_speed", walkSpeed));
  };
};

class PlayerComponent {
 public:
  static constexpr bool s_isSerializable = true;
  using BindingParameters = PlayerComponentBindingParameters;

 public:
  explicit PlayerComponent(float playerHeight);

  void setMovementSpeed(float speed);
  [[nodiscard]] float getMovementSpeed() const;

  void setDistanceToPlayer(float distance);
  void increaseDistanceToPlayer(float delta);
  [[nodiscard]] float getDistanceToPlayer() const;

  void setThirdPersonViewPitch(float pitch);
  void increaseThirdPersonViewPitch(float delta);
  [[nodiscard]] float getThirdPersonViewPitch() const;

  void setThirdPersonViewYaw(float yaw);
  void increaseThirdPersonViewYaw(float delta);
  [[nodiscard]] float getThirdPersonViewYaw() const;

  [[nodiscard]] float getPlayerHeight() const;

  [[nodiscard]] BindingParameters getBindingParameters() const;

 private:
  float m_playerHeight = 0.0f;

  float m_movementSpeed = 2.15f;

  float m_distanceToPlayer = 0.4f;
  float m_thirdPersonViewPitch = 45.0f;
  float m_thirdPersonViewYaw = 0.0f;

  glm::vec3 m_currentSpeed{0.0f};
};

class PlayerComponentBinder : public GameObjectsComponentBinder<PlayerComponent> {
 public:
  explicit PlayerComponentBinder(const ComponentBindingParameters& componentParameters);

  void bindToObject(GameObject& gameObject) override;

 private:
  ComponentBindingParameters m_bindingParameters;
};
