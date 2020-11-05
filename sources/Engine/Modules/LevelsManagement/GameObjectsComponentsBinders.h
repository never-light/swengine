#pragma once

#include "Modules/ResourceManagement/ResourcesManager.h"

#include "GameObjectsComponentBinder.h"
#include "GameObjectsBindingParameters.h"

template<class ParametersType>
class GameObjectsComponentParametricBinder : public GameObjectsComponentBinder {
 public:
  GameObjectsComponentParametricBinder() = default;

  explicit GameObjectsComponentParametricBinder(const ParametersType& componentParameters)
    : m_componentParameters(componentParameters)
  {

  }

  ~GameObjectsComponentParametricBinder() override = default;

  [[nodiscard]] inline const ParametersType& getComponentParameters() const
  {
    return m_componentParameters;
  }

  [[nodiscard]] inline ParametersType& getComponentParameters()
  {
    return m_componentParameters;
  }

  inline void setComponentParameters(ParametersType componentParameters)
  {
    m_componentParameters = componentParameters;
  }

 private:
  ParametersType m_componentParameters;
};

class TransformComponentBinder : public GameObjectsComponentParametricBinder<TransformComponentBindingParameters> {
 public:
  explicit TransformComponentBinder(const TransformComponentBindingParameters& componentParameters);
  TransformComponentBinder() = default;
  ~TransformComponentBinder() override = default;

  void bindToObject(GameObject& gameObject) override;
};

class CameraComponentBinder : public GameObjectsComponentParametricBinder<CameraComponentBindingParameters> {
 public:
  explicit CameraComponentBinder(const CameraComponentBindingParameters& componentParameters);
  CameraComponentBinder() = default;
  ~CameraComponentBinder() override = default;

  void bindToObject(GameObject& gameObject) override;
};

class MeshRendererComponentBinder : public GameObjectsComponentParametricBinder<MeshRendererComponentBindingParameters> {
 public:
  MeshRendererComponentBinder(const MeshRendererComponentBindingParameters& componentParameters,
    std::shared_ptr<ResourcesManager> resourcesManager);
  MeshRendererComponentBinder() = default;
  ~MeshRendererComponentBinder() override = default;

  void bindToObject(GameObject& gameObject) override;

 private:
  std::shared_ptr<ResourcesManager> m_resourcesManager;
};

class AnimationComponentBinder : public GameObjectsComponentParametricBinder<AnimationComponentBindingParameters> {
 public:
  AnimationComponentBinder(const AnimationComponentBindingParameters& componentParameters,
    std::shared_ptr<ResourcesManager> resourcesManager);
  AnimationComponentBinder() = default;
  ~AnimationComponentBinder() override = default;

  void bindToObject(GameObject& gameObject) override;

 private:
  std::shared_ptr<ResourcesManager> m_resourcesManager;
};


class KinematicCharacterComponentBinder : public GameObjectsComponentParametricBinder<KinematicCharacterComponentBindingParameters> {
 public:
  explicit KinematicCharacterComponentBinder(const KinematicCharacterComponentBindingParameters& componentParameters,
    std::shared_ptr<ResourcesManager> resourcesManager);
  KinematicCharacterComponentBinder() = default;
  ~KinematicCharacterComponentBinder() override = default;

  void bindToObject(GameObject& gameObject) override;

 private:
  std::shared_ptr<ResourcesManager> m_resourcesManager;
};

class RigidBodyComponentBinder : public GameObjectsComponentParametricBinder<RigidBodyComponentBindingParameters> {
 public:
  RigidBodyComponentBinder(const RigidBodyComponentBindingParameters& componentParameters,
    std::shared_ptr<ResourcesManager> resourcesManager);
  RigidBodyComponentBinder() = default;
  ~RigidBodyComponentBinder() override = default;

  void bindToObject(GameObject& gameObject) override;

 private:
  std::shared_ptr<ResourcesManager> m_resourcesManager;
};

class AudioSourceComponentBinder : public GameObjectsComponentParametricBinder<AudioSourceComponentBindingParameters> {
 public:
  AudioSourceComponentBinder(const AudioSourceComponentBindingParameters& componentParameters,
    std::shared_ptr<ResourcesManager> resourcesManager);
  AudioSourceComponentBinder() = default;
  ~AudioSourceComponentBinder() override = default;

  void bindToObject(GameObject& gameObject) override;

 private:
  std::shared_ptr<ResourcesManager> m_resourcesManager;
};

class EnvironmentComponentBinder : public GameObjectsComponentParametricBinder<EnvironmentComponentBindingParameters> {
 public:
  EnvironmentComponentBinder(const EnvironmentComponentBindingParameters& componentParameters,
    std::shared_ptr<ResourcesManager> resourcesManager);
  EnvironmentComponentBinder() = default;
  ~EnvironmentComponentBinder() override = default;

  void bindToObject(GameObject& gameObject) override;

 private:
  std::shared_ptr<ResourcesManager> m_resourcesManager;
};
