#include "MainMenuSettingsScreen.h"

#include <utility>

#include <Engine/Modules/Application/StartupSettings.h>

MainMenuSettingsScreen::MainMenuSettingsScreen(
  std::shared_ptr<GUILayout> guiLayout)
  : BaseGameScreen(GameScreenType::MainMenuSettings),
    m_guiLayout(std::move(guiLayout))
{
}

void MainMenuSettingsScreen::activate()
{
}

void MainMenuSettingsScreen::deactivate()
{
}

void MainMenuSettingsScreen::load()
{
  getGUILayout()->addChildWidget(m_guiLayout);

  m_guiLayout->findChildByName("main_menu_settings_return_button")
    ->setMouseButtonCallback([=](const GUIMouseButtonEvent& event) {
      if (event.type == MouseButtonEventType::ButtonDown && event.button == SDL_BUTTON_LEFT) {
        saveSettings();
        activateNextScreen(GameScreenType::MainMenu);
      }
    });

  getGUILayout()->show();
}

void MainMenuSettingsScreen::unload()
{
  getGUILayout()->removeChildren();
}

void MainMenuSettingsScreen::saveSettings()
{
  std::shared_ptr<GUIDropDownList> dimensionsDropDown = std::dynamic_pointer_cast<GUIDropDownList>(
    m_guiLayout->findChildByName("screen_main_menu_settings_dimensions_list"));

  std::shared_ptr<GUIDropDownList> screenModesDropDown = std::dynamic_pointer_cast<GUIDropDownList>(
    m_guiLayout->findChildByName("screen_main_menu_settings_screen_modes_list"));

  StartupSettings settings;

  settings.setScreenMode(StartupSettings::getScreenModeByName(screenModesDropDown->getCurrentItemValue()));
  settings.setScreenDimension(StartupSettings::getScreenDimensionByName(dimensionsDropDown->getCurrentItemValue()));

  StartupSettings::saveToFile(settings);
}

