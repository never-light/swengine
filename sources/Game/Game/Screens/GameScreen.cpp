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

#include "Game/PlayerComponent.h"

GameScreen::GameScreen(std::shared_ptr<InputModule> inputModule)
    : BaseGameScreen(GameScreenType::Game),
      m_inputModule(inputModule),
      m_playerControlSystem(std::make_shared<PlayerControlSystem>(inputModule))
{
}

void GameScreen::activate()
{
    m_inputModule->registerAction("forward", KeyboardInputAction(SDLK_w));
    m_inputModule->registerAction("backward", KeyboardInputAction(SDLK_s));
    m_inputModule->registerAction("left", KeyboardInputAction(SDLK_a));
    m_inputModule->registerAction("right", KeyboardInputAction(SDLK_d));

    m_gameWorld->addGameSystem(m_playerControlSystem);
}

void GameScreen::deactivate()
{
    m_gameWorld->removeGameSystem(m_playerControlSystem);

    m_inputModule->unregisterAction("forward");
    m_inputModule->unregisterAction("backward");
    m_inputModule->unregisterAction("left");
    m_inputModule->unregisterAction("right");
}

void GameScreen::load()
{
    initializeGame();
    initializeGUI();
}

void GameScreen::unload()
{
    deinitializeGUI();
    deinitializeGame();
}

void GameScreen::update(float delta)
{
    ARG_UNUSED(delta);
}

void GameScreen::render()
{

}

void GameScreen::initializeGame()
{
    GameObject* player = m_gameWorld->createGameObject();
    player->addComponent<TransformComponent>();
    player->addComponent<CameraComponent>();
    player->addComponent<PlayerComponent>();

    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    camera->setNearClipDistance(0.1f);
    camera->setFarClipDistance(100.0f);
    camera->setAspectRatio(static_cast<float>(m_graphicsModule->getGraphicsContext()->getBufferWidth())
                           / m_graphicsModule->getGraphicsContext()->getBufferHeight());
    camera->setFOVy(glm::pi<float>() / 4);

    camera->getTransform()->setPosition(0, 0, 0);
    camera->getTransform()->fixYAxis();
    camera->getTransform()->lookAt(0, 0, 1);

    player->getComponent<CameraComponent>()->setCamera(camera);
    m_sharedGraphicsState->setActiveCamera(camera);

    // Game objects

    std::shared_ptr<GLShadersPipeline> phongPipeline = std::make_shared<GLShadersPipeline>(
                m_resourceManager->getResourceFromInstance<ShaderResource>("phong_vertex_shader")->getShader(),
                m_resourceManager->getResourceFromInstance<ShaderResource>("phong_fragment_shader")->getShader(), nullptr);

    std::shared_ptr<Material> material = std::make_shared<Material>();
    material->setShadersPipeline(phongPipeline);

    std::shared_ptr<GLTexture> texture = m_resourceManager->getResourceFromInstance<TextureResource>("simple_texture")->getTexture();
    material->getShadersPipeline()->getShader(GL_FRAGMENT_SHADER)->setParameter("tex", *texture.get(), 0);

    GameObject* obj = m_gameWorld->createGameObject();
    auto transformHandle = obj->addComponent<TransformComponent>();

    transformHandle->getTransform()->move(0.0f, 0.3f, 0.0f);

    std::shared_ptr<Mesh> cubeGeometry = m_resourceManager->getResourceFromInstance<MeshResource>("simple_mesh")->getMesh();

    auto componentHandle = obj->addComponent<MeshRendererComponent>();
    componentHandle->setMeshInstance(cubeGeometry);
    componentHandle->setMaterialsInstances({ material });

}

void GameScreen::deinitializeGame()
{
}

void GameScreen::initializeGUI()
{
    std::shared_ptr<GUISystem> guiSystem = m_gameWorld->getGameSystem<GUISystem>();

    std::shared_ptr<GUIText> text = std::make_shared<GUIText>(guiSystem->getDefaultFont(),
        "Hello, world!\nText rendering is working now!");

    text->setColor({ 1.0f, 0.0f, 0.0f, 1.0f });
    text->setFontSize(12);

    text->setOrigin({ 50, 50 });

    getGUILayout()->addChildWidget(text);

    std::shared_ptr<GUITextBox> textBox = std::make_shared<GUITextBox>(guiSystem->getDefaultFont());
    textBox->setOrigin({ 400, 300 });
    textBox->setSize({ 400, 30 });
    textBox->setText("Hello, world!");
    textBox->setBackgroundColor({ 1.0f, 1.0f, 1.0f, 1.0f });
    textBox->setHoverBackgroundColor({ 1.0f, 0.8f, 0.8f, 1.0f });
    textBox->setFocusBackgroundColor({ 1.0f, 0.5f, 0.5f, 1.0f });
    textBox->setTextFontSize(9);

    getGUILayout()->addChildWidget(textBox);
}

void GameScreen::deinitializeGUI()
{
    getGUILayout()->removeChildren();
}
