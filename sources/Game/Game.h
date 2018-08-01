#pragma once

#include "BaseGame.h"
#include "StartScene.h"
#include "MainMenu.h"
#include "Console\Console.h"
#include "Console\GUIConsoleWidget.h"

class Game : public BaseGame, public InputEventsListener {
public:
	Game(const std::string&, unsigned int, unsigned int);
	~Game();

	void update();
	void render();

private:
	void onKeyPress(Key key, KeyEvent::Modifier mod) override;

private:
	Console* m_console;
	GUIConsoleWidget* m_guiConsoleWidget;

	MainMenu* m_mainMenu;

	StartScene* m_startScene;
};