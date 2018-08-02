#pragma once

#include <Engine\Exceptions\EngineException.h>

class RenderSystemException : public EngineException {
public:
	RenderSystemException(const char* message, const char* file, size_t line, const char* function);
	~RenderSystemException();
};