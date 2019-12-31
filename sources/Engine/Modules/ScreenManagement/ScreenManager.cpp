#include "ScreenManager.h"

ScreenManager::ScreenManager(std::shared_ptr<GameWorld> gameWorld,
                             std::shared_ptr<GraphicsModule> graphicsModule,
                             std::shared_ptr<SharedGraphicsState> sharedGraphicsState,
                             std::shared_ptr<ResourceManager> resourceManager)
    : m_gameWorld(gameWorld),
      m_graphicsModule(graphicsModule),
      m_sharedGraphicsState(sharedGraphicsState),
      m_resourceManager(resourceManager),
      m_commonGUILayout(std::make_shared<GUILayout>())
{
    std::shared_ptr<GLGraphicsContext> graphicsContext = graphicsModule->getGraphicsContext();
    m_commonGUILayout->setSize({ graphicsContext->getBufferWidth(), graphicsContext->getBufferHeight() });
}

ScreenManager::~ScreenManager()
{
    for (auto screenIt : m_screens) {
        if (screenIt.second->isActive()) {
            screenIt.second->deactivate();
        }

        screenIt.second->unload();
    }
}

void ScreenManager::registerScreen(std::shared_ptr<Screen> screen)
{
    screen->performInternalInitialization(shared_from_this(), m_gameWorld,
        m_graphicsModule, m_sharedGraphicsState, m_resourceManager);

    screen->load();

    m_screens.insert({ screen->getName(), screen });
}

void ScreenManager::unregisterScreen(const std::string& name)
{
    auto screenIt = m_screens.find(name);
    screenIt->second->unload();

    m_screens.erase(screenIt);
}

std::shared_ptr<Screen> ScreenManager::getScreen(const std::string& name) const
{
    return m_screens.at(name);
}

void ScreenManager::changeScreen(const std::string& newScreenName)
{
    if (m_activeScreen != nullptr) {
        m_commonGUILayout->removeChildWidget(m_activeScreen->getGUILayout());
        m_activeScreen->performDeactivate();
    }

    m_activeScreen = m_screens.at(newScreenName);
    m_activeScreen->performActivate();
    m_commonGUILayout->addChildWidget(m_activeScreen->getGUILayout());
}

std::shared_ptr<Screen> ScreenManager::getActiveScreen() const
{
    return m_activeScreen;
}

void ScreenManager::update(float delta)
{
    if (m_activeScreen != nullptr) {
        m_activeScreen->update(delta);
    }
}

void ScreenManager::render()
{
    if (m_activeScreen != nullptr) {
        m_activeScreen->render();
    }
}

std::shared_ptr<GUILayout> ScreenManager::getCommonGUILayout() const
{
    return m_commonGUILayout;
}

