#include "LockedDoor.h"

LockedDoor::LockedDoor(SolidMesh * mesh, BaseMaterial * baseMaterial, TimeManager* timeManager, GameHUD* hud)
	: SolidGameObject(mesh, baseMaterial),
	m_timeManager(timeManager),
	m_hud(hud),
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
	if (!m_isOpened) {
		CodePanel* panel = m_hud->getCodePanelWindow();

		panel->setCode(m_secretCode);
		panel->setSuccessulEnterCallback(std::bind(&LockedDoor::successfulCodeEnterCallback, this));
		panel->open();
	}
}

bool LockedDoor::isOpened() const {
	return m_isOpened;
}

void LockedDoor::setOpenCallback(const OpenCallback & callback) {
	m_openCallback = callback;
}

void LockedDoor::setSecretCode(const std::string & secretCode)
{
	m_secretCode = secretCode;
}

std::string LockedDoor::getSecretCode() const
{
	return m_secretCode;
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

void LockedDoor::successfulCodeEnterCallback()
{
	if (!m_isOpened) {
		m_isOpened = true;
		m_timeManager->setIntervalTimer(33, std::bind(&LockedDoor::openAnimationStep, this));
	}
}
