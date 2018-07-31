#include "time.h"
#include <GLFW\glfw3.h>

double TimeUtils::getCurrentTime() {
	return glfwGetTime();
}