#include "precompiled.h"

#pragma hdrstop

#include "Skeleton.h"

Skeleton::Skeleton(const std::vector<Bone>& bones)
  : m_bones(bones)
{
  SW_ASSERT(bones.size() > 0);

  glm::mat4 bindPoseMatrix = glm::inverse(bones[0].getInverseBindPoseMatrix());

  m_rootBoneBindPose.orientation = glm::quat_cast(bindPoseMatrix);
  m_rootBoneBindPose.position = glm::vec3(bindPoseMatrix[3]);
}

const Bone& Skeleton::getRootBone() const
{
  return getBone(0);
}

uint8_t Skeleton::getBoneParentId(uint8_t id) const
{
  return m_bones[id].getParentId();
}

const BonePose& Skeleton::getRootBoneBindPose() const
{
  return m_rootBoneBindPose;
}

uint8_t Skeleton::getBonesCount() const
{
  return static_cast<uint8_t>(m_bones.size());
}

const Bone& Skeleton::getBone(uint8_t id) const
{
  return m_bones[id];
}
