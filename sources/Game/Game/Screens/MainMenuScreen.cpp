#include "MainMenuScreen.h"

#include <Engine/Modules/Graphics/Resources/TextureResource.h>

MainMenuScreen::MainMenuScreen(std::shared_ptr<InputModule> inputModule, std::shared_ptr<GameConsole> gameConsole)
    : BaseGameScreen(GameScreenType::MainMenu),
      m_inputModule(inputModule),
      m_gameConsole(gameConsole) {
}

void MainMenuScreen::activate() {

}

void MainMenuScreen::deactivate() {

}

void MainMenuScreen::load() {
  initializeGUI();
}

void MainMenuScreen::unload() {
  deinitializeGUI();
}

EventProcessStatus MainMenuScreen::receiveEvent(GameWorld* gameWorld, const InputActionToggleEvent& event) {
  ARG_UNUSED(gameWorld);
  ARG_UNUSED(event);

  return EventProcessStatus::Processed;
}

void MainMenuScreen::initializeGUI() {
  // Background
  std::shared_ptr menuBackgroundTexture =
      m_resourceManager->getResourceFromInstance<TextureResource>("tex_main_menu_background")->getTexture();

  ARG_UNUSED(menuBackgroundTexture);

  getGUILayout()->setBackgroundImage(menuBackgroundTexture);
  getGUILayout()->setHoverBackgroundImage(menuBackgroundTexture);

  // New game

  std::shared_ptr newGameButtonImage =
      m_resourceManager->getResourceFromInstance<TextureResource>("tex_main_menu_new_game")->getTexture();

  std::shared_ptr newGameButtonHoverImage =
      m_resourceManager->getResourceFromInstance<TextureResource>("tex_main_menu_new_game_hover")->getTexture();

  std::shared_ptr newGameButton = std::make_shared<GUIButton>();
  newGameButton->setBackgroundImage(newGameButtonImage);
  newGameButton->setHoverBackgroundImage(newGameButtonHoverImage);

  newGameButton->setSize({256, 64});
  newGameButton->setOrigin({getGUILayout()->getSize().x / 2 - 256 / 2, getGUILayout()->getSize().y / 2 - 100});

  newGameButton->setMouseButtonCallback([=](const GUIMouseButtonEvent& event) {
    if (event.type == MouseButtonEventType::ButtonDown && event.button == SDL_BUTTON_LEFT) {
      this->activateNextScreen(GameScreenType::Game);
    }
  });

  getGUILayout()->addChildWidget(newGameButton);

  // Exit

  std::shared_ptr exitButtonImage =
      m_resourceManager->getResourceFromInstance<TextureResource>("tex_main_menu_exit")->getTexture();

  std::shared_ptr exitButtonHoverImage =
      m_resourceManager->getResourceFromInstance<TextureResource>("tex_main_menu_exit_hover")->getTexture();

  std::shared_ptr exitButton = std::make_shared<GUIButton>();
  exitButton->setBackgroundImage(exitButtonImage);
  exitButton->setHoverBackgroundImage(exitButtonHoverImage);

  exitButton->setSize({256, 64});
  exitButton->setOrigin({getGUILayout()->getSize().x / 2 - 256 / 2, getGUILayout()->getSize().y / 2});

  exitButton->setMouseButtonCallback([=](const GUIMouseButtonEvent& event) {
    if (event.type == MouseButtonEventType::ButtonDown && event.button == SDL_BUTTON_LEFT) {
      m_gameConsole->executeCommand("exit");
    }
  });

  getGUILayout()->addChildWidget(exitButton);
}

void MainMenuScreen::deinitializeGUI() {
  getGUILayout()->removeChildren();
}

