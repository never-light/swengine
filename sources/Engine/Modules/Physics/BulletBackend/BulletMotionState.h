#pragma once

#include <functional>
#include <bullet/btBulletDynamicsCommon.h>

#include "Modules/Graphics/GraphicsSystem/Transform.h"

class BulletMotionState : public btMotionState {
 public:
  BulletMotionState(const btTransform& transform);
  ~BulletMotionState() override;

  void getWorldTransform(btTransform& worldTrans) const override;
  void setWorldTransform(const btTransform& worldTrans) override;

  void setUpdateCallback(std::function<void(const btTransform&)> updateCallback);

 private:
  btTransform m_btTransform;
  std::shared_ptr<Transform> m_gameTransform;

  std::function<void(const btTransform&)> m_updateCallback;
};
