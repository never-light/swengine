#include "StateManager.h"

StateManager::StateManager() {

}

StateManager::~StateManager() {

}

void StateManager::registerState(stateId stId, ResourceManager* resMgr) {
	if (stId != STATE_MAIN_MENU && stId != STATE_PLAY) {
		return;
	}

	State* newState = nullptr;

	if (stId == STATE_MAIN_MENU) {
		newState = new MainMenuState();
	}
	else if (stId == STATE_PLAY) {
		newState = new PlayState();
	}

	newState->load(resMgr);
	newState->setStatus(STATE_PASSIVE);
	m_statesMap.insert(std::make_pair(stId, newState));
}

void StateManager::unregisterState(stateId stId, ResourceManager* resMgr) {
	auto it = m_statesMap.find(stId);
	
	if (it != m_statesMap.end()) {
		m_statesMap.at(stId)->unload(resMgr);
		delete m_statesMap.at(stId);
		m_statesMap.erase(it);
	}
}

void StateManager::update() {
	if (m_currentState->getNextStateId() != STATE_UNDEFINED) {
		changeState(m_currentState->getNextStateId());
		m_currentState->resetNextStateId();
	}
}

void StateManager::changeState(stateId newStateId) {
	m_currentStateId = newStateId;
	m_currentState = m_statesMap.at(newStateId);
}

State* StateManager::getCurrentState() const {
	return m_currentState;
}

stateId StateManager::getCurrentStateId() const {
	return m_currentStateId;
}