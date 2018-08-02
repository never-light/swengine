#include "EngineException.h"

#include <cstring>

EngineException::EngineException(const char* message, const char* file, size_t line, const char* function)
	: std::exception(message),
	m_line(line)
{
	m_file = strdup(file);
	m_function = strdup(function);
}

EngineException::~EngineException()
{
	free((void*)m_file);
	free((void*)m_function);
}

const char * EngineException::getFile() const
{
	return m_file;
}

size_t EngineException::getLine() const
{
	return m_line;
}

const char * EngineException::getFunction() const
{
	return m_function;
}
