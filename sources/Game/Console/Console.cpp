#include "Console.h"

#include <Engine\Utils\string.h>
#include <iomanip>
#include <iostream>

Console::Console()
{

}

Console::~Console()
{
}

void Console::print(const std::string & text) {
	m_consoleLog.push_back(text);
	
	if (m_printCallback != nullptr)
		m_printCallback(text);
}

void Console::executeCommand(const std::string & command) {
	std::vector<std::string> parts = StringUtils::split(command, ' ', true);

	if (parts.empty()) {
		return;
	}

	m_lastCommand = command;
	print(command);

	std::string commandName = parts[0];

	auto& commandHandlerIt = m_commandHandlers.find(commandName);
	
	if (commandHandlerIt == m_commandHandlers.end()) {
		print("Unknown command");
		return;
	}

	std::vector<std::string> commandArgs(parts);
	commandArgs.erase(commandArgs.begin());

	commandHandlerIt->second(this, commandArgs);
}

void Console::registerCommandHandler(const std::string & commandName, const CommandHandler & handler)
{
	m_commandHandlers.insert({ commandName, handler });
}

void Console::unregisterCommandHandler(const std::string & commandName)
{
	m_commandHandlers.erase(commandName);
}

void Console::setPrintCallback(const PrintCallback& callback) {
	m_printCallback = callback;
}

std::string Console::getLastCommand() const
{
	return m_lastCommand;
}

std::vector<std::string> Console::getMessagesHistory() const
{
	return m_consoleLog;
}
