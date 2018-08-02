#pragma once

#include <Engine\Exceptions\EngineException.h>

class FatalErrorException : public EngineException {
public:
	FatalErrorException(const char* message, const char* file, size_t line, const char* function);
	~FatalErrorException();
};