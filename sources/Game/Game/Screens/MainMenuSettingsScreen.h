#pragma once

#include "BaseGameScreen.h"
#include <Engine/Modules/Application/GameConsole.h>

class MainMenuSettingsScreen : public BaseGameScreen {
 public:
  explicit MainMenuSettingsScreen(std::shared_ptr<GUILayout> guiLayout);

  void activate() override;
  void deactivate() override;

  void load() override;
  void unload() override;

 private:
  void saveSettings();

 private:
  std::shared_ptr<GUILayout> m_guiLayout;
};

