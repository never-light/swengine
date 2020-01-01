#include "GameScreen.h"

#include <spdlog/spdlog.h>

#include <Engine/Modules/Graphics/GUI/GUIText.h>
#include <Engine/Modules/Graphics/GUI/GUITextBox.h>
#include <Engine/Modules/Graphics/GUI/GUIImage.h>

#include <Engine/Modules/Graphics/GraphicsSystem/TransformComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/MeshRendererComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/Material.h>

#include <Engine/Modules/Graphics/Resources/ShaderResource.h>
#include <Engine/Modules/Graphics/Resources/TextureResource.h>
#include <Engine/Modules/Graphics/Resources/MeshResource.h>

#include <Engine/Modules/Graphics/GraphicsSystem/DebugPainter.h>

#include "Game/PlayerComponent.h"

GameScreen::GameScreen(std::shared_ptr<InputModule> inputModule)
    : BaseGameScreen(GameScreenType::Game),
      m_inputModule(inputModule)
{
}

GameScreen::~GameScreen()
{

}

void GameScreen::activate()
{
    m_game->activate();

    m_inputModule->registerAction("toggle_menu", KeyboardInputAction(SDLK_ESCAPE));

    m_gameWorld->subscribeEventsListener<InputActionToggleEvent>(this);
}

void GameScreen::deactivate()
{
    m_gameWorld->unsubscribeEventsListener<InputActionToggleEvent>(this);

    m_inputModule->unregisterAction("toggle_menu");

    m_game->deactivate();
}

void GameScreen::load()
{
    initializeGame();
}

void GameScreen::unload()
{
    deinitializeGame();
}

void GameScreen::update(float delta)
{
    ARG_UNUSED(delta);
}

void GameScreen::render()
{
    DebugPainter::renderBasis({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
}

EventProcessStatus GameScreen::receiveEvent(GameWorld* gameWorld, const InputActionToggleEvent& event)
{
    ARG_UNUSED(gameWorld);

    if (event.actionName == "toggle_menu" && event.newState == InputActionState::Active) {
        activateNextScreen(GameScreenType::MainMenu);
    }

    return EventProcessStatus::Processed;
}

void GameScreen::initializeGame()
{
    spdlog::info("Load game...");

    m_game = std::make_unique<Game>(m_gameWorld, m_inputModule,
                                    m_graphicsModule->getGraphicsContext(),
                                    m_sharedGraphicsState, m_resourceManager);

    spdlog::info("Game is loaded...");
}

void GameScreen::deinitializeGame()
{
    spdlog::info("Unload game...");

    m_game.reset();
}
