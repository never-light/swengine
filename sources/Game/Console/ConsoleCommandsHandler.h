#pragma once

#include "Console.h"
#include "GUIConsoleWidget.h"

class ConsoleCommandsHandler {
public:
	ConsoleCommandsHandler(Console* console);
	~ConsoleCommandsHandler();

	void attachGUIConsoleWidget(GUIConsoleWidget* widget);
private:
	void fileAvailable(Console* console, const std::vector<std::string>& args);
	void clear(Console* console, const std::vector<std::string>& args);

private:
	Console* m_console;
	GUIConsoleWidget* m_guiConsoleWidget;
};