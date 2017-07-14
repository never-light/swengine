#include "Random.h"
#include <ctime>

void Random::initialize() {
	srand(static_cast <unsigned> (time(0)));
}

int Random::getInteger(int min, int max) {
	return min + (rand() % static_cast<int>(max - min + 1));
}

float Random::getFloat(float min, float max) {
	float random = ((float)rand()) / (float)RAND_MAX;
	return min + random * (max - min);
}