#include "Logger.h"
#include <iostream>

Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::log(const std::string & message)
{
	std::cout << message << std::endl;
}
