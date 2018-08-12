#include "StartScene.h"

#include <Engine\Utils\time.h>

#include <iostream>

StartScene::StartScene(GraphicsContext* graphicsContext, 
	GraphicsResourceFactory* graphicsResourceFactory, 
	ResourceManager* resourceManager, 
	InputManager* inputManager,
	Console* console)
	: Scene(graphicsContext, resourceManager),
	m_inputManager(inputManager),
	m_graphicsResourceFactory(graphicsResourceFactory),
	m_level(nullptr),
	m_activeInputController(nullptr),
	m_levelRenderer(new LevelRenderer(graphicsContext, graphicsResourceFactory)),
	m_phongLightingBaseMaterial(nullptr)
{
	loadResources();
	initializeSceneObjects();

	console->registerCommandHandler("set_camera", 
		std::bind(&StartScene::changeCameraCommandHandler, this, std::placeholders::_1, std::placeholders::_2));
}

StartScene::~StartScene()
{
	if (m_phongLightingBaseMaterial != nullptr)
		delete m_phongLightingBaseMaterial;

	delete m_levelRenderer;

	delete m_level;

	delete m_playerController;
	delete m_freeCameraController;
	delete m_player;

	delete m_boxPrimitive;
}

void StartScene::update()
{
	m_activeInputController->update();

	OBB newPlayerObb = m_player->getWorldPlacedCollider();

	Intersection intersection;
	for (const OBB& obb : m_levelMesh->getColliders()) {
		if (newPlayerObb.intersects(obb, intersection)) {
			m_player->getTransform()->move(intersection.getDirection() * intersection.getDepth());
		}
	}

	Ray downRay(m_player->getTransform()->getPosition(), vector3(0.0f, -1.0f, 0.0f));
	float distance = 0.0f;

	float minDistance = std::numeric_limits<float>::infinity();
	for (const OBB& obb : m_levelMesh->getColliders()) {
		bool isFloorIntersects = obb.intersects(downRay, distance);

		if (isFloorIntersects) {
			minDistance = std::min(minDistance, distance);
		}
	}

	if (minDistance >= 1.7f) {
		vector3 oldPosition = m_player->getTransform()->getPosition();

		oldPosition.y -= 0.30f;
		oldPosition.y = std::max(oldPosition.y, 1.7f);

		m_player->getTransform()->setPosition(oldPosition);
	}


	Bone* head = m_player->getSkeleton()->getBone("HumanHead");

	const matrix4& headBoneLocal = head->getCurrentPoseTransform();
	vector3 boneWorldPosition = m_player->getTransform()->getTransformationMatrix() * vector4(vector3(headBoneLocal[3]), 1.0f);

	m_playerCamera->getTransform()->setOrientation(m_player->getTransform()->getOrientation());
	m_playerCamera->getTransform()->setPosition(boneWorldPosition);
}

void StartScene::render()
{
	m_levelRenderer->render();
}

void StartScene::setActiveCamera(Camera * camera)
{
	Scene::setActiveCamera(camera);
	m_levelRenderer->setActiveCamera(camera);
}

void StartScene::loadResources()
{
	m_lightingGpuProgram = m_resourceManager->load<GpuProgram>("resources/shaders/phong.fx");
	m_boundingVolumeGpuProgram = m_resourceManager->load<GpuProgram>("resources/shaders/bounding_volume.fx");

	m_levelMesh = m_resourceManager->load<SolidMesh>("resources/models/level.mod", "meshes_level");
	m_playerMesh = m_resourceManager->load<SolidMesh>("resources/models/player/arms.mod", "meshes_player_arms");

	// Player animations
	m_resourceManager->load<Animation>("resources/animations/player/arms_idle.anim", "animations_player_arms_idle");
	m_resourceManager->load<Animation>("resources/animations/player/arms_running.anim", "animations_player_arms_running");
	m_resourceManager->load<Animation>("resources/animations/player/arms_taking.anim", "animations_player_arms_taking");
}

void StartScene::initializeSceneObjects() {
	m_phongLightingBaseMaterial = new PhongLightingMaterial("materials.phong_base", m_lightingGpuProgram);

	m_level = new SolidGameObject(m_levelMesh, m_phongLightingBaseMaterial);
	registerSceneObject(m_level);

	// Player initialization
	m_player = new Player(m_playerMesh, m_phongLightingBaseMaterial);
	m_player->getTransform()->setPosition(4.27676010, 11.4990520, -7.80581093);
	registerSceneObject(m_player);

	m_playerCamera = createCamera("playerCamera");
	setActiveCamera(m_playerCamera);

	m_playerCamera->setNearClipDistance(0.1f);
	m_playerCamera->setFarClipDistance(300.0f);
	m_playerCamera->setFOVy(45.0f);
	m_playerCamera->setAspectRatio((float)m_graphicsContext->getViewportWidth() / m_graphicsContext->getViewportHeight());
	m_playerCamera->getTransform()->fixYAxis();

	Animation* playerArmsIdle = m_resourceManager->getResource<Animation>("animations_player_arms_idle");
	playerArmsIdle->setEndBehaviour(Animation::EndBehaviour::Repeat);

	Animation* playerArmsRunning = m_resourceManager->getResource<Animation>("animations_player_arms_running");
	playerArmsRunning->setSpeedFactor(2.0f);
	playerArmsRunning->setEndBehaviour(Animation::EndBehaviour::Repeat);

	Animation* playerArmsTaking = m_resourceManager->getResource<Animation>("animations_player_arms_taking");
	playerArmsTaking->setSpeedFactor(4.0f);
	playerArmsTaking->setEndBehaviour(Animation::EndBehaviour::Stop);

	std::vector<Animation*> playerAnimations(PlayerController::PLAYER_STATES_COUNT);
	playerAnimations[(size_t)PlayerController::PlayerState::Idle] = playerArmsIdle;
	playerAnimations[(size_t)PlayerController::PlayerState::Running] = playerArmsRunning;
	playerAnimations[(size_t)PlayerController::PlayerState::Taking] = playerArmsTaking;

	m_playerController = new PlayerController(m_player, m_playerCamera, m_inputManager, playerAnimations);
	m_playerController->setMovementSpeed(0.15f);

	m_activeInputController = m_playerController;

	// Free camera controller
	m_freeCamera = createCamera("freeCamera");
	m_freeCamera->setNearClipDistance(0.1f);
	m_freeCamera->setFarClipDistance(300.0f);
	m_freeCamera->setFOVy(45.0f);
	m_freeCamera->setAspectRatio((float)m_graphicsContext->getViewportWidth() / m_graphicsContext->getViewportHeight());

	m_freeCamera->getTransform()->fixYAxis();
	m_freeCamera->getTransform()->setPosition(0.0f, 0.0f, 0.0f);
	m_freeCamera->getTransform()->lookAt(0.0f, 0.0f, 10.0f);

	m_freeCameraController = new FreeCameraController(m_freeCamera, m_inputManager);

	initializePrimitives();

	// Register objects in level renderer
	m_levelRenderer->registerBaseMaterial(m_phongLightingBaseMaterial);

	m_levelRenderer->addRenderableObject(m_level);
	m_levelRenderer->addRenderableObject(m_player);
}

void StartScene::initializePrimitives()
{
	m_boxPrimitive = new BoxPrimitive(m_graphicsResourceFactory);
}

void StartScene::changeCameraCommandHandler(Console * console, const std::vector<std::string>& args)
{
	if (args.empty())
		return;

	if (args.front() == "free") {
		m_activeInputController = m_freeCameraController;
		setActiveCamera(m_freeCamera);
	}
	else if (args.front() == "fps") {
		m_activeInputController = m_playerController;
		setActiveCamera(m_playerCamera);
	}
	else
		console->print("Unknown camera type");
}
