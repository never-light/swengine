#pragma once

class TimeUtils {
public:
	static double getCurrentTime();

private:
	TimeUtils() = delete;
	~TimeUtils() = delete;

	TimeUtils(const TimeUtils& other) = delete;
};