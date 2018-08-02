#include "ResourceLoadingException.h"

#include <cstring>

ResourceLoadingException::ResourceLoadingException(ResourceLoadingError error, const char* resourceName, const char* message, const char * file, size_t line, const char * function)
	: EngineException(message, file, line, function),
	m_error(error), m_resourceName(nullptr)
{
	m_resourceName = strdup(resourceName);
}

ResourceLoadingException::~ResourceLoadingException()
{
	if (m_resourceName != nullptr)
		free((void*)m_resourceName);
}

ResourceLoadingError ResourceLoadingException::getError() const
{
	return m_error;
}

const char * ResourceLoadingException::getResourceName() const
{
	return m_resourceName;
}
