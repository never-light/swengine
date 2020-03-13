#pragma once

#include <Engine/Modules/Application/BaseGameApplication.h>
#include <Engine/Modules/Graphics/OpenGL/GLGeometryStore.h>
#include <Engine/Modules/Graphics/OpenGL/GLShadersPipeline.h>
#include <Engine/Modules/Graphics/GraphicsSystem/Mesh.h>

class DemoApplication : public BaseGameApplication
{
public:
    DemoApplication(int argc, char* argv[]);
    ~DemoApplication() override;

    void render() override;
    void load() override;
};

