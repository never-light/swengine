#pragma once

#include <stdexcept>

class EngineRuntimeException : public std::runtime_error {
public:
    EngineRuntimeException(const std::string& message, const std::string& file, size_t line, const std::string& function);

    const std::string& getFile() const;
    const std::string& getFunction() const;
    size_t getLine() const;

protected:
    std::string m_file;
    std::string m_function;
    size_t m_line;
};

#define ENGINE_RUNTIME_ERROR(message) throw EngineRuntimeException((message), __FILE__, __LINE__, __FUNCTION__)
