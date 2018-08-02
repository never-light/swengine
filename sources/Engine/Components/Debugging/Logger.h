#pragma once

#include <string>

class Logger {
public:
	Logger();
	virtual ~Logger();

	virtual void log(const std::string& message);
};