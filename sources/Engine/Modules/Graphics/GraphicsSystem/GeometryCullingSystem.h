#pragma once

#include "Modules/ECS/GameSystem.h"
#include "Modules/Graphics/OpenGL/GLGraphicsContext.h"
#include "SharedGraphicsState.h"

class GeometryCullingSystem : public GameSystem {
public:
    GeometryCullingSystem(std::shared_ptr<GLGraphicsContext> graphicsContext,
                        std::shared_ptr<SharedGraphicsState> sharedGraphicsState);

    ~GeometryCullingSystem() override;

    void configure(GameWorld *gameWorld) override;
    void unconfigure(GameWorld *gameWorld) override;

    void update(GameWorld *gameWorld, float delta) override;

    void beforeRender(GameWorld *gameWorld) override;
    void afterRender(GameWorld *gameWorld) override;

private:
    std::shared_ptr<GLGraphicsContext> m_graphicsContext;
    std::shared_ptr<SharedGraphicsState> m_sharedGraphicsState;
};
