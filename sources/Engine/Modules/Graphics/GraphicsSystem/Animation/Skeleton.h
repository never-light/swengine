#pragma once

#include <string>
#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Bone.h"

class Skeleton {
 public:
  explicit Skeleton(const std::vector<Bone>& bones);
  Skeleton(const Skeleton& skeleton) = default;

  ~Skeleton() = default;

  [[nodiscard]] const Bone& getRootBone() const;

  [[nodiscard]] uint8_t getBonesCount() const;
  [[nodiscard]] const Bone& getBone(uint8_t id) const;

  [[nodiscard]] uint8_t getBoneParentId(uint8_t id) const;

  [[nodiscard]] const BonePose& getRootBoneBindPose() const;

 private:
  std::vector<Bone> m_bones;
  BonePose m_rootBoneBindPose;
};

