#pragma once

#include <Engine/Modules/Application/BaseGameApplication.h>
#include <Engine/Modules/Graphics/OpenGL/GLGeometryStore.h>
#include <Engine/Modules/Graphics/OpenGL/GLShadersPipeline.h>

class GameApplication : public BaseGameApplication
{
public:
    GameApplication(int argc, char* argv[]);
    ~GameApplication() override;

    void render() override;
    void load() override;

private:
    GLGeometryStore* m_triangleGeometry;
    GLShadersPipeline* m_triangleShadersPipeline;
};

