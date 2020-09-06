#include "precompiled.h"

#pragma hdrstop

#include "BulletMotionState.h"

#include <utility>
#include "Modules/Graphics/GraphicsSystem/TransformComponent.h"

BulletMotionState::BulletMotionState(const btTransform& transform)
  : m_btTransform(transform)
{

}

BulletMotionState::~BulletMotionState() = default;

void BulletMotionState::getWorldTransform(btTransform& worldTrans) const
{
  worldTrans = m_btTransform;
}

void BulletMotionState::setWorldTransform(const btTransform& worldTrans)
{
  if (m_updateCallback != nullptr) {
    m_updateCallback(worldTrans);
  }

  m_btTransform = worldTrans;
}

void BulletMotionState::setUpdateCallback(std::function<void(const btTransform&)> updateCallback)
{
  m_updateCallback = std::move(updateCallback);
}
