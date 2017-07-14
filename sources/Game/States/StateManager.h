#pragma once

#include <map>
#include "State.h"
#include "MainMenuState.h"
#include "PlayState.h"

class StateManager {
public:
	StateManager();
	~StateManager();

	void update();

	void registerState(stateId, ResourceManager*);
	void unregisterState(stateId, ResourceManager*);

	void changeState(stateId);
	State* getCurrentState() const;
	stateId getCurrentStateId() const;
private:
	std::map <stateId, State*> m_statesMap;
	stateId m_currentStateId;
	State* m_currentState;
};