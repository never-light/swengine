#include "precompiled.h"

#pragma hdrstop

#include "Bone.h"

#include <utility>

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

Bone::Bone() = default;

Bone::Bone(std::string name,
  uint8_t parentId,
  const glm::mat4& inverseBindPoseMatrix)
  : m_name(std::move(name)),
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