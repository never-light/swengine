#include "StartScene.h"

#include <Engine\Components\Debugging\Log.h>

StartScene::StartScene(GraphicsContext* graphicsContext, ResourceManager* resourceManager, InputManager* inputManager) 
	: Scene(graphicsContext, resourceManager),
	m_inputManager(inputManager),
	m_stalker(nullptr),
	m_house(nullptr)
{
	GpuProgram* phongProgram = m_resourceManager->load<GpuProgram>("resources/shaders/phong.fx");

	m_stalker = new SolidGameObject(m_resourceManager->load<SolidMesh>("resources/models/novice.mod"));
	this->registerSceneObject(m_stalker);

	m_house = new SolidGameObject(m_resourceManager->load<SolidMesh>("resources/models/lab.mod"));
	this->registerSceneObject(m_house);


	Camera* camera = this->createCamera("firstPersonCamera");
	camera->setNearClipDistance(0.1f);
	camera->setFarClipDistance(300.0f);
	camera->setFOVy(45.0f);
	camera->setAspectRatio(m_graphicsContext->getViewportWidth() / m_graphicsContext->getViewportHeight());

	camera->getTransform()->fixYAxis();
	camera->getTransform()->setPosition(0, 0, 5);
	camera->getTransform()->lookAt(0, 0, 0);

	this->setActiveCamera(camera);
}

StartScene::~StartScene()
{
	if (m_stalker != nullptr) {
		delete m_stalker;
		m_stalker = nullptr;
	}

	if (m_house != nullptr) {
		delete m_house;
		m_house = nullptr;
	}
}

void StartScene::update()
{
	const float cameraSpeed = 0.25f;
	const float mouseSensitivity = 0.15f;

	Camera* m_freeCamera = this->getActiveCamera();

	MousePosition mousePosition = m_inputManager->getMousePosition();

	vector3 position = m_freeCamera->getTransform()->getPosition();

	if (m_inputManager->isKeyPressed(GLFW_KEY_W))
		position += m_freeCamera->getTransform()->getFrontDirection() * cameraSpeed;
	else if (m_inputManager->isKeyPressed(GLFW_KEY_S))
		position -= m_freeCamera->getTransform()->getFrontDirection() * cameraSpeed;

	if (m_inputManager->isKeyPressed(GLFW_KEY_A))
		position -= m_freeCamera->getTransform()->getRightDirection() * cameraSpeed;
	else if (m_inputManager->isKeyPressed(GLFW_KEY_D))
		position += m_freeCamera->getTransform()->getRightDirection() * cameraSpeed;

	if (m_inputManager->isKeyPressed(GLFW_KEY_Q))
		m_freeCamera->getTransform()->yaw(1.0f);
	if (m_inputManager->isKeyPressed(GLFW_KEY_E))
		m_freeCamera->getTransform()->yaw(-1.0f);

	real currentPitchAngle = glm::degrees(glm::angle(vector3(0, 1, 0), glm::normalize(m_freeCamera->getTransform()->getFrontDirection())));
	real pitchOffset = mouseSensitivity * mousePosition.y * -1.0f;

	if ((pitchOffset > 0 && currentPitchAngle - pitchOffset > 0.001) || (pitchOffset < 0 && currentPitchAngle - pitchOffset < 179.999))
		m_freeCamera->getTransform()->pitch(pitchOffset);

	m_freeCamera->getTransform()->yaw(mouseSensitivity * mousePosition.x * -1.0f);
	m_freeCamera->getTransform()->setPosition(position);
}

void StartScene::render()
{
	m_graphicsContext->enableDepthTest();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GpuProgram* phongProgram = m_resourceManager->getResource<GpuProgram>("resources/shaders/phong.fx");

	phongProgram->bind();

	phongProgram->setParameter("scene.viewTransform", getActiveCamera()->getViewMatrix());
	phongProgram->setParameter("scene.projectionTransform", getActiveCamera()->getProjectionMatrix());

	m_graphicsContext->enableFaceCulling();
	m_graphicsContext->setFaceCullingMode(GraphicsContext::FaceCullingMode::Back);

	m_stalker->render(phongProgram);
	m_house->render(phongProgram);
}
