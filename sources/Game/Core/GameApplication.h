#pragma once

#include <Engine/Modules/Application/BaseGameApplication.h>
#include <Engine/Modules/Graphics/OpenGL/GLGeometryStore.h>
#include <Engine/Modules/Graphics/OpenGL/GLShadersPipeline.h>
#include <Engine/Modules/Graphics/GraphicsSystem/Mesh.h>

class GameApplication : public BaseGameApplication
{
public:
    GameApplication(int argc, char* argv[]);
    ~GameApplication() override;

    void render() override;
    void load() override;
};

