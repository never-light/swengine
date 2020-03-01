#include "precompiled.h"
#pragma hdrstop

#include "Skeleton.h"

BonePose::BonePose(const glm::vec3& position, const glm::quat& orientation)
    : position(position),
      orientation(orientation)
{

}

glm::mat4 BonePose::getBoneMatrix() const
{
    return glm::translate(glm::identity<glm::mat4>(), position) * glm::mat4_cast(orientation);
}

BonePose BonePose::interpolate(const BonePose& first, const BonePose& second, float factor)
{
    BonePose result;
    result.position = glm::mix(first.position, second.position, factor);
    result.orientation = glm::slerp(first.orientation, second.orientation, factor);

    return result;
}

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
