#pragma once

#include <Engine/Modules/Application/BaseGameApplication.h>
#include <Engine/Modules/Graphics/OpenGL/GLGeometryStore.h>
#include <Engine/Modules/Graphics/OpenGL/GLShadersPipeline.h>
#include <Engine/Modules/Graphics/GraphicsSystem/Mesh.h>

#include "Game/Game/PlayerControlSystem.h"

class GameApplication : public BaseGameApplication
{
public:
    GameApplication(int argc, char* argv[]);
    ~GameApplication() override;

    void render() override;
    void load() override;

private:
    std::shared_ptr<PlayerControlSystem> m_playerControlSystem;

    std::shared_ptr<Mesh> m_triangleGeometry;
    std::shared_ptr<GLShadersPipeline> m_triangleShadersPipeline;
    std::shared_ptr<Material> m_material;
};

