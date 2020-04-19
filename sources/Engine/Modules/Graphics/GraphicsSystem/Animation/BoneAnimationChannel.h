#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

struct BoneAnimationPositionFrame {
  float time;
  glm::vec3 position;
};

struct BoneAnimationOrientationFrame {
  float time;
  glm::quat orientation;
};

class BoneAnimationChannel {
 public:
  BoneAnimationChannel() = default;
  BoneAnimationChannel(const std::vector<BoneAnimationPositionFrame>& positionFrames,
    const std::vector<BoneAnimationOrientationFrame>& orientationFrames);

  ~BoneAnimationChannel() = default;

  [[nodiscard]] std::vector<BoneAnimationPositionFrame>& getPositionFrames();
  [[nodiscard]] const std::vector<BoneAnimationPositionFrame>& getPositionFrames() const;

  [[nodiscard]] const std::vector<BoneAnimationOrientationFrame>& getOrientationFrames() const;
  [[nodiscard]] std::vector<BoneAnimationOrientationFrame>& getOrientationFrames();

 private:
  std::vector<BoneAnimationPositionFrame> m_positionFrames;
  std::vector<BoneAnimationOrientationFrame> m_orientationFrames;
};
