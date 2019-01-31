#include "TimeMeter.h"

#include <Engine/Utils/time.h>
#include <Engine/Utils/string.h>

#include <Engine/assertions.h>

TimeMeter::TimeMeter(Logger * logger)
	: m_logger(logger)
{

}

void TimeMeter::start(const std::string & measurementName)
{
	m_measurements.push({ measurementName, TimeUtils::getCurrentTime() });
}

void TimeMeter::end()
{
	_assert(!m_measurements.empty());

	Measurement measurement = m_measurements.top();
	m_measurements.top();

	m_logger->log(StringUtils::format((measurement.name + ": %.6f").c_str(), TimeUtils::getCurrentTime() - measurement.startTime));
}
