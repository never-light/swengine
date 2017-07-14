#pragma once

#include "BaseApplication.h"
#include "PlayerSpaceshipController.h"
#include "AsteroidsController.h"

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
	PlayerSpaceshipController m_playerSpaceshipController;
	AsteroidsController m_asteroidsController;
};