#pragma once

#include "Player.h"
#include "InputController.h"

#include <Engine\Components\GUI\GUIManager.h>
#include <Game\Graphics\Animation\Animator.h>
#include <Game\Game\GameObjectsStore.h>

#include <Game\Game\Inventory\InventoryViewer.h>

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
		Font* textFont,
		GUIManager* guiManager,
		GUILayout* levelGUILayout,
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

	void blockControl();
	void unblockControl();
	bool isControlBlocked() const;

private:
	void initializeGUI();
	void takeObject(GameObject* object);

	void updateAnimation();

	void checkInteractiveObjects();
	GameObject * findNearestInteractiveObject() const;

	void changeState(PlayerState state);

private:
	PlayerState m_currentPlayerState;

	float m_movementSpeed = 0.10f;
	float m_mouseSensitivity = 0.15f;

	bool m_isControlBlocked;

	std::vector<Animation*> m_statesAnimations;
	Animator* m_playerAnimator;
private:
	GameObjectsStore * m_gameObjectsStore;

	Player* m_player;
	Camera* m_playerCamera;

private:
	GUIManager* m_guiManager;
	GUILayout* m_levelGUILayout;
	GraphicsResourceFactory* m_graphicsResourceFactory;
	Font* m_textFont;

	GUIText * m_takeTextWidget;

	InventoryViewer* m_inventoryViewer;
};