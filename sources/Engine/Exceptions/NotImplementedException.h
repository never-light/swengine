#pragma once

#include "EngineRuntimeException.h"

class NotImplementedException : public EngineRuntimeException {
public:
    NotImplementedException(const std::string& message,
                            const std::string& file,
                            size_t line,
                            const std::string& function);
};

#define ENGINE_NOT_IMPLEMENTED_ERROR(message) throw NotImplementedException((message), __FILE__, __LINE__, __FUNCTION__)
