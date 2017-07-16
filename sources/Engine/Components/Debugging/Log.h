#pragma once

#include <iostream>

enum LogType {
	LOG_ERRORS, LOG_INFO
};

class Log {
public:
	Log(std::ostream&, LogType);
	Log(const Log&);
	~Log();

	template<class T>
	Log &operator<<(const T &x) {
		this->m_stream << x;
		return *this;
	}
private:
	std::ostream& m_stream;
	LogType m_type;
};

Log errlog();
Log infolog();