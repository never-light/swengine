#include "StartScene.h"

#include <Engine\Utils\time.h>
#include <Engine\Components\Math\Random.h>
#include <Engine\Utils\string.h>
#include <Engine\Utils\io.h>

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
	m_levelRenderer(nullptr),
	m_phongLightingBaseMaterial(nullptr)
{
	loadResources();

	m_levelRenderer = new LevelRenderer(graphicsContext, graphicsResourceFactory, m_deferredLightingProgram);

	initializeSceneObjects();

	console->registerCommandHandler("set_camera", 
		std::bind(&StartScene::changeCameraCommandHandler, this, std::placeholders::_1, std::placeholders::_2));

	console->registerCommandHandler("gamma",
		std::bind(&StartScene::changeGammaCorrectionCommandHandler, this, std::placeholders::_1, std::placeholders::_2));

	console->registerCommandHandler("pos",
		std::bind(&StartScene::pickPositionCommandHandler, this, std::placeholders::_1, std::placeholders::_2));

	console->registerCommandHandler("dir",
		std::bind(&StartScene::pickDirectionCommandHandler, this, std::placeholders::_1, std::placeholders::_2));

}

StartScene::~StartScene()
{
	for (Light* light : m_lights)
		delete light;

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

	if (minDistance >= 1.4f) {
		vector3 oldPosition = m_player->getTransform()->getPosition();

		oldPosition.y -= 0.30f;
		oldPosition.y = std::max(oldPosition.y, 1.4f);

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
	m_deferredLightingProgram = m_resourceManager->load<GpuProgram>("resources/shaders/deferred_lighting.fx");
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
	m_player->getTransform()->setPosition(-0.25916, 1.40000, 0.4456);
	m_player->getTransform()->setOrientation(quaternion(-0.6608, 0.22277, 0.67916, 0.22895));
	//m_player->getTransform()->lookAt(0.0, 0.0f, 0.0f);
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

	// Initialize lights
	Light* whileLight = new Light(Light::Type::Point);
	whileLight->setColor(vector3(1.0f, 1.0f, 1.0f));
	whileLight->setAmbientIntensity(0.002f);
	whileLight->setDiffuseIntensity(0.08f);
	whileLight->setPosition(vector3(-0.76580, 1.4, -1.0655));

	whileLight->setAttenuation(1.0f, 0.8f, 0.8f);

	m_lights.push_back(whileLight);

	// Register objects in level renderer
	m_levelRenderer->registerBaseMaterial(m_phongLightingBaseMaterial);

	m_levelRenderer->addRenderableObject(m_level);
	m_levelRenderer->addRenderableObject(m_player);

	for (Light* lightSource : m_lights)
		m_levelRenderer->registerLightSource(lightSource);

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

void StartScene::changeGammaCorrectionCommandHandler(Console * console, const std::vector<std::string>& args)
{
	if (args.empty()) {
		console->print(std::to_string(m_levelRenderer->getGamma()));

		return;
	}

	if (args.front() == "on")
		m_levelRenderer->enableGammaCorrection();
	else if (args.front() == "off")
		m_levelRenderer->disableGammaCorrection();
	else
		m_levelRenderer->setGamma(std::stof(args.front()));
}

void StartScene::pickPositionCommandHandler(Console * console, const std::vector<std::string>& args)
{
	vector3 position = m_player->getPosition();

	std::string result = StringUtils::format("%.5f, %.5f, %.5f", position.x, position.y, position.z);

	console->print(result);
	IOUtils::copyToClipboard(result);
}

void StartScene::pickDirectionCommandHandler(Console * console, const std::vector<std::string>& args)
{
	quaternion orientation = m_player->getTransform()->getOrientation();

	std::string result = StringUtils::format("%.5f, %.5f, %.5f, %.5f", orientation.x, orientation.y, orientation.z, orientation.w);

	console->print(result);
	IOUtils::copyToClipboard(result);
}
