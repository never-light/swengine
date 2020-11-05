#pragma once

#include <string>
#include <optional>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Modules/Math/MathUtils.h"

class TransformComponentBindingParameters {
 public:
  [[nodiscard]] const glm::vec3& getPosition() const;
  void setPosition(const glm::vec3& position);

  [[nodiscard]] const glm::vec3& getScale() const;
  void setScale(const glm::vec3& scale);

  [[nodiscard]] bool isStatic() const;
  void setIsStatic(bool isStatic);

  [[nodiscard]] const glm::vec3& getDirection() const;
  void setDirection(const glm::vec3& direction);

 private:
  glm::vec3 m_position{};
  glm::vec3 m_scale = { 1.0f, 1.0f, 1.0f };
  glm::vec3 m_direction = MathUtils::AXIS_X;

  bool m_isStatic = false;
};

class CameraComponentBindingParameters {
 public:
  [[nodiscard]] const glm::vec3& getPosition() const;
  void setPosition(const glm::vec3& position);

  [[nodiscard]] float getNearDistance() const;
  void setNearDistance(float nearDistance);

  [[nodiscard]] float getFarDistance() const;
  void setFarDistance(float farDistance);

  [[nodiscard]] float getFov() const;
  void setFov(float fov);

  [[nodiscard]] const glm::vec3& getLookAtPoint() const;
  void setLookAtPoint(const glm::vec3& lookAtPoint);

 private:
  glm::vec3 m_position{};
  glm::vec3 m_lookAtPoint{};

  float m_nearDistance = 0.1f;
  float m_farDistance = 100.0f;
  float m_fov = 60.0f;
};

class MeshRendererComponentBindingParameters {
 public:
  [[nodiscard]] const std::string& getMeshResourceName() const;
  void setMeshResourceName(const std::string& meshResourceName);

  [[nodiscard]] const std::string& getSkeletonResourceName() const;
  void setSkeletonResourceName(const std::string& skeletonResourceName);

  [[nodiscard]] const std::vector<std::pair<std::string, size_t>>& getMaterials() const;
  void setMaterials(const std::vector<std::pair<std::string, size_t>>& materials);

  void addSubMeshMaterial(const std::string& materialResourceName, size_t subMeshIndex);

 private:
  std::string m_meshResourceName;
  std::string m_skeletonResourceName;

  std::vector<std::pair<std::string, size_t>> m_materials;
};

class AnimationComponentBindingParameters {
 public:
  [[nodiscard]] const std::string& getSkeletonResourceName() const;
  void setSkeletonResourceName(const std::string& skeletonResourceName);

  [[nodiscard]] const std::string& getStateMachineResourceName() const;
  void setStateMachineResourceName(const std::string& stateMachineResourceName);

  [[nodiscard]] const std::string& getStateMachineInitialState() const;
  void setStateMachineInitialState(const std::string& stateMachineInitialState);

 private:
  std::string m_skeletonResourceName;
  std::string m_stateMachineResourceName;
  std::string m_stateMachineInitialState;
};

class KinematicCharacterComponentBindingParameters {
 public:
  [[nodiscard]] const glm::vec3& getOriginOffset() const;
  void setOriginOffset(const glm::vec3& originOffset);

  [[nodiscard]] float getCapsuleHeight() const;
  void setCapsuleHeight(float capsuleHeight);

  [[nodiscard]] const float& getCapsuleRadius() const;
  void setCapsuleRadius(const float& capsuleRadius);

 private:
  glm::vec3 m_originOffset{};
  float m_capsuleHeight{};
  float m_capsuleRadius{};
};

class RigidBodyComponentBindingParameters {
 public:
  [[nodiscard]] const std::string& getCollisionModelResourceName() const;
  void setCollisionModelResourceName(const std::string& collisionModelResourceName);

  [[nodiscard]] float getMass() const;
  void setMass(float mass);

 private:
  std::string m_collisionModelResourceName;
  float m_mass{};
};

class AudioSourceComponentBindingParameters {
 public:
  [[nodiscard]] const std::string& getClipResourceName() const;
  void setClipResourceName(const std::string& clipResourceName);

  [[nodiscard]] bool isCameraRelative() const;
  void setCameraRelative(bool cameraRelative);

  [[nodiscard]] bool isLooped() const;
  void setIsLooped(bool isLooped);

  [[nodiscard]] float getPitch() const;
  void setPitch(float pitch);

  [[nodiscard]] float getVolume() const;
  void setVolume(float volume);

  [[nodiscard]] const glm::vec3& getPosition() const;
  void setPosition(const glm::vec3& position);

 private:
  std::string m_clipResourceName;
  bool m_cameraRelative{};
  bool m_isLooped{};

  float m_pitch = 1.0f;
  float m_volume = 1.0f;

  glm::vec3 m_position{};
};

class EnvironmentComponentBindingParameters {
 public:
  [[nodiscard]] const std::string& getMaterialResourceName() const;
  void setMaterialResourceName(const std::string& materialResourceName);

 private:
  std::string m_materialResourceName;
};
