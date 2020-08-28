#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include "Modules/Graphics/GraphicsSystem/Transform.h"

class BulletMotionState : public btMotionState {
 public:
  BulletMotionState(const btTransform& transform, std::shared_ptr<Transform> gameTransform);
  ~BulletMotionState() override;

  void getWorldTransform(btTransform& worldTrans) const override;
  void setWorldTransform(const btTransform& worldTrans) override;

 private:
  btTransform m_btTransform;
  std::shared_ptr<Transform> m_gameTransform;
};
