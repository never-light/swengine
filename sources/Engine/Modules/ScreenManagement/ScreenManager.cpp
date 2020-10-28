#include "precompiled.h"

#pragma hdrstop

#include "ScreenManager.h"

#include <utility>

ScreenManager::ScreenManager(
  std::shared_ptr<GameWorld> gameWorld,
  std::shared_ptr<GraphicsModule> graphicsModule,
  std::shared_ptr<ResourcesManager> resourceManager,
  int guiLayoutWidth,
  int guiLayoutHeight)
  : m_gameWorld(std::move(gameWorld)),
    m_graphicsModule(std::move(graphicsModule)),
    m_resourceManager(std::move(resourceManager)),
    m_commonGUILayout(std::make_shared<GUILayout>())
{
  m_commonGUILayout->setSize({guiLayoutWidth, guiLayoutHeight});
  m_commonGUILayout->show();
}

ScreenManager::~ScreenManager()
{
  for (const auto& screenIt : m_screens) {
    if (screenIt.second->isActive()) {
      screenIt.second->deactivate();
    }

    screenIt.second->unload();
  }
}

void ScreenManager::registerScreen(std::shared_ptr<Screen> screen)
{
  screen->performInternalInitialization(shared_from_this(), m_gameWorld,
    m_graphicsModule, m_resourceManager);

  screen->load();

  m_screens.insert({screen->getName(), screen});
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
  const Screen* previousScreen = m_activeScreen.get();

  if (m_activeScreen != nullptr) {
    m_commonGUILayout->removeChildWidget(m_activeScreen->getGUILayout());
    m_activeScreen->performDeactivate();
  }

  m_activeScreen = m_screens.at(newScreenName);
  m_activeScreen->performActivate();
  m_commonGUILayout->addChildWidget(m_activeScreen->getGUILayout());

  m_gameWorld->emitEvent<ScreenSwitchEvent>(ScreenSwitchEvent{previousScreen, m_activeScreen.get()});
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

