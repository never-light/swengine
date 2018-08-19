#include "LockedDoor.h"

LockedDoor::LockedDoor(SolidMesh * mesh, BaseMaterial * baseMaterial, TimeManager* timeManager)
	: SolidGameObject(mesh, baseMaterial),
	m_timeManager(timeManager),
	m_isOpened(false),
	m_openCallback(nullptr)
{
	setGameObjectUsage(GameObject::Usage::DynamicObject);
	setGameObjectLocation(GameObject::Location::World);
	setGameObjectInteractiveMode(GameObject::InteractiveMode::Usable);

	setUseCallback(std::bind(&LockedDoor::useCallback, this, std::placeholders::_1));
}

LockedDoor::~LockedDoor() {
}

void LockedDoor::open()
{
	if (!m_isOpened)
		m_timeManager->setIntervalTimer(33, std::bind(&LockedDoor::openAnimationStep, this));
}

bool LockedDoor::isOpened() const {
	return m_isOpened;
}

void LockedDoor::setOpenCallback(const OpenCallback & callback) {
	m_openCallback = callback;
}

void LockedDoor::useCallback(GameObject * object) {
	open();
}

bool LockedDoor::openAnimationStep()
{
	this->getTransform()->move(0.0f, 0.01f, 0.0f);

	if (this->getPosition().y >= 5.0f) {
		m_isOpened = true;

		if (m_openCallback)
			m_openCallback(this);

		return false;
	}

	return true;

}
