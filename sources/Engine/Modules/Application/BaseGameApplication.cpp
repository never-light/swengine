#include "precompiled.h"

#pragma hdrstop

#include "BaseGameApplication.h"

#include <Exceptions/exceptions.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <glm/gtx/string_cast.hpp>

#include "Modules/Graphics/GUI/GUIConsole.h"

#include "Modules/Graphics/Resources/ShaderResourceManager.h"
#include "Modules/Graphics/Resources/MeshResourceManager.h"
#include "Modules/Graphics/Resources/TextureResourceManager.h"
#include "Modules/Graphics/Resources/BitmapFontResourceManager.h"
#include "Modules/Graphics/Resources/MaterialResourceManager.h"
#include "Modules/Graphics/Resources/SkeletonResourceManager.h"
#include "Modules/Graphics/Resources/SkeletalAnimationResourceManager.h"
#include "Modules/Graphics/Resources/AnimationStatesMachineResourceManager.h"

#include "Modules/Physics/Resources/CollisionShapeResourceManager.h"
#include "Modules/Audio/Resources/AudioClipResourceManager.h"

#include "Modules/LevelsManagement/GameObjectsGenericClassLoader.h"

#include "StartupSettings.h"

BaseGameApplication::BaseGameApplication(int argc,
  char* argv[],
  const std::string& windowTitle)
  : m_mainWindow(nullptr)
{
  spdlog::set_level(spdlog::level::debug);
  std::set_terminate([]() { BaseGameApplication::handleAppTerminate(); });

  spdlog::info("Application start...");
  initializePlatform(argc, argv, windowTitle);
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
  SDL_Quit();
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

EventProcessStatus BaseGameApplication::receiveEvent(const GameConsoleCommandEvent& event)
{
  if (event.command == "exit") {
    shutdown();

    return EventProcessStatus::Processed;
  }
  else if (event.command == "debug-draw-physics") {
    m_physicsSystem->enableDebugDrawing(!m_physicsSystem->isDebugDrawingEnabled());

    return EventProcessStatus::Processed;
  }
  else if (event.command == "debug-draw-bounds") {
    m_meshRenderingSystem->enableBoundsRendering(!m_meshRenderingSystem->isBoundsRenderingEnabled());

    return EventProcessStatus::Processed;
  }
  else if (event.command == "debug-draw-skeletons") {
    m_meshRenderingSystem->enableSkeletonsRendering(!m_meshRenderingSystem->isSkeletonsRenderingEnabled());

    return EventProcessStatus::Processed;
  }
  else if (event.command == "camera-position") {
    m_gameConsole->print("Position: " +
      glm::to_string(m_graphicsScene->getActiveCamera()->getTransform()->getPosition()));

    m_gameConsole->print("Direction: " +
      glm::to_string(m_graphicsScene->getActiveCamera()->getTransform()->getFrontDirection()));

    return EventProcessStatus::Processed;
  }

  return EventProcessStatus::Skipped;
}

EventProcessStatus BaseGameApplication::receiveEvent(const InputActionToggleEvent& event)
{
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
  const std::string& windowTitle)
{
  ARG_UNUSED(argc);
  ARG_UNUSED(argv);

  StartupSettings startupSettings = StartupSettings::loadFromFile();

  int initStatus = SDL_Init(SDL_INIT_EVERYTHING & (~SDL_INIT_SENSOR));

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

  int windowWidth = 0;
  int windowHeight = 0;

  switch (startupSettings.getScreenDimension()) {
    case StartupOptionScreenDimension::_1280x720:
      windowWidth = 1280;
      windowHeight = 720;
      break;

    case StartupOptionScreenDimension::_1366x768:
      windowWidth = 1366;
      windowHeight = 768;
      break;

    case StartupOptionScreenDimension::_1440x900:
      windowWidth = 1440;
      windowHeight = 900;
      break;

    default:
      SW_ASSERT(false);
  }

  Uint32 windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN;

  switch (startupSettings.getScreenMode()) {
    case StartupOptionScreenMode::Windowed:
      break;

    case StartupOptionScreenMode::Fullscreen:
      windowFlags |= SDL_WINDOW_FULLSCREEN;
      break;

    default:
      SW_ASSERT(false);
  }

  m_mainWindow = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    windowWidth, windowHeight, windowFlags);

  if (m_mainWindow == nullptr) {
    THROW_EXCEPTION(EngineRuntimeException, std::string(SDL_GetError()));
  }

  spdlog::info("Window is created");
}

void BaseGameApplication::initializeEngine()
{
  m_inputModule = std::make_shared<InputModule>(m_mainWindow);

  m_graphicsModule = std::make_shared<GraphicsModule>(m_mainWindow);
  m_graphicsScene = std::make_shared<GraphicsScene>();

  m_graphicsModule->getGraphicsContext()->setupGraphicsScene(m_graphicsScene);

  m_resourceManagementModule = std::make_shared<ResourceManagementModule>();

  std::shared_ptr<ResourcesManager> resourceManager = m_resourceManagementModule->getResourceManager();
  resourceManager->registerResourceType<GLShader>("shader",
    std::make_unique<ShaderResourceManager>(resourceManager.get()));
  resourceManager->registerResourceType<Mesh>("mesh",
    std::make_unique<MeshResourceManager>(resourceManager.get()));
  resourceManager->registerResourceType<GLTexture>("texture",
    std::make_unique<TextureResourceManager>(resourceManager.get()));
  resourceManager->registerResourceType<BitmapFont>("bitmap_font",
    std::make_unique<BitmapFontResourceManager>(resourceManager.get()));
  resourceManager->registerResourceType<GLMaterial>("material",
    std::make_unique<MaterialResourceManager>(resourceManager.get()));
  resourceManager->registerResourceType<Skeleton>("skeleton",
    std::make_unique<SkeletonResourceManager>(resourceManager.get()));
  resourceManager->registerResourceType<AnimationClip>("animation",
    std::make_unique<SkeletalAnimationResourceManager>(resourceManager.get()));
  resourceManager->registerResourceType<AnimationStatesMachine>("animation_states_machine",
    std::make_unique<AnimationStatesMachineResourceManager>(resourceManager.get()));
  resourceManager->registerResourceType<CollisionShape>("collision",
    std::make_unique<CollisionShapeResourceManager>(resourceManager.get()));
  resourceManager->registerResourceType<AudioClip>("audio",
    std::make_unique<AudioClipResourceManager>(resourceManager.get()));

  resourceManager->loadResourcesMapFile("../resources/engine_resources.xml");

  m_gameWorld = GameWorld::createInstance();
  m_gameWorld->registerComponentBinderFactory<TransformComponent>(
    std::make_shared<GameObjectsComponentsGenericBindersFactory<TransformComponent, TransformComponentBinder>>());
  m_gameWorld->registerComponentBinderFactory<AudioSourceComponent>(
    std::make_shared<GameObjectsComponentsGenericBindersFactory<AudioSourceComponent,
                                                                AudioSourceComponentBinder,
                                                                GameObjectsComponentsBinderInjectParameters::ResourcesManager>>(
      resourceManager));
  m_gameWorld->registerComponentBinderFactory<CameraComponent>(
    std::make_shared<GameObjectsComponentsGenericBindersFactory<CameraComponent, CameraComponentBinder>>());
  m_gameWorld->registerComponentBinderFactory<EnvironmentComponent>(
    std::make_shared<GameObjectsComponentsGenericBindersFactory<EnvironmentComponent,
                                                                EnvironmentComponentBinder,
                                                                GameObjectsComponentsBinderInjectParameters::ResourcesManager>>(
      resourceManager));
  m_gameWorld->registerComponentBinderFactory<ObjectSceneNodeComponent>(
    std::make_shared<GameObjectsComponentsGenericBindersFactory<ObjectSceneNodeComponent,
                                                                ObjectSceneNodeComponentBinder>>());
  m_gameWorld->registerComponentBinderFactory<MeshRendererComponent>(
    std::make_shared<GameObjectsComponentsGenericBindersFactory<MeshRendererComponent,
                                                                MeshRendererComponentBinder,
                                                                GameObjectsComponentsBinderInjectParameters::ResourcesManager>>(
      resourceManager));
  m_gameWorld->registerComponentBinderFactory<SkeletalAnimationComponent>(
    std::make_shared<GameObjectsComponentsGenericBindersFactory<SkeletalAnimationComponent,
                                                                AnimationComponentBinder,
                                                                GameObjectsComponentsBinderInjectParameters::ResourcesManager>>(
      resourceManager));
  m_gameWorld->registerComponentBinderFactory<RigidBodyComponent>(
    std::make_shared<GameObjectsComponentsGenericBindersFactory<RigidBodyComponent,
                                                                RigidBodyComponentBinder,
                                                                GameObjectsComponentsBinderInjectParameters::ResourcesManager>>(
      resourceManager));
  m_gameWorld->registerComponentBinderFactory<KinematicCharacterComponent>(
    std::make_shared<GameObjectsComponentsGenericBindersFactory<KinematicCharacterComponent,
                                                                KinematicCharacterComponentBinder,
                                                                GameObjectsComponentsBinderInjectParameters::ResourcesManager>>(
      resourceManager));
  m_gameWorld->registerComponentBinderFactory<GameObjectSpawnComponent>(
    std::make_shared<GameObjectsComponentsGenericBindersFactory<GameObjectSpawnComponent,
                                                                GameObjectSpawnComponentBinder>>());

  m_screenManager = std::make_shared<ScreenManager>(
    m_gameWorld,
    m_graphicsModule,
    resourceManager,
    m_graphicsModule->getGraphicsContext()->getViewportWidth(),
    m_graphicsModule->getGraphicsContext()->getViewportHeight());

  DebugPainter::initialize(m_resourceManagementModule->getResourceManager(), m_graphicsScene);
}

void BaseGameApplication::initializeEngineSystems()
{
  std::shared_ptr<ResourcesManager> resourceManager = m_resourceManagementModule->getResourceManager();

  m_engineGameSystems = std::make_shared<GameSystemsGroup>();
  m_gameWorld->getGameSystemsGroup()->addGameSystem(m_engineGameSystems);

  // Online management system
  m_onlineManagementSystem = std::make_shared<OnlineManagementSystem>();
  m_engineGameSystems->addGameSystem(m_onlineManagementSystem);

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
  m_renderingSystemsPipeline = std::make_shared<RenderingSystemsPipeline>(m_graphicsModule->getGraphicsContext());

  m_engineGameSystems->addGameSystem(m_renderingSystemsPipeline);

  // Mesh rendering system
  m_meshRenderingSystem = std::make_shared<MeshRenderingSystem>(m_graphicsModule->getGraphicsContext(),
    m_graphicsScene);
  m_renderingSystemsPipeline->addGameSystem(m_meshRenderingSystem);

  // Environment rendering
  auto environmentRenderingSystem = std::make_shared<EnvironmentRenderingSystem>(m_graphicsModule->getGraphicsContext(),
    m_graphicsScene,
    resourceManager->getResource<Mesh>("mesh_identity_sphere"));

  m_renderingSystemsPipeline->addGameSystem(environmentRenderingSystem);

  // GUI system
  ResourceHandle<GLShader> guiVertexShader = resourceManager->
    getResource<GLShader>("gui_vertex_shader");

  ResourceHandle<GLShader> guiFragmentShader = resourceManager->
    getResource<GLShader>("gui_fragment_shader");

  std::shared_ptr<GLShadersPipeline> guiShadersPipeline = std::make_shared<GLShadersPipeline>(
    guiVertexShader, guiFragmentShader, std::optional<ResourceHandle<GLShader>>());

  m_guiSystem = std::make_shared<GUISystem>(
    m_inputModule,
    m_resourceManagementModule->getResourceManager(),
    m_graphicsModule->getGraphicsContext(),
    guiShadersPipeline);

  ResourceHandle<BitmapFont> guiDefaultFont = resourceManager->
    getResource<BitmapFont>("gui_default_font");
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

  // Spawn system
  m_spawnSystem = std::make_shared<GameObjectsSpawnSystem>(m_levelsManager);
  m_engineGameSystems->addGameSystem(m_spawnSystem);

  // Scripting system
  m_scriptingSystem = std::make_shared<ScriptingSystem>(m_gameWorld);
  m_engineGameSystems->addGameSystem(m_scriptingSystem);

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

  DebugPainter::deinitialize();

  m_spawnSystem.reset();
  m_screenManager.reset();

  m_levelsManager->unloadLevel();
  m_graphicsScene.reset();

  m_gameWorld->reset();

  m_graphicsModule->getGraphicsContext()->unloadResources();
  m_graphicsModule.reset();
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

  m_graphicsScene->getFrameStats().reset();

  m_gameWorld->beforeRender();

  m_gameWorld->render();
  m_screenManager->render();

  render();

  DebugPainter::flushRenderQueue(m_graphicsModule->getGraphicsContext().get());
  m_graphicsModule->getGraphicsContext()->executeRenderTasks();
  DebugPainter::resetRenderQueue();

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
