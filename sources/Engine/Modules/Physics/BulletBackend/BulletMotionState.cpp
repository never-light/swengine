#include "precompiled.h"

#pragma hdrstop

#include "BulletMotionState.h"

#include <utility>
#include "Modules/Graphics/GraphicsSystem/TransformComponent.h"

BulletMotionState::BulletMotionState(const btTransform& transform, std::shared_ptr<Transform> gameTransform)
  : m_btTransform(transform),
  m_gameTransform(std::move(gameTransform))
{

}

BulletMotionState::~BulletMotionState()
{

}

void BulletMotionState::getWorldTransform(btTransform& worldTrans) const
{
  worldTrans = m_btTransform;
}

void BulletMotionState::setWorldTransform(const btTransform& worldTrans)
{
  btQuaternion orientation = worldTrans.getRotation();
  btVector3 origin = worldTrans.getOrigin();

  m_gameTransform->setOrientation(glm::quat(orientation.w(), orientation.x(), orientation.y(), orientation.z()));
  m_gameTransform->setPosition(origin.x(), origin.y(), origin.z());

  m_btTransform = worldTrans;
}
