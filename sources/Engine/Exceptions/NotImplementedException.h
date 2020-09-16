#pragma once

#include <string>
#include "EngineRuntimeException.h"

#include "TraceHelpers.h"

class NotImplementedException : public EngineRuntimeException {
 public:
  NotImplementedException(const std::string& message,
      const std::string& file,
      size_t line,
      const std::string& function);
};

#define NOT_IMPLEMENTED_EXCEPTION(message) GENERIC_ERROR(NotImplementedException, message)
