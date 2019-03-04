#include "LevelScene.h"

#include <Engine\Utils\time.h>
#include <Engine\Components\Math\Random.h>
#include <Engine\Utils\string.h>
#include <Engine\Utils\io.h>

#include <Engine/Components/ResourceManager/TextureLoader.h>

#include <Game\config.h>

#include <iostream>

LevelScene::LevelScene(GraphicsContext* graphicsContext, 
	ResourceManager* resourceManager, 
	InputManager* inputManager,
	GUIManager* guiManager,
	Console* console)
	: Scene(graphicsContext, resourceManager),
	m_inputManager(inputManager),
	m_guiManager(guiManager),
	m_levelGUILayout(new GUILayout()),
	m_gameWorld(new GameWorld())
{
	m_levelGUILayout->setPosition(0, 0);
	m_levelGUILayout->setSize(m_graphicsContext->getViewportWidth(), m_graphicsContext->getViewportHeight());
	m_levelGUILayout->disableBackgroundRendering();

	loadResources();

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
	//if (m_testMaterial != nullptr)
		//delete m_testMaterial;

	delete m_sceneEnvironment;

	delete m_graphicsPipeline;
	delete m_gameWorld;

	delete m_meshRenderingSystem;
	delete m_postProcessingSystem;

	delete m_winText;
	delete m_hud;

	delete m_taskManager;
	delete m_infoportionsStore;
	delete m_timeManager;

	delete m_levelGUILayout;
}

void LevelScene::update() {
	if (m_hud->isControlLocked())
		return;

	if (m_infoportionsStore->hasInfoportion("task_intro_room_leaved")) {
		m_winText->show();
		return;
	}

	const float m_movementSpeed = 0.15f;
	const float m_mouseSensitivity = 0.2f;

	MousePosition mousePosition = m_inputManager->getMousePosition();

	vector3 position = m_freeCamera->getTransform()->getPosition();
	float oldY = position.y;

	if (m_inputManager->isKeyPressed(GLFW_KEY_W)) {
		position += m_freeCamera->getTransform()->getFrontDirection() * m_movementSpeed;
	}
	else if (m_inputManager->isKeyPressed(GLFW_KEY_S)) {
		position -= m_freeCamera->getTransform()->getFrontDirection() * m_movementSpeed;
	}

	if (m_inputManager->isKeyPressed(GLFW_KEY_A)) {
		position -= m_freeCamera->getTransform()->getRightDirection() * m_movementSpeed;
	}
	else if (m_inputManager->isKeyPressed(GLFW_KEY_D)) {
		position += m_freeCamera->getTransform()->getRightDirection() * m_movementSpeed;
	}

	real currentPitchAngle = glm::degrees(glm::angle(vector3(0, 1, 0), glm::normalize(m_freeCamera->getTransform()->getFrontDirection())));
	real pitchOffset = m_mouseSensitivity * mousePosition.y * -1.0f;

	if ((pitchOffset > 0 && currentPitchAngle - pitchOffset > 0.001) || (pitchOffset < 0 && currentPitchAngle - pitchOffset < 179.999))
		m_freeCamera->getTransform()->pitch(pitchOffset);

	m_freeCamera->getTransform()->yaw(m_mouseSensitivity * mousePosition.x * -1.0f);
	m_freeCamera->getTransform()->setPosition(position);


	m_timeManager->update();
	m_gameWorld->update(1.0f / 30);
	m_hud->update();


	if (m_inputManager->isKeyPressed(GLFW_KEY_F)) {
		Light* light2 = new Light(Light::Type::Point);
		light2->setPosition(position);
		light2->setColor(vector3(5.0f, 5.0f, 5.0f));

		m_sceneEnvironment->addLightSource(light2);
	}

	if (m_inputManager->isKeyPressed(GLFW_KEY_G)) {
		Light* light2 = new Light(Light::Type::Point);
		light2->setPosition(position);
		light2->setColor(vector3(5.0f, 0.0f, 0.0f));

		m_sceneEnvironment->addLightSource(light2);
	}


	if (m_inputManager->isKeyPressed(GLFW_KEY_H)) {
		Light* light2 = new Light(Light::Type::Point);
		light2->setPosition(position);
		light2->setColor(vector3(0.0f, 5.0f, 0.0f));

		m_sceneEnvironment->addLightSource(light2);
	}

	if (m_inputManager->isKeyPressed(GLFW_KEY_J)) {
		Light* light2 = new Light(Light::Type::Point);
		light2->setPosition(position);
		light2->setColor(vector3(0.0f, 0.0f, 5.0f));

		m_sceneEnvironment->addLightSource(light2);
	}

	if (m_inputManager->isKeyPressed(GLFW_KEY_E))
		m_graphicsPipeline->enableNormalMapping();
	else if (m_inputManager->isKeyPressed(GLFW_KEY_T))
		m_graphicsPipeline->enableNormalMapping(false);

	if (m_inputManager->isKeyPressed(GLFW_KEY_U))
		m_gameWorld->getGameObjectByIndex(0)->getComponent<Transform>()->yaw(5.0f);
	else if (m_inputManager->isKeyPressed(GLFW_KEY_I))
		m_gameWorld->getGameObjectByIndex(0)->getComponent<Transform>()->yaw(-5.0f);

	if (m_inputManager->isKeyPressed(GLFW_KEY_K))
		m_sceneEnvironment->getActiveCamera()->setFOVy(m_sceneEnvironment->getActiveCamera()->getFOVy() + 2.0f);
	else if (m_inputManager->isKeyPressed(GLFW_KEY_L))
		m_sceneEnvironment->getActiveCamera()->setFOVy(m_sceneEnvironment->getActiveCamera()->getFOVy() - 2.0f);

}

void LevelScene::render()
{
	m_gameWorld->render();
}

void LevelScene::setActiveCamera(Camera * camera)
{
	Scene::setActiveCamera(camera);
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
	m_deferredGeometryPassProgram = m_resourceManager->loadResource<GpuProgram>("resources/shaders/deferred_gpass.fx").getRawPtr();
	m_deferredLigthingPassProgram = m_resourceManager->loadResource<GpuProgram>("resources/shaders/deferred_lightpass.fx").getRawPtr();
	m_boundingVolumeGpuProgram = m_resourceManager->loadResource<GpuProgram>("resources/shaders/bounding_volume.fx").getRawPtr();

	m_brightFilterProgram = m_resourceManager->loadResource<GpuProgram>("resources/shaders/bright_filter.fx").getRawPtr();
	m_blurProgram = m_resourceManager->loadResource<GpuProgram>("resources/shaders/blur.fx").getRawPtr();
	m_postProcessingProgram = m_resourceManager->loadResource<GpuProgram>("resources/shaders/post_processing.fx").getRawPtr();

	// Textures
	Texture* bookIcon = m_resourceManager->loadResource<Texture>("resources/textures/icons/book.png", "textues_dynamic_book_icon").getRawPtr();
	bookIcon->bind();
	bookIcon->setMinificationFilter(Texture::Filter::Linear);
	bookIcon->setMagnificationFilter(Texture::Filter::Linear);

	// Player animations
	m_resourceManager->loadResource<Animation>("resources/animations/player/arms_idle.anim", "animations_player_arms_idle");
	m_resourceManager->loadResource<Animation>("resources/animations/player/arms_running.anim", "animations_player_arms_running");
	m_resourceManager->loadResource<Animation>("resources/animations/player/arms_taking.anim", "animations_player_arms_taking");

}

void LevelScene::initializeSceneObjects() {
	// Initialize player
	initializePlayer();
	
	// Initialize free camera
	initializeFreeCamera();

	m_sceneEnvironment = new SceneEnvironment();
	m_sceneEnvironment->setActiveCamera(m_freeCamera);

	m_graphicsPipeline = new GraphicsPipeline(m_graphicsContext);
	m_meshRenderingSystem = new MeshRenderingSystem(m_graphicsPipeline, m_sceneEnvironment,
		m_deferredGeometryPassProgram, m_deferredLigthingPassProgram);
	m_postProcessingSystem = new PostProcessingSystem(m_graphicsPipeline, m_sceneEnvironment,
		m_brightFilterProgram, m_blurProgram, m_postProcessingProgram);

	m_gameWorld->addGameSystem(m_meshRenderingSystem);
	m_gameWorld->addGameSystem(m_postProcessingSystem);

	PBRMaterial* armorMtl = new PBRMaterial();
	armorMtl->setColor(m_resourceManager->loadResource<Texture>("resources/textures/pbr/armor/albedo.png", "armor_albedo", TextureLoadingOptions(TextureLoadingOptions::Format::sRGB)).getRawPtr());
	armorMtl->setMetallic(m_resourceManager->loadResource<Texture>("resources/textures/pbr/armor/metallic.png").getRawPtr());
	armorMtl->setNormal(m_resourceManager->loadResource<Texture>("resources/textures/pbr/armor/normal.png").getRawPtr());
	armorMtl->setRoughness(m_resourceManager->loadResource<Texture>("resources/textures/pbr/armor/roughness.png").getRawPtr());

	PBRMaterial* brickMtl = new PBRMaterial();
	brickMtl->setColor(m_resourceManager->loadResource<Texture>("resources/textures/pbr/brick/albedo.tga", "brick_albedo", TextureLoadingOptions(TextureLoadingOptions::Format::sRGB)).getRawPtr());
	brickMtl->setMetallic(0.0f);
	//armorMtl->setMetallic(m_resourceManager->loadResource<Texture>("resources/textures/pbr/armor/metallic.png").getRawPtr());
	brickMtl->setNormal(m_resourceManager->loadResource<Texture>("resources/textures/pbr/brick/normal.tga").getRawPtr());
	brickMtl->setRoughness(m_resourceManager->loadResource<Texture>("resources/textures/pbr/brick/roughness.tga").getRawPtr());

	PBRMaterial* flannelMtl = new PBRMaterial();
	flannelMtl->setColor(m_resourceManager->loadResource<Texture>("resources/textures/pbr/flannel/albedo.tga", "flannel_albedo", TextureLoadingOptions(TextureLoadingOptions::Format::sRGB)).getRawPtr());
	flannelMtl->setMetallic(0.0f);
	//armorMtl->setMetallic(m_resourceManager->loadResource<Texture>("resources/textures/pbr/armor/metallic.png").getRawPtr());
	flannelMtl->setNormal(m_resourceManager->loadResource<Texture>("resources/textures/pbr/flannel/normal.tga").getRawPtr());
	flannelMtl->setRoughness(m_resourceManager->loadResource<Texture>("resources/textures/pbr/flannel/roughness.tga").getRawPtr());

	PBRMaterial* goldMtl = new PBRMaterial();
	goldMtl->setColor(m_resourceManager->loadResource<Texture>("resources/textures/pbr/gold/albedo.tga", "gold_albedo", TextureLoadingOptions(TextureLoadingOptions::Format::sRGB)).getRawPtr());
	goldMtl->setMetallic(m_resourceManager->loadResource<Texture>("resources/textures/pbr/gold/metallic.tga").getRawPtr());
	goldMtl->setNormal(m_resourceManager->loadResource<Texture>("resources/textures/pbr/gold/normal.tga").getRawPtr());
	goldMtl->setRoughness(m_resourceManager->loadResource<Texture>("resources/textures/pbr/gold/roughness.tga").getRawPtr());

	PBRMaterial* parketMtl = new PBRMaterial();
	parketMtl->setColor(m_resourceManager->loadResource<Texture>("resources/textures/pbr/parket/albedo.tga", "parket_albedo", TextureLoadingOptions(TextureLoadingOptions::Format::sRGB)).getRawPtr());
	parketMtl->setMetallic(0.0f);
	//armorMtl->setMetallic(m_resourceManager->loadResource<Texture>("resources/textures/pbr/armor/metallic.png").getRawPtr());
	parketMtl->setNormal(m_resourceManager->loadResource<Texture>("resources/textures/pbr/parket/normal.tga").getRawPtr());
	parketMtl->setRoughness(m_resourceManager->loadResource<Texture>("resources/textures/pbr/parket/roughness.tga").getRawPtr());

	PBRMaterial* plasticMtl = new PBRMaterial();
	plasticMtl->setColor(m_resourceManager->loadResource<Texture>("resources/textures/pbr/plastic/albedo.tga", "plastic_albedo", TextureLoadingOptions(TextureLoadingOptions::Format::sRGB)).getRawPtr());
	plasticMtl->setMetallic(0.0f);
	//armorMtl->setMetallic(m_resourceManager->loadResource<Texture>("resources/textures/pbr/armor/metallic.png").getRawPtr());
	plasticMtl->setNormal(m_resourceManager->loadResource<Texture>("resources/textures/pbr/plastic/normal.tga").getRawPtr());
	plasticMtl->setRoughness(m_resourceManager->loadResource<Texture>("resources/textures/pbr/plastic/roughness.tga").getRawPtr());

	PBRMaterial* rockMtl = new PBRMaterial();
	rockMtl->setColor(m_resourceManager->loadResource<Texture>("resources/textures/pbr/rock/albedo.tga", "rock_albedo", TextureLoadingOptions(TextureLoadingOptions::Format::sRGB)).getRawPtr());
	rockMtl->setMetallic(0.0f);
	//armorMtl->setMetallic(m_resourceManager->loadResource<Texture>("resources/textures/pbr/armor/metallic.png").getRawPtr());
	rockMtl->setNormal(m_resourceManager->loadResource<Texture>("resources/textures/pbr/rock/normal.tga").getRawPtr());
	rockMtl->setRoughness(m_resourceManager->loadResource<Texture>("resources/textures/pbr/rock/roughness.tga").getRawPtr());

	PBRMaterial* steelMtl = new PBRMaterial();
	steelMtl->setColor(m_resourceManager->loadResource<Texture>("resources/textures/pbr/steel/albedo.tga", "steel_albedo", TextureLoadingOptions(TextureLoadingOptions::Format::sRGB)).getRawPtr());
	steelMtl->setMetallic(m_resourceManager->loadResource<Texture>("resources/textures/pbr/steel/metallic.tga").getRawPtr());
	steelMtl->setNormal(m_resourceManager->loadResource<Texture>("resources/textures/pbr/steel/normal.tga").getRawPtr());
	steelMtl->setRoughness(m_resourceManager->loadResource<Texture>("resources/textures/pbr/steel/roughness.tga").getRawPtr());

	PBRMaterial* tilesMtl = new PBRMaterial();
	tilesMtl->setColor(m_resourceManager->loadResource<Texture>("resources/textures/pbr/tiles/albedo.tga", "tiles_albedo", TextureLoadingOptions(TextureLoadingOptions::Format::sRGB)).getRawPtr());
	tilesMtl->setMetallic(0.0f);
	//tilesMtl->setMetallic(m_resourceManager->loadResource<Texture>("resources/textures/pbr/steel/metallic.png").getRawPtr());
	tilesMtl->setNormal(m_resourceManager->loadResource<Texture>("resources/textures/pbr/tiles/normal.tga").getRawPtr());
	tilesMtl->setRoughness(m_resourceManager->loadResource<Texture>("resources/textures/pbr/tiles/roughness.tga").getRawPtr());


	Mesh* mesh = m_resourceManager->loadResource<Mesh>("resources/models/testpbr.mesh").getRawPtr();
	mesh->getSubMeshes()[0]->setMaterial(brickMtl);
	mesh->getSubMeshes()[1]->setMaterial(armorMtl);
	mesh->getSubMeshes()[2]->setMaterial(rockMtl);
	mesh->getSubMeshes()[3]->setMaterial(steelMtl);
	mesh->getSubMeshes()[4]->setMaterial(tilesMtl);
	mesh->getSubMeshes()[5]->setMaterial(flannelMtl);
	mesh->getSubMeshes()[6]->setMaterial(plasticMtl);
	mesh->getSubMeshes()[7]->setMaterial(goldMtl);
	mesh->getSubMeshes()[8]->setMaterial(parketMtl);

	GameObject* testObject = m_gameWorld->createGameObject();
	
	testObject->addComponent<MeshComponent>(mesh);
	auto transform = testObject->addComponent<Transform>();
	transform->setPosition(0, 0, 0);
	transform->setScale(1, 1, 1);
	transform->yaw(0.0f);
}

void LevelScene::initializePrimitives()
{
}

void LevelScene::initializePlayer()
{
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
	m_freeCamera->getTransform()->setPosition(0.0f, 0.0f, 5.0f);
	m_freeCamera->getTransform()->lookAt(0.0f, 0.0f, 0.0f);
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
		m_resourceManager->getResource<Font>("fonts_tuffy").getRawPtr(),
		m_guiManager, 
		m_levelGUILayout
	);

	m_winText = new GUIText(m_graphicsContext);
	m_winText->setPosition(535, 235);
	m_winText->setFont(m_resourceManager->getResource<Font>("fonts_tuffy").getRawPtr());
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


void LevelScene::changeCameraCommandHandler(Console * console, const std::vector<std::string>& args)
{
	if (args.empty())
		return;

	if (args.front() == "free") {
		setActiveCamera(m_freeCamera);
	}
	else if (args.front() == "fps") {
		//m_activeInputController = m_playerController;
		//setActiveCamera(m_playerCamera);
	}
	else
		console->print("Unknown camera type");
}

void LevelScene::changeGammaCorrectionCommandHandler(Console * console, const std::vector<std::string>& args)
{
}

void LevelScene::pickPositionCommandHandler(Console * console, const std::vector<std::string>& args)
{
	//vector3 position = (m_activeInputController == m_playerController) ? 
		//m_player->getPosition() : m_freeCamera->getTransform()->getPosition();

	//std::string result = StringUtils::format("%.5f, %.5f, %.5f", position.x, position.y, position.z);

	//console->print(result);
	//IOUtils::copyToClipboard(result);
}

void LevelScene::pickDirectionCommandHandler(Console * console, const std::vector<std::string>& args)
{
	//quaternion orientation = (m_activeInputController == m_playerController) ?
		//m_player->getTransform()->getOrientation() : m_freeCamera->getTransform()->getOrientation();

	//std::string result = StringUtils::format("%.5f, %.5f, %.5f, %.5f", orientation.w, orientation.x, orientation.y, orientation.z);

	//console->print(result);
	//IOUtils::copyToClipboard(result);
}