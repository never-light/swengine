#include "precompiled.h"

#pragma hdrstop

#include "ALDebug.h"

#include "Exceptions/exceptions.h"

void ALClearErrorStack()
{
  while (alGetError() != AL_NO_ERROR) {
    // Do nothing
  }
}

void ALCheckNewError(const char* file, size_t line, const char* function)
{
  bool isErrorsDetected = false;

  std::string errorsMessages = "OpenAL errors:/n";

  ALenum errorCode;
  while ((errorCode = alGetError()) != AL_NO_ERROR) {
    std::string errorType;

    switch (errorCode) {
      case AL_INVALID_ENUM:
        errorType = "INVALID_ENUM";
        break;
      case AL_INVALID_VALUE:
        errorType = "INVALID_VALUE";
        break;
      case AL_INVALID_OPERATION:
        errorType = "INVALID_OPERATION";
        break;
      case AL_INVALID_NAME:
        errorType = "AL_INVALID_NAME";
        break;
      case AL_OUT_OF_MEMORY:
        errorType = "OUT_OF_MEMORY";
        break;
      case AL_NO_ERROR:
        // Do nothing
        break;
      default:
        SW_ASSERT(false);
        break;
    }

    errorsMessages += errorType;
    isErrorsDetected = true;
  }

  if (isErrorsDetected) {
    throw EngineRuntimeException(errorsMessages, file, line, function);
  }
}
