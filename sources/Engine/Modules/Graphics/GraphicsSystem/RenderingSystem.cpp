#include "precompiled.h"

#pragma hdrstop

#include "RenderingSystem.h"

RenderingSystem::RenderingSystem(std::shared_ptr<GLGraphicsContext> graphicsContext,
  std::shared_ptr<SharedGraphicsState> sharedGraphicsState)
  : m_graphicsContext(graphicsContext),
    m_sharedGraphicsState(sharedGraphicsState)
{

}

void RenderingSystem::renderDeferred(GameWorld* gameWorld)
{
  ARG_UNUSED(gameWorld);
}

void RenderingSystem::renderForward(GameWorld* gameWorld)
{
  ARG_UNUSED(gameWorld);
}

void RenderingSystem::renderPostProcess(GameWorld* gameWorld)
{
  ARG_UNUSED(gameWorld);
}

void RenderingSystem::render(GameWorld* gameWorld)
{
  ARG_UNUSED(gameWorld);

  /*
   * Direct usage of render method for RenderingSystem is forbidden
   *
   * Every rendering system must use one or more of the specific rendering stages
   * (e.g. deferred, forward, postprocessing)
   */
  SW_ASSERT(false);
}
