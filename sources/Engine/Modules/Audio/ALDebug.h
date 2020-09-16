#pragma once

#include <AL/al.h>
#include <AL/alc.h>

void ALClearErrorStack();
void ALCheckNewError(const char* file, size_t line, const char* function);

#define AL_CALL(call) do { ALClearErrorStack();\
    call;\
    ALCheckNewError(__FILE__, __LINE__, __FUNCTION__); } while (0)

#define AL_CALL_BLOCK_BEGIN() ALClearErrorStack()
#define AL_CALL_BLOCK_END() ALCheckNewError(__FILE__, __LINE__, __FUNCTION__)
