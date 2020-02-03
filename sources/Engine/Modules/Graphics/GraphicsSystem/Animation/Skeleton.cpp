#include "precompiled.h"
#pragma hdrstop

#include "Skeleton.h"

Bone::Bone()
{

}

Bone::Bone(const std::string& name,
           uint8_t parentId,
           const glm::mat4& inverseBindPoseMatrix)
    : m_name(name),
      m_parentId(parentId),
      m_inverseBindPoseMatrix(inverseBindPoseMatrix)
{

}

void Bone::setName(const std::string& name)
{
    m_name = name;
}

const std::string& Bone::getName() const
{
    return m_name;
}

void Bone::setParentId(const uint8_t parentId)
{
    m_parentId = parentId;
}

uint8_t Bone::getParentId() const
{
    return m_parentId;
}

void Bone::setInverseBindPoseMatrix(const glm::mat4& inverseBindPoseMatrix)
{
    m_inverseBindPoseMatrix = inverseBindPoseMatrix;
}

const glm::mat4& Bone::getInverseBindPoseMatrix() const
{
    return m_inverseBindPoseMatrix;
}

Skeleton::Skeleton(const std::vector<Bone>& bones)
    : m_bones(bones)
{
    SW_ASSERT(bones.size() > 0);
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
