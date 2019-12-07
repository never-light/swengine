#pragma once

#include <SDL.h>
#include <string>
#include <memory>

#include "Modules/Graphics/GraphicsModule.h"
#include "Modules/ResourceManagement/ResourceManagementModule.h"
#include "Modules/Input/InputSystem.h"
#include "Modules/ECS/ECS.h"

#include "Modules/Graphics/GraphicsSystem/MeshRenderingSystem.h"
#include "Modules/Graphics/GraphicsSystem/TransformComponent.h"
#include "Modules/Graphics/GraphicsSystem/MeshRendererComponent.h"
#include "Modules/Graphics/GraphicsSystem/SharedGraphicsState.h"

class BaseGameApplication
{
public:
    BaseGameApplication(int argc, char* argv[], const std::string& windowTitle, int windowWidth, int windowHeight);
    virtual ~BaseGameApplication();

    virtual void load();
    virtual void unload();

    virtual void update(float delta);
    virtual void render();

    virtual int execute();

private:
    void performLoad();
    void performUnload();

    void performUpdate(float delta);
    void performRender();

protected:
    SDL_Window* m_mainWindow;

    std::shared_ptr<GraphicsModule> m_graphicsModule;
    std::shared_ptr<ResourceManagementModule> m_resourceManagementModule;
    std::shared_ptr<InputModule> m_inputModule;
    std::shared_ptr<InputSystem> m_inputSystem;

    std::shared_ptr<SharedGraphicsState> m_sharedGraphicsState;

    std::shared_ptr<GameWorld> m_gameWorld;

    std::shared_ptr<MeshRenderingSystem> m_meshRenderingSystem;
};

