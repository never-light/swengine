#pragma once

#include <iostream>

enum SGLogType {
	SGLOG_ERRORS, SGLOG_INFO
};

class SGLog {
public:
	SGLog(std::ostream&, SGLogType);
	SGLog(const SGLog&);
	~SGLog();

	template<class T>
	SGLog &operator<<(const T &x) {
		this->m_stream << x;
		return *this;
	}
private:
	std::ostream& m_stream;
	SGLogType m_type;
};

SGLog errlog();
SGLog infolog();