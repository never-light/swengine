#pragma once

#include <string>
#include <stdexcept>

class EngineRuntimeException : public std::runtime_error {
 public:
  EngineRuntimeException(const std::string& message, std::string file, size_t line, std::string function);

  [[nodiscard]] const std::string& getFile() const;
  [[nodiscard]] const std::string& getFunction() const;
  [[nodiscard]] size_t getLine() const;

 protected:
  std::string m_file;
  std::string m_function;
  size_t m_line;
};

#define ENGINE_RUNTIME_ERROR(message) throw EngineRuntimeException((message), __FILE__, __LINE__, __FUNCTION__)
