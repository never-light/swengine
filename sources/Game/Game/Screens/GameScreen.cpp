#include "GameScreen.h"

#include <Engine/Modules/Graphics/Resources/BitmapFontResource.h>
#include <Engine/Modules/Graphics/GraphicsSystem/TransformComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/DebugPainter.h>

#include <utility>

GameScreen::GameScreen(
  std::shared_ptr<InputModule> inputModule,
  std::shared_ptr<GameSystemsGroup> gameApplicationSystemsGroup,
  std::shared_ptr<LevelsManager> levelsManager,
  std::shared_ptr<GUILayout> debugGUILayout,
  std::shared_ptr<InventoryUI> inventoryUILayout)
  : BaseGameScreen(GameScreenType::Game),
    m_inputModule(std::move(inputModule)),
    m_gameApplicationSystemsGroup(std::move(gameApplicationSystemsGroup)),
    m_levelsManager(std::move(levelsManager)),
    m_gameGUILayout(std::make_shared<GUILayout>()),
    m_debugGUILayout(std::move(debugGUILayout)),
    m_inventoryUILayout(std::move(inventoryUILayout))
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
  auto& screenFramebuffer = m_graphicsModule->getGraphicsContext()->getDefaultFramebuffer();
  m_gameGUILayout->setSize({screenFramebuffer.getWidth(), screenFramebuffer.getHeight()});

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

  const FrameStats& stats = m_sharedGraphicsState->getFrameStats();

  m_primivitesCountText->setText("Primitives: " + std::to_string(stats.getPrimitivesCount()));
  m_subMeshesCountText->setText("Meshes: " + std::to_string(stats.getSubMeshesCount()));
  m_culledSubMeshesCountText->setText("Culled: " + std::to_string(stats.getCulledSubMeshesCount()));
}

void GameScreen::render()
{
  DebugPainter::renderBasis({0.0f, 0.0f, 0.0f}, {2.0f, 0.0f, 0.0f}, {0.0f, 2.0f, 0.0f}, {0.0f, 0.0f, 2.0f});
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
    m_sharedGraphicsState,
    m_resourceManager,
    m_levelsManager,
    m_gameGUILayout,
    m_inventoryUILayout);

  spdlog::info("Game is loaded...");
}

void GameScreen::deinitializeGame()
{
  spdlog::info("Unload game...");

  m_game.reset();
}

void GameScreen::initializeDebugGUI()
{
  m_gameGUILayout->addChildWidget(m_debugGUILayout);
  m_primivitesCountText = std::dynamic_pointer_cast<GUIText>(m_debugGUILayout
    ->findChildByName("game_debug_ui_layout_frame_stat_primitives_count"));
  m_subMeshesCountText = std::dynamic_pointer_cast<GUIText>(m_debugGUILayout
    ->findChildByName("game_debug_ui_layout_frame_stat_meshes_count"));
  m_culledSubMeshesCountText = std::dynamic_pointer_cast<GUIText>(m_debugGUILayout
    ->findChildByName("game_debug_ui_layout_frame_stat_culled_meshes_count"));

  SW_ASSERT(m_primivitesCountText != nullptr &&
    m_subMeshesCountText != nullptr &&
    m_culledSubMeshesCountText != nullptr);
}

void GameScreen::deinitializeDebugGUI()
{
  m_primivitesCountText.reset();
  m_subMeshesCountText.reset();
  m_subMeshesCountText.reset();

  m_gameGUILayout->removeChildWidget(m_debugGUILayout);
}
