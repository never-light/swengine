#pragma once

#include <functional>
#include <vector>
#include <string>
#include <unordered_map>

class Console {
public:
	using PrintCallback = std::function<void(const std::string&)>;
	using CommandHandler = std::function<void(Console*, const std::vector<std::string>&)>;
public:
	Console();
	~Console();

	void print(const std::string& text);

	void executeCommand(const std::string& command);

	void registerCommandHandler(const std::string& commandName, const CommandHandler& handler);
	void unregisterCommandHandler(const std::string& commandName);

	void setPrintCallback(const PrintCallback& callback);

	std::string getLastCommand() const;

	std::vector<std::string> getMessagesHistory() const;
protected:
	std::unordered_map<std::string, CommandHandler> m_commandHandlers;
	std::vector<std::string> m_consoleLog;
	std::string m_lastCommand;

	PrintCallback m_printCallback;
};
