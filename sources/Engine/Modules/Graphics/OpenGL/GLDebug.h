#pragma once

#include <cstdint>

void GLClearErrorStack();
void GLCheckNewError(const char* file, size_t line, const char* function);

#define GL_CALL(call) do { GLClearErrorStack();\
    call;\
    GLCheckNewError(__FILE__, __LINE__, __FUNCTION__); } while (0)

#define GL_CALL_BLOCK_BEGIN() GLClearErrorStack()
#define GL_CALL_BLOCK_END() GLCheckNewError(__FILE__, __LINE__, __FUNCTION__)
