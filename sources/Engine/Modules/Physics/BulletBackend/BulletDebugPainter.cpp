#include "precompiled.h"

#pragma hdrstop

#include "BulletDebugPainter.h"

#include <utility>
#include "Modules/Graphics/GraphicsSystem/DebugPainter.h"
#include "BulletUtils.h"

BulletDebugPainter::BulletDebugPainter()
{

}

BulletDebugPainter::~BulletDebugPainter()
{

}

void BulletDebugPainter::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
  DebugPainter::renderSegment(BulletUtils::btVec3ToGlm(from),
    BulletUtils::btVec3ToGlm(to),
    glm::vec4(BulletUtils::btVec3ToGlm(color), 0.0f));
}

void BulletDebugPainter::drawTriangle(const btVector3& v0,
  const btVector3& v1,
  const btVector3& v2,
  const btVector3& color,
  btScalar /*alpha*/)
{
  DebugPainter::renderTriangle(BulletUtils::btVec3ToGlm(v0),
    BulletUtils::btVec3ToGlm(v1),
    BulletUtils::btVec3ToGlm(v2),
    glm::vec4(BulletUtils::btVec3ToGlm(color), 0.0f));
}

void BulletDebugPainter::drawContactPoint(const btVector3& PointOnB,
  const btVector3& normalOnB,
  btScalar distance,
  int lifeTime,
  const btVector3& color)
{
  ARG_UNUSED(PointOnB);
  ARG_UNUSED(normalOnB);
  ARG_UNUSED(distance);
  ARG_UNUSED(lifeTime);
  ARG_UNUSED(color);
}

void BulletDebugPainter::reportErrorWarning(const char* warningString)
{
  spdlog::warn(warningString);
}

void BulletDebugPainter::draw3dText(const btVector3& location, const char* textString)
{
  ARG_UNUSED(location);
  ARG_UNUSED(textString);
}

void BulletDebugPainter::setDebugMode(int debugMode)
{
  ARG_UNUSED(debugMode);
}

int BulletDebugPainter::getDebugMode() const
{
  return DBG_DrawWireframe;
}

