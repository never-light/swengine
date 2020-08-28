#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include "Modules/Graphics/GraphicsSystem/Transform.h"

class BulletDebugPainter : public btIDebugDraw {
 public:
  BulletDebugPainter();
  ~BulletDebugPainter() override;

  void drawTriangle(const btVector3& v0,
    const btVector3& v1,
    const btVector3& v2,
    const btVector3& color,
    btScalar /*alpha*/) override;

  void drawLine(const btVector3& from,
    const btVector3& to,
    const btVector3& color
  ) override;

  void drawContactPoint(const btVector3& PointOnB,
    const btVector3& normalOnB,
    btScalar distance,
    int lifeTime,
    const btVector3& color) override;
  void reportErrorWarning(const char* warningString) override;

  void draw3dText(const btVector3& location, const char* textString) override;
  void setDebugMode(int debugMode) override;
  int getDebugMode() const override;

};
