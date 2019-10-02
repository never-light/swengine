#pragma once

#include <SDL.h>
#include <string>
#include <memory>

#include "Modules/Graphics/GraphicsModule.h"
#include "Modules/ECS/ECS.h"

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

    std::unique_ptr<GraphicsModule> m_graphicsModule;
    std::unique_ptr<GameWorld> m_gameWorld;
};

