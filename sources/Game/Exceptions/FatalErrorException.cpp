#include "FatalErrorException.h"

FatalErrorException::FatalErrorException(const char * message, const char * file, size_t line, const char * function)
	: EngineException(message, file, line, function)
{
}

FatalErrorException::~FatalErrorException()
{
}
