#include "StartScene.h"

StartScene::StartScene(GraphicsContext* graphicsContext, ResourceManager* resourceManager, InputManager* inputManager) 
	: Scene(graphicsContext, resourceManager),
	m_inputManager(inputManager),
	m_level(nullptr)
{
	loadResources();
	initializeSceneObjects();
}

StartScene::~StartScene()
{
	delete m_level;

	delete m_playerController;
	delete m_player;
}

void StartScene::update()
{
	m_playerController->update();
}

void StartScene::render()
{
	m_graphicsContext->enableDepthTest();
	m_lightingGpuProgram->bind();

	m_lightingGpuProgram->setParameter("scene.viewTransform", getActiveCamera()->getViewMatrix());
	m_lightingGpuProgram->setParameter("scene.projectionTransform", getActiveCamera()->getProjectionMatrix());

	m_graphicsContext->enableFaceCulling();
	m_graphicsContext->setFaceCullingMode(GraphicsContext::FaceCullingMode::Back);

	m_level->render(m_lightingGpuProgram);
	m_player->render(m_lightingGpuProgram);
}

void StartScene::loadResources()
{
	m_lightingGpuProgram = m_resourceManager->load<GpuProgram>("resources/shaders/phong.fx");

	m_resourceManager->load<SolidMesh>("resources/models/level.mod", "meshes_level");
	m_resourceManager->load<SolidMesh>("resources/models/hands.mod", "meshes_hands");
}

void StartScene::initializeSceneObjects()
{
	m_level = new SolidGameObject(m_resourceManager->getResource<SolidMesh>("meshes_level"));
	this->registerSceneObject(m_level);

	m_player = new Player(m_resourceManager->getResource<SolidMesh>("meshes_hands"));
	this->registerSceneObject(m_player);

	m_playerCamera = createCamera("playerCamera");
	setActiveCamera(m_playerCamera);

	m_playerCamera->setNearClipDistance(0.1f);
	m_playerCamera->setFarClipDistance(300.0f);
	m_playerCamera->setFOVy(45.0f);
	m_playerCamera->setAspectRatio((float)m_graphicsContext->getViewportWidth() / m_graphicsContext->getViewportHeight());

	m_playerCamera->getTransform()->fixYAxis();
	m_playerCamera->getTransform()->setPosition(0, 0, 0);
	m_playerCamera->getTransform()->lookAt(0, 0, 10);

	m_playerController = new PlayerController(m_player, m_playerCamera, m_inputManager);
}
