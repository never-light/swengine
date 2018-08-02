#pragma once

void OpenGL3ClearError();
void OpenGL3CheckErrors(const char* file, size_t line, const char* function);

#define OPENGL3_CALL(call) OpenGL3ClearError();\
	call;\
	OpenGL3CheckErrors(__FILE__, __LINE__, __FUNCTION__);

#define OPENGL3_CALL_BLOCK_BEGIN() OpenGL3ClearError()
#define OPENGL3_CALL_BLOCK_END() OpenGL3CheckErrors(__FILE__, __LINE__, __FUNCTION__)