#pragma once

#include <memory>
#include <glm/vec3.hpp>

#include "Modules/ResourceManagement/ResourceManager.h"
#include "AudioSource.h"

class AudioSourceComponent {
 public:
  explicit AudioSourceComponent(ResourceHandle<AudioClip> clip);
  ~AudioSourceComponent();

  [[nodiscard]] const AudioSource& getSource() const;
  [[nodiscard]] AudioSource& getSource();
  [[nodiscard]] AudioSource* getSourcePtr() const;

 private:
  std::shared_ptr<AudioSource> m_source;
};
