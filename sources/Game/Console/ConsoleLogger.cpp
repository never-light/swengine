#include "ConsoleLogger.h"

ConsoleLogger::ConsoleLogger(Console* console)
	: m_console(console)
{
}

ConsoleLogger::~ConsoleLogger()
{
}

void ConsoleLogger::log(const std::string & message)
{
	m_console->print(message);
}
