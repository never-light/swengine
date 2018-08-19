#include "TimeManager.h"

TimeManager::TimeManager()
	: m_currentRealTime(0),
	m_realTimeFactor(0)
{
}

TimeManager::~TimeManager()
{
}

void TimeManager::update() {
	m_currentRealTime += m_realTimeFactor;

	auto& it = m_activeTimers.begin();
	
	while (it != m_activeTimers.end()) {
		if (it->type == TimerType::Timeout) {
			if (m_currentRealTime - it->lastTime >= it->checkTime) {
				it->callback();

				it = m_activeTimers.erase(it);
				continue;
			}
		}
		else if (it->type == TimerType::Interval) {
			if (m_currentRealTime - it->lastTime >= it->checkTime) {
				bool status = it->callback();
				
				if (!status) {
					it = m_activeTimers.erase(it);
					continue;
				}
			}
		}

		it->lastTime = m_currentRealTime;
		it++;
	}

}

void TimeManager::setRealTimeFactor(unsigned int factor)
{
	m_realTimeFactor = factor;
}

unsigned int TimeManager::getRealTimeFactor() const
{
	return m_realTimeFactor;
}

void TimeManager::setTimeout(unsigned int timeout, const TimeCallback & callback)
{
	Timer timer;
	timer.type = TimerType::Timeout;
	timer.lastTime = m_currentRealTime;
	timer.checkTime = timeout;
	timer.callback = callback;

	m_activeTimers.push_back(timer);
}

void TimeManager::setIntervalTimer(unsigned int timeout, const TimeCallback & callback)
{
	Timer timer;
	timer.type = TimerType::Interval;
	timer.lastTime = m_currentRealTime;
	timer.checkTime = timeout;
	timer.callback = callback;

	m_activeTimers.push_back(timer);
}
