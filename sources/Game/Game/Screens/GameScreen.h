#pragma once

#include "BaseGameScreen.h"
#include "Game/PlayerControlSystem.h"
#include "Game/Game.h"

class GameScreen : public BaseGameScreen, public EventsListener<InputActionToggleEvent>
{
public:
    GameScreen(std::shared_ptr<InputModule> inputModule);

    void activate() override;
    void deactivate() override;

    void load() override;
    void unload() override;

    void update(float delta) override;
    void render() override;

    EventProcessStatus receiveEvent(GameWorld *gameWorld, const InputActionToggleEvent &event) override;

private:
    void initializeGame();
    void deinitializeGame();

private:
    std::shared_ptr<InputModule> m_inputModule;
    std::unique_ptr<Game> m_game;
};

