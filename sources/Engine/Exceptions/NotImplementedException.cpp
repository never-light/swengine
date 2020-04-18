#include "NotImplementedException.h"

NotImplementedException::NotImplementedException(const std::string& message,
  const std::string& file,
  size_t line,
  const std::string& function)
  : EngineRuntimeException(message, file, line, function)
{

}
