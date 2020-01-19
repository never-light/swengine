#include "RenderingSystemsPipeline.h"
#include "SharedGraphicsState.h"
#include "DebugPainter.h"

RenderingSystemsPipeline::RenderingSystemsPipeline(std::shared_ptr<GameWorld> gameWorld,
                                                   std::shared_ptr<GLGraphicsContext> graphicsContext,
                                                   std::shared_ptr<SharedGraphicsState> sharedGraphicsState)
    : GameSystemsGroup(gameWorld),
      m_graphicsContext(graphicsContext),
      m_sharedGraphicsState(sharedGraphicsState)
{

}

void RenderingSystemsPipeline::addGameSystem(std::shared_ptr<GameSystem> system)
{
    SW_ASSERT(dynamic_cast<RenderingSystem*>(system.get()) != nullptr);

    GameSystemsGroup::addGameSystem(system);
}

void RenderingSystemsPipeline::render(GameWorld* gameWorld)
{
    m_sharedGraphicsState->getDeferredFramebuffer().clearColor({ 0.0f, 0.0f, 0.0f, 0.0f }, 0);
    m_sharedGraphicsState->getDeferredFramebuffer().clearColor({ 0.0f, 0.0f, 0.0f, 0.0f }, 1);
    m_sharedGraphicsState->getDeferredFramebuffer().clearColor({ 0.0f, 0.0f, 0.0f, 0.0f }, 2);

    m_sharedGraphicsState->getDeferredFramebuffer().clearDepthStencil(1.0f, 0);

    for (auto& system : getGameSystems()) {
        RenderingSystem* renderingSystem = dynamic_cast<RenderingSystem*>(system.get());
        renderingSystem->renderDeferred(gameWorld);
    }

    m_sharedGraphicsState->getDeferredFramebuffer().copyColor(m_sharedGraphicsState->getForwardFramebuffer(), 0, 0);

    for (auto& system : getGameSystems()) {
        RenderingSystem* renderingSystem = dynamic_cast<RenderingSystem*>(system.get());
        renderingSystem->renderForward(gameWorld);
    }

    for (auto& system : getGameSystems()) {
        RenderingSystem* renderingSystem = dynamic_cast<RenderingSystem*>(system.get());
        renderingSystem->renderPostProcess(gameWorld);
    }

    m_sharedGraphicsState->getForwardFramebuffer().copyColor(m_graphicsContext->getDefaultFramebuffer());
    m_sharedGraphicsState->getForwardFramebuffer().copyDepthStencil(m_graphicsContext->getDefaultFramebuffer());

    DebugPainter::flushRenderQueue(m_graphicsContext.get());
}
