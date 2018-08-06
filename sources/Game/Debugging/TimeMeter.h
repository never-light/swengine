#pragma once

#include <stack>
#include <Engine\Components\Debugging\Logger.h>

class TimeMeter {
private:
	struct Measurement {
		std::string name;
		double startTime;
	};

public:
	TimeMeter(Logger* logger);

	void start(const std::string& measurementName);
	void end();

private:
	Logger * m_logger;
	
	std::stack<Measurement> m_measurements;
};