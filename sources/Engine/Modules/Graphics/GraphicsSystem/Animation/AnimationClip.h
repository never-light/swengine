#pragma once

#include <memory>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Modules/ResourceManagement/ResourceManager.h"
#include "Skeleton.h"
#include "BoneAnimationChannel.h"
#include "Bone.h"

struct AnimationMatrixPalette {
  explicit AnimationMatrixPalette(const std::vector<glm::mat4>& bonesTransforms);

  std::vector<glm::mat4> bonesTransforms;
};

class AnimationClip : public Resource {
 public:
  AnimationClip(const std::string& name,
    float duration,
    float rate,
    const std::vector<BoneAnimationChannel>& bonesAnimationChannels);

  ~AnimationClip() override = default;

  [[nodiscard]] const std::string& getName() const;

  void setDuration(float duration);
  [[nodiscard]] float getDuration() const;

  [[nodiscard]] float getDurationInSeconds() const;

  void setRate(float rate);
  [[nodiscard]] float getRate() const;

  [[nodiscard]] BonePose getBoneRelativePose(uint8_t boneIndex, float time) const;

 private:
  template<class T, class S>
  T getMixedAdjacentFrames(const std::vector<S>& frames, float time) const;

  template<class T>
  T getIdentity() const;

  template<class T, class S>
  T getFrameValue(const S& frame) const;

  template<class T>
  T getMixedValue(const T& first, const T& second, float delta) const;

 private:
  std::string m_name;
  std::vector<BoneAnimationChannel> m_bonesAnimationChannels;

  /**
   * @brief Animation clip duration in frames
   */
  float m_duration = 0.0f;

  /**
   * @brief Animation clip frame rate
   */
  float m_rate = 0.0f;
};

