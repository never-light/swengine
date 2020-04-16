#include "precompiled.h"
#pragma hdrstop

#include "EngineRuntimeException.h"
#include <utility>


EngineRuntimeException::EngineRuntimeException(const std::string& message,
                                               std::string file,
                                               size_t line,
                                               std::string function)
    : runtime_error(message),
      m_file(std::move(file)),
      m_function(std::move(function)),
      m_line(line) {
}

const std::string& EngineRuntimeException::getFile() const {
  return m_file;
}

const std::string& EngineRuntimeException::getFunction() const {
  return m_function;
}

size_t EngineRuntimeException::getLine() const {
  return m_line;
}
