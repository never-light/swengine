#include "GameScreen.h"

#include <Engine/Modules/Graphics/Resources/BitmapFontResourceManager.h>
#include <Engine/Modules/Graphics/GraphicsSystem/TransformComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/DebugPainter.h>

#include <Engine/Utility/files.h>

#include <utility>

GameScreen::GameScreen(
  std::shared_ptr<InputModule> inputModule,
  std::shared_ptr<GameSystemsGroup> gameApplicationSystemsGroup,
  std::shared_ptr<LevelsManager> levelsManager,
  std::shared_ptr<GraphicsScene> graphicsScene,
  std::shared_ptr<GUISystem> guiSystem)
  : BaseGameScreen(GameScreenType::Game),
    m_inputModule(std::move(inputModule)),
    m_gameApplicationSystemsGroup(std::move(gameApplicationSystemsGroup)),
    m_levelsManager(std::move(levelsManager)),
    m_graphicsScene(std::move(graphicsScene)),
    m_guiSystem(std::move(guiSystem)),
    m_gameGUILayout(std::make_shared<GUILayout>())
{
  m_debugGUILayout = m_guiSystem->loadScheme(
    FileUtils::getGUISchemePath("screen_game_debug"));
}

GameScreen::~GameScreen() = default;

void GameScreen::activate()
{
  m_game->activate();

  m_inputModule->registerAction("toggle_menu", KeyboardInputAction(SDLK_ESCAPE));

  m_gameWorld->subscribeEventsListener<InputActionToggleEvent>(this);
  m_gameWorld->subscribeEventsListener<GameConsoleChangeVisibilityEvent>(this);

  if (!m_game->isLoaded()) {
    m_game->createNewGame("../../../bin/crossroads/agency_room_export");
  }
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
  m_gameGUILayout->setSize({
    m_graphicsModule->getGraphicsContext()->getViewportWidth(),
    m_graphicsModule->getGraphicsContext()->getViewportHeight()});

  getGUILayout()->addChildWidget(m_gameGUILayout);

  initializeGame();
  initializeDebugGUI();
}

void GameScreen::unload()
{
  deinitializeGame();
  deinitializeDebugGUI();

  getGUILayout()->removeChildWidget(m_gameGUILayout);
}

void GameScreen::update(float delta)
{
  ARG_UNUSED(delta);

  const FrameStats& stats = m_graphicsScene->getFrameStats();

  m_primitivesCountText->setText("Primitives: " + std::to_string(stats.getPrimitivesCount()));
  m_subMeshesCountText->setText("Meshes: " + std::to_string(stats.getSubMeshesCount()));
  m_culledSubMeshesCountText->setText("Culled: " + std::to_string(stats.getCulledSubMeshesCount()));
}

void GameScreen::render()
{
  DebugPainter::renderBasis({0.0f, 0.0f, 0.0f}, {2.0f, 0.0f, 0.0f}, {0.0f, 2.0f, 0.0f}, {0.0f, 0.0f, 2.0f});
}

EventProcessStatus GameScreen::receiveEvent(const InputActionToggleEvent& event)
{
  if (event.actionName == "toggle_menu" && event.newState == InputActionState::Active) {
    activateNextScreen(GameScreenType::MainMenu);
  }

  return EventProcessStatus::Processed;
}

EventProcessStatus GameScreen::receiveEvent(const GameConsoleChangeVisibilityEvent& event)
{
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

  m_game = std::make_shared<Game>(m_gameWorld,
    m_gameApplicationSystemsGroup,
    m_inputModule,
    m_graphicsModule->getGraphicsContext(),
    m_graphicsScene,
    m_guiSystem,
    m_resourceManager,
    m_levelsManager,
    m_gameGUILayout);

  spdlog::info("Game is loaded...");
}

void GameScreen::deinitializeGame()
{
  m_game->unload();
  spdlog::info("Unload game...");
}

void GameScreen::initializeDebugGUI()
{
  m_gameGUILayout->addChildWidget(m_debugGUILayout);
  m_primitivesCountText = std::dynamic_pointer_cast<GUIText>(m_debugGUILayout
    ->findChildByName("game_debug_ui_layout_frame_stat_primitives_count"));
  m_subMeshesCountText = std::dynamic_pointer_cast<GUIText>(m_debugGUILayout
    ->findChildByName("game_debug_ui_layout_frame_stat_meshes_count"));
  m_culledSubMeshesCountText = std::dynamic_pointer_cast<GUIText>(m_debugGUILayout
    ->findChildByName("game_debug_ui_layout_frame_stat_culled_meshes_count"));

  SW_ASSERT(m_primitivesCountText != nullptr &&
    m_subMeshesCountText != nullptr &&
    m_culledSubMeshesCountText != nullptr);
}

void GameScreen::deinitializeDebugGUI()
{
  m_primitivesCountText.reset();
  m_subMeshesCountText.reset();
  m_subMeshesCountText.reset();

  m_gameGUILayout->removeChildWidget(m_debugGUILayout);
}

std::shared_ptr<Game> GameScreen::getGame() const
{
  return m_game;
}
