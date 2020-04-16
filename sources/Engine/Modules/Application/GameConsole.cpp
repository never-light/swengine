#include "precompiled.h"

#pragma hdrstop

#include "GameConsole.h"

GameConsole::GameConsole(std::shared_ptr<GameWorld> gameWorld)
    : m_gameWorld(gameWorld) {

}

GameConsole::~GameConsole() = default;

void GameConsole::print(const std::string& command) {
  m_guiConsole.lock()->print(command);
}

void GameConsole::executeCommand(const std::string& command) {
  m_gameWorld->emitEvent<GameConsoleCommandEvent>({command});
  m_guiConsole.lock()->print(command);
}

void GameConsole::executeCommand(const std::string& command, GUIConsole& console) {
  ARG_UNUSED(console);

  executeCommand(command);
}

void GameConsole::setGUIConsole(std::weak_ptr<GUIConsole> console) {
  m_guiConsole = console;
}

std::shared_ptr<GUIConsole> GameConsole::getGUIConsole() const {
  return m_guiConsole.lock();
}
