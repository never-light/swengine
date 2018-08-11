#pragma once

#include "BaseGame.h"
#include "StartScene.h"
#include "MainMenu.h"
#include "Console\Console.h"
#include "Console\GUIConsoleWidget.h"
#include "Console\ConsoleCommandsHandler.h"
#include "Console\ConsoleLogger.h"

class Game : public BaseGame, public InputEventsListener {
public:
	Game(const std::string&, unsigned int, unsigned int);
	~Game();

	void update();
	void render();

private:
	virtual void preLoadCommonResources();
	void onKeyPress(Key key, KeyEvent::Modifier mod) override;

	void initializeInternalConsole();
	void initializeConsoleGUI();
	void addConsoleCommandHotkey(Key key, const std::string& command);

	void loadScenes();

	void processResourceLoadingError(const ResourceLoadingException& error);
private:
	GUIManager * m_guiMgr;

	GraphicsSystem* m_graphicsSystem;
	GraphicsContext* m_graphicsContext;

	ResourceManager* m_resMgr;

	Console* m_console;
	GUIConsoleWidget* m_guiConsoleWidget;
	ConsoleCommandsHandler* m_consoleCommandsHandler;
	ConsoleLogger* m_consoleLogger;
	
	std::unordered_map<Key, std::string> m_consoleCommandsHotkeys;

	MainMenu* m_mainMenu;

	StartScene* m_startScene;
};