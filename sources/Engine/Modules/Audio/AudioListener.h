#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

class AudioListener {
 public:
  AudioListener();
  ~AudioListener();

  void setPosition(const glm::vec3& position);
  [[nodiscard]] glm::vec3 getPosition() const;

  void setVelocity(const glm::vec3& velocity);
  [[nodiscard]] glm::vec3 getVelocity() const;

  void setOrientation(const glm::quat& orientation);
  [[nodiscard]] glm::quat getOrientation() const;
};
