#pragma once

#include "Modules/ECS/GameSystem.h"
#include "Modules/Graphics/OpenGL/GLGraphicsContext.h"

#include "Material.h"
#include "Mesh.h"
#include "SharedGraphicsState.h"

class EnvironmentComponent
{
public:
    EnvironmentComponent();

    void setEnvironmentMaterial(const std::shared_ptr<Material> material);
    Material* getEnvironmentMaterial() const;

private:
    std::shared_ptr<Material> m_environmentMaterial;
};


class EnvironmentRenderingSystem : public GameSystem {
public:
    EnvironmentRenderingSystem(std::shared_ptr<GLGraphicsContext> graphicsContext,
                        std::shared_ptr<SharedGraphicsState> sharedGraphicsState,
                        std::shared_ptr<Mesh> identityBox);

    ~EnvironmentRenderingSystem() override;

    void configure(GameWorld *gameWorld) override;
    void unconfigure(GameWorld *gameWorld) override;

    void update(GameWorld *gameWorld, float delta) override;
    void render(GameWorld *gameWorld) override;

private:
    std::shared_ptr<GLGraphicsContext> m_graphicsContext;
    std::shared_ptr<SharedGraphicsState> m_sharedGraphicsState;

    std::shared_ptr<Mesh> m_identityBox;
};


