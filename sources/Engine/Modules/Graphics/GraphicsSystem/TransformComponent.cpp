#include "precompiled.h"

#pragma hdrstop

#include "TransformComponent.h"

TransformComponent::TransformComponent()
  : m_transform(std::make_shared<Transform>())
{

}

Transform& TransformComponent::getTransform()
{
  return *m_transform;
}

const Transform& TransformComponent::getTransform() const
{
  return *m_transform;
}

std::shared_ptr<Transform> TransformComponent::getTransformPtr() const
{
  return m_transform;
}

