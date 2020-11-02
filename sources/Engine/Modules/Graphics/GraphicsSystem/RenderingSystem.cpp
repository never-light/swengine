#include "precompiled.h"

#pragma hdrstop

#include "RenderingSystem.h"

#include <utility>

RenderingSystem::RenderingSystem(std::shared_ptr<GLGraphicsContext> graphicsContext,
  std::shared_ptr<GraphicsScene> graphicsScene)
  : m_graphicsContext(std::move(graphicsContext)),
    m_graphicsScene(std::move(graphicsScene))
{

}

void RenderingSystem::render()
{
}
