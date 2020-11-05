#include "precompiled.h"

#pragma hdrstop

#include "GameObjectsBindingParameters.h"

const glm::vec3& TransformComponentBindingParameters::getPosition() const
{
  return m_position;
}

void TransformComponentBindingParameters::setPosition(const glm::vec3& position)
{
  m_position = position;
}

const glm::vec3& TransformComponentBindingParameters::getScale() const
{
  return m_scale;
}

void TransformComponentBindingParameters::setScale(const glm::vec3& scale)
{
  m_scale = scale;
}

bool TransformComponentBindingParameters::isStatic() const
{
  return m_isStatic;
}

void TransformComponentBindingParameters::setIsStatic(bool isStatic)
{
  m_isStatic = isStatic;
}

const glm::vec3& TransformComponentBindingParameters::getDirection() const
{
  return m_direction;
}

void TransformComponentBindingParameters::setDirection(const glm::vec3& direction)
{
  m_direction = direction;
}

const glm::vec3& CameraComponentBindingParameters::getPosition() const
{
  return m_position;
}

void CameraComponentBindingParameters::setPosition(const glm::vec3& position)
{
  m_position = position;
}

float CameraComponentBindingParameters::getNearDistance() const
{
  return m_nearDistance;
}

void CameraComponentBindingParameters::setNearDistance(float nearDistance)
{
  m_nearDistance = nearDistance;
}

float CameraComponentBindingParameters::getFarDistance() const
{
  return m_farDistance;
}

void CameraComponentBindingParameters::setFarDistance(float farDistance)
{
  m_farDistance = farDistance;
}

float CameraComponentBindingParameters::getFov() const
{
  return m_fov;
}

void CameraComponentBindingParameters::setFov(float fov)
{
  m_fov = fov;
}

const glm::vec3& CameraComponentBindingParameters::getLookAtPoint() const
{
  return m_lookAtPoint;
}

void CameraComponentBindingParameters::setLookAtPoint(const glm::vec3& lookAtPoint)
{
  m_lookAtPoint = lookAtPoint;
}

const std::string& MeshRendererComponentBindingParameters::getMeshResourceName() const
{
  return m_meshResourceName;
}

void MeshRendererComponentBindingParameters::setMeshResourceName(const std::string& meshResourceName)
{
  m_meshResourceName = meshResourceName;
}

const std::string& MeshRendererComponentBindingParameters::getSkeletonResourceName() const
{
  return m_skeletonResourceName;
}

void MeshRendererComponentBindingParameters::setSkeletonResourceName(const std::string& skeletonResourceName)
{
  m_skeletonResourceName = skeletonResourceName;
}

const std::vector<std::pair<std::string, size_t>>& MeshRendererComponentBindingParameters::getMaterials() const
{
  return m_materials;
}

void MeshRendererComponentBindingParameters::setMaterials(const std::vector<std::pair<std::string, size_t>>& materials)
{
  m_materials = materials;
}

void MeshRendererComponentBindingParameters::addSubMeshMaterial(const std::string& materialResourceName,
  size_t subMeshIndex)
{
  m_materials.emplace_back( materialResourceName, subMeshIndex );
}

const std::string& AnimationComponentBindingParameters::getSkeletonResourceName() const
{
  return m_skeletonResourceName;
}

void AnimationComponentBindingParameters::setSkeletonResourceName(const std::string& skeletonResourceName)
{
  m_skeletonResourceName = skeletonResourceName;
}

const std::string& AnimationComponentBindingParameters::getStateMachineResourceName() const
{
  return m_stateMachineResourceName;
}

void AnimationComponentBindingParameters::setStateMachineResourceName(const std::string& stateMachineResourceName)
{
  m_stateMachineResourceName = stateMachineResourceName;
}

const std::string& AnimationComponentBindingParameters::getStateMachineInitialState() const
{
  return m_stateMachineInitialState;
}

void AnimationComponentBindingParameters::setStateMachineInitialState(const std::string& stateMachineInitialState)
{
  m_stateMachineInitialState = stateMachineInitialState;
}

const glm::vec3& KinematicCharacterComponentBindingParameters::getOriginOffset() const
{
  return m_originOffset;
}

void KinematicCharacterComponentBindingParameters::setOriginOffset(const glm::vec3& originOffset)
{
  m_originOffset = originOffset;
}

float KinematicCharacterComponentBindingParameters::getCapsuleHeight() const
{
  return m_capsuleHeight;
}

void KinematicCharacterComponentBindingParameters::setCapsuleHeight(float capsuleHeight)
{
  m_capsuleHeight = capsuleHeight;
}

const float& KinematicCharacterComponentBindingParameters::getCapsuleRadius() const
{
  return m_capsuleRadius;
}

void KinematicCharacterComponentBindingParameters::setCapsuleRadius(const float& capsuleRadius)
{
  m_capsuleRadius = capsuleRadius;
}

const std::string& AudioSourceComponentBindingParameters::getClipResourceName() const
{
  return m_clipResourceName;
}

void AudioSourceComponentBindingParameters::setClipResourceName(const std::string& clipResourceName)
{
  m_clipResourceName = clipResourceName;
}

bool AudioSourceComponentBindingParameters::isCameraRelative() const
{
  return m_cameraRelative;
}

void AudioSourceComponentBindingParameters::setCameraRelative(bool cameraRelative)
{
  m_cameraRelative = cameraRelative;
}

bool AudioSourceComponentBindingParameters::isLooped() const
{
  return m_isLooped;
}

void AudioSourceComponentBindingParameters::setIsLooped(bool isLooped)
{
  m_isLooped = isLooped;
}

float AudioSourceComponentBindingParameters::getPitch() const
{
  return m_pitch;
}

void AudioSourceComponentBindingParameters::setPitch(float pitch)
{
  m_pitch = pitch;
}

float AudioSourceComponentBindingParameters::getVolume() const
{
  return m_volume;
}

void AudioSourceComponentBindingParameters::setVolume(float volume)
{
  m_volume = volume;
}

const glm::vec3& AudioSourceComponentBindingParameters::getPosition() const
{
  return m_position;
}

void AudioSourceComponentBindingParameters::setPosition(const glm::vec3& position)
{
  m_position = position;
}

const std::string& EnvironmentComponentBindingParameters::getMaterialResourceName() const
{
  return m_materialResourceName;
}

void EnvironmentComponentBindingParameters::setMaterialResourceName(const std::string& materialResourceName)
{
  m_materialResourceName = materialResourceName;
}
