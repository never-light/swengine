#pragma once

#include <Engine\Components\Debugging\Logger.h>
#include "Console.h"

class ConsoleLogger : public Logger {
public:
	ConsoleLogger(Console* console);
	virtual ~ConsoleLogger();
	
	virtual void log(const std::string& message) override;

private:
	Console* m_console;
};