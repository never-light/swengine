#include "OpenGL3Errors.h"

#include "OpenGL3.h"
#include <string>
#include <Engine\Components\Graphics\RenderSystem\RenderSystemException.h>

void OpenGL3ClearError()
{
	while (glGetError() != GL_NO_ERROR) {
		// Do nothing
	}
}

void OpenGL3CheckErrors(const char * file, size_t line, const char * function)
{
	bool isErrorsDetected = false;

	std::string errorsMessages = "OpenGL errors:\n";

	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string errorType;

		switch (errorCode)
		{
		case GL_INVALID_ENUM: 
			errorType = "INVALID_ENUM";
			break;
		case GL_INVALID_VALUE: 
			errorType = "INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION: 
			errorType = "INVALID_OPERATION";
			break;
		case GL_STACK_OVERFLOW: 
			errorType = "STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			errorType = "STACK_UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			errorType = "OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: 
			errorType = "INVALID_FRAMEBUFFER_OPERATION";
			break;
		}
		
		errorsMessages += errorType;
		isErrorsDetected = true;
	}

	if (isErrorsDetected) {
		throw RenderSystemException(errorsMessages.c_str(), file, line, function);
	}
}
