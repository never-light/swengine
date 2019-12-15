#pragma once

#include "Modules/Graphics/GUI/GUIConsole.h"
#include "Modules/ECS/ECS.h"

struct GameConsoleCommandEvent {
    std::string command;
};

class GameConsole : public GUIConsoleCommandsExecutor
{
public:
    GameConsole(std::shared_ptr<GameWorld> gameWorld);
    ~GameConsole() override;

    void print(const std::string& command);

    void executeCommand(const std::string& command);
    void executeCommand(const std::string& command, GUIConsole& console) override;

    void setGUIConsole(std::weak_ptr<GUIConsole> console);
    std::shared_ptr<GUIConsole> getGUIConsole() const;

private:
    std::shared_ptr<GameWorld> m_gameWorld;
    std::weak_ptr<GUIConsole> m_guiConsole;
};

