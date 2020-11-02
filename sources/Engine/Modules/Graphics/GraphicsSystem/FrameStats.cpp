#include "precompiled.h"

#pragma hdrstop

#include "FrameStats.h"

void FrameStats::reset()
{
  m_primitivesCount = 0;
  m_subMeshesCount = 0;
  m_culledSubMeshesCount = 0;
}

void FrameStats::increasePrimitivesCount(size_t count)
{
  m_primitivesCount += count;
}

void FrameStats::increaseSubMeshesCount(size_t count)
{
  m_subMeshesCount += count;
}

void FrameStats::increaseCulledSubMeshesCount(size_t count)
{
  m_culledSubMeshesCount += count;
}

size_t FrameStats::getPrimitivesCount() const
{
  return m_primitivesCount;
}

size_t FrameStats::getSubMeshesCount() const
{
  return m_subMeshesCount;
}

size_t FrameStats::getCulledSubMeshesCount() const
{
  return m_culledSubMeshesCount;
}
