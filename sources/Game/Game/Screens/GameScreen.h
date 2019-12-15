#pragma once

#include "BaseGameScreen.h"
#include "Game/PlayerControlSystem.h"

class GameScreen : public BaseGameScreen
{
public:
    GameScreen(std::shared_ptr<InputModule> inputModule);

    void activate() override;
    void deactivate() override;

    void load() override;
    void unload() override;

    void update(float delta) override;
    void render() override;

private:
    void initializeGame();
    void deinitializeGame();

    void initializeGUI();
    void deinitializeGUI();

private:
    std::shared_ptr<InputModule> m_inputModule;
    std::shared_ptr<PlayerControlSystem> m_playerControlSystem;
};

