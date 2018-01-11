#include "Application.h"
#include "config.h"

Application::Application(const std::string& windowName, unsigned int width, unsigned int height) 
	: BaseApplication(windowName, width, height),
	InputEventsListener() 
{
	infolog() << "Инициализация начата...";

	m_inputMgr->registerEventListener(this);

	// Load materials
	m_resMgr->loadMaterialsPackage("resources/materials/standart.materials");
	m_resMgr->loadMaterialsPackage("resources/materials/level.materials");

	// Create scene
	m_mainScene = m_sceneMgr->createEmptyScene("main");
	
	// Create sun light
	Light* sunLight = m_mainScene->createLight("sun", LightType::Directional);
	sunLight->setDirection({ 0, 0, -1 });
	sunLight->setAmbientColor({ 0.2f, 0.2f, 0.2f });
	sunLight->setDuffuseColor({ 1.0f, 1.0f, 1.0f });
	sunLight->setSpecularColor({ 1.0f, 1.0f, 1.0f });

	m_renderer->addLight(sunLight);

	// Load level model
	auto levelModel = m_mainScene->createModel("resources/models/level.model", "level");
	levelModel->getSubModel(0)->setMaterial(m_resMgr->getMaterial("brick"));
	levelModel->getSubModel(1)->setMaterial(m_resMgr->getMaterial("grass"));

	// Create level scene node
	m_levelNode = m_mainScene->getRootSceneNode()->createChildSceneNode("level");
	m_levelNode->attachObject(levelModel);

	m_levelNode->setPosition({ 0, 0, 0 });
	m_levelNode->setScale({ 0.06f, 0.06f, 0.06f });

	// Create free camera
	m_freeCamera = m_mainScene->createCamera("free");
	m_freeCamera->setFOVy(45.0f);
	m_freeCamera->setAspectRatio(
		static_cast<float32>(m_window->getWidth()) / static_cast<float32>(m_window->getWidth())
	);
	m_freeCamera->setNearClipDistance(0.3f);
	m_freeCamera->setFarClipDistance(100.0f);
	m_freeCamera->setPosition(0, 5, 0);

	m_cameraSpeed = 0.15f;
	m_mouseSensitivity = 0.15f;

	// Create player instance
	m_player = new Player(m_mainScene);

	// Set free view type
	toggleViewType(ViewType::Free);

	infolog() << "Инициализация завершена...";
}

Application::~Application() {
	delete m_player;
	infolog() << "Завершение работы...";
}

void Application::update() {
	if (m_viewType == ViewType::Free) {
		MousePosition pos = m_inputMgr->getMousePosition();
		real currentPitchAngle = glm::degrees(glm::angle(vector3(0, 1, 0), glm::normalize(m_freeCamera->getFrontDirection())));
		real pitchOffset = m_mouseSensitivity * pos.y * -1.0f;

		if ((pitchOffset > 0 && currentPitchAngle - pitchOffset > 0.001) || (pitchOffset < 0 && currentPitchAngle - pitchOffset < 179.999))
			m_freeCamera->pitch(pitchOffset);

		m_freeCamera->yaw(m_mouseSensitivity * pos.x * -1.0f);

		if (m_inputMgr->isKeyPressed(GLFW_KEY_W)) {
			m_freeCamera->move(m_freeCamera->getFrontDirection() * m_cameraSpeed);
		}
		else if (m_inputMgr->isKeyPressed(GLFW_KEY_S)) {
			m_freeCamera->move(-m_freeCamera->getFrontDirection() * m_cameraSpeed);
		}

		if (m_inputMgr->isKeyPressed(GLFW_KEY_A)) {
			m_freeCamera->move(-m_freeCamera->getRightDirection() * m_cameraSpeed);
		}
		else if (m_inputMgr->isKeyPressed(GLFW_KEY_D)) {
			m_freeCamera->move(m_freeCamera->getRightDirection() * m_cameraSpeed);
		}
	}

	m_player->update();
	BaseApplication::update();
}

void Application::render() {
	BaseApplication::render();

	m_renderer->beginRendering({ 0.6f, 0.6f, 0.6f });

	m_player->render();
	m_renderer->drawModel((Model*)m_levelNode->getAttachedObjects()[0]);

	m_renderer->endRendering();
}

void Application::toggleViewType(ViewType type) {
	m_viewType = type;

	switch (type) {
	case ViewType::Free:
		m_renderer->setCurrentCamera(m_freeCamera);
		m_player->disableViews();
		break;
		
	case ViewType::FirstPerson:
		m_player->enableFirstPersonView();
		break;

	case ViewType::ThirdPerson:
		m_player->enableThirdPersonView();
		break;
	}
}

void Application::onKeyPress(int16 key, KeyEvent::Modifier mod) {
	if (key == GLFW_KEY_1) {
		toggleViewType(ViewType::Free);
	}
	else if (key == GLFW_KEY_2) {
		toggleViewType(ViewType::FirstPerson);
	}
	else if (key == GLFW_KEY_3) {
		toggleViewType(ViewType::ThirdPerson);
	}

	if (key == GLFW_KEY_ESCAPE) {
		m_window->close();
	}
}