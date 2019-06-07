#include "time.h"

#include <chrono>

double TimeUtils::getCurrentTime() {
	return std::chrono::duration_cast<std::chrono::microseconds>(
		std::chrono::system_clock::now().time_since_epoch()).count() / 1000.0;
}