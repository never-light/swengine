#pragma once

#include "Modules/ECS/GameSystem.h"
#include "Modules/Graphics/OpenGL/GLGraphicsContext.h"

class GraphicsSystem : public GameSystem {
public:
    GraphicsSystem(std::shared_ptr<GLGraphicsContext> graphicsContext);
    ~GraphicsSystem() override;

    void configure(GameWorld *gameWorld) override;
    void unconfigure(GameWorld *gameWorld) override;

    void update(GameWorld *gameWorld, float delta) override;
    void render(GameWorld *gameWorld) override;

private:
    std::shared_ptr<GLGraphicsContext> m_graphicsContext;
};

