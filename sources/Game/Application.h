#pragma once

#include "Framework/BaseApplication.h"
#include "States/StateManager.h"

class Application : public BaseApplication {
public:
	Application();

	virtual ~Application();

	virtual void draw();
	virtual void initialize(const std::string&, unsigned int, unsigned int);
	virtual void shutdown();
	virtual void update();
private:
	virtual void onKeyPress(int);
	Application(const Application&);

private:
	StateManager* m_stateMgr;
};