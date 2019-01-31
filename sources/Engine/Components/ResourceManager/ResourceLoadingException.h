#pragma once

#include <Engine/Exceptions/EngineException.h>

enum class ResourceLoadingError {
	Unknown, FileNotAvailable, InvalidType, InvalidData
};

class ResourceLoadingException : public EngineException {
public:
	ResourceLoadingException(ResourceLoadingError error, const char* resourceName, const char* message, const char* file, size_t line, const char* function);
	~ResourceLoadingException();

	ResourceLoadingError getError() const;
	const char* getResourceName() const;
protected:
	ResourceLoadingError m_error;

	const char* m_resourceName;
};