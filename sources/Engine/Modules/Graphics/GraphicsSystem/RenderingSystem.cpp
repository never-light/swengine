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

void RenderingSystem::renderDeferred()
{
}

void RenderingSystem::renderForward()
{
}

void RenderingSystem::renderPostProcess()
{
}

void RenderingSystem::render()
{
  /*
   * Direct usage of render method for RenderingSystem is forbidden
   *
   * Every rendering system should use one or more of the specific rendering stages
   * (e.g. deferred, forward, postprocessing)
   */
  SW_ASSERT(false);
}
