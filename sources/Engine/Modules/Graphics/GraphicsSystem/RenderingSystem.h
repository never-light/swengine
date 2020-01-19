#pragma once

#include "Modules/ECS/GameSystem.h"
#include "Modules/Graphics/OpenGL/GLGraphicsContext.h"

class RenderingSystem : public GameSystem
{
public:
    RenderingSystem(std::shared_ptr<GLGraphicsContext> graphicsContext,
                    std::shared_ptr<SharedGraphicsState> sharedGraphicsState);

    virtual void renderDeferred(GameWorld* gameWorld);
    virtual void renderForward(GameWorld* gameWorld);
    virtual void renderPostProcess(GameWorld* gameWorld);

    void render(GameWorld *gameWorld) override final;

protected:
    std::shared_ptr<GLGraphicsContext> m_graphicsContext;
    std::shared_ptr<SharedGraphicsState> m_sharedGraphicsState;
};

