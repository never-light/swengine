#include "GameScreen.h"

#include <spdlog/spdlog.h>

#include <Engine/Modules/Graphics/GUI/GUIText.h>
#include <Engine/Modules/Graphics/GUI/GUITextBox.h>
#include <Engine/Modules/Graphics/GUI/GUIImage.h>
#include <Engine/Modules/Graphics/Resources/BitmapFontResource.h>

#include <Engine/Modules/Graphics/GraphicsSystem/TransformComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/MeshRendererComponent.h>

#include <Engine/Modules/Graphics/Resources/ShaderResource.h>
#include <Engine/Modules/Graphics/Resources/TextureResource.h>
#include <Engine/Modules/Graphics/Resources/MeshResource.h>

#include <Engine/Modules/Graphics/GraphicsSystem/DebugPainter.h>

#include "Game/PlayerComponent.h"

GameScreen::GameScreen(std::shared_ptr<InputModule> inputModule,
  std::shared_ptr<GameSystemsGroup> gameApplicationSystemsGroup)
  : BaseGameScreen(GameScreenType::Game),
    m_inputModule(inputModule),
    m_gameApplicationSystemsGroup(gameApplicationSystemsGroup)
{
}

GameScreen::~GameScreen() = default;

void GameScreen::activate()
{
  m_game->activate();

  m_inputModule->registerAction("toggle_menu", KeyboardInputAction(SDLK_ESCAPE));

  m_gameWorld->subscribeEventsListener<InputActionToggleEvent>(this);
  m_gameWorld->subscribeEventsListener<GameConsoleChangeVisibilityEvent>(this);
}

void GameScreen::deactivate()
{
  m_gameWorld->subscribeEventsListener<GameConsoleChangeVisibilityEvent>(this);
  m_gameWorld->unsubscribeEventsListener<InputActionToggleEvent>(this);

  m_inputModule->unregisterAction("toggle_menu");

  m_game->deactivate();
}

void GameScreen::load()
{
  initializeGame();
  initializeDebugGUI();
}

void GameScreen::unload()
{
  deinitializeGame();
  deinitialzieDebugGUI();
}

void GameScreen::update(float delta)
{
  ARG_UNUSED(delta);

  const FrameStats& stats = m_sharedGraphicsState->getFrameStats();

  m_primivitesCountText->setText("Privimites: " + std::to_string(stats.getPrimivitesCount()));
  m_subMeshesCountText->setText("Meshes: " + std::to_string(stats.getSubMeshesCount()));
  m_culledSubMeshesCountText->setText("Culled: " + std::to_string(stats.getCulledSubMeshesCount()));
}

void GameScreen::render()
{
  //DebugPainter::renderFrustum(m_sharedGraphicsState->getActiveCamera()->getFrustum(), { 1.0f, 0.0f, 0.0f, 1.0f });
  DebugPainter::renderSphere(m_resourceManager->getResourceFromInstance<MeshResource>("ground_mesh")->getMesh()
      ->getAABB().toSphere(),
    {1.0f, 0.0f, 0.0f, 1.0f}, true);

  DebugPainter::renderBasis({0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f});
}

EventProcessStatus GameScreen::receiveEvent(GameWorld* gameWorld, const InputActionToggleEvent& event)
{
  ARG_UNUSED(gameWorld);

  if (event.actionName == "toggle_menu" && event.newState == InputActionState::Active) {
    activateNextScreen(GameScreenType::MainMenu);
  }

  return EventProcessStatus::Processed;
}

EventProcessStatus GameScreen::receiveEvent(GameWorld* gameWorld, const GameConsoleChangeVisibilityEvent& event)
{
  ARG_UNUSED(gameWorld);

  if (event.isVisible) {
    m_game->enterConsoleMode();
  }
  else {
    m_game->leaveConsoleMode();
  }

  return EventProcessStatus::Processed;
}

void GameScreen::initializeGame()
{
  spdlog::info("Load game...");

  m_game = std::make_unique<Game>(m_gameWorld,
    m_gameApplicationSystemsGroup,
    m_inputModule,
    m_graphicsModule->getGraphicsContext(),
    m_sharedGraphicsState, m_resourceManager);

  spdlog::info("Game is loaded...");
}

void GameScreen::deinitializeGame()
{
  spdlog::info("Unload game...");

  m_game.reset();
}

void GameScreen::initializeDebugGUI()
{
  m_debugGUILayout = std::make_shared<GUILayout>();
  m_debugGUILayout->setSize({150, 120});
  m_debugGUILayout->setOrigin({getGUILayout()->getSize().x - 150, 0});
  m_debugGUILayout->setBackgroundColor({0.0f, 1.0f, 0.0f, 0.3f});

  getGUILayout()->addChildWidget(m_debugGUILayout);

  std::shared_ptr<BitmapFont> textFont = m_resourceManager->
    getResourceFromInstance<BitmapFontResource>("gui_default_font")->getFont();

  m_primivitesCountText = std::make_shared<GUIText>(textFont, "Hello1");
  m_primivitesCountText->setFontSize(9);
  m_debugGUILayout->addChildWidget(m_primivitesCountText);

  m_primivitesCountText->setOrigin({5, 10});

  m_subMeshesCountText = std::make_shared<GUIText>(textFont, "Hello2");
  m_subMeshesCountText->setFontSize(9);
  m_debugGUILayout->addChildWidget(m_subMeshesCountText);

  m_subMeshesCountText->setOrigin({5, 40});

  m_culledSubMeshesCountText = std::make_shared<GUIText>(textFont, "Hello3");
  m_culledSubMeshesCountText->setFontSize(9);
  m_debugGUILayout->addChildWidget(m_culledSubMeshesCountText);

  m_culledSubMeshesCountText->setOrigin({5, 70});
}

void GameScreen::deinitialzieDebugGUI()
{
  getGUILayout()->removeChildWidget(m_debugGUILayout);
}
