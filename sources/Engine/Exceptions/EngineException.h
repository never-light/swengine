#pragma once

#include <exception>

class EngineException : public std::exception {
public:
	EngineException(const char* message, const char* file, size_t line, const char* function);
	~EngineException();

	const char* getFile() const;
	size_t getLine() const;
	const char* getFunction() const;

protected:
	const char* m_file;
	size_t m_line;
	const char* m_function;
};

#define THROW_ENGINE_ERROR(message) throw EngineException((message), __FILE__, __LINE__, __FUNCTION__);