#pragma once

#include <functional>
#include <list>

class TimeManager {
public:
	using TimeCallback = std::function<bool()>;

private:
	enum class TimerType {
		Timeout, Interval
	};

	struct Timer {
		TimerType type;
		unsigned int lastTime;
		unsigned int checkTime;

		TimeCallback callback;
	};

public:
	TimeManager();
	~TimeManager();

	void update();

	void setRealTimeFactor(unsigned int factor);
	unsigned int getRealTimeFactor() const;

	void setTimeout(unsigned int timeout, const TimeCallback& callback);
	void setIntervalTimer(unsigned int timeout, const TimeCallback& callback);
private:
	unsigned int m_realTimeFactor;
	unsigned int m_currentRealTime;

	std::list<Timer> m_activeTimers;
};