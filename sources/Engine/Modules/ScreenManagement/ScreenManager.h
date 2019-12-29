#pragma once

#include <unordered_map>
#include <string>

#include "Screen.h"

class ScreenManager : public std::enable_shared_from_this<ScreenManager>
{
public:
    ScreenManager(std::shared_ptr<GameWorld> gameWorld,
                  std::shared_ptr<GraphicsModule> graphicsModule,
                  std::shared_ptr<SharedGraphicsState> sharedGraphicsState,
                  std::shared_ptr<ResourceManager> resourceManager);

    ~ScreenManager();

    void registerScreen(std::shared_ptr<Screen> screen);
    void unregisterScreen(const std::string& name);

    std::shared_ptr<Screen> getScreen(const std::string& name) const;

    void changeScreen(const std::string& newScreenName);
    std::shared_ptr<Screen> getActiveScreen() const;

    void update(float delta);
    void render();

    std::shared_ptr<GUILayout> getCommonGUILayout() const;

private:
    std::shared_ptr<Screen> m_activeScreen;
    std::unordered_map<std::string, std::shared_ptr<Screen>> m_screens;

    std::shared_ptr<GameWorld> m_gameWorld;
    std::shared_ptr<GraphicsModule> m_graphicsModule;
    std::shared_ptr<SharedGraphicsState> m_sharedGraphicsState;
    std::shared_ptr<ResourceManager> m_resourceManager;

    std::shared_ptr<GUILayout> m_commonGUILayout;
};

