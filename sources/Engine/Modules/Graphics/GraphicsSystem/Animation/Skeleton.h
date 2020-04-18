#pragma once

#include <string>
#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

struct BonePose {
 public:
  BonePose() = default;
  BonePose(const glm::vec3& position, const glm::quat& orientation);

  [[nodiscard]] glm::mat4 getBoneMatrix() const;

 public:
  [[nodiscard]] static BonePose interpolate(const BonePose& first, const BonePose& second, float factor);

 public:
  glm::vec3 position = glm::vec3(0.0f);
  glm::quat orientation = glm::identity<glm::quat>();
};

inline BonePose operator*(const BonePose& a, const BonePose& b)
{
  BonePose result;
  result.orientation = a.orientation * b.orientation;
  result.position = a.position + glm::vec3(a.orientation * glm::vec4(b.position, 1.0f));

  return result;
}

struct Bone {
 public:
  Bone();
  Bone(const std::string& name, uint8_t parentId, const glm::mat4& inverseBindPoseMatrix);
  ~Bone() = default;

  void setName(const std::string& name);
  [[nodiscard]] const std::string& getName() const;

  void setParentId(const uint8_t parentId);
  [[nodiscard]] uint8_t getParentId() const;

  void setInverseBindPoseMatrix(const glm::mat4& inverseBindPoseMatrix);
  [[nodiscard]] const glm::mat4& getInverseBindPoseMatrix() const;

 public:
  static constexpr uint8_t ROOT_BONE_PARENT_ID = 255;

 private:
  std::string m_name;

  uint8_t m_parentId;
  glm::mat4 m_inverseBindPoseMatrix;
};

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

