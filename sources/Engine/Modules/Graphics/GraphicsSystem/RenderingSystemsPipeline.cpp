#include "RenderingSystemsPipeline.h"

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
    for (auto& system : getGameSystems()) {
        RenderingSystem* renderingSystem = dynamic_cast<RenderingSystem*>(system.get());
        renderingSystem->renderDeferred(gameWorld);
    }

    for (auto& system : getGameSystems()) {
        RenderingSystem* renderingSystem = dynamic_cast<RenderingSystem*>(system.get());
        renderingSystem->renderForward(gameWorld);
    }

    for (auto& system : getGameSystems()) {
        RenderingSystem* renderingSystem = dynamic_cast<RenderingSystem*>(system.get());
        renderingSystem->renderPostProcess(gameWorld);
    }
}
