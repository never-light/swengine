#pragma once

#include <string>
#include "EngineRuntimeException.h"

class NotImplementedException : public EngineRuntimeException {
 public:
  NotImplementedException(const std::string& message,
      const std::string& file,
      size_t line,
      const std::string& function);
};
