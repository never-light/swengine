#pragma once

#include <Engine/Modules/ScreenManagement/ScreenManager.h>
#include <Engine/Modules/ECS/ECS.h>

enum class GameScreenType {
  MainMenu, Game
};

class BaseGameScreen : public Screen {
 public:
  BaseGameScreen(GameScreenType type);

 protected:
  void activateNextScreen(GameScreenType type);

 public:
  static std::string getScreenName(GameScreenType type);

 private:
  static const std::unordered_map<GameScreenType, std::string> s_gameScreensNames;
};

