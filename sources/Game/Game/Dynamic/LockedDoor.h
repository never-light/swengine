#pragma once

#include <Game\SolidGameObject.h>
#include <Game\Game\Inventory\InventoryObject.h>

#include <Game\Game\Time\TimeManager.h>

class LockedDoor : public SolidGameObject {
public:
	using OpenCallback = std::function<void(LockedDoor*)>;

public:
	LockedDoor(SolidMesh* mesh, BaseMaterial* baseMaterial, TimeManager* timeManager);
	virtual ~LockedDoor();

	void open();
	bool isOpened() const;

	void setOpenCallback(const OpenCallback& callback);

protected:
	void useCallback(GameObject* object);
	bool openAnimationStep();

protected:
	bool m_isOpened;

	OpenCallback m_openCallback;

	TimeManager* m_timeManager;
};