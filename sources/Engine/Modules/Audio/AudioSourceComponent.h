#pragma once

#include <memory>
#include <glm/vec3.hpp>

#include "Modules/ResourceManagement/ResourcesManagement.h"
#include "Modules/ECS/GameObjectsFactory.h"
#include "AudioSource.h"

class AudioSourceComponentBindingParameters {
 public:
  std::string clipResourceName;
  bool cameraRelative{};
  bool isLooped{};

  float pitch = 1.0f;
  float volume = 1.0f;

  glm::vec3 position{};

  template<class Archive>
  void serialize(Archive& archive)
  {
    archive(
      cereal::make_nvp("clip_resource", clipResourceName),
      cereal::make_nvp("is_camera_relative", cameraRelative),
      cereal::make_nvp("is_looped", isLooped),
      cereal::make_nvp("pitch", pitch),
      cereal::make_nvp("volume", volume),
      cereal::make_nvp("position", position));
  }

};

class AudioSourceComponent {
 public:
  static constexpr bool s_isSerializable = true;
  using BindingParameters = AudioSourceComponentBindingParameters;

 public:
  explicit AudioSourceComponent(ResourceHandle<AudioClip> clip);
  ~AudioSourceComponent();

  [[nodiscard]] const AudioSource& getSource() const;
  [[nodiscard]] AudioSource& getSource();
  [[nodiscard]] AudioSource* getSourcePtr() const;

  [[nodiscard]] BindingParameters getBindingParameters() const;

 private:
  std::shared_ptr<AudioSource> m_source;
};

class AudioSourceComponentBinder : public GameObjectsComponentBinder<AudioSourceComponent> {
 public:
  explicit AudioSourceComponentBinder(const ComponentBindingParameters& componentParameters,
    std::shared_ptr<ResourcesManager> resourcesManager);

  void bindToObject(GameObject& gameObject) override;

 private:
  ComponentBindingParameters m_bindingParameters;
  std::shared_ptr<ResourcesManager> m_resourcesManager;
};
