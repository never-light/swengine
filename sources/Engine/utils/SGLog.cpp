#include "SGLog.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

SGLog::SGLog(std::ostream& stream = std::cout, SGLogType type = SGLOG_INFO) 
	: m_stream(stream), m_type(type) {
	if (m_type == SGLOG_ERRORS) {
		m_stream << "[ERROR]";
	}

	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);
	m_stream << "[" << std::put_time(std::localtime(&in_time_t), "%X") << "] ";
}

SGLog::SGLog(const SGLog& debug) : m_stream(debug.m_stream), m_type(debug.m_type) {

}

SGLog::~SGLog() {
	m_stream << std::endl;
}

SGLog errlog() {
	return SGLog(std::cout, SGLOG_ERRORS);
}

SGLog infolog() {
	return SGLog(std::cout, SGLOG_INFO);
}