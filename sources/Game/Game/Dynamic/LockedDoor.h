#pragma once

#include <Game\SolidGameObject.h>
#include <Game\Game\Inventory\InventoryObject.h>

#include <Game\Game\Time\TimeManager.h>

#include <Game\Game\HUD\GameHUD.h>
#include <Game\Game\HUD\CodePanel.h>

class LockedDoor : public SolidGameObject {
public:
	using OpenCallback = std::function<void(LockedDoor*)>;

public:
	LockedDoor(SolidMesh* mesh, BaseMaterial* baseMaterial, TimeManager* timeManager,
		GameHUD* hud);
	virtual ~LockedDoor();

	void open();
	bool isOpened() const;

	void setOpenCallback(const OpenCallback& callback);

	void setSecretCode(const std::string& secretCode);
	std::string getSecretCode() const;

protected:
	void useCallback(GameObject* object);
	bool openAnimationStep();

	void successfulCodeEnterCallback();
protected:
	std::string m_secretCode;

	bool m_isOpened;

	OpenCallback m_openCallback;

	TimeManager* m_timeManager;

	GameHUD* m_hud;
};