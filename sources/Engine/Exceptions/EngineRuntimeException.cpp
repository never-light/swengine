#include "precompiled.h"
#pragma hdrstop

#include "EngineRuntimeException.h"

EngineRuntimeException::EngineRuntimeException(const std::string& message, const std::string& file, size_t line, const std::string& function)
    : runtime_error(message),
    m_file(file),
    m_function(function),
    m_line(line)
{
}

const std::string& EngineRuntimeException::getFile() const
{
    return m_file;
}

const std::string& EngineRuntimeException::getFunction() const
{
    return m_function;
}

size_t EngineRuntimeException::getLine() const
{
    return m_line;
}
