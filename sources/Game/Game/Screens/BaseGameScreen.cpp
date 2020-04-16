#include "BaseGameScreen.h"

const std::unordered_map<GameScreenType, std::string> BaseGameScreen::s_gameScreensNames = {
    {GameScreenType::MainMenu, "MainMenu"},
    {GameScreenType::Game, "Game"},
};

BaseGameScreen::BaseGameScreen(GameScreenType type)
    : Screen(getScreenName(type)) {

}

void BaseGameScreen::activateNextScreen(GameScreenType type) {
  Screen::activateNextScreen(getScreenName(type));
}

std::string BaseGameScreen::getScreenName(GameScreenType type) {
  return s_gameScreensNames.at(type);
}
