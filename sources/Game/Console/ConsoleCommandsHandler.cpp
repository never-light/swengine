#include "ConsoleCommandsHandler.h"

#include <Engine\Utils\files.h>

ConsoleCommandsHandler::ConsoleCommandsHandler(Console * console)
	: m_console(console), m_guiConsoleWidget(nullptr)
{
	m_console->registerCommandHandler("file-available",
		std::bind(&ConsoleCommandsHandler::fileAvailable, this, std::placeholders::_1, std::placeholders::_2));

	m_console->registerCommandHandler("clear",
		std::bind(&ConsoleCommandsHandler::clear, this, std::placeholders::_1, std::placeholders::_2));

}

ConsoleCommandsHandler::~ConsoleCommandsHandler()
{
}

void ConsoleCommandsHandler::attachGUIConsoleWidget(GUIConsoleWidget * widget)
{
	m_guiConsoleWidget = widget;
}

void ConsoleCommandsHandler::fileAvailable(Console * console, const std::vector<std::string>& args) {
	if (args.empty())
		return;

	std::string filename = args.front();

	console->print((FilesUtils::isExists(filename)) ? "true" : "false");
}

void ConsoleCommandsHandler::clear(Console * console, const std::vector<std::string>& args)
{
	if (m_guiConsoleWidget != nullptr)
		m_guiConsoleWidget->clear();
}
