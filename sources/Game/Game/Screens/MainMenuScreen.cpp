#include "MainMenuScreen.h"

#include <Engine/Modules/Graphics/Resources/TextureResourceManager.h>

#include <utility>

MainMenuScreen::MainMenuScreen(
  std::shared_ptr<InputModule> inputModule,
  std::shared_ptr<GUILayout> guiLayout,
  std::shared_ptr<GameConsole> gameConsole)
  : BaseGameScreen(GameScreenType::MainMenu),
    m_inputModule(std::move(inputModule)),
    m_guiLayout(std::move(guiLayout)),
    m_gameConsole(std::move(gameConsole))
{
}

void MainMenuScreen::activate()
{

}

void MainMenuScreen::deactivate()
{

}

void MainMenuScreen::load()
{
  initializeGUI();
}

void MainMenuScreen::unload()
{
  deinitializeGUI();
}

EventProcessStatus MainMenuScreen::receiveEvent(const InputActionToggleEvent& event)
{
  ARG_UNUSED(event);

  return EventProcessStatus::Processed;
}

void MainMenuScreen::initializeGUI()
{
  getGUILayout()->addChildWidget(m_guiLayout);

  m_guiLayout->findChildByName("main_menu_new_game_button")
    ->setMouseButtonCallback([=](const GUIMouseButtonEvent& event) {
      if (event.type == MouseButtonEventType::ButtonDown && event.button == SDL_BUTTON_LEFT) {
        this->activateNextScreen(GameScreenType::Game);
      }
    });

  m_guiLayout->findChildByName("main_menu_settings_button")
    ->setMouseButtonCallback([=](const GUIMouseButtonEvent& event) {
      if (event.type == MouseButtonEventType::ButtonDown && event.button == SDL_BUTTON_LEFT) {
        spdlog::debug("help");
        this->activateNextScreen(GameScreenType::MainMenuSettings);
      }
    });

  m_guiLayout->findChildByName("main_menu_exit_button")
    ->setMouseButtonCallback([=](const GUIMouseButtonEvent& event) {
      if (event.type == MouseButtonEventType::ButtonDown && event.button == SDL_BUTTON_LEFT) {
        m_gameConsole->executeCommand("exit");
      }
    });

  getGUILayout()->show();
}

void MainMenuScreen::deinitializeGUI()
{
  getGUILayout()->removeChildren();
}

