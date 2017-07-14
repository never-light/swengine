#include "State.h"

State::State() : m_stateStatus(STATE_PASSIVE), m_nextStateId(STATE_UNDEFINED) {

}

State::~State() {

}

StateStatus State::is() const {
	return m_stateStatus;
}

stateId State::getNextStateId() const {
	return m_nextStateId;
}

void State::setStatus(StateStatus newStatus) {
	m_stateStatus = newStatus;
}

void State::changeState(stateId newState) {
	setStatus(STATE_IS_WAITING_CHANGE);
	m_nextStateId = newState;
}

void State::resetNextStateId() {
	m_nextStateId = STATE_UNDEFINED;
}