#include "precompiled.h"

#pragma hdrstop

#include "Skeleton.h"

Skeleton::Skeleton(const std::vector<Bone>& bones)
  : m_bones(bones)
{
  SW_ASSERT(!bones.empty());
}

const Bone& Skeleton::getRootBone() const
{
  return getBone(0);
}

uint8_t Skeleton::getBoneParentId(uint8_t id) const
{
  return m_bones[id].getParentId();
}

uint8_t Skeleton::getBonesCount() const
{
  return static_cast<uint8_t>(m_bones.size());
}

const Bone& Skeleton::getBone(uint8_t id) const
{
  return m_bones[id];
}
