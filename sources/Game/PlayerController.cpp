#include "PlayerController.h"

#include <Engine\assertions.h>
#include <Game\config.h>

PlayerController::PlayerController(Player * player, Camera * camera, InputManager * inputManager, 
	const std::vector<Animation*>& statesAnimations, GameObjectsStore* gameObjectsStore,
	Font* textFont,
	GUIManager* guiManager,
	GUILayout* levelGUILayout,
	GraphicsResourceFactory* graphicsResourceFactory)
	: InputController(inputManager), 
	m_player(player), 
	m_playerCamera(camera), 
	m_statesAnimations(statesAnimations),
	m_playerAnimator(nullptr),
	m_gameObjectsStore(gameObjectsStore),
	m_isControlBlocked(false),
	m_guiManager(guiManager),
	m_textFont(textFont),
	m_levelGUILayout(levelGUILayout),
	m_graphicsResourceFactory(graphicsResourceFactory)
{
	_assert(statesAnimations.size() == PLAYER_STATES_COUNT);

	inputManager->registerEventListener(this);
	initializeGUI();

	m_player->getTransform()->fixYAxis();

	m_playerAnimator = new Animator(m_player->getSkeleton());
	changeState(PlayerState::Idle);
	m_playerAnimator->play();
}

PlayerController::~PlayerController()
{
	if (m_playerAnimator != nullptr) {
		delete m_playerAnimator;
		m_playerAnimator = nullptr;
	}

	delete m_takeTextWidget;
	delete m_inventoryViewer;
}

void PlayerController::update()
{
	if (isControlBlocked())
		return;

	updateAnimation();

	bool needMove = false;

	MousePosition mousePosition = m_inputManager->getMousePosition();

	vector3 position = m_player->getTransform()->getPosition();
	float oldY = position.y;

	if (m_inputManager->isKeyPressed(GLFW_KEY_W)) {
		position += m_player->getTransform()->getFrontDirection() * m_movementSpeed;
		needMove = true;
	}
	else if (m_inputManager->isKeyPressed(GLFW_KEY_S)) {
		position -= m_player->getTransform()->getFrontDirection() * m_movementSpeed;
		needMove = true;
	}

	if (m_inputManager->isKeyPressed(GLFW_KEY_A)) {
		position -= m_player->getTransform()->getRightDirection() * m_movementSpeed;
		needMove = true;
	}
	else if (m_inputManager->isKeyPressed(GLFW_KEY_D)) {
		position += m_player->getTransform()->getRightDirection() * m_movementSpeed;
		needMove = true;
	}

	real currentPitchAngle = glm::degrees(glm::angle(vector3(0, 1, 0), glm::normalize(m_player->getTransform()->getFrontDirection())));
	real pitchOffset = m_mouseSensitivity * mousePosition.y * -1.0f;

	if ((pitchOffset > 0 && currentPitchAngle - pitchOffset > 0.001) || (pitchOffset < 0 && currentPitchAngle - pitchOffset < 179.999))
		m_player->getTransform()->pitch(pitchOffset);

	position.y = oldY;

	m_player->getTransform()->yaw(m_mouseSensitivity * mousePosition.x * -1.0f);
	m_player->getTransform()->setPosition(position);

	if (needMove && m_currentPlayerState != PlayerState::Running)
		changeState(PlayerState::Running);
	
	if (!needMove && m_currentPlayerState == PlayerState::Running)
		changeState(PlayerState::Idle);

	checkInteractiveObjects();
}

Camera * PlayerController::getAttachedCamera() const
{
	return m_playerCamera;
}

void PlayerController::setMovementSpeed(float speed)
{
	m_movementSpeed = speed;
}

float PlayerController::getMovementSpeed() const
{
	return m_movementSpeed;
}

void PlayerController::setMouseSensitivity(float sensitivity)
{
	m_mouseSensitivity = sensitivity;
}

float PlayerController::getMouseSensitivity() const
{
	return m_mouseSensitivity;
}

PlayerController::PlayerState PlayerController::getCurrentPlayerState() const
{
	return m_currentPlayerState;
}

void PlayerController::onKeyPress(Key key, KeyEvent::Modifier mod)
{
	if (key == GLFW_KEY_I) {
		if (m_inventoryViewer->isVisible()) {
			m_inventoryViewer->onClose();
			m_inventoryViewer->hide();
			m_guiManager->setCursorType(CursorType::Hidden);
			unblockControl();
		}
		else {
			m_inventoryViewer->show();
			m_guiManager->setCursorType(CursorType::Default);
			blockControl();
		}
	}

	if (key == GLFW_KEY_F && !isControlBlocked()) {
		if (!m_inputManager->isKeyPressed(GLFW_KEY_W) && 
			!m_inputManager->isKeyPressed(GLFW_KEY_A) &&
			!m_inputManager->isKeyPressed(GLFW_KEY_S) &&
			!m_inputManager->isKeyPressed(GLFW_KEY_D)
			) 
		{
			changeState(PlayerState::Taking);

			GameObject* interactiveObject = findNearestInteractiveObject();

			if (interactiveObject == nullptr)
				return;

			if (interactiveObject->isTakeable()) {
				takeObject(interactiveObject);
			}
		}
	}
}

void PlayerController::blockControl()
{
	m_isControlBlocked = true;
}

void PlayerController::unblockControl()
{
	m_isControlBlocked = false;
}

bool PlayerController::isControlBlocked() const
{
	return m_isControlBlocked;
}

void PlayerController::initializeGUI()
{
	m_takeTextWidget = new GUIText(m_graphicsResourceFactory);
	m_takeTextWidget->setPosition(m_levelGUILayout->getWidth() / 2, m_levelGUILayout->getHeight() - 185);
	m_takeTextWidget->setFont(m_textFont);
	m_takeTextWidget->setFontSize(12);
	m_takeTextWidget->setColor(1.0, 1.0, 1.0);
	m_takeTextWidget->hide();

	m_levelGUILayout->addWidget(m_takeTextWidget);

	m_inventoryViewer = new InventoryViewer(m_player->getInventory(), m_graphicsResourceFactory, m_textFont);
	m_inventoryViewer->setBackgroundColor(vector4(0.168f, 0.172f, 0.25f, 0.8f));
	m_inventoryViewer->setSize(800, 600);
	m_inventoryViewer->setPosition(m_levelGUILayout->getWidth() / 2 - 400,
		m_levelGUILayout->getHeight() / 2 - 300);
	m_inventoryViewer->enableBackgroundRendering();
	m_inventoryViewer->hide();

	m_levelGUILayout->addWidget(m_inventoryViewer);
}

void PlayerController::takeObject(GameObject * object)
{
	InventoryObject* inventoryObject = dynamic_cast<InventoryObject*>(object);

	m_player->getInventory()->addObject(inventoryObject);
	m_gameObjectsStore->relocateObject(object, GameObject::Location::Inventory);
	inventoryObject->triggerTake();
}

void PlayerController::updateAnimation()
{
	if (m_currentPlayerState == PlayerState::Taking && m_playerAnimator->isStopped()) {
		changeState(PlayerState::Idle);
		m_playerAnimator->play();
	}

	m_playerAnimator->increaseAnimationTime(1.0f / GAME_STATE_UPDATES_PER_SECOND);

	m_player->applyPose(m_playerAnimator->getAnimatedPose());
}

void PlayerController::checkInteractiveObjects()
{
	m_takeTextWidget->hide();

	GameObject* nearestInteractiveObject = findNearestInteractiveObject();

	if (nearestInteractiveObject == nullptr)
		return;

	if (nearestInteractiveObject->isTakeable()) {
		std::string title = nearestInteractiveObject->getGameObjectInteractiveTitle();
		title += " - Взять (F)";

		m_takeTextWidget->setText(title);

		uivector2 oldPosition = m_takeTextWidget->getPosition();
		m_takeTextWidget->setPosition(oldPosition.x, oldPosition.y);
		m_takeTextWidget->show();
	}
}

GameObject * PlayerController::findNearestInteractiveObject() const
{
	for (GameObject* gameObject : m_gameObjectsStore->getObjects()) {
		if (!gameObject->isInteractive() || !gameObject->isLocatedInWorld())
			continue;

		vector3 distanceVector = gameObject->getPosition() - m_player->getPosition();

		if (glm::length(distanceVector) <= 1.4f &&
			glm::degrees(glm::angle(distanceVector, m_player->getTransform()->getFrontDirection())) <= 20.0f)
		{
			return gameObject;
		}
	}


	return nullptr;
}

void PlayerController::changeState(PlayerState state)
{
	m_currentPlayerState = state;

	Animation* newStateAnimation = m_statesAnimations[(size_t)state];
	m_playerAnimator->setCurrentAnimation(newStateAnimation);
}
