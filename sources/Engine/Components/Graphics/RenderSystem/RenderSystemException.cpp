#include "RenderSystemException.h"

RenderSystemException::RenderSystemException(const char * message, const char * file, size_t line, const char * function)
	: EngineException(message, file, line, function)
{
}

RenderSystemException::~RenderSystemException()
{
}
