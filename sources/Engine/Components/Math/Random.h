#pragma once

#include <random>

class Random {
public:
	static void initialize();
	static int getInteger(int, int);
	static float getFloat(float, float);

private:
	Random() = default;
	Random(const Random&) = default;
	~Random() = default;
};