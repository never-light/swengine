#include "precompiled.h"

#pragma hdrstop

#include "GpuStateParameters.h"

void GpuStateParameters::setDepthTestMode(DepthTestMode mode)
{
  m_depthTestMode = mode;
}

DepthTestMode GpuStateParameters::getDepthTestMode() const
{
  return m_depthTestMode;
}

void GpuStateParameters::setFaceCullingMode(FaceCullingMode mode)
{
  m_faceCullingMode = mode;
}

FaceCullingMode GpuStateParameters::getFaceCullingMode() const
{
  return m_faceCullingMode;
}

void GpuStateParameters::setPolygonFillingMode(PolygonFillingMode mode)
{
  m_polygonFillingMode = mode;
}

PolygonFillingMode GpuStateParameters::getPolygonFillingMode() const
{
  return m_polygonFillingMode;
}

void GpuStateParameters::setBlendingMode(BlendingMode mode)
{
  m_materialBlendingMode = mode;
}

BlendingMode GpuStateParameters::getBlendingMode() const
{
  return m_materialBlendingMode;
}

void GpuStateParameters::setDepthWritingMode(DepthWritingMode mode)
{
  m_depthWritingMode = mode;
}

DepthWritingMode GpuStateParameters::getDepthWritingMode() const
{
  return m_depthWritingMode;
}

void GpuStateParameters::setScissorsTestMode(ScissorsTestMode mode)
{
  m_scissorsTestMode = mode;
}

ScissorsTestMode GpuStateParameters::getScissorsTestMode() const
{
  return m_scissorsTestMode;
}