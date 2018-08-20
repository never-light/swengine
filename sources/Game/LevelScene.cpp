#include "LevelScene.h"

#include <Engine\Utils\time.h>
#include <Engine\Components\Math\Random.h>
#include <Engine\Utils\string.h>
#include <Engine\Utils\io.h>

#include <Game\Game\Dynamic\Book.h>
#include <Game\config.h>

#include <iostream>

LevelScene::LevelScene(GraphicsContext* graphicsContext, 
	GraphicsResourceFactory* graphicsResourceFactory, 
	ResourceManager* resourceManager, 
	InputManager* inputManager,
	GUIManager* guiManager,
	Console* console)
	: Scene(graphicsContext, resourceManager),
	m_inputManager(inputManager),
	m_graphicsResourceFactory(graphicsResourceFactory),
	m_guiManager(guiManager),
	m_level(nullptr),
	m_activeInputController(nullptr),
	m_levelRenderer(nullptr),
	m_phongLightingBaseMaterial(nullptr),
	m_gameObjectsStore(new GameObjectsStore()),
	m_levelGUILayout(new GUILayout())
{
	m_levelGUILayout->setPosition(0, 0);
	m_levelGUILayout->setSize(m_graphicsContext->getViewportWidth(), m_graphicsContext->getViewportHeight());
	m_levelGUILayout->disableBackgroundRendering();

	loadResources();

	m_levelRenderer = new LevelRenderer(graphicsContext, graphicsResourceFactory, m_deferredLightingProgram);

	m_gameObjectsStore->setRemoveObjectCallback(
		std::bind(&LevelScene::removeGameObjectCallback, this, std::placeholders::_1));

	m_gameObjectsStore->setRegisterObjectCallback(
		std::bind(&LevelScene::registerGameObjectCallback, this, std::placeholders::_1));

	m_gameObjectsStore->setRelocateObjectCallback(
		std::bind(&LevelScene::relocateGameObjectCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	console->registerCommandHandler("set_camera", 
		std::bind(&LevelScene::changeCameraCommandHandler, this, std::placeholders::_1, std::placeholders::_2));

	console->registerCommandHandler("gamma",
		std::bind(&LevelScene::changeGammaCorrectionCommandHandler, this, std::placeholders::_1, std::placeholders::_2));

	console->registerCommandHandler("pos",
		std::bind(&LevelScene::pickPositionCommandHandler, this, std::placeholders::_1, std::placeholders::_2));

	console->registerCommandHandler("dir",
		std::bind(&LevelScene::pickDirectionCommandHandler, this, std::placeholders::_1, std::placeholders::_2));

	m_timeManager = new TimeManager();
	m_timeManager->setRealTimeFactor(1000 / GAME_STATE_UPDATES_PER_SECOND);

	initializeGUI();

	initializeSceneObjects();
	initializeInfoportions();
	initializeTasks();

	startGame();
}

LevelScene::~LevelScene()
{
	delete m_winText;
	delete m_hud;

	delete m_taskManager;
	delete m_infoportionsStore;
	delete m_timeManager;

	delete m_levelGUILayout;

	delete m_gameObjectsStore;

	if (m_phongLightingBaseMaterial != nullptr)
		delete m_phongLightingBaseMaterial;

	delete m_levelRenderer;
	delete m_playerController;
	delete m_freeCameraController;

	delete m_boxPrimitive;
}

void LevelScene::update() {
	if (m_hud->isControlLocked())
		return;
	
	if (m_player->getPosition().z < -5.5f && !m_infoportionsStore->hasInfoportion("task_intro_room_leaved")) {
		m_infoportionsStore->addInfoportion("task_intro_room_leaved");
	}

	if (m_infoportionsStore->hasInfoportion("task_intro_room_leaved")) {
		m_winText->show();
		return;
	}

	m_timeManager->update();
	m_activeInputController->update();
	m_hud->update();
	m_gameObjectsStore->update();

	OBB newPlayerObb = m_player->getWorldPlacedCollider();

	Intersection intersection;

	for (const OBB& obb : m_levelMesh->getColliders()) {
		if (newPlayerObb.intersects(obb, intersection)) {
			m_player->getTransform()->move(intersection.getDirection() * intersection.getDepth());
		}
	}

	if (m_levelDoor != nullptr) {
		if (newPlayerObb.intersects(OBB(m_levelDoor->getColliders()[0], m_levelDoor->getTransform()->getTransformationMatrix()), intersection)) {
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

void LevelScene::render()
{
	m_levelRenderer->render();
}

void LevelScene::setActiveCamera(Camera * camera)
{
	Scene::setActiveCamera(camera);
	m_levelRenderer->setActiveCamera(camera);
}

void LevelScene::activate()
{
	m_guiManager->getMainLayout()->addWidget(m_levelGUILayout);
}

void LevelScene::deactivate()
{
	m_guiManager->getMainLayout()->removeWidget(m_levelGUILayout);
}

void LevelScene::loadResources()
{
	m_deferredLightingProgram = m_resourceManager->load<GpuProgram>("resources/shaders/deferred_lighting.fx");
	m_lightingGpuProgram = m_resourceManager->load<GpuProgram>("resources/shaders/phong.fx");
	m_boundingVolumeGpuProgram = m_resourceManager->load<GpuProgram>("resources/shaders/bounding_volume.fx");

	m_levelMesh = m_resourceManager->load<SolidMesh>("resources/models/level.mod", "meshes_level");
	m_playerMesh = m_resourceManager->load<SolidMesh>("resources/models/player/arms.mod", "meshes_player_arms");
	m_resourceManager->load<SolidMesh>("resources/models/book.mod", "meshes_dynamic_book");
	m_resourceManager->load<SolidMesh>("resources/models/door.mod", "meshes_dynamic_door");

	// Textures
	Texture* bookIcon = m_resourceManager->load<Texture>("resources/textures/icons/book.png", "textues_dynamic_book_icon");
	bookIcon->bind();
	bookIcon->setMinificationFilter(Texture::Filter::Linear);
	bookIcon->setMagnificationFilter(Texture::Filter::Linear);

	// Player animations
	m_resourceManager->load<Animation>("resources/animations/player/arms_idle.anim", "animations_player_arms_idle");
	m_resourceManager->load<Animation>("resources/animations/player/arms_running.anim", "animations_player_arms_running");
	m_resourceManager->load<Animation>("resources/animations/player/arms_taking.anim", "animations_player_arms_taking");
}

void LevelScene::initializeSceneObjects() {
	m_phongLightingBaseMaterial = new PhongLightingMaterial("materials.phong_base", m_lightingGpuProgram);

	// Initialize player
	initializePlayer();
	
	setActiveCamera(m_playerCamera);
	m_activeInputController = m_playerController;

	// Initialize free camera
	initializeFreeCamera();

	// Initialize primitives for rendering
	initializePrimitives();

	// Initialize objects
	std::string strangeDiaryTitle = "Странные записки";
	std::string strangeDiaryText = "Ж чжунъ хшпеъ рнэб ьецчб фнцбсе,\nЕ ижйхб ъхетнч ънчхйоэно пуи,\nТу пргь цйпхйче фхуцч жйцбсе\nЧа жцфустн рнэб чйпшюно зуи!";

	Book* strangeDiary = new Book(m_resourceManager->getResource<SolidMesh>("meshes_dynamic_book"),
		m_phongLightingBaseMaterial, m_resourceManager->getResource<Texture>("textues_dynamic_book_icon"), strangeDiaryTitle, strangeDiaryText);
	strangeDiary->getTransform()->setPosition(-2.60989, 1.18929, -2.3509);

	strangeDiary->setGameObjectUsage(GameObject::Usage::DynamicObject);
	strangeDiary->setGameObjectLocation(GameObject::Location::World);
	strangeDiary->setGameObjectInteractiveMode(GameObject::InteractiveMode::Takeable);
	strangeDiary->setGameObjectInteractiveTitle(strangeDiaryTitle);

	strangeDiary->setTakeCallback([&infoportionsStore = m_infoportionsStore](GameObject* object) {
		infoportionsStore->addInfoportion("task_intro_book_found");
	});

	strangeDiary->setUseCallback([&infoportionsStore = m_infoportionsStore](GameObject* object) {
		infoportionsStore->addInfoportion("task_intro_book_read");
	});

	std::string historyArticleTitle = "Заметки";
	std::string historyArticleText = "Гай Юлий Цезарь - один из самых известных древнеримских\nполитических деятелей.\n"
		"Пять - третье простое число.";

	Book* historyArticle = new Book(m_resourceManager->getResource<SolidMesh>("meshes_dynamic_book"),
		m_phongLightingBaseMaterial, m_resourceManager->getResource<Texture>("textues_dynamic_book_icon"), historyArticleTitle, historyArticleText);
	historyArticle->getTransform()->setPosition(-2.60989, 1.18929, -1.3509);

	historyArticle->setGameObjectUsage(GameObject::Usage::DynamicObject);
	historyArticle->setGameObjectLocation(GameObject::Location::World);
	historyArticle->setGameObjectInteractiveMode(GameObject::InteractiveMode::Takeable);
	historyArticle->setGameObjectInteractiveTitle(historyArticleTitle);

	// Initialize level
	m_level = new SolidGameObject(m_levelMesh, m_phongLightingBaseMaterial);
	m_level->setGameObjectUsage(GameObject::Usage::StaticEnvironmentObject);

	m_levelDoor = new LockedDoor(
		m_resourceManager->getResource<SolidMesh>("meshes_dynamic_door"), 
		m_phongLightingBaseMaterial, m_timeManager, m_hud);

	m_levelDoor->setGameObjectInteractiveTitle("Панель");
	m_levelDoor->getTransform()->setPosition(-1.316f, 1.752f, -4.66f);

	m_levelDoor->setSecretCode("2018");

	m_levelDoor->setOpenCallback([
			&infoportionsStore = m_infoportionsStore,
			&gameObjectsStore = m_gameObjectsStore,
			&m_door = m_levelDoor
		] (LockedDoor* door) {
			gameObjectsStore->removeGameObject(door);

			infoportionsStore->addInfoportion("task_intro_door_opened");
			m_door = nullptr;
		}
	);

	// Initialize lights
	Light* whileLight = new Light(Light::Type::Point);
	whileLight->setColor(vector3(1.0f, 1.0f, 1.0f));
	whileLight->setAmbientIntensity(0.002f);
	whileLight->setDiffuseIntensity(0.96f);
	whileLight->setPosition(vector3(-0.76580, 2.7, -1.0655));

	whileLight->setAttenuation(1.0f, 0.8f, 0.8f);

	m_lights.push_back(whileLight);

	// Register objects in level renderer
	m_levelRenderer->registerBaseMaterial(m_phongLightingBaseMaterial);

	m_gameObjectsStore->registerGameObject(m_player);
	m_gameObjectsStore->registerGameObject(m_level);
	m_gameObjectsStore->registerGameObject(m_levelDoor);


	m_gameObjectsStore->registerGameObject(strangeDiary);
	m_gameObjectsStore->registerGameObject(historyArticle);

	for (Light* lightSource : m_lights) {
		m_gameObjectsStore->registerGameObject(lightSource);
	}
}

void LevelScene::initializePrimitives()
{
	m_boxPrimitive = new BoxPrimitive(m_graphicsResourceFactory);
}

void LevelScene::initializePlayer()
{
	// Player initialization
	m_player = new Player(m_playerMesh, m_phongLightingBaseMaterial);
	m_player->getTransform()->setPosition(-0.25916, 1.40000, 0.4456);
	m_player->getTransform()->setOrientation(quaternion(-0.6608, 0.22277, 0.67916, 0.22895));

	m_playerCamera = createCamera("playerCamera");
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

	m_playerController = new PlayerController(m_player, m_playerCamera, 
		m_inputManager, playerAnimations, m_gameObjectsStore, m_hud, m_graphicsResourceFactory);

	m_playerController->setMovementSpeed(0.15f);
}

void LevelScene::initializeFreeCamera()
{
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
}

void LevelScene::initializeInfoportions()
{
	m_infoportionsStore = new InfoportionsStore();
}

void LevelScene::initializeTasks()
{
	m_taskManager = new TaskManager(m_infoportionsStore);
	m_taskManager->setChangeCurrentTaskCallback(std::bind(&LevelScene::changeCurrentTaskCallback, this, std::placeholders::_1));

	Objective taskIntroLookAround("Осмотреться");
	taskIntroLookAround.setCompleteInfoportion("task_intro_book_found");
	
	Objective taskIntroReadBook("Изучить записки");
	taskIntroReadBook.setCompleteInfoportion("task_intro_book_read");

	Objective taskIntroOpenDoor("Открыть дверь");
	taskIntroOpenDoor.setCompleteInfoportion("task_intro_door_opened");

	Objective taskIntroLeaveRoom("Выбраться из комнаты");
	taskIntroLeaveRoom.setCompleteInfoportion("task_intro_room_leaved");

	Task taskIntro(
		"Где я?", {
			taskIntroLookAround,
			taskIntroReadBook,
			taskIntroOpenDoor,
			taskIntroLeaveRoom
		}
	);

	taskIntro.setStartInfoportion("game_started");

	m_taskManager->addTask(taskIntro);
}

void LevelScene::initializeGUI()
{
	m_hud = new GameHUD(
		m_graphicsContext,
		m_graphicsResourceFactory, 
		m_resourceManager->getResource<Font>("fonts_tuffy"),
		m_guiManager, 
		m_levelGUILayout
	);

	m_winText = new GUIText(m_graphicsResourceFactory);
	m_winText->setPosition(535, 235);
	m_winText->setFont(m_resourceManager->getResource<Font>("fonts_tuffy"));
	m_winText->setFontSize(32);
	m_winText->setColor(1.0, 1.0, 1.0);
	m_winText->setText("Игра завершена!");
	m_winText->hide();

	m_levelGUILayout->addWidget(m_winText);
}

void LevelScene::startGame()
{
	m_infoportionsStore->addInfoportion("game_started");
}

void LevelScene::changeCurrentTaskCallback(const Task * newCurrentTask)
{
	if (newCurrentTask != nullptr) {
		m_hud->setCurrentTaskInfo(newCurrentTask->getTitle(), newCurrentTask->getCurrentObjective()->getTitle());
	}
	else {
		m_hud->setNoActiveTask();
	}

	m_hud->showTaskInfo();
}

void LevelScene::removeGameObjectCallback(GameObject * object)
{
	switch (object->getGameObjectUsage()) {
	case GameObject::Usage::LightSource:
		m_levelRenderer->removeLightSource(dynamic_cast<Light*>(object));
		break;

	case GameObject::Usage::StaticEnvironmentObject:
	case GameObject::Usage::Player:
		m_levelRenderer->removeRenderableObject(dynamic_cast<Renderable*>(object));
		break;

	case GameObject::Usage::DynamicObject:
		if (object->isLocatedInWorld())
			m_levelRenderer->removeRenderableObject(dynamic_cast<Renderable*>(object));
		break;
	}
}

void LevelScene::registerGameObjectCallback(GameObject * object)
{
	switch (object->getGameObjectUsage()) {
	case GameObject::Usage::LightSource:
		m_levelRenderer->registerLightSource(dynamic_cast<Light*>(object));
		break;

	case GameObject::Usage::StaticEnvironmentObject:
	case GameObject::Usage::Player:
		m_levelRenderer->addRenderableObject(dynamic_cast<Renderable*>(object));
		break;

	case GameObject::Usage::DynamicObject:
		if (object->isLocatedInWorld())
			m_levelRenderer->addRenderableObject(dynamic_cast<Renderable*>(object));
		break;
	}
 }

void LevelScene::relocateGameObjectCallback(GameObject * object, GameObject::Location oldLocation, GameObject::Location newLocation)
{
	if (object->getGameObjectUsage() == GameObject::Usage::DynamicObject) {
		if (oldLocation == GameObject::Location::World && newLocation == GameObject::Location::Inventory)
			m_levelRenderer->removeRenderableObject(dynamic_cast<Renderable*>(object));

		if (oldLocation == GameObject::Location::Inventory && newLocation == GameObject::Location::World)
			m_levelRenderer->addRenderableObject(dynamic_cast<Renderable*>(object));

	}
}

void LevelScene::changeCameraCommandHandler(Console * console, const std::vector<std::string>& args)
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

void LevelScene::changeGammaCorrectionCommandHandler(Console * console, const std::vector<std::string>& args)
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

void LevelScene::pickPositionCommandHandler(Console * console, const std::vector<std::string>& args)
{
	vector3 position = (m_activeInputController == m_playerController) ? 
		m_player->getPosition() : m_freeCamera->getTransform()->getPosition();

	std::string result = StringUtils::format("%.5f, %.5f, %.5f", position.x, position.y, position.z);

	console->print(result);
	IOUtils::copyToClipboard(result);
}

void LevelScene::pickDirectionCommandHandler(Console * console, const std::vector<std::string>& args)
{
	quaternion orientation = (m_activeInputController == m_playerController) ?
		m_player->getTransform()->getOrientation() : m_freeCamera->getTransform()->getOrientation();

	std::string result = StringUtils::format("%.5f, %.5f, %.5f, %.5f", orientation.w, orientation.x, orientation.y, orientation.z);

	console->print(result);
	IOUtils::copyToClipboard(result);
}