#pragma once

#include <random>

class Random {
public:
	static void initialize();
	static int getInteger(int, int);
	static float getFloat(float, float);

private:
	Random();
	Random(const Random&);
	~Random();
};