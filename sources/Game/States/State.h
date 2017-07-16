#pragma once

#include <Engine\Engine.h>

enum StateStatus {
	STATE_ACTIVE, STATE_PASSIVE, STATE_IS_WAITING_CHANGE
};

enum stateId {
	STATE_MAIN_MENU, STATE_PLAY, STATE_GAME_OVER, STATE_UNDEFINED
};

class State {
public:
	State();
	virtual ~State();

	virtual void load(ResourceManager*) = 0;
	virtual void update(InputManager*) = 0;
	virtual void draw(Renderer*) = 0;
	virtual void unload(ResourceManager*) = 0;

	StateStatus is() const;
	void setStatus(StateStatus);
	stateId getNextStateId() const;
	void resetNextStateId();
protected:
	void changeState(stateId);

private:
	StateStatus m_stateStatus;
	stateId m_nextStateId;
};