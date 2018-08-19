#pragma once

#include "Player.h"
#include "InputController.h"

#include <Engine\Components\GUI\GUIManager.h>
#include <Game\Graphics\Animation\Animator.h>
#include <Game\Game\GameObjectsStore.h>

#include <Game\Game\Inventory\InventoryViewer.h>
#include <Game\Game\HUD\GameHUD.h>

class PlayerController : public InputController, public InputEventsListener {
public:
	enum class PlayerState {
		Idle = 0, Running = 1, Taking = 2
	};

	static const size_t PLAYER_STATES_COUNT = 3;
public:
	PlayerController(Player* player, 
		Camera* camera, 
		InputManager* inputManager, 
		const std::vector<Animation*>& statesAnimations,
		GameObjectsStore* gameObjectsStore,
		GameHUD* hud,
		GraphicsResourceFactory* graphicsResourceFactory);

	~PlayerController();

	void update();

	Camera* getAttachedCamera() const;
	
	void setMovementSpeed(float speed);
	float getMovementSpeed() const;

	void setMouseSensitivity(float sensitivity);
	float getMouseSensitivity() const;

	PlayerState getCurrentPlayerState() const;

	virtual void onKeyPress(Key key, KeyEvent::Modifier mod);

private:
	void initializeGUI();
	void takeObject(GameObject* object);
	void useObject(GameObject* object);

	void updateAnimation();

	void checkInteractiveObjects();
	GameObject * findNearestInteractiveObject() const;

	void changeState(PlayerState state);

private:
	PlayerState m_currentPlayerState;

	float m_movementSpeed = 0.10f;
	float m_mouseSensitivity = 0.15f;

	std::vector<Animation*> m_statesAnimations;
	Animator* m_playerAnimator;
private:
	GameObjectsStore * m_gameObjectsStore;

	Player* m_player;
	Camera* m_playerCamera;

private:
	GameHUD * m_hud;
	InventoryViewer* m_inventoryViewer;

	GraphicsResourceFactory* m_graphicsResourceFactory;
};