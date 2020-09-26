#include "precompiled.h"

#pragma hdrstop

#include "BaseGameApplication.h"

#include <Exceptions/exceptions.h>
#include <spdlog/spdlog.h>

#include "Modules/Graphics/GUI/GUIConsole.h"

#include "Modules/Graphics/Resources/ShaderResource.h"
#include "Modules/Graphics/Resources/MeshResource.h"
#include "Modules/Graphics/Resources/TextureResource.h"
#include "Modules/Graphics/Resources/BitmapFontResource.h"
#include "Modules/Graphics/Resources/MaterialResource.h"
#include "Modules/Graphics/Resources/SkeletonResource.h"
#include "Modules/Graphics/Resources/SkeletalAnimationResource.h"
#include "Modules/Graphics/Resources/AnimationStatesMachineResource.h"

#include "Modules/Physics/Resources/CollisionDataResource.h"
#include "Modules/Audio/Resources/AudioClipResource.h"

#include "Modules/LevelsManagement/GameObjectsGenericClassLoader.h"

BaseGameApplication::BaseGameApplication(int argc,
  char* argv[],
  const std::string& windowTitle,
  int windowWidth,
  int windowHeight)
  : m_mainWindow(nullptr)
{
  spdlog::set_level(spdlog::level::debug);
  std::set_terminate([]() { BaseGameApplication::handleAppTerminate(); });

  spdlog::info("Application start...");
  initializePlatform(argc, argv, windowTitle, windowWidth, windowHeight);
  spdlog::info("Application is started");

  spdlog::info("Initialize engine modules...");
  initializeEngine();
  spdlog::info("Engine modules are initialized");

  spdlog::info("Initialize game systems...");
  initializeEngineSystems();
  spdlog::info("Game systems are initialized");
}

BaseGameApplication::~BaseGameApplication()
{
  SDL_DestroyWindow(m_mainWindow);
}

void BaseGameApplication::load()
{

}

void BaseGameApplication::unload()
{

}

void BaseGameApplication::update(float delta)
{
  ARG_UNUSED(delta);
}

void BaseGameApplication::render()
{

}

int BaseGameApplication::execute()
{
  spdlog::info("Perform game application loading...");
  performLoad();
  spdlog::info("Game application is loaded and ready...");

  SDL_ShowWindow(m_mainWindow);

  const int FRAMES_PER_SECOND = 30;
  const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;

  unsigned long nextTick = SDL_GetTicks();

  long sleepTime = 0;

  SDL_Event event;

  spdlog::info("Starting main loop...");

  m_isMainLoopActive = true;

  while (m_isMainLoopActive) {
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT || (event.type == SDL_WINDOWEVENT &&
        event.window.event == SDL_WINDOWEVENT_CLOSE)) {
        m_isMainLoopActive = false;
        break;
      }

      m_inputModule->processRawSDLEvent(event);
    }

    if (!m_isMainLoopActive) {
      break;
    }

    performUpdate(1.0f / FRAMES_PER_SECOND);

    if (!m_isMainLoopActive) {
      break;
    }

    performRender();

    bool isFrameRateUnlimited = m_inputModule->isActionActive("unlimited_framerate");

    if (!isFrameRateUnlimited) {
      nextTick += SKIP_TICKS;
      sleepTime = static_cast<long>(nextTick) - static_cast<long>(SDL_GetTicks());

      if (sleepTime >= 0) {
        SDL_Delay(static_cast<unsigned long>(sleepTime));
      }
    }
  }

  spdlog::info("Perform game application unloading...");
  performUnload();
  spdlog::info("Game application is unloaded...");

  return 0;
}

EventProcessStatus BaseGameApplication::receiveEvent(GameWorld* gameWorld, const GameConsoleCommandEvent& event)
{
  ARG_UNUSED(gameWorld);

  if (event.command == "exit") {
    shutdown();

    return EventProcessStatus::Processed;
  }
  else if (event.command == "physics-debug-draw") {
    m_physicsSystem->enableDebugDrawing(!m_physicsSystem->isDebugDrawingEnabled());

    return EventProcessStatus::Processed;
  }

  return EventProcessStatus::Skipped;
}

EventProcessStatus BaseGameApplication::receiveEvent(GameWorld* gameWorld, const InputActionToggleEvent& event)
{
  ARG_UNUSED(gameWorld);

  if (event.actionName == "console" && event.newState == InputActionState::Active) {
    if (m_gameConsole->getGUIConsole()->isShown()) {
      m_gameConsole->getGUIConsole()->hide();
      m_gameWorld->emitEvent<GameConsoleChangeVisibilityEvent>(GameConsoleChangeVisibilityEvent{false});
    }
    else {
      m_gameConsole->getGUIConsole()->show();
      m_gameWorld->emitEvent<GameConsoleChangeVisibilityEvent>(GameConsoleChangeVisibilityEvent{true});
    }
  }

  return EventProcessStatus::Processed;
}

void BaseGameApplication::shutdown()
{
  m_isMainLoopActive = false;
}

void BaseGameApplication::initializePlatform(int argc,
  char* argv[],
  const std::string& windowTitle,
  int windowWidth,
  int windowHeight)
{
  ARG_UNUSED(argc);
  ARG_UNUSED(argv);

  int initStatus = SDL_Init(SDL_INIT_EVERYTHING);

  if (initStatus != 0) {
    THROW_EXCEPTION(EngineRuntimeException, std::string(SDL_GetError()));
  }

  spdlog::info("SDL is initialized");

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  spdlog::info("Create main window...");

  m_mainWindow = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    windowWidth, windowHeight, SDL_WINDOW_OPENGL);

  if (m_mainWindow == nullptr) {
    THROW_EXCEPTION(EngineRuntimeException, std::string(SDL_GetError()));
  }

  spdlog::info("Window is created");
}

void BaseGameApplication::initializeEngine()
{
  m_inputModule = std::make_shared<InputModule>(m_mainWindow);

  m_graphicsModule = std::make_shared<GraphicsModule>(m_mainWindow);
  m_sharedGraphicsState = std::make_shared<SharedGraphicsState>(m_graphicsModule->getGraphicsContext());
  m_graphicsScene = std::make_shared<GraphicsScene>(m_sharedGraphicsState);

  m_resourceManagementModule = std::make_shared<ResourceManagementModule>();

  std::shared_ptr<ResourceManager> resourceManager = m_resourceManagementModule->getResourceManager();
  resourceManager->declareResourceType<ShaderResource>("shader");
  resourceManager->declareResourceType<MeshResource>("mesh");
  resourceManager->declareResourceType<TextureResource>("texture");
  resourceManager->declareResourceType<BitmapFontResource>("bitmap_font");
  resourceManager->declareResourceType<MaterialResource>("material");
  resourceManager->declareResourceType<SkeletonResource>("skeleton");
  resourceManager->declareResourceType<SkeletalAnimationResource>("animation");
  resourceManager->declareResourceType<AnimationStatesMachineResource>("animation_states_machine");

  resourceManager->declareResourceType<CollisionDataResource>("collision");
  resourceManager->declareResourceType<AudioClipResource>("audio");

  resourceManager->loadResourcesMapFile("../resources/engine_resources.xml");

  m_gameWorld = GameWorld::createInstance();
  m_screenManager = std::make_shared<ScreenManager>(m_gameWorld, m_graphicsModule,
    m_sharedGraphicsState, resourceManager);

  DebugPainter::initialize(m_resourceManagementModule->getResourceManager(), m_graphicsScene);
}

void BaseGameApplication::initializeEngineSystems()
{
  std::shared_ptr<ResourceManager> resourceManager = m_resourceManagementModule->getResourceManager();

  m_engineGameSystems = std::make_shared<GameSystemsGroup>();
  m_gameWorld->getGameSystemsGroup()->addGameSystem(m_engineGameSystems);

  // Input system
  m_inputSystem = std::make_shared<InputSystem>(m_inputModule);
  m_engineGameSystems->addGameSystem(m_inputSystem);

  m_gameWorld->subscribeEventsListener<GameConsoleCommandEvent>(this);
  m_gameWorld->subscribeEventsListener<InputActionToggleEvent>(this);

  // Skeletal animation system
  auto skeletalAnimationSystem = std::make_shared<SkeletalAnimationSystem>();
  m_engineGameSystems->addGameSystem(skeletalAnimationSystem);

  // Scene management system
  auto sceneManagementSystem = std::make_shared<GraphicsSceneManagementSystem>(m_graphicsScene);

  m_engineGameSystems->addGameSystem(sceneManagementSystem);

  // Rendering pipeline
  m_renderingSystemsPipeline = std::make_shared<RenderingSystemsPipeline>(m_graphicsModule->getGraphicsContext(),
    m_graphicsScene);

  m_engineGameSystems->addGameSystem(m_renderingSystemsPipeline);

  // Mesh rendering system
  auto meshRenderingSystem = std::make_shared<MeshRenderingSystem>(m_graphicsModule->getGraphicsContext(),
    m_graphicsScene);
  m_renderingSystemsPipeline->addGameSystem(meshRenderingSystem);

  // Environment rendering
  auto environmentRenderingSystem = std::make_shared<EnvironmentRenderingSystem>(m_graphicsModule->getGraphicsContext(),
    m_graphicsScene,
    resourceManager->getResourceFromInstance<MeshResource>("mesh_identity_sphere")->getMesh());

  m_renderingSystemsPipeline->addGameSystem(environmentRenderingSystem);

  // GUI system
  std::shared_ptr<GLShader> guiVertexShader = resourceManager->
    getResourceFromInstance<ShaderResource>("gui_vertex_shader")->getShader();

  std::shared_ptr<GLShader> guiFragmentShader = resourceManager->
    getResourceFromInstance<ShaderResource>("gui_fragment_shader")->getShader();

  std::shared_ptr<GLShadersPipeline> guiShadersPipeline = std::make_shared<GLShadersPipeline>(
    guiVertexShader, guiFragmentShader, nullptr);

  m_guiSystem = std::make_shared<GUISystem>(
    m_inputModule,
    m_resourceManagementModule->getResourceManager(),
    m_graphicsModule->getGraphicsContext(),
    guiShadersPipeline);

  std::shared_ptr<BitmapFont> guiDefaultFont = resourceManager->
    getResourceFromInstance<BitmapFontResource>("gui_default_font")->getFont();
  m_guiSystem->setDefaultFont(guiDefaultFont);
  m_guiSystem->setActiveLayout(m_screenManager->getCommonGUILayout());

  m_engineGameSystems->addGameSystem(m_guiSystem);

  // Physics system
  m_physicsSystem = std::make_shared<PhysicsSystem>();

  m_engineGameSystems->addGameSystem(m_physicsSystem);

  m_physicsSystem->setUpdateStepCallback([this](float delta) {
    this->m_gameWorld->fixedUpdate(delta);
  });

  // Audio system
  m_audioSystem = std::make_shared<AudioSystem>(m_graphicsScene);

  m_engineGameSystems->addGameSystem(m_audioSystem);

  // Levels manager

  m_levelsManager =
    std::make_shared<LevelsManager>(m_gameWorld, m_resourceManagementModule->getResourceManager());

  m_levelsManager->getObjectsLoader().registerClassLoader("generic",
    std::make_unique<GameObjectsGenericClassLoader>(m_levelsManager));

  // Game application systems
  m_gameApplicationSystems = std::make_shared<GameSystemsGroup>();
  m_engineGameSystems->addGameSystem(m_gameApplicationSystems);

  // Game console
  m_gameConsole = std::make_shared<GameConsole>(m_gameWorld);

  std::shared_ptr<GUIConsole> guiConsole = std::make_shared<GUIConsole>(
    m_gameConsole,
    20,
    m_guiSystem->getDefaultFont());
  guiConsole->setName("engine_builtin_console");

  m_gameConsole->setGUIConsole(guiConsole);

  guiConsole->setWidth(m_guiSystem->getScreenWidth());

  guiConsole->setZIndex(10);
  guiConsole->hide();

  m_screenManager->getCommonGUILayout()->addChildWidget(guiConsole);
  m_inputModule->registerAction("console", KeyboardInputAction(SDLK_BACKQUOTE));

  m_inputModule->registerAction("unlimited_framerate", KeyboardInputAction(SDLK_RALT));

  m_gameConsole->print("Engine is initialized...");
}

void BaseGameApplication::performLoad()
{
  load();
}

void BaseGameApplication::performUnload()
{
  unload();

  m_screenManager.reset();

  SDL_Quit();
}

void BaseGameApplication::performUpdate(float delta)
{
  m_gameWorld->update(delta);
  m_screenManager->update(delta);

  update(delta);
}

void BaseGameApplication::performRender()
{
  GLGraphicsContext* graphicsContext = m_graphicsModule->getGraphicsContext().get();

  m_sharedGraphicsState->getFrameStats().reset();

  m_gameWorld->beforeRender();

  m_gameWorld->render();
  m_screenManager->render();

  render();

  m_gameWorld->afterRender();
  graphicsContext->swapBuffers();
}

void BaseGameApplication::handleAppTerminate()
{
  auto trace = boost::stacktrace::stacktrace();
  std::string traceRepresentation = boost::stacktrace::detail::to_string(&trace.as_vector()[0], trace.size());

  spdlog::critical("The application crashed");
  spdlog::debug(traceRepresentation);

  std::abort();
}

std::shared_ptr<GameSystemsGroup> BaseGameApplication::getGameApplicationSystemsGroup() const
{
  return m_gameApplicationSystems;
}
