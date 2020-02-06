#include "precompiled.h"
#pragma hdrstop

#include "BaseGameApplication.h"

#include <Exceptions/EngineRuntimeException.h>
#include <spdlog/spdlog.h>

#include "Modules/Graphics/GUI/GUIConsole.h"

#include "Modules/Graphics/Resources/ShaderResource.h"
#include "Modules/Graphics/Resources/MeshResource.h"
#include "Modules/Graphics/Resources/TextureResource.h"
#include "Modules/Graphics/Resources/BitmapFontResource.h"
#include "Modules/Graphics/Resources/MaterialResource.h"
#include "Modules/Graphics/Resources/SkeletonResource.h"
#include "Modules/Graphics/Resources/SkeletalAnimationResource.h"

BaseGameApplication::BaseGameApplication(int argc, char* argv[], const std::string& windowTitle, int windowWidth, int windowHeight)
    : m_mainWindow(nullptr)
{
    spdlog::set_level(spdlog::level::debug);

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

    unsigned long nextTick = GetTickCount();

    long sleepTime = 0;

    SDL_Event event;

    spdlog::info("Starting main loop...");

    m_isMainLoopActive = true;

    while (m_isMainLoopActive) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT || (event.type == SDL_WINDOWEVENT &&
                event.window.event == SDL_WINDOWEVENT_CLOSE))
            {
                m_isMainLoopActive = false;
                break;
            }

            m_inputModule->processRawSDLEvent(event);
        }

        if (!m_isMainLoopActive)
            break;

        performUpdate(1.0f / FRAMES_PER_SECOND);

        if (!m_isMainLoopActive)
            break;

        performRender();

        bool isFrameRateUnlimited = m_inputModule->isActionActive("unlimited_framerate");

        if (!isFrameRateUnlimited) {
            nextTick += SKIP_TICKS;
            sleepTime = static_cast<long>(nextTick) - static_cast<long>(GetTickCount());

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

        return EventProcessStatus::Prevented;
    }

    return EventProcessStatus::Processed;
}

EventProcessStatus BaseGameApplication::receiveEvent(GameWorld* gameWorld, const InputActionToggleEvent& event)
{
    ARG_UNUSED(gameWorld);

    if (event.actionName == "console" && event.newState == InputActionState::Active) {
        if (m_gameConsole->getGUIConsole()->isShown()) {
            m_gameConsole->getGUIConsole()->hide();
        }
        else {
            m_gameConsole->getGUIConsole()->show();
        }
    }

    return EventProcessStatus::Processed;
}

void BaseGameApplication::shutdown()
{
    m_isMainLoopActive = false;
}

void BaseGameApplication::initializePlatform(int argc, char* argv[], const std::string& windowTitle, int windowWidth, int windowHeight)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    int initStatus = SDL_Init(SDL_INIT_EVERYTHING);

    if (initStatus != 0) {
        ENGINE_RUNTIME_ERROR(std::string(SDL_GetError()));
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
        ENGINE_RUNTIME_ERROR(std::string(SDL_GetError()));
    }

    spdlog::info("Window is created");
}

void BaseGameApplication::initializeEngine()
{
    m_inputModule = std::make_shared<InputModule>(m_mainWindow);

    m_graphicsModule = std::make_shared<GraphicsModule>(m_mainWindow);
    m_sharedGraphicsState = std::make_shared<SharedGraphicsState>(m_graphicsModule->getGraphicsContext());

    m_resourceManagementModule = std::make_shared<ResourceManagementModule>();

    std::shared_ptr<ResourceManager> resourceManager = m_resourceManagementModule->getResourceManager();
    resourceManager->declareResourceType<ShaderResource>("shader");
    resourceManager->declareResourceType<MeshResource>("mesh");
    resourceManager->declareResourceType<TextureResource>("texture");
    resourceManager->declareResourceType<BitmapFontResource>("bitmap_font");
    resourceManager->declareResourceType<MaterialResource>("material");
    resourceManager->declareResourceType<SkeletonResource>("skeleton");
    resourceManager->declareResourceType<SkeletalAnimationResource>("animation");

    resourceManager->addResourcesMap("../resources/engine_resources.xml");

    m_gameWorld = std::make_shared<GameWorld>();
    m_screenManager = std::make_shared<ScreenManager>(m_gameWorld, m_graphicsModule,
                                                      m_sharedGraphicsState, resourceManager);

    DebugPainter::initialize(m_resourceManagementModule->getResourceManager(), m_sharedGraphicsState);
}

void BaseGameApplication::initializeEngineSystems()
{
    std::shared_ptr<ResourceManager> resourceManager = m_resourceManagementModule->getResourceManager();

    m_gameWorld->setGameSystemsGroup(std::make_unique<GameSystemsGroup>(m_gameWorld));

    std::shared_ptr<GameSystemsGroup> engineGameSystems = std::make_shared<GameSystemsGroup>(m_gameWorld);
    m_gameWorld->getGameSystemsGroup()->addGameSystem(engineGameSystems);

    // Input system
    m_inputSystem = std::make_shared<InputSystem>(m_gameWorld, m_inputModule);
    engineGameSystems->addGameSystem(m_inputSystem);

    m_gameWorld->subscribeEventsListener<GameConsoleCommandEvent>(this);
    m_gameWorld->subscribeEventsListener<InputActionToggleEvent>(this);

    // Skeletal animation system
    auto skeletalAnimationSystem = std::make_shared<SkeletalAnimationSystem>();
    engineGameSystems->addGameSystem(skeletalAnimationSystem);

    // Rendering pipeline
    m_renderingSystemsPipeline = std::make_shared<RenderingSystemsPipeline>(m_gameWorld,
        m_graphicsModule->getGraphicsContext(), m_sharedGraphicsState);

    engineGameSystems->addGameSystem(m_renderingSystemsPipeline);

    // Geometry culling
    auto geometryCullingSystem = std::make_shared<GeometryCullingSystem>(m_graphicsModule->getGraphicsContext(),
        m_sharedGraphicsState);

    m_renderingSystemsPipeline->addGameSystem(geometryCullingSystem);

    // Mesh rendering system
    auto meshRenderingSystem = std::make_shared<MeshRenderingSystem>(m_graphicsModule->getGraphicsContext(),
                                                                  m_sharedGraphicsState);
    m_renderingSystemsPipeline->addGameSystem(meshRenderingSystem);

    // Environment rendering
    auto environmentRenderingSystem = std::make_shared<EnvironmentRenderingSystem>(m_graphicsModule->getGraphicsContext(),
        m_sharedGraphicsState,
        resourceManager->getResourceFromInstance<MeshResource>("mesh_identity_sphere")->getMesh());

    m_renderingSystemsPipeline->addGameSystem(environmentRenderingSystem);

    // GUI system
    std::shared_ptr<GLShader> guiVertexShader = resourceManager->
            getResourceFromInstance<ShaderResource>("gui_vertex_shader")->getShader();

    std::shared_ptr<GLShader> guiFragmentShader = resourceManager->
            getResourceFromInstance<ShaderResource>("gui_fragment_shader")->getShader();

    std::shared_ptr<GLShadersPipeline> guiShadersPipeline = std::make_shared<GLShadersPipeline>(
                guiVertexShader, guiFragmentShader, nullptr);

    auto guiSystem = std::make_shared<GUISystem>(m_gameWorld, m_inputModule,
        m_graphicsModule->getGraphicsContext(), guiShadersPipeline);

    std::shared_ptr<BitmapFont> guiDefaultFont = resourceManager->
            getResourceFromInstance<BitmapFontResource>("gui_default_font")->getFont();
    guiSystem->setDefaultFont(guiDefaultFont);
    guiSystem->setActiveLayout(m_screenManager->getCommonGUILayout());

    engineGameSystems->addGameSystem(guiSystem);

    // Game console
    m_gameConsole = std::make_shared<GameConsole>(m_gameWorld);

    std::shared_ptr<GUIConsole> guiConsole = std::make_shared<GUIConsole>(m_gameConsole, 20, guiSystem->getDefaultFont());
    m_gameConsole->setGUIConsole(guiConsole);

    glm::vec4 guiConsoleBackgroundColor = { 0.168f, 0.172f, 0.25f, 0.8f };

    guiConsole->setBackgroundColor(guiConsoleBackgroundColor);
    guiConsole->setHoverBackgroundColor(guiConsoleBackgroundColor);
    guiConsole->setWidth(guiSystem->getScreenWidth());

    glm::vec4 guiConsoleTextBoxBackgroundColor = { 0.118f, 0.112f, 0.15f, 1.0f };

    guiConsole->getTextBox()->setBackgroundColor(guiConsoleTextBoxBackgroundColor);
    guiConsole->getTextBox()->setHoverBackgroundColor(guiConsoleTextBoxBackgroundColor);
    guiConsole->getTextBox()->setFocusBackgroundColor(guiConsoleTextBoxBackgroundColor);
    guiConsole->getTextBox()->setTextColor({ 1.0f, 1.0f, 1.0f, 1.0f });
    guiConsole->getTextBox()->setTextHoverColor({ 1.0f, 1.0f, 1.0f, 1.0f });
    guiConsole->getTextBox()->setTextFontSize(9);

    guiConsole->setTextFontSize(9);
    guiConsole->setTextColor({ 1.0f, 1.0f, 1.0f, 1.0f });
    guiConsole->setTextHoverColor({ 1.0f, 1.0f, 1.0f, 1.0f });

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
