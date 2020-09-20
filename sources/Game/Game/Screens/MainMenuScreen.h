#pragma once

#include "BaseGameScreen.h"
#include <Engine/Modules/Application/GameConsole.h>

class MainMenuScreen : public BaseGameScreen,
                       public EventsListener<InputActionToggleEvent> {
 public:
  MainMenuScreen(std::shared_ptr<InputModule> inputModule,
    std::shared_ptr<GUILayout> guiLayout,
    std::shared_ptr<GameConsole> gameConsole);

  void activate() override;
  void deactivate() override;

  void load() override;
  void unload() override;

  EventProcessStatus receiveEvent(GameWorld* gameWorld, const InputActionToggleEvent& event) override;

 private:
  void initializeGUI();
  void deinitializeGUI();

 private:
  std::shared_ptr<InputModule> m_inputModule;
  std::shared_ptr<GUILayout> m_guiLayout;

  std::shared_ptr<GameConsole> m_gameConsole;
};

